// task 6 : using readline module scrap IP from "www logs" : create your own test data
// from nodejs.org

const fs = require('fs'); // imporujemy sobie potrzebne modóły
const readline = require('readline');

async function processLineByLine() {
    const fileStream = fs.createReadStream('task6data.txt');

    const rl = readline.createInterface({ // "konfiguracja" readline
        input: fileStream, // skąd dane
        crlfDelay: Infinity, // z tego co przeczytałem to zapewnia że mamy łądnie "\n"
    });
    // Note: we use the crlfDelay option to recognize all instances of CR LF // w sumie tu to jest napisane
    // ('\r\n') in input.txt as a single line break.

    for await (const line of rl) { // for awawit : asynchroniczna iteracja 
    // Each line in input.txt will be successively available here as `line`.
        let res = line.split(" "); // dzielimy linie na części
        console.log(`Line from file: ${res[1]}`); // i bierzemy ip
    }
}

processLineByLine();

// odpalać z command line !!!!!