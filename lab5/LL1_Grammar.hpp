#include<vector>
#include<string>
#include<bitset>
#include<set>
#include<iostream>
#include<fstream>
using namespace std;
#ifndef _LL1_
#define _LL1_
//LL(1)文法

class LL1_G
{
private:
    set<char> VN;               //非终结符
    set<char> VT;               //终结符
    vector<vector<string>> P;   //产生式
    vector<bool> Empty;         //Empty
    vector<bitset<128>> First;  //First
    vector<bitset<128>> Follow; //Follow
    vector<vector<bitset<128>>> Select; //Select
    char S;                     //开始符
    int num=0;                  //产生式个数

    void add_VN(char c);
    void add_VT(char c);
    void cal_Empty();
    void cal_First();
    void cal_Follow();
    void cal_Select();
    bitset<128> first(string s);

public:
    LL1_G();
    void set_S(char c);
    void add_grammar(char left,string s);
    void display();
    void init();
    bool is_LL1();
    void Gen_Parser(string path);
};

LL1_G::LL1_G()
{
    VN.clear();
    VT.clear();
    P.resize(128);
    for(int i=0;i<128;++i) P[i].clear();
    First.resize(128);
    for(int i=0;i<128;++i) First[i].reset();
    Follow.resize(128);
    for(int i=0;i<128;++i) Follow[i].reset();
    Select.resize(128);
    for(int i=0;i<128;++i) Select[i].clear();
    Empty.resize(128);
    for(int i=0;i<128;++i) Empty[i]=0;
    S=0;
    num=0;
}
void LL1_G::add_VN(char c)
{
    VN.insert(c);
}
void LL1_G::add_VT(char c)
{
    VT.insert(c);
}
void LL1_G::set_S(char c)
{
    add_VN(c);
    S=c;
}
void LL1_G::add_grammar(char left,string s)
{
    ++num;
    add_VN(left);
    for(auto ch:s)
        if(ch>='A'&&ch<='Z') add_VN(ch);else add_VT(ch);
    P[left].push_back(s);
}
void LL1_G::display()
{
    cout<<"This LL(1) Grammar has "<<VN.size()<<" VN and "<<VT.size()<<" VT and "<<num<<" productions."<<endl;
    cout<<"Productions : (the 'e' denote Epsilon)"<<endl;
    for(auto left:VN)
        for(auto right:P[left]) cout<<left<<"->"<<right<<endl;
    cout<<"The start word is "<<S<<endl;
}
void LL1_G::init()
{
    cal_Empty();
    cal_First();
    /*for(auto x:VT)
    {
        cout<<x<<" : ";
        for(auto y:VT)
            if(First[x][y]) cout<<y<<" ";
        cout<<endl;
    }
    for(auto x:VN)
    {
        cout<<x<<" : ";
        for(auto y:VT)
            if(First[x][y]) cout<<y<<" ";
        cout<<endl;
    }*/
    cal_Follow();
    /*for(auto x:VN)
    {
        cout<<x<<" (follow) : ";
        for(int i=0;i<128;++i)
            if(Follow[x][i]) cout<<char(i)<<" ";
        cout<<endl;
    }*/
    cal_Select();
    /*for(auto left:VN)
    {
        cout<<left<<" (select) : ";
        for(int i=0;i<Select[left].size();++i)
        {
            cout<<P[left][i]<<" : ";
            for(int j=0;j<128;++j)
                if(Select[left][i][j]) cout<<char(j)<<" ";
            cout<<endl;
        }
        cout<<endl;
    }*/
}
bitset<128> LL1_G::first(string s)
{
    bitset<128> ans;
    ans.reset();
    for(auto ch:s)
        if(!Empty[ch])
        {
            ans|=First[ch];
            return ans;
        }
        else
        {
            ans|=First[ch];
            ans['e']=0;
        }
    ans.set('e');
    return ans;
}
bool LL1_G::is_LL1()
{
    for(auto left:VN)
    {
        int n=P[left].size();
        for(int i=1;i<n;++i)
            for(int j=0;j<i;++j)
                if((Select[left][i]&Select[left][j]).count()!=0) return false;
    }
    return true;
}
void LL1_G::cal_Empty()
{
    while(true)
    {
        bool update=false;
        for(auto left:VN)
            if(!Empty[left])
            {
                for(auto s:P[left])
                {
                    if(s.length()==1&&s[0]=='e') Empty[left]=1;
                    else
                    {
                        Empty[left]=1;
                        for(auto ch:s)
                            if(!Empty[ch]) Empty[left]=0;
                    }
                    if(Empty[left]) break;
                }
                if(Empty[left]) update=true;
            }
        if(!update) break;
    }
}
void LL1_G::cal_First()
{
    //for(auto x:VT) cout<<x<<" : "<<Empty[x]<<endl;
    //for(auto x:VN) cout<<x<<" : "<<Empty[x]<<endl;
    for(auto x:VT) First[x].set(x);
    while(true)
    {
        bool update=false;
        for(auto left:VN)
        {
            int last=First[left].count();
            for(auto s:P[left]) First[left]|=first(s);
            if(Empty[left]) First[left].set('e');
            if(First[left].count()!=last) update=true;
        }
        if(!update) break;
    }
}
void LL1_G::cal_Follow()
{
    Follow[S].set('#');
    vector<int> last;
    last.resize(128);

    while(true)
    {
        bool update=false;
        for(auto left:VN) last[left]=Follow[left].count();
        for(auto left:VN)
            for(auto s:P[left])
                for(int i=0;i<s.length();++i)
                    if(VN.find(s[i])!=VN.end())
                    {
                        bitset<128> tmp=first(s.substr(i+1,int(s.length())-1-i));
                        if(tmp['e']) Follow[s[i]]|=Follow[left];
                        tmp['e']=0;
                        Follow[s[i]]|=tmp;
                    }
        for(auto left:VN)
            if(Follow[left].count()!=last[left]) update=true;
        if(!update) break;
    }
}
void LL1_G::cal_Select()
{
    for(auto left:VN)
    {
        Select[left].resize(P[left].size());
        for(int i=0;i<P[left].size();++i)
        {
            string s=P[left][i];
            bitset<128> tmp=first(s);
            if(tmp['e'])
            {
                tmp['e']=0;
                Select[left][i]=tmp|Follow[left];
            }
            else Select[left][i]=tmp;
        }
    }
}
void LL1_G::Gen_Parser(string path)
{
    ofstream out(path);
    out<<"#include<stdio.h>"<<endl;
    out<<"char ch;"<<endl;
    out<<"char get_char()"<<endl;
    out<<"{"<<endl;
    out<<"\treturn getchar();"<<endl;
    out<<"}"<<endl;
    out<<"void error()"<<endl;
    out<<"{"<<endl;
    out<<"\tprintf(\"该字符串是不合法的单词 \\n\");"<<endl;
    out<<"\texit(0);"<<endl;
    out<<"}"<<endl;

    for(auto left:this->VN)
    {
        out<<"void Parser_"<<char(left)<<"()"<<endl;
        out<<"{"<<endl;
        out<<"\tswitch(ch)"<<endl;
        out<<"\t{"<<endl;
        for(int i=0;i<P[left].size();++i)
        {
            string tmp="";
            for(auto ch:P[left][i])
                if(ch=='e') ;
                else
                if(this->VN.find(ch)!=this->VN.end()) tmp+="\t\tParser_"+string(1,char(ch))+"();\n";
                else tmp+="\t\tif(ch=='"+string(1,char(ch))+"') ch=get_char(); else error();\n";
            tmp+="\t\tbreak;\n";
            //cout<<tmp<<endl;
            bitset<128> sec=this->Select[left][i];
            for(int ch=sec._Find_first(); ch != sec.size(); ch = sec._Find_next(ch))
            {
                //cout<<char(ch)<<endl;
                out<<"\tcase '"<<char(ch)<<"':"<<endl;
                out<<tmp;
            }
        }
        //out<<"\tcase '#': return;"<<endl;
        out<<"\tdefault: error();"<<endl;
        out<<"\t}"<<endl;
        out<<"}"<<endl;
    }

    out<<"int main()"<<endl;
    out<<"{"<<endl;
    out<<"\tch=get_char();"<<endl;
    out<<"\tParser_"<<char(this->S)<<"();"<<endl;
    out<<"\tif(ch!='#') error();"<<endl;
    out<<"\tprintf(\"该字符串是合法的单词 \\n\");"<<endl;
    out<<"\treturn 0;"<<endl;
    out<<"}"<<endl;

}
#endif // _LL1_
