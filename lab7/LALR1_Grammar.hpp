#include<vector>
#include<string>
#include<bitset>
#include<set>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstring>
#include<queue>
#include<map>
using namespace std;
#ifndef _LALR1_
#define _LALR1_
//LALR(1)文法
/*
用set数据结构表示终结符、非终结符
*/
class LALR1_G
{
private:
    set<char> VN;                       //非终结符
    set<char> VT;                       //终结符
    vector<vector<string>> P;           //产生式

    vector<map<string,string>> I;       //LALR(1)项目集
    vector<vector<char>> go;            //DFA里的转移函数
    vector<vector<int>> GOTO;           //GOTO表
    vector<vector<string>> ACTION;      //ACTION表

    vector<bool> Empty;                 //Empty集
    vector<bitset<128>> First;          //First集

    char S;                             //开始符
    int num=0;                          //产生式个数
    bool IS_LALR1;                      //该文法是否是LALR1文法
    void add_VN(char c);                //添加一个非终结符
    void add_VT(char c);                //添加一个终结符
    void cal_I();                       //计算自动机（项目集）
    bool cal_AG();                      //计算ACTION表和GOTO表
    int check(const map<string,string> &s);                         //判断该状态是否已经出现过
    map<string,string> move(const map<string,string> &s,char ch);   //计算move(s,ch)
    map<string,string> e_closure(const map<string,string> &S);      //计算closure(s)
    void merge(map<string,string> &a,map<string,string> &b);        //将两个同心集合并
    bool merge(string&,const string&);                              //将两个超前搜索字符集合并
    bitset<128> first(string );                                     //求一个字符串的first集(bitset形式)
    string bitset_to_string(const bitset<128>&);                    //将一个bitset集转成字符串形式
    void cal_Empty();                                               //计算Empty集
    void cal_First();                                               //计算First集

public:
    LALR1_G();                                  //构造函数，对内存初始化
    void set_S(char c);                         //指定开始符是谁
    void add_grammar(char left,string s);       //添加一个文法
    void display();                             //展示文法
    void display_table();                       //展示GOTO表和ACTION表
    void init();                                //初始化
    bool is_LALR1();                            //判断该文法是否是LALR1文法

};

LALR1_G::LALR1_G()
{
    VN.clear();
    VT.clear();
    P.resize(128);
    for(int i=0;i<128;++i) P[i].clear();
    I.clear();
    go.clear();
    GOTO.clear();
    ACTION.clear();
    S=0;
    num=0;

    First.resize(128);
    for(int i=0;i<128;++i) First[i].reset();
    Empty.resize(128);
    for(int i=0;i<128;++i) Empty[i]=0;
}
void LALR1_G::add_VN(char c)
{
    VN.insert(c);
}
void LALR1_G::add_VT(char c)
{
    VT.insert(c);
}
void LALR1_G::set_S(char c)
{
    add_VN(c);
    S=c;
}
void LALR1_G::add_grammar(char left,string s)
{
    ++num;
    add_VN(left);
    for(auto ch:s)
        if(ch>='A'&&ch<='Z') add_VN(ch);else add_VT(ch);
    P[left].push_back(s);
}
void LALR1_G::display()
{
    cout<<"This LALR(1) Grammar has "<<VN.size()<<" VN and "<<VT.size()<<" VT and "<<num<<" productions."<<endl;
    cout<<"Productions : (the 'e' denote Epsilon)"<<endl;
    for(auto left:VN)
        for(auto right:P[left]) cout<<left<<"->"<<right<<endl;
    cout<<"The start word is "<<S<<endl;
}
void LALR1_G::display_table()
{
    //cout<<I.size()<<endl;
    int width=10;
    cout<<setw(width*((2+this->VT.size())/2+1))<<"ACTION";
    cout<<setw(width*((1+this->VN.size())/2+3+this->VT.size()-((2+this->VT.size())/2+1)))<<"GOTO";
    cout<<endl;
    cout<<setw(width)<<"";
    for(auto x:this->VT)
        cout<<setw(width)<<x;
    cout<<setw(width)<<"#";
    cout<<setw(width)<<"";
    for(auto x:this->VN)
        cout<<setw(width)<<x;
    cout<<endl<<endl;
    for(int i=0;i<I.size();++i)
    {
        cout<<setw(width)<<i;
        for(auto x:this->VT)
            cout<<setw(width)<<ACTION[i][x];
        cout<<setw(width)<<ACTION[i]['#'];
        cout<<setw(width)<<"";
        for(auto x:this->VN)
            if(GOTO[i][x]!=-1)
                cout<<setw(width)<<GOTO[i][x];
            else cout<<setw(width)<<" ";
        cout<<endl;
    }
}
void LALR1_G::init()
{
    cal_Empty();
    cal_First();
    IS_LALR1=cal_AG();
}
bool LALR1_G::is_LALR1()
{
    return IS_LALR1;
}
bool LALR1_G::cal_AG()
{
    //return true;
    cal_I();
    //cout<<"cal_I pass!"<<endl;
    int n=I.size();
    ACTION.resize(n);
    GOTO.resize(n);
    for(int i=0;i<n;++i)
    {
        GOTO[i].resize(128),ACTION[i].resize(128);
        for(int j=0;j<128;++j) GOTO[i][j]=-1;
        //memset(GOTO[i],-1,sizeof(GOTO[i]));
    }
    for(int i=0;i<n;++i)
    {
        for(auto ch:this->VT)
            if(go[i][ch]!=-1) ACTION[i][ch]="S"+to_string(go[i][ch]);
        for(auto ch:this->VN)
            if(go[i][ch]!=-1) GOTO[i][ch]=go[i][ch];
    }
    for(int i=0;i<n;++i)
        if(I[i].find("Z->"+string(1,this->S)+".")!=I[i].end()) ACTION[i]['#']="acc";
    for(auto left:VN)
        for(int i=0;i<P[left].size();++i)
        {
            string tmp=string(1,left)+"->"+P[left][i]+".";
            for(int j=0;j<n;++j)
                if(I[j].find(tmp)!=I[j].end())
                {
                    string pre_search_set=I[j][tmp];
                    for(auto x:pre_search_set)
                        if(ACTION[j][x]=="")
                            ACTION[j][x]="r("+string(1,left)+","+to_string(i)+")";
                        else return false;
                }
        }
    return true;
}
map<string,string> LALR1_G::e_closure(const map<string,string> &S)
{
    map<string,string> ans;
    ans.clear();
    for(auto s:S) ans.insert(s);
    while(true)
    {
        int update=0;
        for(auto x:ans)
        {
            string now=x.first;     //kernel
            int pos=now.find('.');
            if(pos+1!=now.length()&&this->VN.find(now[pos+1])!=this->VN.end())
            {
                char ch=now[pos+1];
                //cout<<"xixi"<<endl;
                string pre_search_set=bitset_to_string(first(now.substr(pos+2,int(now.length())-pos-2)+x.second));
                //cout<<"debug pre_search_set : "<<pre_search_set<<endl;
                for(auto r:P[ch])
                {
                    string kernel=string(1,ch)+"->."+r;
                    if(ans.find(kernel)==ans.end())
                    {
                        ans.insert(make_pair(kernel,pre_search_set));
                        update=1;
                    }
                    else update=merge(ans[kernel],pre_search_set);
                }
            }
        }
        //break;
        if(!update) break;
    }
    return ans;
}
map<string,string> LALR1_G::move(const map<string,string> &s,char ch)
{
    map<string,string> ans;
    ans.clear();
    for(auto x:s)
    {
        string now=x.first;
        int pos=now.find('.');
        if(pos+1!=now.length()&&now[pos+1]==ch)
        {
            //0~pos-1 pos+1 . pos+2~length()-1
            string tmp=now.substr(0,pos)+now.substr(pos+1,1)+now.substr(pos,1)+now.substr(pos+2,int(now.length())-pos-2);
            ans.insert(make_pair(tmp,x.second));
        }
    }
    return ans;
}
void LALR1_G::cal_I()
{
    //cout<<"ok"<<endl;
    int now=1;
    I.push_back(e_closure(
                          map<string,string>{{"Z->."+string(1,this->S),"#"}}
                          )
                );
    //cout<<I[0].size()<<endl;
    //cout<<"ok"<<endl;
    //this->add_grammar('Z',string(1,this->S));
    go.resize(1);
    go[0].resize(128);
    for(int j=0;j<128;++j) go[0][j]=-1;
    //this->set_S('Z');
    queue<int> q;
    while(!q.empty()) q.pop();
    q.push(0);
    //cout<<"ok"<<endl;
    while(!q.empty())
    {
        int id=q.front();
        q.pop();
        for(auto ch:this->VN)
        {
            map<string,string> tmp=e_closure(move(I[id],ch));
            if(tmp.empty()) continue;
            int to=check(tmp);
            if(to==-1)
            {
                I.push_back(tmp);
                ++now;
                go.resize(now);
                go[now-1].resize(128);
                for(int j=0;j<128;++j) go[now-1][j]=-1;
                //memset(go[now-1],-1,sizeof(go[now-1]));
                go[id][ch]=now-1;
                q.push(now-1);
            }
            else
            {
                go[id][ch]=to;
                merge(I[to],tmp);
            }
        }
        for(auto ch:this->VT)
        {
            map<string,string> tmp=e_closure(move(I[id],ch));
            if(tmp.empty()) continue;
            int to=check(tmp);
            if(to==-1)
            {
                I.push_back(tmp);
                ++now;
                go.resize(now);
                go[now-1].resize(128);
                for(int j=0;j<128;++j) go[now-1][j]=-1;
                //memset(go[now-1],-1,sizeof(go[now-1]));
                go[id][ch]=now-1;
                q.push(now-1);
            }
            else
            {
                go[id][ch]=to;
                merge(I[to],tmp);
            }
        }
    }
}
int LALR1_G::check(const map<string,string> &s)
{
    for(int i=0;i<I.size();++i)
    {
        if(I[i].size()!=s.size()) continue;
        bool flag=1;
        for(auto x:I[i])
            if(s.find(x.first)==s.end())
            {
                flag=0;
                break;
            }
        if(flag) return i;
    }
    return -1;
}
void LALR1_G::merge(map<string,string> &a,map<string,string> &b)
{
    map<string,string>::iterator i,j;
    for(i=a.begin(),j=b.begin();i!=a.end();++i,++j)
    {
        string &s=(*i).second;
        string t=(*j).second;
        for(auto ch:t)
            if(s.find(ch)==string::npos) s.push_back(ch);
    }
}
bool LALR1_G::merge(string &a,const string &b)
{
    bool update=0;
    for(auto ch:b)
        if(a.find(ch)==string::npos) a.push_back(ch),update=1;
    return update;
}
void LALR1_G::cal_Empty()
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
void LALR1_G::cal_First()
{
    for(auto x:VT) First[x].set(x);
    First['#'].set('#');        // !!!!
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
bitset<128> LALR1_G::first(string s)
{
    bitset<128> ans;
    ans.reset();
    //cout<<"first="<<s<<endl;
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
string LALR1_G::bitset_to_string(const bitset<128> &a)
{

    string ans="";
    for(char ch=a._Find_first();ch<a.size();ch=a._Find_next(ch)) ans.push_back(ch);
    return ans;
}
#endif // _LALR1_
