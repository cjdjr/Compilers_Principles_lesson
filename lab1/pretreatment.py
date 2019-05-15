
def remove_space(s):
    '''
    将字符串s中多余的空格缩成1个
    '''
    n=len(s)
    ans=""
    flag=0
    for i in range(n):
        if s[i]=='"':
            flag=1-flag
            ans+='""'
        elif s[i]==' ' and flag==0 and i>0 and s[i-1]==' ':
            pass
        else:
            ans+=str(s[i])
            
    return ans

if __name__=="__main__":

    s=""
    
    with open('in.txt', 'r', encoding='utf-8') as f:
        s=f.readlines()

    ans=[]
    for now in s:
        tmp=now
        tmp.replace('\n','')        #去回车
        tmp=tmp[0:tmp.find('//')]   #去注释
        tmp=tmp.replace('\t',' ')   #去tab
        tmp=remove_space(tmp)       #去空格
        tmp=tmp.strip()             #去两边空白字符
        if(tmp!=''):
            ans.append(tmp)
        
    with open('out.txt', 'w', encoding='utf-8') as f:
        f.writelines(ans)
