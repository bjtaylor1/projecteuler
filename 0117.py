N=50
e=[0,1,1,2,4]
r=[0,0,1,1,2]
g=[0,0,0,1,1]
b=[0,0,0,0,1]

for n in range(5,N+1):
    e.append(e[n-1]+r[n-1]+g[n-1]+b[n-1])
    r.append(e[n-2]+r[n-2]+g[n-2]+b[n-2])
    g.append(e[n-3]+r[n-3]+g[n-3]+b[n-3])
    b.append(e[n-4]+r[n-4]+g[n-4]+b[n-4])
    
print(e[N]+r[N]+g[N]+b[N])