N=50
wr=[0]*(N+1)
wb=[0]*(N+1)
wb[1]=1
wb[2]=1
wb[3]=1
wr[3]=1
for n in range(4,N+1):
    wb[n]=wb[n-1]+1
    wr[n]=wr[n-1]+1+wb[n-4]+1
    
ans=wr[N]+wb[N]
print(ans)
    