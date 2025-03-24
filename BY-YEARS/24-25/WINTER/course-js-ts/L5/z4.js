// z4 : printing out text from txt
var fs = require('fs');  
fs.readFile('a.txt', 'utf-8', function(err, data) {     
    
    if (err) {
        console.log(";c");
        return;
    }

    console.log(data);

}); 
// pamietac o odpalaniu w odpowiednim katalogu !