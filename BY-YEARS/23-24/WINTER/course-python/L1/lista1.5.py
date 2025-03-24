#comone prefix hhee
from collections import Counter

def common_prefix(args):
    res = ""
    i = 0
    while True:
        tmp = []
        for x in args: 
            tmp.append(x[i].lower())

        values = list(Counter(tmp).values())
        keys = list(Counter(tmp).keys())
        
        if values == [] or values[0] < 3:
            return res
        else:
            res += keys[0]
            tmp2 = []    
            for y in args:
                if y[i].lower() == keys[0]:
                    tmp2.append(y)
            args = tmp2
            i += 1
        
        
print("LISTA 1 ZADANIE 5\n")
print("Common_prefix dla zadanej listy z tresci zadania:")
print("[Cyprian, cyberotoman, cynik, ceniąc, czule] \n")


lista_slow = ["Cyprian", "cyberotoman", "cynik", "ceniąc", "czule"]

print(common_prefix(lista_slow))