#zadanie is palindrom
#33,34,39,40,41,46,58,59,63,91,93,123,125
sgn = ["!", "?", ".", ",", ";", ":", "(", ")", "-", " "]    

def IsPalindrome(arg):
    i = -1
    arg = arg.lower()
    for a in arg:
        if a in sgn:
            continue

        while arg[i] in sgn:
            i -= 1
            

        if a != arg[i]:
            print("nie jest to palindrom")
            return False
        i -= 1  
    print("to jest palindrom")
    return True 

text = "kajak"
text1 = "Eine güldne, gute Tugend: Lüge nie!"
text2 = "Míč omočím."
text3 = "To nie jest palindrom"

print("LISTA 1 ZADANIE 2\n")
print("PALINDROMY\n")
print("\n")
print("ODPOWIEDZI DLA PRZYKŁADÓW: \n")

print("kajak")
IsPalindrome(text)
print("\n")

print("Eine güldne, gute Tugend: Lüge nie!")
IsPalindrome(text1)
print("\n")

print("Míč omočím.")
IsPalindrome(text2)
print("\n")

print("To nie jest palindrom")
IsPalindrome(text3)
print("\n")
