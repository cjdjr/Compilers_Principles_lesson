#include<vector>
#include<string>
using namespace std;
#ifndef _RG_
#define _RG_
//右线性正规文法
struct Production
{
    vector<char> star;  //A->(a|b)A
    vector<char> endchar;  //A->a|b
    vector<string> Next;    //A->aB|cC
    Production(){}
};
struct RG
{
    set<char> VN;       //非终结符
    set<char> VT;       //终结符
    Production P[128];  //产生式
    string re[128];     //Regular Expression
    char S;             //开始符
    int num=0;          //产生式个数
    void add_VN(char c)
    {
        VN.insert(c);
    }
    void add_VT(char c)
    {
        VT.insert(c);
    }
    void set_S(char c)
    {
        VN.insert(c);
        S=c;
    }
    void add_grammar(char left,char a,char b=0)
    {
        //cout<<left<<" "<<a<<" "<<b<<endl;
        ++num;
        add_VN(left);
        add_VT(a);
        if(b==0)
        {
            P[left].endchar.push_back(a);
        }
        else
        {
            add_VN(b);
            if(b==left) P[left].star.push_back(a);
            else P[left].Next.push_back(string(1,a)+string(1,b));
        }
    }
    void display()
    {
        cout<<"This Regular Grammar has "<<VN.size()<<" VN and "<<VT.size()<<" VT and "<<num<<" productions."<<endl;
        cout<<"Productions : (the 'e' denote Epsilon)"<<endl;
        for(auto left:VN)
        {
            for(auto x:P[left].star) cout<<left<<"->"<<x<<left<<endl;
            for(auto x:P[left].Next) cout<<left<<"->"<<x<<endl;
            for(auto x:P[left].endchar) cout<<left<<"->"<<x<<endl;
            //if(P[left].star.size()+P[left].Next.size()>0&&P[left].endchar.size()==0) cout<<left<<"->e"<<endl;
        }
        cout<<"The start word is "<<S<<endl;
    }

};


#endif // _RG_
