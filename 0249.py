import primes
ps = primes.values(5000)
size=sum(ps)
allps=primes.values(size)
w = [0]*(size+1)
w[0]=1
max=2
for p in allps:
    print(p)
    for i in reversed(range(p, min(max,size)+1)):
        w[i]+=w[i-p]
    max += p

ans=sum(w[p] for p in allps)
print(ans)
