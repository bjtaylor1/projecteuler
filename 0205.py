def roll(numFaces, diceLeft, total, totals):
    if(diceLeft == 0):
        totals[total] = (totals[total] if total in totals else 0) + 1
        return
    
    for n in range(1,numFaces+1):
        roll(numFaces, diceLeft - 1, total + n, totals)
      
pete = {}
colin = {}

roll(4, 9, 0, pete)
roll(6, 6, 0, colin)

peteWins = 0
total = 0
for peteScore in pete:
    for colinScore in colin:
        occurrence = colin[colinScore] * pete[peteScore]
        total += occurrence
        if peteScore > colinScore:
            peteWins += occurrence
        

print(peteWins)
print(total)
print(round(peteWins/total,7))

