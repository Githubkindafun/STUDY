// fibo bez oraz z memoizacja przerobione na typeScript

// to sa te poprzednie implementacje w js

// function rec_Fibo(n) {
//     if (n === 0 ) {
//         return 0;
//     }
//     if (n === 1) {
//         return 1;
//     }
//     return rec_Fibo(n - 1) + rec_Fibo(n - 2);
// }

// function memoize(fn) {
//     var cache = {};
//     return function xd(n) {

//         if ( n in cache ) {
//             return cache[n]         
//         } else {             
//             var result = rec_Fibo(n);             
//             cache[n] = result;             
//             return result;         
//         }      
//     } 
// }


function rec_Fibo(n: number): number {
    if (n === 0) {
        return 0;
    }
    if (n === 1) {
        return 1;
    }
    return rec_Fibo(n - 1) + rec_Fibo(n - 2);
}

function memoize(fn: (n: number) => number): (n: number) => number {

    let cache: { [key: number]: number } = {};  

    return function xd(n: number): number {
        if (n in cache) {
            return cache[n];
        } else {
            let result = fn(n);
            cache[n] = result;
            return result;
        }
    };
}

let memoizedFibo = memoize(rec_Fibo);
console.log(memoizedFibo(10)); 
