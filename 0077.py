import primes
limit=10**6
ps = primes.values(limit)


def S(n):
    best=limit
    w=[0]*(limit+1)
    w[0]=1
    b=0
    #max = 2
    for p in ps:
        for j in range(p, limit+1):
            w[j]+=w[j-p]
            if w[j] > n and j < best:
                best = j
        #print(locals())     
        if p > best:
            break
        #max += p
        
    return best

print(S(5000))        
