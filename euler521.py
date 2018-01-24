import primes
from operator import mul

def Cp(n,p,prod,primes):
    cp = n//prod
    lowerprimes = list(filter(lambda op : op < p, primes))
    if lowerprimes:
        subtractions = {po: Cp(n,po,prod*po,lowerprimes) for po in lowerprimes}
        cp -= sum(subtractions.itervalues())
    return cp

def S(n):
    r = int(n**0.5)
    primestoroot = list(primes.values(r))
    tot = sum(p * Cp(n,p,p,primestoroot) for p in primestoroot ) \
        + primes.sum(n) - primes.sum(r)
    
    return tot

print(S(10**12)%(10**9))
