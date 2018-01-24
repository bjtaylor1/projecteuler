from datetime import datetime
import primes

cache = dict()

def Cp(n,p,prod,primes):
    if p in cache:
        print("cache hit!")
        return cache[p]
    
    cp = 0
    subt=0
    if prod <= n:
        cp = n//prod
        lowerprimes = list(filter(lambda op : op < p, primes))
        if lowerprimes:
            subtractions = {po: Cp(n,po,prod*po,lowerprimes) for po in lowerprimes}
            subt += sum(subtractions.itervalues())
    cache[p]=cp
    return cp-subt

def S(n):
    r = int(n**0.5)
    primestoroot = list(primes.values(r))
    print(str(datetime.now()) + " made primestoroot")
    tot = 0
    tot += sum(p * Cp(n,p,p,primestoroot) for p in primestoroot )
    print(str(datetime.now()) + " summed up primestoroot's smpf")
    tot += primes.sum(n)
    print("Summed up higher primes " + str(datetime.now()))
    tot -=sum(primestoroot)
    return tot

print(S(100))
#print(S(10**12)%(10**9))
