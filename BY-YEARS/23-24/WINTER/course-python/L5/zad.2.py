# zadanie 2 z listy 5 python
from abc import abstractmethod, ABC #abstract class
from itertools import product

class Formula(ABC):
    @abstractmethod
    def __str__(self) -> str:
        pass

    def __add__(self, formula): # wynik and
        if not isinstance(formula, Formula): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się Formuly dostalem {type(formula)}")
        return And(self, formula)
    
    def __mul__(self, formula): # wynik or
        if not isinstance(formula, Formula): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się Formuly dostalem {type(formula)}")
        return Or(self, formula)
    
    @abstractmethod # kazdy operator będzie troche inaczej to implementował
    def solve(self, vars: dict) -> bool:
        pass

    @abstractmethod
    def get_variables(self) -> set: # zbieram zmienne do tautologii
        pass

    def tautology(self):
        vars_keys = self.get_variables()
        b_permutations = list(product([True, False], repeat = len(vars_keys))) # tutaj dostajemy potencjalne permutacje T and F dla n zmiennych
        val_permutations = [dict(zip(vars_keys, permutation)) for permutation in b_permutations] # wszystkie mozliwe wartosciowania zmiennych
        return all(self.solve(permutation) for permutation in val_permutations) # iteruje po wszystkich wynikach i jak all true to true wpp. false
    
class Stala(Formula):
    def __init__(self, value: bool):
        if not isinstance(value, bool): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się boola dostalem: {type(value)} ;c")
        self.value = value

    def __str__(self) -> str: # do printa
        return str(self.value).lower()

    def get_variables(self) -> set: # do tautologii
        return {}

    def solve(self, vars: dict) -> bool: # stała poprostu zwraca sama siebie
        return self.value

class Zmienna(Formula):
    def __init__(self, name: str):
        if not isinstance(name, str): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się stringa dostalem {type(name)} ;c") 
        self.name = name

    def __str__(self) -> str: # do printa
        return self.name

    def get_variables(self) -> set: # do tautologii
        return {self.name}

    def solve(self, vars: dict) -> bool: # zmienna zwraca wartosc przypisana sobie 
        if self.name not in vars.keys(): # sprawdzmy czy nasz klucz jest w danych
            raise KeyError(f"W przekazanym slowniku brakuje zmiennej \"{self.name}\" ")
        return vars[self.name]

class Not(Formula):
    def __init__(self, formula: Formula):
        if not isinstance(formula, Formula): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się Formuly dostalem {type(formula)} ;c")
        self.formula = formula

    def __str__(self): # do printa
        return f"~({self.formula})"
    
    def get_variables(self) -> set: # do tautologii
        return self.formula.get_variables()

    def solve(self, vars: dict) -> bool: # poprostu negacja
        return not self.formula.solve(vars)

#abstract class
class BinaryOperator(Formula, ABC): # tutaj mamy operatory binarne: And, Or
    def __init__(self, formula_1: Formula, formula_2: Formula):
        if not isinstance(formula_1, Formula): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się Formuly dostalem {type(formula_1)} chodzi o f_1 jak cos ;c")
        if not isinstance(formula_2, Formula): # sprawdzamy typ
            raise TypeError(f"Spodziewałem się Formuly dostalem {type(formula_2)} chodzi o f_2 jak cos ;c")
        
        self.formula_1 = formula_1
        self.formula_2 = formula_2

    def get_variables(self) -> set: # do tautologii 
        return self.formula_1.get_variables().union(self.formula_2.get_variables()) # sumujemy sety

class And(BinaryOperator):
    def __init__(self, formula_1: Formula, formula_2: Formula):
        super().__init__(formula_1, formula_2)

    def __str__(self): # do printa
        return f"({self.formula_1} ^ {self.formula_2})"
    
    def solve(self, vars: dict) -> bool: # and
        return self.formula_1.solve(vars) and self.formula_2.solve(vars)
        
class Or(BinaryOperator):
    def __init__(self, formula_1: Formula, formula_2: Formula):
        super().__init__(formula_1, formula_2)

    def __str__(self): # do printa
        return f"({self.formula_1} v {self.formula_2})"
    
    def solve(self, vars: dict) -> bool: # or
        return self.formula_1.solve(vars) or self.formula_2.solve(vars)



def main(): # testy
    top = Stala(True)
    bottom = Stala(False)
    print(f"top: {top} \nbottom: {bottom}")

    zmienna = Zmienna("p")
    print(f"{zmienna}: {zmienna.solve({'x': True, 'p': False})}")
    
    formula_1 = Or(Not(Zmienna("x")), And(Zmienna("y"), Stala(True)))
    formula_2 = And(Zmienna("x"), Or(Zmienna("y"), Stala(False)))
    print(formula_1)
    print(formula_2)
    print(f"formula_1 + formula_2 = {formula_1 + formula_2} = {(formula_1 + formula_2).solve({'x': True, 'y': False})}")
    print(f"formula_1 * formula_2 = {formula_1 * formula_2} = {(formula_1 * formula_2).solve({'x': True, 'y': False})}")
    
    vars = formula_1.get_variables()
    print(vars)

    formula_taut_1 = Or(Not(Zmienna("p")), Zmienna("p"))
    print(f"czy {formula_taut_1} jest tautologia: {formula_taut_1.tautology()}")

    formula_taut_2 = And(Not(Zmienna("p")), Zmienna("p"))
    print(f"czy {formula_taut_2} jest tautologia: {formula_taut_2.tautology()}")


if __name__ == '__main__':
    main()
