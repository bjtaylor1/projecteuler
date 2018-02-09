import primes



def IsPrimeProof(p):
    ds=[int(c) for c in str(p)]
    for i in range(0,len(ds)):
        da=ds[:]
        for d in range(1 if i==0 else 0,10):
            if(da[i]!=d):
                da[i]=d
                pc=int("".join(str(c) for c in da))
                if(pc in ps):
                    return False
    return True
        
ps = primes.values((10**5))
sq = sorted(set((p**2)*(q**3) for p in ps for q in ps if p != q))
found=0
for i in sq:
    if("200" in str(i) and IsPrimeProof(i)):
        found += 1
        print(str(found) + ": " + str(i))
        if(found == 200):
            break