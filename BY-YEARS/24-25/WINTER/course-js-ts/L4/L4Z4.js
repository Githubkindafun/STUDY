var n = 1; 
// liczba ma prototyp? 
console.log( typeof Object.getPrototypeOf( n ) ); 
// można jej dopisać pole/funkcję? 
n.foo = 'foo'; 
console.log( n.foo );

// [1] Czy wartości typów prostych też mają prototypy?
// [2] Czy to znaczy że one są obiektami i można im dodawać 
// dowolne pola/funkcje, jak wszystkim obiektom?
// [3] Wyjaśnić wynik działania ostatniej linii poniższego programu:

// [1] :
/*
nie maja same z siebie ale js i je automatycznie "nadaje".
Konkretniej opakowuje je w odpowiedniki obiektów.
*/

// [2] :

// 2.1 nie to nie są obiekty
// var num = 42;

// // Próba dodania nowego pola
// num.customProperty = "test";

// console.log(num.customProperty);

// var name = 'Alice';

// // 2.2 nie mozna dodawac funkcji
// name.sayHello = function () {
//     return `Hello, ${this}!`;
// };

// console.log(name.sayHello()); 

// wyjatkiem jest null i undefined ich nie mozemy dotykać



// [3] :
/*
n to typ prosty (number), nie obiekt.
JavaScript opakowuje go na chwilę w obiekt Number, ale ten
obiekt zostaje natychmiast usunięty po przypisaniu.

Gdy próbujesz odczytać n.foo, nie ma już obiektu, 
na którym była przypisana właściwość, więc zwracane jest undefined.

*/