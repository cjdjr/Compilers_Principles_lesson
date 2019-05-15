#include<sstream>
#include<iostream>
#include<stdio.h>
#include "DFA.hpp"
#include "NFA.hpp"
#include "RG.hpp"
using namespace std;
DFA dfa;
int main()
{
    freopen("RE.txt","r",stdin);
    string s;
    cin>>s;
    NFA nfa(s);
    nfa.display();
    dfa=nfa.trans_to_DFA();
    dfa.display();
    RG rg=dfa.trans_to_RG();
    rg.display();
    return 0;
}
