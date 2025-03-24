# drzewa tri
# dobra nie trzeba drzew tri wystarczy dp i powinno być git
import gzip


def Z2(t, S):
    # dp[i] - max sumy kwadratow na przedziale 0, ..., i
    lt = len(t)
    dp = [-1] * (lt + 1)  # nasz dp
    backtracker = [-1] * (lt + 1)  # do odwtworzenia wyniku
    dp[0] = 0

    for i in range(1, lt + 1):
        for j in range(0, i):
            word = t[j:i]
            if word in S and dp[j] != -1:
                contender = dp[j] + (i - j) ** 2
                if contender > dp[i]:
                    dp[i] = contender
                    backtracker[i] = j

    # jak przeszlismy przez calosc i na koncu dalej jest -1 no to ewidentnie cos poszlo nie tak
    if dp[lt] == -1:
        return None

    res = []
    i = lt
    while i > 0:
        j = backtracker[i]
        res.append(t[j:i])
        i = j
    return " ".join(res[::-1])


# tylko dla sprawdzaczki
def load_words(file):
    with gzip.open(file, "rt", encoding="utf-8") as f:
        return set(line.strip() for line in f)


words = load_words("zad2_words.txt.gz")
with open("zad2_input.txt", "r", encoding="utf-8") as fin, open(
    "zad2_output.txt", "w", encoding="utf-8"
) as fout:
    for line in fin:
        line = line.strip()
        fout.write(Z2(line, words) + "\n")
