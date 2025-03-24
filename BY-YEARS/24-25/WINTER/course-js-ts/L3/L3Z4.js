// zadanie 4 : funckja o dowolnej ilośći argumentów sumująca podane liczby

function freaky_sum(...args) {
    let sum = 0;
    for (let i = 0; i < args.length; i++) {
        sum += args[i];
    }
    return sum;
}

console.log(freaky_sum(1, 2, 3, 4));

console.log(freaky_sum(1, 1, 1, 1, 1, 5));