#include<vector>
using namespace std;
#ifndef _RE_
#define _RE_
#define mp make_pair
namespace RE
{
    bool check_atom(string ans)
    {
        int s=0;
        int now=0;
        for(auto x:ans)
            if(x=='(') ++now;
            else
                if(x==')') --now;
                else
                    if(x=='|'&&now==0) ++s;
        return s==0;
    }

    string strip(string s)
    {
        while(true)
        {
            bool flag=0;
            int now=0;
            int pos=-1;
            for(int i=0;i<s.length();++i)
            {
                char x=s[i];
                if(x=='(') ++now;
                else if(x==')') --now;
                if(now==0)
                {
                    pos=i;
                    break;
                }
            }
            if(pos==s.length()) s=s.substr(1,int(s.length())-2);
            else break;
        }
        return s;
    }

    vector<string> split_or(string s)
    {
        s=strip(s);
        vector<string> ans;
        ans.clear();
        int n=s.length();
        int now=0;
        int last=-1;
        for(int i=0;i<n;++i)
            if(s[i]=='(') ++now;
            else if(s[i]==')') --now;
            else if(s[i]=='|'&&now==0)
            {
                ans.push_back(strip(s.substr(last+1,i-1-last)));
                last=i;
            }
        ans.push_back(strip(s.substr(last+1,n-1-last)));
        return ans;
    }

    pair<string,string> split_atom(string s)
    {
        s=strip(s);
        int n=s.length();
        if(s[0]!='(') return mp(s.substr(0,1),s.substr(1,n-1));
        int now=0;
        for(int i=0;i<n;++i)
        {
            if(s[i]=='(') ++now;
            else if(s[i]==')') --now;
            if(now==0)
            {
                int pos=i;
                if(i<n-1&&s[i+1]=='*') ++pos;
                return mp(s.substr(0,pos+1),s.substr(pos+1,n-1-pos));
            }
        }

    }
}

#endif // _RE_
