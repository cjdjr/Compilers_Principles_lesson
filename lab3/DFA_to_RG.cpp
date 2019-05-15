#include<sstream>
#include<iostream>
#include<stdio.h>
#include "DFA.hpp"
#include "RG.hpp"
using namespace std;
DFA dfa;
int main()
{
    freopen("DFA.txt","r",stdin);
    string tmp;
    getline(cin,tmp);
    stringstream ss(tmp);
    char S;
    ss>>S;
    dfa.set_S(S);
    getline(cin,tmp);
   // cout<<tmp<<endl;
    ss.clear();
    ss.str(tmp);
    while(ss>>S)
    {
        //cout<<"wmr"<<S<<endl;
        dfa.add_Z(S);
    }
    int n;
    cin>>n;
    getline(cin,tmp);
    while(n--)
    {
        getline(cin,tmp);
        //cout<<"wmr"<<tmp<<endl;
        ss.clear();
        ss.str(tmp);
        string from,c,to;
        ss>>from;
        ss>>c;
        ss>>to;
        //cout<<from<<" "<<c<<" "<<to<<endl;
        //cout<<from[0]<<" "<<c[0]<<" "<<to[0]<<endl;
        dfa.add_trans(from[0],c[0],to[0]);
    }
    dfa.display();
    RG rg=dfa.trans_to_RG();
    rg.display();
    /*rg.add_grammar('S','e');
    rg.add_grammar('A','e');
    rg.add_grammar('B','e');
    rg.display();
    cout<<rg.transfer_to_RE()<<endl;*/
    return 0;
}
