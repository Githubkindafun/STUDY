// filter / map / for each tylko ,ze w ts


function filter<T>(a: T[], f: (item: T) => boolean): T[] {
    let b: T[] = [];
    for (let i = 0; i < a.length; i++) {
        if (f(a[i])) {
            b.push(a[i]);
        }
    }
    return b;
}


function for_each<T>(a: T[], f: (item: T) => void): T[] {
    for (let i = 0; i < a.length; i++) {
        f(a[i]);
    }
    return a;
}


function map<T,U>(a: T[], f: (item: T) => U): U[] {
    let b: U[] = [];    
    for (let i = 0; i < a.length; i++) {
        b[i] = f(a[i]);
    }
    return b;
}
