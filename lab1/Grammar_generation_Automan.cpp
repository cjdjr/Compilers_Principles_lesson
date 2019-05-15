#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5;
int ch[maxn+50][10];
int sz=0,root=0;
int danger[maxn+50];
int nx[maxn+50];
int last[maxn+50];
queue<int> q;
bool vis[maxn+50];
vector<string> G[10];
int index[128];
pair<char,int> value[maxn+5];
void buildtrie(string s,char id,int num)
{
    int u=root;
    int len=s.length();
    for(int i=0;i<len;++i)
    {
        int id=index[s[i]];
        if(!ch[u][id]) ch[u][id]=++sz;
        u=ch[u][id];
    }
    danger[u]=1;
    value[u]=make_pair(id,num);
}
void buildfail()
{
    while(!q.empty()) q.pop();
    for(int i=0;i<10;++i) if(ch[root][i]) q.push(ch[root][i]),nx[ch[root][i]]=root,last[ch[root][i]]=0;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=0;i<10;++i)
            if(ch[u][i])
            {
                int k=nx[u];
                while(!ch[k][i]&&k) k=nx[k];
                nx[ch[u][i]]=ch[k][i];
                danger[ch[u][i]]|=danger[ch[k][i]];
                if(danger[ch[k][i]]) last[ch[u][i]]=ch[k][i];else last[ch[u][i]]=last[ch[k][i]];
                q.push(ch[u][i]);
            }
            else ch[u][i]=u==0?0:ch[nx[u]][i];
    }
}
string query(string s)
{
    int len=s.length();
    int u=root;
    int pos=-1;
    for(int i=0;i<len;++i)
    {
        int id=index[s[i]];
        u=ch[u][id];
        //cout<<u<<endl;
        if(danger[u])
        {
            if(value[u].first==0) u=last[u];
            pos=i;
            break;
        }
    }
    if(pos==-1) return "!";
    string tmp=G[index[value[u].first]][value[u].second];
    string T=s.replace(pos-tmp.length()+1,tmp.length(),string(1,value[u].first));
    return T;
}
void init()
{
    for(int i=0;i<=sz;++i) memset(ch[i],0,sizeof(ch[i]));
    for(int i=0;i<=sz;++i) danger[i]=0;
    for(int i=0;i<=sz;++i) last[i]=0;
    for(int i=0;i<=sz;++i) nx[i]=0;
    for(int i=0;i<=sz;++i) vis[i]=0;
    sz=0;
    index['S']=0;
    index['A']=1;
    index['B']=2;
    index['+']=3;
    index['*']=4;
    index['(']=5;
    index[')']=6;
    index['i']=7;
    G[index['S']]=vector<string>{"A","S+A"};
    G[index['A']]=vector<string>{"B","A*B"};
    G[index['B']]=vector<string>{"(S)","i"};
}
int main()
{
    init();
    buildtrie("A",'S',0);
    buildtrie("S+A",'S',1);
    buildtrie("B",'A',0);
    buildtrie("A*B",'A',1);
    buildtrie("(S)",'B',0);
    buildtrie("i",'B',1);

    buildfail();
    //printf("%d\n",danger[ch[root][index['i']]]);
    string s;
    while(cin>>s)
    {
        vector<string> ans;
        ans.clear();
        ans.push_back(s);
        //cout<<"duru"<<s<<endl;
        cout<<"ce "<<query(s)<<endl;

        /*while(s!="S"&&s!="!")
        {
            s=query(s);
            ans.push_back(s);
        }*/
        if(s=="!") puts("Error!");
        else
        {
            reverse(ans.begin(),ans.end());
            for(auto x:ans) cout<<x<<"-->";
            cout<<endl;
        }
    }
    return 0;
}
