import math
import random
import time 

def zsn(X, m, n): 
    for i in range(n):
        sumCol = 0
        for j in range(m):
            sumCol += X[j][i]
        mean = sumCol / m

        variance = 0
        for j in range(m):
            variance += (X[j][i] - mean) ** 2
        stDev = math.sqrt(variance / m)

        if stDev != 0:
            stDevInv = 1/stDev
            for j in range(m):
                X[j][i] = (X[j][i] - mean) * stDevInv
        else:
            for j in range(m):
                X[j][i] = 0

    return X

def main():
    userInput = input("Enter n: ")
    try:
        n = int(userInput)
        if n <= 0:
            print("Please enter a positive integer.")
            return
    except ValueError:
        print("Invalid input!!")
        return
    
    #create nxn matrix with random integers
    X = []
    for i in range(n):
        row = []
        for j in range(n):
            row.append(random.randint(1, 9))
        X.append(row)
    # print("Original Matrix:")
    # for row in X:
    #     print(row)

    before = time.time()
    zsn(X, n, n)
    after = time.time()

    # print("\nZ-Score Normalized Matrix:")
    # for row in X:
    #     print(row)
    elapsed = after - before

    print(f"\nTime taken: {elapsed:.6f} seconds")
    
main()