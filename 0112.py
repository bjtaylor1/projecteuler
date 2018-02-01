dsc=[[0]+[1]*9]
asc=dsc[:]
N=2
prop=0

def sign(d):
    if d>0:
        return 1
    elif d==0:
        return 0
    else:
        return -1
        
def isB(b):
    s = [int(c) for c in str(b)]
    sn=[sign(s[i]-s[i-1]) for i in range(1,len(s))]
    return 1 in sn and -1 in sn
    
def NB(p):
    dsc=[[0]+[1]*9]
    asc=dsc[:]
    for n in range(1,p):
        asc.append([sum(asc[n-1][0:i+1]) for i in range(0,10)])
        dsc.append([sum(dsc[n-1][i:10]) for i in range(0,10)])
    return sum(sum(a) - 9 for a in asc) + sum(sum(d) for d in dsc)

def pNB():
    prop = 0
    p=1
    while(True):
        nb=NB(p)
        tot=(10**p)-1
        prop = nb/tot
        if(prop <0.01):
            return p-1
        else:
            p += 1
         
pnb=pNB()         
totnb=NB(pnb)
total=(10**pnb)-1
for ans in range(10**pnb, 10**(pnb+1)):
    total+=1
    if(not isB(ans)):
        totnb+=1
    if(totnb*100==total):
        print(ans)
        break
 
