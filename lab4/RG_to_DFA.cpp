#include<bits/stdc++.h>
#include "NFA.hpp"
using namespace std;
NFA nfa;
DFA dfa;
int main()
{
    freopen("RG.txt","r",stdin);
    /*nfa.set_S('S');
    nfa.add_Z('Z');
    //RG to NFA
    int T;
    cin>>T;
    while(T--)
    {
        string s;
        cin>>s;
        int n=s.length();
        if(n==4) nfa.add_trans(s[0],s[3],'Z');
        else
            nfa.add_trans(s[0],s[3],s[4]);
    }*/
    nfa.set_S('S');
    nfa.add_trans('S','a','S');
    nfa.add_trans('S','b','S');
    nfa.add_trans('S','a','C');
    nfa.add_trans('S','b','A');
    nfa.add_trans('A','b','B');
    nfa.add_trans('B','a','B');
    nfa.add_trans('B','b','B');
    nfa.add_trans('C','a','D');
    nfa.add_trans('D','a','D');
    nfa.add_trans('D','b','D');
    nfa.add_Z('B');
    nfa.add_Z('D');
    nfa.display();
    cout<<endl;
    //dfa=nfa.trans_to_DFA();     //NFA to DFA
    //dfa.display();
    cout<<"Regular Expression : "<<nfa.trans_to_RE();
   // dfa=dfa.
    //cout<<check_atom("a")<<endl;
    return 0;
}
