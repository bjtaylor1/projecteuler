limit = 10**6
best = limit
m=10**6
w=[0]*(limit+1)
w[0]=1
best=limit
for i in range(1,limit+1):
    if(i%10==0):
        print(i)
    
    for j in range(i,limit+1):
        w[j]+=w[j-i]

    if(w[i]%m==0):
        print("w[" + str(i) + "]=" + str(w[i]))
        break
    
