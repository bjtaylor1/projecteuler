import primes
import time
    
def test(n):
    starttime = time.time()
    result = primes.sum(n)
    took = time.time() - starttime
    print("sumprimes(" + str(n) + ") = " + str(result) + ", took " + str(took))
    
    

test(2*10**6)