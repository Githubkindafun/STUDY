// zadanie 5 : zabawa z interatorami / generatorami

function parameter(start, stop, step) {
    return function createGenerator() { 
        var _state = 0; 
        return { 
            next : function() { 
                tmp_obj = {
                    value : _state,
                    done : _state >= stop // gdy true to koniec
                }
                _state += step; 
                return tmp_obj
            } 
        } 
    }
}


// let start = 0;
// let stop = 10;
// let step = 2;

let start = 0;
let stop = 10;
let step = 1;


var foo = { 
    [Symbol.iterator] : parameter(start, stop, step) 
}; 

for ( var f of foo ) {
    console.log(f);
}

/*
primo dlaczego działa:
troche jak z var i let mamy sytuacje ;>
basicly przy przekazywaniu _state przez tmp to value zostanie takie jakie było "początkowo" a przy returnie  
gdzie tam zwiększamy to w value ma "utworzony obiekt" więc trzyma wartość a w done będzie referencja na wartość _value w tym scope
dos:
value = wartość _state
done = referencja na _state (takie jakby function () return i)
*/