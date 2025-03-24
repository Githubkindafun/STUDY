// z5 : pola prywatne

// function Foo() { } 

// Foo.prototype.Bar = function() { 
//     this.Qux(); 
// } 
// Foo.prototype.Qux = function() { 
//     console.log( "Foo::Qux");
// }

// var foo = new Foo(); 
// // foo.Bar(); // ale w tym, niepoprawnym rozwiązaniu można też foo.Qux();
// foo.Qux();

function Foo() {

    function Qux () {
        console.log( "private Qux");
    }

    this.Bar = function() {
        Qux();
    }
}


var foo = new Foo();
// tutaj "przepisujemy wszystko co ma this"
// dlatego qux zostanie jako prywatna metoda


//foo.Qux();

foo.Bar();

