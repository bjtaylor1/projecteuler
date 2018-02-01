N=50
tot=0
for c in [2,3,4]:
    e = [0]+[1]*(c)
    f = [0]*(c)+[1]
    for n in range(c+1,N+1):
        e.append(e[n-1]+f[n-1])
        f.append(e[n-c]+f[n-c])
    tot+=e[N]+f[N] - 1 #-1 is because there's one that's all empty that we are told to exclude
    

print(tot)    
    