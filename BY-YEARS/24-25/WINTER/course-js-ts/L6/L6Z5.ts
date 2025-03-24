// z5
// odpalac z command line
type User = {
    type: 'user';
    name: string;
    age: number;
    occupation: string;
}

type Admin = {
    type: 'admin';
    name: string;
    age: number;
    role: string;
}

export type Person = User | Admin;

export const persons: Person[] = [
    {
        type : "user",
        name: 'Max Mustermann',
        age: 25,
        occupation: 'Chimney sweep'
    },
    {
        type : "admin",
        name: 'Jane Doe',
        age: 32,
        role: 'Administrator'
    }
];

export function isAdmin(person: Person) {
    return person.type === 'admin';
} // nic nie zmienialem i dziala

export function isUser(person: Person) {
    return person.type === 'user';
}



export function logPerson(person: Person) {
    let additionalInformation: string = '';
    if (isAdmin(person)) {
        additionalInformation = person.role;
    }
    if (isUser(person)) {
        additionalInformation = person.occupation;
    }
    console.log(` - ${person.name}, ${person.age}, ${additionalInformation}`);
}

logPerson(persons[0]);
logPerson(persons[1]);