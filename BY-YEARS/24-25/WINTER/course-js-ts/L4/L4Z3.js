// z3 : [1]  Dlaczego nie używamy Worker.prototype = Person.prototype; ?
//      [2]  Dlaczego nie używamy Worker.prototype = new Person(); ?

// [1] :
/*
Przy takiej instrukcji worker.prototype stanie się dosłownie (dokładnie) tym samym obiektem co 
Person.prototype. Oznaczać to będzie że każda zmiana w Worker.prototype będzie wpływać na Person.prototype. Może być to problematyczne.
*/

// [2] :

/*
tutaj załamiemy konwencje bo : przypisujemy nowego Person co będzie oznaczać ,że również przypiszemy pola
a w prototypach chcieliśmy jedynie mieć funkcję. Dodatkowo wywoła to konstruktor a zazwyczaj chcieli byśmy mieć tam 
jakieś dane do przypisania do pól czego tutaj nie robimy.
*/