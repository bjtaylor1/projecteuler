ss=list(range(1,21))+[25]
ds=[i*2 for i in ss]
all=sorted(ss+ds)
#print(all)
def W(n, d, si, di):
    if(d<=0 or n <= 1 or si < 0 or di < 0):
        res=0
    elif(d==1):
        res = 1 if n in ds else 0
    elif(d==2):
        res=1
    else: 
        wis=W(n-ss[si], d-1,     si, di)
        wos=W(n,        d,   si-1, di)
        wid=W(n-ds[di], d-1,     si, di)
        wod=W(n,        d,   si, di-1)
        res=wis+wos+wid+wod
    
    #if(res>0):
        #print("W(" + str(n) + ") w " + str(d) + " = " + str(res) + "  " + str(locals()))
    # print()
    return res
    
print(W(3, 3, len(ss)-1, len(ds)-1))