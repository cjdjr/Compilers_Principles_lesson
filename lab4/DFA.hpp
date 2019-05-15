#include<set>
#include<iostream>
#include "RG.hpp"
#include "RE.hpp"
using namespace std;
#ifndef _DFA_
#define _DFA_


struct DFA
{
    set<char> K;                //状态集合
    set<char> Sigma;            //字符集
    vector<vector<char>> f;     //转移
    char S;                     //起始状态
    set<char> Z;                //终态集合
    int numer_of_transfer=0;
    DFA()
    {
        f.resize(128);
        for(int i=0;i<128;++i) f[i].resize(128);
        for(int i=0;i<128;++i)
            for(int j=0;j<128;++j)
                f[i][j]=0;
    }
    DFA(const DFA& x)
    {
        K=x.K;
        Sigma=x.Sigma;
        for(int i=0;i<128;++i)
            for(int j=0;j<128;++j)
                f[i][j]=x.f[i][j];
        S=x.S;
        Z=x.Z;
        numer_of_transfer=x.numer_of_transfer;
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
        //if(c==0) return;
        add_K(from);
        add_K(to);
        add_Sigma(c);
        f[from][c]=to;
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
        cout<<"This DFA has "<<K.size()<<" states and "<<numer_of_transfer<<" transfers."<<endl;
        cout<<"State : ";for(auto x:K) cout<<x<<" ";cout<<endl;
        cout<<"Sigma : ";for(auto x:Sigma) cout<<x<<" ";cout<<endl;
        cout<<"Transfer : (the 'e' denote Epsilon)"<<endl;
        for(auto from:K)
            for(auto c:Sigma)
                if(f[from][c]!=0) cout<<"f("<<from<<","<<c<<")="<<f[from][c]<<endl;
        cout<<"Start state : "<<S<<endl;
        cout<<"End state : ";for(auto x:Z) cout<<x<<" ";cout<<endl;
    }
    RG trans_to_RG();
    string trans_to_RE();
};

RG DFA::trans_to_RG()
{
    RG rg;
    rg.set_S(S);
    for(auto from:K)
        for(auto c:Sigma)
            if(f[from][c]!=0)
            {
                //cout<<"debug : "<<from << " "<<c<<" "<<f[from][c]<<endl;
                rg.add_grammar(from,c,f[from][c]);
            }
    for(auto x:Z) rg.add_grammar(x,'e');
    return rg;
}

string DFA::trans_to_RE()
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
            if(this->f[from][c]!=0)
            {
                //cout<<"debug : "<<from<<" "<<to<<endl;
                string ans="";
                ans.push_back(c);
                if(g[from][f[from][c]]!="") g[from][f[from][c]]+="|"+ans;else g[from][f[from][c]]=ans;
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

#endif // _DFA_
