import math
best = math.inf
m=10**6
limit=10
for limit in range(100,10**6,100):
    print(limit)
    limit += 100
    w=[0]*(limit+1)
    w[0]=1
    for i in range(1,limit+1):
        for j in range(i,limit+1):
            w[j]+=w[j-i]

        if(w[i]%m==0):
            best = i
            print("w[" + str(i) + "]=" + str(w[i]))
        
    
