# max sublist 
# from collections import deque # tutaj  deque srednio mi się przyda

# 0 <= i <= j < len(lista)
# in list of numbers 
# out (i,j)

# version 1 using sliding window approach
def max_sublist_sum(list):
    if list == []: return ()
    
    res = float("-inf") #-inf
    res_id = (0,0) 
    L = len(list)
    
    for i in range(L):
        curr = 0
        
        for j in range(i + 1, L + 1):
            curr = sum(list[i : j])

            if curr > res:
                res_id = (i,j-1)
                res = curr

    return res_id

# version 2 
# basicly verion 1 converted to list comprehension
def max_sublist_sum2(list):
    return max([(sum(list[i:j]), (i, j - 1)) for i in range(len(list)) for j in range(i + 1, len(list) + 1)])[1] if list else ()

t = [
    [],
    [-15,123,-111,432],
    [1,2,3,4,-10],
    [42, 24, -24, -42],
    [-15, -2, -1, -4],
    [42,12,32,42,1234],
    [-1,1,-1,1,-1,1] 
    ]

for i in t:
    print("for: " + str(i))
    print("v1.: " + str(max_sublist_sum(i)))
    print("v2.: " + str(max_sublist_sum2(i)))
    print("\n")
        




