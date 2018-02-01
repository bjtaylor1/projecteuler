def S_asc(n, last):
    if(n==1):
        res = 1 if last >= 1 else 0
    else:
        res=0
        for i in range(0,last+1):
            res += S_asc(n-1, i)
        
    return res
    
def S_desc(n, last):
    if(n==1):
        res = 1 if last >= 1 else 0
    else:
        res=0
        for i in range(last,10):
            res += S_desc(n-1, i)
        
    return res    
    
    
def NB(n):
    print(n)
    if n==1:
        res = 9
    else:
        res = -9 #as we've counted the ones below 10 twice
        res += sum(S_asc(n,i) for i in range(0,10))
        res += sum(S_desc(n,i) for i in range(0,10))
    
    return res
    
def NBTot(n):
    res = sum(NB(i) for i in range(1,n+1))
    return res
    
# print(NBTot(1))
# print(NBTot(2))
# print(NBTot(6))
print(NBTot(100))