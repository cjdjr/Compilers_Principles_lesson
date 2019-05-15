#include "LALR1_Grammar.hpp"
using namespace std;
LALR1_G G;
int main()
{
    freopen("Grammar.txt","r",stdin);
    int T;
    cin>>T;
    while(T--)
    {
        string s;
        cin>>s;
        G.add_grammar(s[0],s.substr(3,int(s.length())-3));
    }
    G.set_S('S');
    G.init();
    if(!G.is_LALR1()) cout<<"Error! This grammar is not LALR(1)!"<<endl;
    //cout<<"Is this grammar LL(1) ?  answer="<<G.is_LL1()<<endl;
    //G.display();
    else
        {
            G.display_table();
        }
    return 0;
}
