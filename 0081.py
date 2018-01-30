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
mins=[[0]*n]*n
for y in range(0,n):
    for x in range(0,n):
        if x==0 and y==0:
            mins[y][x] = matrix[y][x]
        elif x==0:
            mins[y][x]=mins[y-1][x]+matrix[y][x]
        elif y==0:
            mins[y][x]=mins[y][x-1]+matrix[y][x]
        else:
            mins[y][x]=min(mins[y-1][x]+matrix[y][x], mins[y][x-1]+matrix[y][x])

print(mins[-1][-1])            