// zadanie 7 : generatory / iteratory zabawa ostatnia część trylogii


function *fib() { 
    let i = 0;
    let j = 1;
    while(true) {
        yield i;
        j = j + i;
        i = j - i;
    }
}


function* take(it, top) {
    let cnt = 0;
    for ( var i of it ) { 
        if (cnt >= top) {
            break;
        }
        cnt++;
        yield i 
    }
}
    

 // zwróć dokładnie 10 wartości z potencjalnie
 // "nieskończonego" iteratora/generatora 
 for (let num of take( fib(), 10 ) ) { 
    console.log(num); 
}