// task 5 : scraping www

// [1] : Napisać program używający modułu (http/https), który odczyta zawartość jakiegoś zasobu sieciowego (np.strony) i wypisze ją na konsoli.
// [2] : Pokazać jak dostępny w bibliotece standardowej podstawowy interfejs programowania (metoda get i zdarzenia data/end) 
// opakować w wygodniejszy interfejs programowania zwracający obiekt Promise.

// [1] :

// var https = require('https'); 

// const url = 'https://pl.wikipedia.org/wiki/Alpaka_(zwierz%C4%99)'; 

// https.get(url, (resp) => {
//     let buffor = '';

//     resp.on('data', (piece) => {
//         buffor += piece;
//     });

//     resp.on('end', () => { // end : czyli skonczylismy 
//         console.log("Zobczamy co tam mamy:");
//         console.log(buffor);
//     });
// });

// [2] : (opakowałem tak jak na wykładzie było pokazane)
var https = require('https'); 

var getThatUrl = (url) => {
    return new Promise((resolve, reject) => { // zasadniczo to co zrobiliśmy to opakowaliśmy nasz poprzedni kod
                                              // w promis'a aby możliwe było wykorzysatanie async i await

        https.get(url, (resp) => {
            let buffor = '';
        
            resp.on('data', (piece) => {
                buffor += piece;
            });
        
            resp.on('end', () => { 
                console.log("Zobczamy co tam mamy:");
                console.log(buffor);
            });
        });

    });
};

const url = 'https://pl.wikipedia.org/wiki/Alpaka_(zwierz%C4%99)';  
(async () => {
    try {
        var data = await getThatUrl(url); // Pobranie danych z zasobu
        console.log(data);
    } catch (e) {
        console.error("Coś się popsuło ;c", e);
    }
})();
