# matrix=[
# [131,673,234,103,18],
# [201,96,342,965,150],
# [630,803,746,422,111],
# [537,699,497,121,956],
# [805,732,524,37,331]
# ]
matrix=list()
file=open("p081_matrix.txt", "r")
for line in file:
    matrix.append([int(n) for n in line.split(",")])
    
n=len(matrix)
mins=[[0]*n for i in range(0,n)]
for y in range(0,n):
    mins[y][0]=matrix[y][0]
    
for x in range(1,n):    
    for y in range(0,n):
        moves=[m for m in [(x-1,y), (x,y-1)] if m[1]>=0 and m[1]<n]
        mins[y][x]=matrix[y][x] + min(mins[m[1]][m[0]] for m in moves)            

    for y in reversed(range(0,n-1)):
        mins[y][x]=min(mins[y][x], mins[y+1][x] + matrix[y][x])
    
print(min(m[-1] for m in mins)) 