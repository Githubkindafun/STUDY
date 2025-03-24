// Pokazać jak używać następujących mechanizmów systemu typów:
// typy wyższego rzędu- Extract, Exclude
// typy wyższego rzędu- Record, Required, Readonly, Partial
// typy wyższego rzędu- Pick, Omit
// typy dostępu indeksowanego (Indexed Acess Types)
// Jakie jest zastosowanie dla tych mechanizmów? 
// Jaką wartość wnoszą do systemu typów?

// Extract tworzy typ wyciągając z właściwości z jednego typu i przypisuje do drugiego
// type Union = 'a' | 'b' | 'c';
// type Extracted = Extract<Union, 'a' | 'b'>; // 'a' | 'b'
// Zastosowanie : uwzględnianie tylko określone typy


// w zasadzie analogicznie tylko wykluczamy jakas wlasnosc i zostawiamy pozostale
// type Union = 'a' | 'b' | 'c';
// type Excluded = Exclude<Union, 'a'>; //  'b' | 'c'
// Zastosowanie : mozemy cos sobie usunac


// Record tworzy typ obiektu z kluczami i wartosciami innego typu
// type Keys = 'a' | 'b';
// type RecordType = Record<Keys, number>; // { a: number; b: number; }
// Zastosowanie : coś nawpewno fajnego da się wymyślić z tym


// Required sprawa ,że wszystkie właściwości typu są wymagane
// type Optional = { a?: number; b?: string };
// type AllRequired = Required<Optional>; // { a: number; b: string; }
// Zastosowanie : wymuszanie obecnosci wszystkich własciwosciw type (bo ? oznaczna opcjonalne)


// Readonly sprawia ,że wszystkie właściwości typu są tylko do odczytu (niezmienne)
// type Mutable = { a: number; b: string };
// type Immutable = Readonly<Mutable>; // { readonly a: number; readonly b: string; }
// Zastosowanie : zaby zapobiec zmanie wlasciwosci


// Partial sprawia ze wszystkie wlasciwosci typu sa opcjonalne
// type Full = { a: number; b: string };
// type PartialType = Partial<Full>; //  { a?: number; b?: string; }
// napewno cos cool da sie wymyslic

// Pick tworzy typ wybierajac okreslosne wlasciwosci 
// type Original = { a: number; b: string; c: boolean };
// type Picked = Pick<Original, 'a' | 'b'>; // { a: number; b: string; }
// mozna sobie wybierac co sie chce 

// Omit tworzy typ pomijajac wybrane wlasciwosci
// type Original = { a: number; b: string; c: boolean };
// type Omitted = Omit<Original, 'a'>; // { b: string; c: boolean; }
// mozemy sobie cos wykluczac z innego typu yey!


// Index Access Types pozwala na dostep do kokretnej wlasciwosci 
// type Person = { name: string; age: number };
// type NameType = Person['name']; // string
// type AgeType = Person['age']; // number
// Zastosowanie : napewno da sie czadersko wykorzystaac

// Zastosowania tych mechanizmow :
// pozwala uzywac ponownie kodu ktory juz mamy
// pozwala rowniez na pewnego typu elastycznosc
