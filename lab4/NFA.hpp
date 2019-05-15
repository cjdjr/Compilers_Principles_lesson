#include<set>
#include<iostream>
#include<queue>
#include<map>
#include "DFA.hpp"
#include "RE.hpp"
using namespace std;
#ifndef _NFA_
#define _NFA_
struct NFA
{
    set<char> K;                //状态集合
    set<char> Sigma;            //字符集
    vector<vector<set<char>>> f;      //转移
    char S;                     //起始状态
    set<char> Z;                //终态集合
    int numer_of_transfer=0;
    NFA(string re)
    {
        f.resize(128);
        for(int i=0;i<128;++i) f[i].resize(128);
        for(int i=0;i<128;++i)
            for(int j=0;j<128;++j)
                f[i][j].clear();
        RE_to_NFA(re);
    }
    NFA()
    {
        f.resize(128);
        for(int i=0;i<128;++i) f[i].resize(128);
        for(int i=0;i<128;++i)
            for(int j=0;j<128;++j)
                f[i][j].clear();
    }

    void add_K(char c)
    {
        K.insert(c);
    }
    void add_Sigma(char c)
    {
        Sigma.insert(c);
    }
    void add_trans(char from,char c,char to)
    {
        add_K(from);
        add_K(to);
        add_Sigma(c);
        f[from][c].insert(to);
        ++numer_of_transfer;
    }
    void set_S(char c)
    {
        add_K(c);
        S=c;
    }
    void add_Z(char c)
    {
        add_K(c);
        Z.insert(c);
    }
    void display()
    {
        cout<<"This NFA has "<<K.size()<<" states and "<<numer_of_transfer<<" transfers."<<endl;
        cout<<"State : ";for(auto x:K) cout<<x<<" ";cout<<endl;
        cout<<"Sigma : ";for(auto x:Sigma) cout<<x<<" ";cout<<endl;
        cout<<"Transfer : (the 'e' denote Epsilon)"<<endl;;
        for(auto from:K)
            for(auto c:Sigma)
                if(!f[from][c].empty())
                {
                    cout<<"f("<<from<<","<<c<<")={";
                    for(auto to:f[from][c])
                    {
                        cout<<to;
                        if(to!=*f[from][c].rbegin()) cout<<",";
                    }
                    cout<<"}"<<endl;
                }
        cout<<"Start state : "<<S<<endl;
        cout<<"End state : ";for(auto x:Z) cout<<x<<" ";cout<<endl;
    }
    bool is_end_state(const set<char> &a);
    set<char> e_closure(const set<char> &a);
    set<char> Move(const set<char> &a,char c);
    DFA trans_to_DFA();
    void RE_to_NFA(string re);
    string trans_to_RE();
};
bool NFA::is_end_state(const set<char> &a)
{
    for(auto x:a)
        if(Z.find(x)!=Z.end()) return true;
    return false;
}
set<char> NFA::e_closure(const set<char> &a)
{
    set<char> ans;
    ans.clear();
    queue<char> q;
    while(!q.empty()) q.pop();
    for(auto x:a) q.push(x);
    bool vis[128];
    for(int i=0;i<128;++i) vis[i]=0;
    while(!q.empty())
    {
        char now=q.front();
        q.pop();
        if(vis[now]) continue;
        vis[now]=1;
        ans.insert(now);
        for(auto Next:f[now]['e']) q.push(Next);
    }
    return ans;
}
set<char> NFA::Move(const set<char> &a,char c)
{
    set<char> ans;
    ans.clear();
    for(auto from:a)
        for(auto to:f[from][c]) ans.insert(to);
    return ans;
}
DFA NFA::trans_to_DFA()
{
    DFA dfa;
    map<set<char>,int> Hash;
    Hash.clear();
    char id='A';
    queue<set<char>> q;
    while(!q.empty()) q.pop();
    set<char> start;
    start.clear();
    start.insert(S);
    start=e_closure(start);
    dfa.set_S('A');
    q.push(start);
    Hash[start]=id++;
    if(is_end_state(start)) dfa.add_Z('A');
    while(!q.empty())
    {
        set<char> now=q.front();
        q.pop();
        for(auto c:Sigma)
        {
            if(c=='e'||c==0) continue;
            set<char> u=e_closure(Move(now,c));
            if(u.empty()) continue;
            if(Hash.find(u)==Hash.end())
            {
                Hash[u]=id++;
                if(is_end_state(u)) dfa.add_Z(id-1);
                q.push(u);
            }
            cout<<"c="<<c<<endl;
            dfa.add_trans(Hash[now],c,Hash[u]);
        }
    }
    return dfa;
}


void NFA::RE_to_NFA(string re)
{
    vector<vector<vector<string>>> g;
    g.resize(128);
    for(int i=0;i<128;++i)
    {
        g[i].resize(128);
        for(int j=0;j<128;++j) g[i][j].clear();
    }
    g['A']['B'].push_back(re);
    char now='B';
    queue<pair<pair<char,char>,int>> q;
    while(!q.empty()) q.pop();
    q.push(mp(mp('A','B'),0));
    //cout<<"ok"<<endl;
    while(!q.empty())
    {
        pair<pair<char,char>,int> t=q.front();
        q.pop();
        char u=t.first.first,v=t.first.second;
        int id=t.second;
        if(g[u][v][id].length()==1) continue;
        //cout<<u<<" "<<v<<" "<<id<<endl;
        vector<string> s=RE::split_or(g[u][v][id]);
        //cout<<s.size()<<endl;
        if(s.size()==1)
        {
            pair<string,string> tmp=RE::split_atom(s[0]);
            //cout<<tmp.first<<" "<<tmp.second<<endl;
            if(tmp.second=="")
            {
                ++now;
                g[u][now].push_back("e");
                q.push(mp(mp(u,now),int(g[u][now].size())-1));
                g[now][v].push_back("e");
                q.push(mp(mp(now,v),int(g[now][v].size())-1));
                g[now][now].push_back(tmp.first.substr(1,int(tmp.first.length())-3));
                q.push(mp(mp(now,now),int(g[now][now].size())-1));
            }
            else
            {
                ++now;
                g[u][now].push_back(tmp.first);
                q.push(mp(mp(u,now),int(g[u][now].size())-1));
                g[now][v].push_back(tmp.second);
                q.push(mp(mp(now,v),int(g[now][v].size())-1));
            }
        }
        else
            for(auto x:s)
            {
                g[u][v].push_back(RE::strip(x));
                q.push(mp(mp(u,v),int(g[u][v].size())-1));
            }

    }
    f.resize(128);
    for(int i=0;i<128;++i) f[i].resize(128);
    for(int i=0;i<128;++i)
        for(int j=0;j<128;++j)
            f[i][j].clear();
    K.clear();
    Sigma.clear();
    Z.clear();
    numer_of_transfer=0;
    set_S('A');
    add_Z('B');
    for(char i='A';i<=now;++i)
        for(char j='A';j<=now;++j)
            for(auto x:g[i][j])
                if(x.length()==1) add_trans(i,x[0],j);
}

string NFA::trans_to_RE()
{

    vector<vector<string>> g;
    g.resize(128);
    for(int i=0;i<128;++i)
    {
        g[i].resize(128);
        for(int j=0;j<128;++j)
            g[i][j]="";
    }

    for(auto from:this->K)
        for(auto c:this->Sigma)
            for(auto to:this->f[from][c])
            {
                //cout<<"debug : "<<from<<" "<<to<<endl;
                string ans="";
                ans.push_back(c);
                if(g[from][to]!="") g[from][to]+="|"+ans;else g[from][to]=ans;
            }
    //cout<<"debug : "<<g['B']['B']<<endl;
    g['X'][this->S]='e';
    for(auto end_state:this->Z)
        g[end_state]['Y']='e';
    if(this->Z.find(this->S)!=this->Z.end()) g['X']['Y']="e";
    set<char> K(this->K);
    K.insert('X');
    K.insert('Y');

    while(true)
    {
        bool update=false;
        for(auto mid:K)
        {
            set<int> L;
            set<int> R;
            L.clear();
            R.clear();
            bool flag=false;
            if(g[mid][mid]!="") flag=true;
            for(auto l:K)
                if(l!=mid&&g[l][mid]!="")
                    for(auto r:K)
                        if(r!=mid&&g[mid][r]!="")
                        {
                            L.insert(l);
                            R.insert(r);
                            string ans="";
                            if(g[l][mid]!="e")
                                if(!RE::check_atom(g[l][mid])) ans+="("+g[l][mid]+")";
                                else ans+=g[l][mid];
                            if(flag) ans+="("+g[mid][mid]+")*";
                            if(g[mid][r]!="e")
                                if(!RE::check_atom(g[mid][r])) ans+="("+g[mid][r]+")";
                                else ans+=g[mid][r];
                            if(ans!=""&&!RE::check_atom(ans)) ans="("+ans+")";
                            if(ans!="")
                            {
                                update=true;
                                if(g[l][r]=="") g[l][r]=ans;else g[l][r]+="|"+ans;
                            }
                        }
            for(auto l:L) g[l][mid]="";
            for(auto r:R) g[mid][r]="";
        }
        if(!update) break;
    }
    return g['X']['Y'];
}
#endif // _NFA_
