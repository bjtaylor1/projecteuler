def S2(N):
    w = [0]*(N+1)
    w[0] = 1
    for i in range(1, N+1):
        w2 = w[:]
        for j in range(i, N+1):
            w2[j] += w[j - i]
        w = w2
    return w
        
print(S2(10))        

# def S(N):
    # w = [0]*(N+1); #w[n] = the number of ways of making n
    # max = 0
    # for i in range(1, N+1): #i is the next number in the subset
        # max += i #the max number we can make with numbers up to i
        # if max>=N:
            # max = N
            
        # for j in range(i, max+1):
            # w[j]+=1 # using i adds one more possibility to the ways of making j.
                
    # return w
        
# print(S(10))        