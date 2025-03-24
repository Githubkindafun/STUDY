#"kompresja"
# [(1, ’s’), (4, ’u’), (1, ’p’), (1, ’e’), (1, ’r’)]
# kompresja(tekst) i dekompresja(tekst_skompresowany)

def kompresja(text):
    if len(text) == 1: return [1, text]
    else:
        res = []
        last = None
        cnt = 1
        for letter in text:
            if letter == last:
                cnt += 1
            else:
                res.append((cnt, letter))
                last = letter
                cnt = 1
        return res 

def dekompresja(list):
    res = ""
    for tuple in list:
        res += tuple[0] * tuple[1]
    return res

compressed = kompresja("Ala, ma ?kota")
print(compressed)
decompressed = dekompresja(compressed)
print(decompressed)

# zatem wiele tekstów mi nie wchodziło spowodu jakiś dziwnych errorów encode
# więc końcowo udało mi się znaleźć tekst bez znaków które były problematyczne 
# i staneło na Arystotelesie : https://www.gutenberg.org/ebooks/2412

with open("The Categories by Aristotle.txt", "r", encoding="utf-8") as f:
    text = f.read()
    compressed = kompresja(text)
    print(compressed)
    #decompressed = dekompresja(compressed)
    #print(decompressed)

