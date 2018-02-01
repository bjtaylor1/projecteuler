dsc=[[0]+[1]*9]
asc=dsc[:]
for n in range(1,100):
    asc.append([sum(asc[n-1][0:i+1]) for i in range(0,10)])
    dsc.append([sum(dsc[n-1][i:10]) for i in range(0,10)])
print(sum(sum(a) - 9 for a in asc) + sum(sum(d) for d in dsc))
