m=50
wr=[0]*m+[1]
wb=[0]+[1]*m
n=m
while(True):
    n+=1
    wb.append(wb[n-1]+wr[n-1])
    wr.append((wb[n-m] if n >= m else 0) +wr[n-1])
        
    if(wr[n] + wb[n] > 10**6):
        print(n)
        break
        
    