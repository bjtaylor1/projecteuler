from math import floor, sqrt
import time

start = time.time()


# compute the Fibonacci numbers                                                 
F = [0,1,1]
while len(F) < 25:
    F.append(F[-1]+F[-2])


def eratosthenes(n):
    """Uses the sieve of Eratosthenes to compute all primes not exceeding n"""
    is_prime = bytearray(b'\x01'*((n-1)//2))
    for p in range(3,int(floor(sqrt(n)))+1,2):
        if is_prime[(p-3)//2]:
            is_prime[(p**2-3)//2::p] = b'\x00'*((n-p**2)//(2*p)+1)
    return [2]+[2*i+3 for i in range((n-1)//2) if is_prime[i]]


# find the primes that are no larger than the 24th Fibonacci number             
primes = eratosthenes(F[-1])


mod = 10**9

def S(k):
    d = [1]+[0]*k
    for p in primes:
        if p > k:
            break
            
        for i in range(p,k+1):
            d[i] = (d[i]+d[i-p]*p) % mod
    return d


d = S(F[-1])

print(sum([d[F[i]] for i in range(2,25)]) % mod)
print("time = "+str(time.time()-start))