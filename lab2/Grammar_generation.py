import copy

g={}
base=""
path=""
limit=0

def dfs(s,dep,sub_path):

    if s==base:
        global path                     # 修改全局变量要global声明
        path=copy.deepcopy(sub_path)
        return True

    if len(s)>len(base):
        return False

    for V_T in g.keys():
        if s.find(V_T)!=-1:
            for each in g[V_T]:
                news=s.replace(V_T,each,len(each))
                if dfs(news,dep+1,sub_path+"==>"+news)==True:
                    return True
                
    return False

if __name__=="__main__":

    

    g["S"]=["A","S+A"]
    g["A"]=["B","A*B"]
    g["B"]=["(S)","i"]
    
    while True:
        base=input()

        flag=dfs("S",0,"S")

        if flag==True:
            print(path)
        else:
            print("此表达式不是该文法的句型！")
    
