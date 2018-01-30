coins=[1,2,5,10,20,50,100,200]
def W(c, n):
    if n==0:
        return 1
    
    if len(c)==0 or n <= 0:
        return 0
        
    cw=c[:]
    last=cw.pop()
    res=W(cw, n) + W(c,n-last) #ways excluding last, + ways including last
    return res
    
print(W(coins,200))