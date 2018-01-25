import primes
import math

contingency=10 

def Tri(n): #nth triangular number
    return n*(n+1)/2

def M(lbexc, ubinc, n): #sum of kn where kn>lbexc and kn<=ubinc for any int k
    lbm=lbexc//n
    ubm=ubinc//n
    return int(n * (Tri(ubm)-Tri(lbm)))
    
def S(n):
    r = int(n**0.5)
    ps = primes.values(r+contingency)
    tot=0
    for lbr in range(2, r+1):
        lbexc=lbr*lbr
        ubinc = (lbr+1)*(lbr+1) - 1
        if ubinc > n:
            ubinc = n
            
        lps = max(filter(lambda p: p <= lbr, ps))
        ups = min(filter(lambda p: p >= lbr + 1, ps))
        sumlps = M(lbexc, ubinc, lps)
        sumups = M(lbexc, ubinc, ups)
        sumboth = M(lbexc, ubinc, lps*ups)
        tot += sumlps + sumups - 2*sumboth
    return tot
    
print(S(999966663333))