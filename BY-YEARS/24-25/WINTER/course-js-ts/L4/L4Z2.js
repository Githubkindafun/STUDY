// z2

// prototyp : skąd obiekt dziedzyczy właściwości

// jak chcemy sprawdzić czy coś pochodzi z obiektu a nie z jego 
// prototypu to jest metoda : obj.hasOwnProperty("propertyName")
// metoda zwraca true : jeśli propertyName należy do obj 
//              false : wpp


/* przykład użycia 
const proto = { inheritedProp: "from prototype" }; // To będzie prototyp
const obj = Object.create(proto); // Tworzymy obiekt dziedziczący z `proto`
obj.ownProp = "own value"; // Dodajemy własną właściwość

console.log(obj.hasOwnProperty("ownProp")); // true (należy bezpośrednio do `obj`)
console.log(obj.hasOwnProperty("inheritedProp")); // false (pochodzi z prototypu)
*/

function isMyOwn(object, property) {
    return object.hasOwnProperty(property);
}
// to zwraca true : property nalezy do obiektu
//          false : wpp

// Tworzymy prototyp
const proto = {
    inheritedField: "I am from the prototype",
    inheritedMethod() {
        console.log("This method is inherited");
    }
};

// Tworzymy obiekt, który dziedziczy z prototypu
const obj = Object.create(proto);
obj.ownField = "This is my own field";
obj.ownMethod = function () {
    console.log("This is my own method");
};
obj.otherField = 42;

console.log(isMyOwn(obj, "ownField"));
console.log(isMyOwn(obj, "inheritedField"));

function splitOwnAndInheritedProperties(obj) {
    const ownProps = [];
    const inheritedProps = [];

    for (let prop in obj) {
        if (isMyOwn(obj, prop)) {
            ownProps.push(prop);
        } else {
            inheritedProps.push(prop);
        }
    }

    return { ownProps, inheritedProps };
}

console.log(splitOwnAndInheritedProperties(obj));