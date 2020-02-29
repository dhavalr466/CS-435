#problem 3

from random import sample

def getRandomArray(i):
    j = range(1, 100)
    numArr = sample(j, i)
    return numArr

print(getRandomArray(9))

def getSortedArray(i):
    numArr = []
    for j in range(i, 0, -1):
        numArr.append(j)
    return numArr

print(getSortedArray(9))
