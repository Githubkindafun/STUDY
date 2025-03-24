// zadanie 1 łańcuszki 

function getLastProto(o) {
    var p = o;     
    do {         
        o = p;         
        p = Object.getPrototypeOf(o);     
    } while (p);
          
    return o; 
} 

const o1 = {tmp : 'hello'};
const o2 = 42;
const o3 = true;
const o4 = [1, 2, 3, 4]; // obiekt bez prototypu
// obiekt może mieć pusty prototyp


// sprawdzimy teraz czy obiekty "kończą" łańcuch w Object.prototype
// a ten bez prototypu zwróci null
// z wykładu :
// pokazuje że wszystkie obiekty Javascript 
// mają jedną, tę samą instancję obiektu jako swój prototyp.

console.log(getLastProto(o1) === getLastProto(o2));
console.log(getLastProto(o2) === getLastProto(o3));
console.log(getLastProto(o3) === getLastProto(o4));
// console.log(Object.getPrototypeOf(onull))