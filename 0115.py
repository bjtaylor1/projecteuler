m=3
wr=[0,0,0,1]
wb=[0,1,1,1]
n=3
while(True):
    n+=1
    wb.append(wb[n-1]+wr[n-1])
    wr.append((wb[n-m] if n >= m else 0) +wr[n-1])
    if(n==29):
        print(wr[n]+wb[n])
    if(n==30):
        print(wr[n]+wb[n])
        break
        
    if(wr[n] + wb[n] > 10**6):
        print(n)
        break
        
    