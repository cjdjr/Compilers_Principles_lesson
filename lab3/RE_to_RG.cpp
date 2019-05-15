/*
正规式转正规文法
author: Chelly

对输入格式有严格要求：
    1、要求输入的正规式合法(有右线性正规文法与其对应)且没有多余的括号
    2、没有多余的空白字符
    3、不出现e (因为e是作为空字的存在)

example:
(b(a|b)*c|z)
*/
#include<bits/stdc++.h>
using namespace std;

struct RG
{
    vector<char> star;  //A->(a|b)A
    vector<char> endchar;  //A->a|b
    vector<string> Next;    //A->aB|cC
}G[128];
void add_grammar(char s,char t1,char t2)
{
    if(t2==0) G[s].endchar.push_back(t1);
    else
        if(t2==s) G[s].star.push_back(t1);
        else G[s].Next.push_back(string(1,t1)+string(1,t2));
}
char letter[27];
int now=1;
string s;
void dfs(string s,char father)
{

    int n=s.length();
    if(s[0]=='('&&s[n-1]==')')
    {
        s=s.substr(1,n-2);
        n=s.length();
    }
    int num=0;
    int last=-1;
    for(int i=0;i<n;++i)
        if(s[i]=='(') ++num;
        else if(s[i]==')') --num;
        else if(s[i]=='|'&&num==0)
        {
            dfs(s.substr(last+1,i-last-1),father);
            last=i;
        }
    if(last!=-1)
        dfs(s.substr(last+1,n-last-1),father);
    else
    {
        //if(n==1) add_grammar(father,s[0],0);
        if(n>=2&&(s[0]=='('||s[1]=='*'))        //去*闭包
        {
            int pos;
            pos=s.find('*');
            string tmp=s.substr(0,pos);
            if(tmp[0]=='(') tmp=tmp.substr(1,int(tmp.size())-2);
            for(int i=0;i<tmp.size();i+=2) add_grammar(father,tmp[i],father);
            s=s.substr(pos+1,n-pos);
            dfs(s,father);
            return;
        }
        if(n==1)
        {
            add_grammar(father,s[0],0);
            return;
        }
        ++now;
        add_grammar(father,s[0],letter[now]);
        dfs(s.substr(1,int(s.size())-1),letter[now]);
    }
    //return letter[k];
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    freopen("in.txt","r",stdin);
    cin>>s;
    letter[1]='S';
    for(int i='A';i<'S';++i) letter[i-'A'+2]=i;
    for(int i='T';i<='Z';++i) letter[i-'A'+1]=i;
    //for(int i=1;i<=26;++i) cout<<letter[i]<<" ";
    dfs(s,'S');
    for(int i=1;i<=26;++i)
    {
        char c=letter[i];
        for(auto x:G[c].star) cout<<c<<"->"<<x<<c<<endl;
        for(auto x:G[c].endchar) cout<<c<<"->"<<x<<endl;
        for(auto x:G[c].Next) cout<<c<<"->"<<x<<endl;
    }
    return 0;
}
