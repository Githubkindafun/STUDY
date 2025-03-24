// zadanie 6 : generatory / iteratory zabawa ciąg dalszy

function *fib() { 
    let i = 0;
    let j = 1;
    while(true) {
        yield i;
        j = j + i;
        i = j - i;
    }
}

function fibo() { 
    var state1 = 0; 
    var state2 = 1;
    return { 
        next : function() { 
            tmp_obj = {
                value : state1,
                done : false // gdy true to koniec
            }
            state2 = state2 + state1
            state1 = state2 - state1
            return tmp_obj
        } 
    } 
}

// var _it = fib(); 
// for ( var _result; _result = _it.next(), !_result.done; ) { 
//     console.log( _result.value ); 
// }

// var fi = fibo(); 
// for ( var _result; _result = fi.next(), !_result.done; ) { 
//     console.log( _result.value ); 
// }

// for ( var i of fib() ) { 
//     console.log( i ); 
// }
//------

// for ( var i of fibo() ) { 
//     console.log( i ); 
// }
// TypeError: fibo is not a function or its return value is not iterable
// w takiej formie nie jest iterablem
    
var foo = { 
    [Symbol.iterator] : fibo
}; 

for ( var i of foo ) { 
    console.log( i ); 
}

//------