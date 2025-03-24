import math
import re
from collections import Counter

# języki
# stats - wylicza statystyki dla 3 języków co do występowania liter w tekstach w poszczególnych językach
# f2 - na podstawie danej statyski stwierdza jaki to język

def stats_helper(dict_x):
    all = sum(dict_x.values())
    for x in dict_x:
        dict_x[x] = 100 * (float(dict_x[x]) / float(all))
    return dict_x

def stats(eng, fi, de):
    #
    res_e = stats_helper(Counter(re.findall("\w" , eng.lower())))
    # najpierw zliczam wszystkie litery i potem wyliczam ile ich jest w całości tekstu
    res_f = stats_helper(Counter(re.findall("\w" , fi.lower())))
    res_d = stats_helper(Counter(re.findall("\w" , de.lower())))
    
    
    # i teraz tak, dla własnej czytelności zostawiłem to w 4 linijkach nie w jednej
    #return dict(res_e), dict(res_f), dict(res_d)
    return res_e, res_f, res_d

def stats_for_1(text): 
    return stats_helper(Counter(re.findall("\w" , text.lower())))

def what_language_it_is(text, stats):
    #
    text_stats = stats_for_1(text)

    res = [0,0,0] # 0 - eng, 1 - de, 2 - fin
    for i in range(97, 123): # tutaj sprawdzam dla [a, z) 25 % 3 = 1 
        letter = chr(i) # literka
        for i in range(0,3):
            res[i] += pow((text_stats[letter] - stats[i][letter]), 2)
    
    for i in range(0, 3):
        res[i] = math.sqrt(res[i])
    
    i = res.index(min(res))
    if i == 0: 
        return "Angielski"
    elif i == 1: 
        return "Niemiecki" 
    elif i == 2:
        return "Finski"
                
                    








# dane
with open("boskaEng.txt", "r", encoding="utf-8") as f1:
    eng = f1.read()
with open("d_eng.txt", "r", encoding="utf-8") as f11:
    eng2 = f11.read()
    eng += eng2

with open("boskaDe.txt", "r", encoding="utf-8") as f2:
    d = f2.read()
    de = re.sub("[_\ufeff\xe8\xef\xe0]" ,"" , d)
with open("d_de.txt", "r", encoding="utf-8") as f21:
    d2 = f21.read()
    de2 = re.sub("[_\ufeff\xe8\xef\xe0]" ,"" , d2)
    de += de2

with open("boskaFi.txt", "r", encoding="utf-8") as f3:
    f = f3.read()
    fi = re.sub("[_\ufeff\xe8\xef\xe0\xf2]" ,"" , f)
with open("d_fi.txt", "r", encoding="utf-8") as f31:
    fi2 = f31.read()
    fin2 = re.sub("[_\ufeff\xe8\xef\xe0\xf2]" ,"" , fi2)
    fi += fin2

res_eng, res_de, res_fi = stats(eng, fi, de)
statics = [res_eng, res_de, res_fi]

# sekcja testów


#po angielsku:
V_eng = "Beneath this mask, there is more than just flesh, Mr. Creedy. Beneath this mask, there is an idea. And ideas are bulletproof."
eng_res = what_language_it_is(V_eng, statics)
print(V_eng + "\n" + "jezyk to " + eng_res)
print("czy kod poprawnie wskazal?: " + str(eng_res == "Angielski"))
#po niemiecku:
V_de = "Unter dieser Maske steckt mehr als nur Fleisch, Mr. Creedy. Hinter dieser Maske verbirgt sich eine Idee. Und Ideen sind kugelsicher."
de_res = what_language_it_is(V_de, statics)
print(V_de + "\n" + "jezyk to " + de_res)
print("czy kod poprawnie wskazal?: " + str(de_res == "Niemiecki"))
#po fińsku:
V_fi = "Tämän naamion alla on muutakin kuin pelkkää lihaa, herra Creedy. Tämän naamion alla on ajatus. Ja ideat ovat luodinkestäviä."
fi_res = what_language_it_is(V_fi, statics)
print(V_fi + "\n" + "jezyk to " + fi_res)
print("czy kod poprawnie wskazal?: " + str(fi_res == "Finski"))

#cytat pochodzi z filmu "V for Vendetta" - 2005