#include<vector>
#include<string>
#include<bitset>
#include<set>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstring>
#include<queue>
using namespace std;
#ifndef _LR0_
#define _LR0_
//LR(0)文法

class LR0_G
{
private:
    set<char> VN;               //非终结符
    set<char> VT;               //终结符
    vector<vector<string>> P;   //产生式

    vector<set<string>> I;        //LR(0)项目集
    vector<vector<char>> go;
    vector<vector<int>> GOTO;
    vector<vector<string>> ACTION;
    char S;                     //开始符
    int num=0;                  //产生式个数
    bool IS_LR0;
    void add_VN(char c);
    void add_VT(char c);
    void cal_I();
    bool cal_AG();
    int check(const set<string> &s);
    set<string> move(const set<string> &s,char ch);
    set<string> e_closure(const set<string> &S);


public:
    LR0_G();
    void set_S(char c);
    void add_grammar(char left,string s);
    void display();
    void display_table();
    void init();
    bool is_LR0();

};

LR0_G::LR0_G()
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
}
void LR0_G::add_VN(char c)
{
    VN.insert(c);
}
void LR0_G::add_VT(char c)
{
    VT.insert(c);
}
void LR0_G::set_S(char c)
{
    add_VN(c);
    S=c;
}
void LR0_G::add_grammar(char left,string s)
{
    ++num;
    add_VN(left);
    for(auto ch:s)
        if(ch>='A'&&ch<='Z') add_VN(ch);else add_VT(ch);
    P[left].push_back(s);
}
void LR0_G::display()
{
    cout<<"This LL(1) Grammar has "<<VN.size()<<" VN and "<<VT.size()<<" VT and "<<num<<" productions."<<endl;
    cout<<"Productions : (the 'e' denote Epsilon)"<<endl;
    for(auto left:VN)
        for(auto right:P[left]) cout<<left<<"->"<<right<<endl;
    cout<<"The start word is "<<S<<endl;
}
void LR0_G::display_table()
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
void LR0_G::init()
{
    IS_LR0=cal_AG();
}
bool LR0_G::is_LR0()
{
    return IS_LR0;
}
bool LR0_G::cal_AG()
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
                    for(auto x:this->VT)
                        if(ACTION[j][x]=="")
                            ACTION[j][x]="r("+string(1,left)+","+to_string(i)+")";
                        else return false;
                    if(ACTION[j]['#']=="")
                        ACTION[j]['#']="r("+string(1,left)+","+to_string(i)+")";
                    else return false;
                }
        }
    return true;
}
set<string> LR0_G::e_closure(const set<string> &S)
{
    set<string> ans;
    ans.clear();
    for(auto s:S) ans.insert(s);
    while(true)
    {
        int update=ans.size();
        for(auto now:ans)
        {
            int pos=now.find('.');
            if(pos+1!=now.length()&&this->VN.find(now[pos+1])!=this->VN.end())
            {
                char ch=now[pos+1];
                for(auto r:P[ch]) ans.insert(string(1,ch)+"->."+r);
            }
        }
        if(update==ans.size()) break;
    }
    return ans;
}
set<string> LR0_G::move(const set<string> &s,char ch)
{
    set<string> ans;
    ans.clear();
    for(auto now:s)
    {
        int pos=now.find('.');
        if(pos+1!=now.length()&&now[pos+1]==ch)
        {
            //0~pos-1 pos+1 . pos+2~length()-1
            string tmp=now.substr(0,pos)+now.substr(pos+1,1)+now.substr(pos,1)+now.substr(pos+2,int(now.length())-pos-2);
            ans.insert(tmp);
        }
    }
    return ans;
}
void LR0_G::cal_I()
{
    int now=1;
    I.push_back(e_closure(set<string>{"Z->."+string(1,this->S)}));
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
    while(!q.empty())
    {
        int id=q.front();
        q.pop();
        for(auto ch:this->VN)
        {
            set<string> tmp=e_closure(move(I[id],ch));
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
            else go[id][ch]=to;
        }
        for(auto ch:this->VT)
        {
            set<string> tmp=e_closure(move(I[id],ch));
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
            else go[id][ch]=to;
        }
    }
}
int LR0_G::check(const set<string> &s)
{
    for(int i=0;i<I.size();++i)
    {
        if(I[i].size()!=s.size()) continue;
        bool flag=1;
        for(auto x:I[i])
            if(s.find(x)==s.end())
            {
                flag=0;
                break;
            }
        if(flag) return i;
    }
    return -1;
}
#endif // _LR0_
