// zadanie 1 : fibonacci z memoizacją

function rec_Fibo(n) {
    if (n === 0 ) {
        return 0;
    }
    if (n === 1) {
        return 1;
    }
    return rec_Fibo(n - 1) + rec_Fibo(n - 2);
}

function memoize(fn) {
    var cache = {};
    return function xd(n) {

        if ( n in cache ) {
            return cache[n]         
        } else {             
            var result = rec_Fibo(n);             
            cache[n] = result;             
            return result;         
        }      
    } 
}






memofac2 = memoize(rec_Fibo);

// pierwsze wyliczenie – napełni cache 

console.time("memo-one-1");
console.log( memofac2(40) ); 
console.timeEnd("memo-one-1")

// drugie wyliczenie – z cache 

console.time("memo-one-2-39");
console.log( memofac2(39) ); 
console.timeEnd("memo-one-2-39")

console.time("memo-one-2-40");
console.log( memofac2(40) ); 
console.timeEnd("memo-one-2-40")
//-------------------------------

function memoize2() {
    var cache = [0, 1];
    return function xd(n) {

        if ( n in cache ) {
            return cache[n]         
        } else {             
            var result = xd(n - 1) + xd(n - 2);             
            cache[n] = result;             
            return result;         
        }      
    } 
}


memofac3 = memoize2();

// pierwsze wyliczenie – napełni cache 

console.time("memo-two-1");
console.log( memofac2(40) ); 
console.timeEnd("memo-two-1")

// drugie wyliczenie – z cache 

console.time("memo-two-2-39");
console.log( memofac2(39) ); 
console.timeEnd("memo-two-2-39")

console.time("memo-two-2-40");
console.log( memofac2(40) ); 
console.timeEnd("memo-two-2-40")

/*
Dobra komentarz do całości co zasadniczo się tutaj dzieje.
Zauważamy ,że w przypadku nie zmienonego "dobrze" kodu z wykładu to 
zostanie tylko zapisywana liczba która chcemy policzyć więc zysk z memoizacji
będzie tylko dla tej konkretnej liczby więc w przypadku fibo to nam mało pomaga

Stąd druga wersja zmodyfikowana:
Tutaj pozbywamy się funkcji rec_fibo bo samą rekurencję realizujemy w środku funkcji
"xd(n)". Tutaj rekurencyjnie zapisujemy już polczone klocki, więc jak w przykładzie wyliczamy dla 40
to 39 też będzie zapisana ;>.
*/
