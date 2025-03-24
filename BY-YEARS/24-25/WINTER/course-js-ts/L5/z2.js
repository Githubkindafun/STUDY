// task 2 : what is your name ? from console

console.log("What is your name my dear user?")

process.stdin.on('data', (input) => { // ściągamy dane z konsoli (słuchamy danych)
                                      // on. wywołuje (input) => , za każdym razem jak użytkownik wpisze dane i kliknie enter

    const name = input.toString().trim() // dane wejsciowe zamieniamy na string
                                         // trim usuine nam endline po enterze
    console.log(`Oh hi ${name}! ;>`);

    process.exit(); // koniec naszej funkcji
});