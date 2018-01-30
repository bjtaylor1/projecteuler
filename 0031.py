coins=[1,2,5,10,20,50,100,200]
def W(ci, n):
    if n==0:
        return 1
    
    if ci < 0 or n <= 0:
        return 0
        
    last=coins[ci]
    res=W(ci - 1, n) + W(ci, n-last) #ways excluding last, + ways including last
    return res
    
print(W(len(coins)-1,200))