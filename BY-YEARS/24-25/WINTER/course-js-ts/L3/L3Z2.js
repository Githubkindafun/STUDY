// zadanie 2 : własna implementacja funckji
// map
// for each
// filter

// map nakłada na każdy argument a funckje f i zwraca zmodyfikowane a
// a to tablica tak rozumiem
// f to funkcja

function map(a, f) {
    for(let i = 0; i < a.length; i++) {
        a[i] = f(a[i]);
    }
    return a;
}

// var a = [1,2,3,4];

// console.log(a);
// map( a, _ => _ * 2 );
// console.log(a);

// var a = [1,2,3,4];

// console.log(a);
// map(a, function(num) {
//     return num*2;
// });
// console.log(a);


// for each to takie coś co dla każdego elementu coś robi ;>

function for_each(a, f) {
    for (let i = 0; i < a.length; i++) {
        f(a[i]);
    }
    return a;
}

// var a = [1,2,3,4];

// console.log(a);
// for_each( a, _ => { console.log( _ ); } );
// console.log(a);

// var a = [1,2,3,4];

// console.log(a);
// for_each( a, function(i) {
//     i = i * 2;
// });
// console.log(a);

// krótki komentarz jak widzimy wartości for_each nam nie zmienia ;>



function filter(a, f) {
    var b = [];

    for (let i = 0; i < a.length; i++) {

        if(f(a[i])) {
            b.push(a[i]);
        }
    }
    return b;
}

var a = [1,2,3,4];

console.log(a);
console.log(filter( a, _ => _ < 3 ));

var a = [1,2,3,4];

console.log(a);
console.log(filter( a, function(i) {
    if (i < 3) {
        return true;
    }
    return false;
}));
