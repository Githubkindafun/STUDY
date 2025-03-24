import re
import random

"""
Krótki opis tego co robie:
    Funkcja uprosc_zdanie():
      (1) pozbywa się "przeszkadzających" znaków
      (2) usówa "za długie" słowa
      (3) jeżeli jest więcej słów niż podany max losowo usówa aż zejdzie poniżej max'a 
      (4) funkcja wywołana na tekscie "literackim"
"""


#text - tekst; word_l - max długość słowa; word_a - max ilość słów
def uprosc_zdanie(text, word_l, word_a):
    text = re.sub("[',!.?()-;\ufeff]","" , text).split() #(1)
    #
    for word in text[:]: # (2) [:] - kopia textu aby remove się nie wywalał
        if len(word) > word_l:  text.remove(word)       
    #
    if len(text) > word_a: # (3)
        while len(text) > word_a:
            text.remove(random.choice(text))
    return text

#t = "a a a a a a a a" - 8 a
#print(uprosc_zdanie(t, 7, 7))
#zwraca "a a a a a a a" - 7 a

with open("The Categories by Aristotle.txt", "r", encoding="utf-8") as f:
    text = f.read()
    print(uprosc_zdanie(text, 6, 42))
    