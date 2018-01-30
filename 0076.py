n=100
w=[0]*(n+1)
w[0]=1
for i in range(1,n+1):
    for j in range(i,n+1):
        w[j]+=w[j-i]

print(w[n]-1)        
