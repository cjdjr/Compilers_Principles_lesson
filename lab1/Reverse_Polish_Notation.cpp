#include<bits/stdc++.h>
using namespace std;
const int maxn=100;
int priority[128];
void init()
{
    priority['#']=1;
    priority['$']=2;
    priority['(']=3;
    priority[')']=3;
    priority['+']=priority['-']=4;
    priority['*']=priority['/']=5;
}
string char_to_string(char c)
{
    stringstream ss;
    ss << c;
    string str2 = ss.str();
    return str2;
}
string double_to_string(double c)
{
    stringstream ss;
    ss << c;
    string str2 = ss.str();
    return str2;
}
string Reverse_Polish_Symbol(string s)
{
    s+="$";
    string ans="";
    stack<char> op;
    stack<string> num;
    while(!op.empty()) op.pop();
    while(!num.empty()) num.pop();
    op.push('#');
    int len=s.length();
    for(int i=0;i<len;++i)
    {
        char now=s[i];
        if(now>='a'&&now<='z')
        {
            num.push(char_to_string(now));
        }
        else
            if(priority[now]==0)
            {
                ans="Error!";
                return ans;
            }
            else
            {
                if(now=='('||priority[now]>priority[op.top()]) op.push(now);
                else
                {
                    while(priority[op.top()]>=priority[now])
                    {
                        if((op.top())=='('&&now==')')
                        {
                            op.pop();
                            break;
                        }
                        string pre=num.top();
                        num.pop();
                        string prepre=num.top();
                        num.pop();
                        num.push(prepre+" "+pre+" "+char_to_string(op.top()));
                        op.pop();
                    }
                    if(now!=')') op.push(now);
                }
            }
    }
    ans=num.top();
    return ans;
}
double cal(double a,double b,char op)
{
    if(op=='+') return a+b;
    if(op=='-') return a-b;
    if(op=='*') return a*b;
    if(op=='/') return a/b;
}
string Reverse_Polish_double(string s)
{
    s+="$";
    stack<char> op;
    stack<string> num;
    stack<double> ans;
    while(!op.empty()) op.pop();
    while(!num.empty()) num.pop();
    while(!num.empty()) ans.pop();
    op.push('#');
    int len=s.length();
    for(int i=0;i<len;++i)
    {
        char now=s[i];
        if((now>='0'&&now<='9')||(now=='-'&&i>0&&s[i-1]=='('))
        {
            bool flag=0;
            string tmp="";
            if(s[i]=='-') flag=1,++i,tmp="-";
            now=s[i];
            double x=now-'0';
            tmp+=char_to_string(now);
            bool point=0;
            double value=1;
            while((s[i+1]>='0'&&s[i+1]<='9')||s[i+1]=='.')
            {
                ++i;
                tmp+=char_to_string(s[i]);
                if(s[i]=='.') point=1;
                else
                    if(!point) x=x*10+s[i]-'0';
                    else value/=10,x=x+value*(s[i]-'0');

            }
            num.push(tmp);
            if(flag) x=-x;
            ans.push(x);
        }
        else
            if(priority[now]==0) return "Error!";
            else
            {
                if(now=='('||priority[now]>priority[op.top()]) op.push(now);
                else
                {
                    while(priority[op.top()]>=priority[now])
                    {
                        if((op.top())=='('&&now==')')
                        {
                            op.pop();
                            break;
                        }
                        string pre=num.top();
                        num.pop();
                        string prepre=num.top();
                        num.pop();
                        num.push(prepre+" "+pre+" "+char_to_string(op.top()));
                        double prevalue=ans.top();
                        ans.pop();
                        double preprevalue=ans.top();
                        ans.pop();
                        ans.push(cal(preprevalue,prevalue,op.top()));
                        op.pop();
                    }
                    if(now!=')') op.push(now);
                }
            }
    }
    return num.top()+"\nresult="+double_to_string(ans.top());
}
int main()
{
    //freopen("ce.in","r",stdin);
    init();
    string s;
    cin>>s;
    string s_reverse;
    s_reverse=Reverse_Polish_double(s);
    cout<<s_reverse<<endl;
    return 0;
}
