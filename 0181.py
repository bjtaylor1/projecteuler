MAX_B, MAX_W = 1,1

cache = {}
def dp(cb, cw, mb, mw):
    if (cb, cw, mb, mw) not in cache:
        print("Calculating ways of making " + str((cw, cb)) + " with first group of " + str((mw, mb)))
        t = 0
        
        if mb == 0 and mw == 0:
            t = 1 if (cb == 0 and cw == 0) else 0
            
        else:
            tb, tw = mb, mw
            while tb >= 0 and tw >= 0:
                if cb-tb >= 0 and cw-tw >= 0:
                    t += dp(cb-tb, cw-tw, tb, tw)
                tb, tw = (tb, tw-1) if tw-1 >= 0 else (tb-1, MAX_W)
                
        print("Ways of making " + str((cw, cb)) + " with first group of " + str((mw, mb)) + " = " + str(t))
        cache[(cb, cw, mb, mw)] = t
    return cache[(cb, cw, mb, mw)]

print(dp(1,1,1,1))
    
# for b in range(1,6):
    # for w in range(b,6):
        # MAX_B, MAX_W = b,w
        # print(str(b) + ", " + str(w) + " = " + str(dp(MAX_B , MAX_W, MAX_B, MAX_W)))