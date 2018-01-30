import math

def values(x):
    result=[False,True,True]+[True,False]*((x+1)//2)
    for i in range(3, int(math.ceil(x**0.5)), 2):
        if result[i]:
            for pr in range(i*2, x+1, i):
                result[pr] = False

    return list(filter(lambda j: result[j], range(2,x+1)))
     
    
    
