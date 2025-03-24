#zadanie z VAT
from decimal import *

def vat_faktura(args):
    res = 0
    for x in args:
        res += x
    res *= 0.23
    return res

def vat_paragon(args):
    res = 0
    for x in args:
        res += x * 0.23
    return res

print("LISTA 1 ZADANIE 1\nVAT FAKTURA I VAT PARAGON\n")
print("LISTA ZAKUPOW: [23.23, 42.42, 1523.10, 21.42]\n")
print("CZY VAT WYNOSI TYLE SAMO UZYWAJAC FLOATOW:")



zakupy = [23.23, 42.42, 1523.10, 21.42]
print(vat_faktura(zakupy) == vat_paragon(zakupy))

#koniec pierwszej części zadania pora na eksperyment
#odzielam to na osobną część ponieważ mnożenie float i decimal wywala Type errora


def decimalvat(args):
    res = []
    for x in args:
        x = Decimal(x)
        res.append(x)
    return res

def vat_fakturaD(args):
    res = 0
    for x in args:
        res += x
    res *= Decimal(0.23)
    return res

def vat_paragonD(args):
    res = 0
    for x in args:
        res += x * Decimal(0.23)
    return res

print("TERAZ ODPOWIEDZ NA PYTANIE CZY VAT WYNOSI TYLE SAMO GDY LICZBY BĘDA DECIMAL:")
#zakupy after decimal
new_zakupy = decimalvat(zakupy)
print(vat_fakturaD(new_zakupy) == vat_paragonD(new_zakupy))