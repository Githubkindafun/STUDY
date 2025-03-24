// zadanie 3 : var vs let

// function createFs(n) { 
//     // tworzy tablicę n funkcji 
//     var fs = []; 
//     // i-ta funkcja z tablicy ma zwrócić i 
//     for ( var i = 0; i < n; i++ ) { 
//         fs[i] = function() { 
//             return i; 
//         }; 
//     }; 
//     return fs; 
// }

/*
Myk jest taki, var na samym szczycie domknięcia (początek -- koniec funckji) czyli jak będziemy chcieli dostać odpowiedź
myfs[0]() to będziemy "callowac" function() ktore return i; i w tym kontekście będzie to "pointer" na tą wartość var i; które po
var myfs = createFs(10); i tym samym przejściu całej pętli for , będzie miało wartość 10 dlatego każdy będzie miał wartość 10.
*/

function createFs(n) { 
    // tworzy tablicę n funkcji 
    var fs = []; 
    // i-ta funkcja z tablicy ma zwrócić i 
    for ( var i = 0; i < n; i++ ) { 
        
        fs[i] = function(i) {
             return function() { 
                return i; 
            };
        }(i);  
    }; 
    return fs; 
}

/*
dobra co się stało się:
mamy tutaj zagnierzdżoną funkcję jako fs[i] którą odrazu wywołujemy dzięki czemu zachowuję "aktualny stan" i.
*/

var myfs = createFs(10); 
console.log( myfs[0]() ); 
// zerowa funkcja miała zwrócić 0 
console.log( myfs[2]() ); 
// druga miała zwrócić 2 
console.log( myfs[7]() ); 
// 10 10 10 // ale wszystkie zwracają 10!?