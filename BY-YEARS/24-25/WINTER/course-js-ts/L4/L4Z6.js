// z6 : BFS

// z wykładu
class Tree {   
    constructor (val, left, right) {     
        this.left  = left;     
        this.right = right;     
        this.val   = val;   
    }    

    // DFS
    // [Symbol.iterator] = function*() {     
    //     yield this.val;       
    //     if ( this.left )  yield* this.left;     
    //     if ( this.right ) yield* this.right;   
    // }
    
    // BFS : używajac kolejki 

    [Symbol.iterator] = function* () {
        let queue = [this] // nasza kolejka zainicjalizowana rootem
        
        while (queue.length > 0) { // lecimy dopóki nie skończą nam się węzły
            let node = queue.splice(0, 1)[0]; // splice zwraca array więc bieżemy pierwszy element
            yield node.val; // yield-ujemy wartość node-a

            // teraz jeżeli istnieją dzieci lewe/prawe to
            if (node.left) {
                queue.push(node.left)
            } // jeżeli istnieją to wrzucamy na koniec kolejki
            if (node.right) {
                queue.push(node.right)
            }
            
        }
    }


}  
var root = new Tree( 1, new Tree( 2, new Tree( 3 ) ), new Tree( 4 ));  
for ( var e of root ) {   
    console.log( e ); 
} 


// to jest nasze drzewo
//       1
//      / \
//     2   4
//    /
//   3
// i jak widać output to 1 2 4 3 więc działa ;>