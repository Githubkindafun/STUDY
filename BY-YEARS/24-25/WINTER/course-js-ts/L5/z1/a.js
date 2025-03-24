module.exports = { work_a };  // export [1]
let b = require('./b');  
function work_a(n) {     
    if ( n > 0 ) {         
        console.log( `a: ${n}`);         
        b.work_b(n-1);     
    } 
}  