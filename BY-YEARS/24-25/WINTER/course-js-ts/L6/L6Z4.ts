// z4

type User = {
    name: string;
    age: number;
    occupation: string;
}

type Admin = {
    name: string;
    age: number;
    role: string;
}

export type Person = User | Admin;

export const persons: Person[] = [
    {
        name: 'Max Mustermann',
        age: 25,
        occupation: 'Chimney sweep'
    },
    {
        name: 'Jane Doe',
        age: 32,
        role: 'Administrator'
    }
];

// kod ze strony bo sie latwiej wkleja a to still to samo

export function logPerson(person: Person) {
    let additionalInformation: string;

    // if (person.role) {
    //     additionalInformation = person.role;
    // } else {
    //     additionalInformation = person.occupation;
    // }

    additionalInformation = (person as Admin).role || (person as User).occupation;
    // mowimy kompilatorowi ,że jak person jest Admin to wez role a jak User to wez occupation
    console.log(` - ${person.name}, ${person.age}, ${additionalInformation} `);
}

logPerson(persons[0]);
logPerson(persons[1]);