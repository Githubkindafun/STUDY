// task 3 : number guessing game

console.log("Guessing game start! What am i thinking about?")



let numToGuess = Math.floor(Math.random() * 101); // basicly random daje nam liczbe z przedzialu 0 do 1, mnożymy to przez 101 aby przeskalowac to do 0 do 100.9...
                                                  // następnie za pomocą podłogi upewniamy sie ze to beda liczby z przedziału 0 do 100
let guess = -1;
process.stdin.on('data', (input) => {
    guess = parseInt(input.toString().trim()); // zamieniamy na int
    
    if (isNaN(guess)) {
        console.log("Please enter a valid number.");
        return;
    }

    if (guess === numToGuess) {
        console.log("That's it! You guessed correctly!");
        process.exit(); 
    } else if (guess < numToGuess) {
        console.log("Too small ;c");
    } else {
        console.log("Too big :o");
    }
});