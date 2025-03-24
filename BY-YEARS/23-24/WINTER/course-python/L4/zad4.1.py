import timeit as T
# szukamy liczb pierwszych w przedziale [1, n]
# 3 wersje
# (1) imperatywna / (2) skladana / (3) funkcyjna 

def prime_tab(n): # przyda sie do sita
    return [True for b in range(n + 1)]


def prime_imp(n): # (1)
    #if n < 2: return []
    #primes = prime_tab(n) nie jestem pewien czy moge tego w tej wersji uzyc
    primes = []
    for i in range(0, n + 1):
        primes.append(True)
    it = 2
    while (it * it <= n):
        if (primes[it]): 
            for i in range(it * it, n + 1, it): 
                primes[i] = False
        it += 1
    #return [i for i in range(2, n + 1) if primes[i]]
    res = []
    for num in range(2, n + 1):
        if primes[num]: res.append(num)
    return res



def prime_lc(n): # (2) to do with erasthotenes
    if n < 2: return []
    
    it = [i for i in range(2, n + 1) if i * i <= n ]
    #print(it ,"it") # tu dostaje inedksy z while 
    tmp = [j for i in it for j in range(i * i, n + 1, i)] 
    #to nie jest wersja z erastotenesem perse ale based on
    #
    return [x for x in range(2, n + 1) if x not in tmp]
    



def prime_fun(n): # (3) 
    if n < 2: return []
    primes = [*range(2, n+1, 1)]

    def IsPF(num, q=2):
        if num == q: return True
        elif num % q == 0: return False
        return IsPF(num, q + 1)
    
    res = filter(IsPF, primes)
    return list(res)
    

def time_measure(fun, n):
    timer = T.Timer(lambda: fun(n))
    elapsed_time = timer.timeit(number=1)
    return elapsed_time

print(" n  imp       lc        fun")
for i in range(10, 91, 10):
    print(f"{i}: {time_measure(prime_imp, i):.7f} {time_measure(prime_lc, i):.7f} {time_measure(prime_fun, i):.7f}")

print("\n")
for i in range(10, 91, 10):
    print(prime_imp(i))
    print(prime_lc(i))
    print(prime_fun(i))
    print("\n")

