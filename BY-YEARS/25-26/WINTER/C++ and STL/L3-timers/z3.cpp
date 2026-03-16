#include<bits/stdc++.h>

using namespace std;

/*
Zdefiniuj początkowe liczby harmoniczne posługując się statyczną arytmetyką liczb wymiernych  
z wykorzystaniem szablonu ratio<>. Jaką największą liczbę harmoniczną udało Ci się zdefiniować?
*/

int main() {

    using H1 = ratio<1, 1>;
    using H2 = ratio_add<H1, ratio<1, 2>>;
    using H3 = ratio_add<H2, ratio<1, 3>>;
    using H4 = ratio_add<H3, ratio<1, 4>>;
    using H5 = ratio_add<H4, ratio<1, 5>>;
    using H6 = ratio_add<H5, ratio<1, 6>>;
    using H7 = ratio_add<H6, ratio<1, 7>>;
    using H8 = ratio_add<H7, ratio<1, 8>>;
    using H9 = ratio_add<H8, ratio<1, 9>>;
    using H10 = ratio_add<H9, ratio<1, 10>>;
    using H11 = ratio_add<H10, ratio<1, 11>>;
    using H12 = ratio_add<H11, ratio<1, 12>>;
    using H13 = ratio_add<H12, ratio<1, 13>>;
    using H14 = ratio_add<H13, ratio<1, 14>>;
    using H15 = ratio_add<H14, ratio<1, 15>>;
    using H16 = ratio_add<H15, ratio<1, 16>>;
    using H17 = ratio_add<H16, ratio<1, 17>>;
    using H18 = ratio_add<H17, ratio<1, 18>>;
    using H19 = ratio_add<H18, ratio<1, 19>>;
    using H20 = ratio_add<H19, ratio<1, 20>>;
    using H21 = ratio_add<H20, ratio<1, 21>>;
    using H22 = ratio_add<H21, ratio<1, 22>>;
    using H23 = ratio_add<H22, ratio<1, 23>>;
    using H24 = ratio_add<H23, ratio<1, 24>>;
    using H25 = ratio_add<H24, ratio<1, 25>>;
    using H26 = ratio_add<H25, ratio<1, 26>>;
    using H27 = ratio_add<H26, ratio<1, 27>>;
    using H28 = ratio_add<H27, ratio<1, 28>>;
    using H29 = ratio_add<H28, ratio<1, 29>>;
    using H30 = ratio_add<H29, ratio<1, 30>>;
    using H31 = ratio_add<H30, ratio<1, 31>>;
    using H32 = ratio_add<H31, ratio<1, 32>>;
    using H33 = ratio_add<H32, ratio<1, 33>>;
    using H34 = ratio_add<H33, ratio<1, 34>>;
    using H35 = ratio_add<H34, ratio<1, 35>>;
    using H36 = ratio_add<H35, ratio<1, 36>>;
    using H37 = ratio_add<H36, ratio<1, 37>>;
    using H38 = ratio_add<H37, ratio<1, 38>>;
    using H39 = ratio_add<H38, ratio<1, 39>>;
    using H40 = ratio_add<H39, ratio<1, 40>>;
    using H41 = ratio_add<H40, ratio<1, 41>>;
    using H42 = ratio_add<H41, ratio<1, 42>>;
    using H43 = ratio_add<H42, ratio<1, 43>>;
    using H44 = ratio_add<H43, ratio<1, 44>>;
    using H45 = ratio_add<H44, ratio<1, 45>>;
    using H46 = ratio_add<H45, ratio<1, 46>>;
    // using H47 = ratio_add<H46, ratio<1, 47>>; // na tym sie wywala
    // https://www.wolframalpha.com/input?i2d=true&i=47th+harmonic+series+value
    // cout << numeric_limits<intmax_t>::max() << endl;
    // to daje                       9223372036854775807
    // wolfram mowi ze mianownik to: 63245806209101973600
    // 46 to juz                     1345655451257488800
    // czyli fakt miesci sie

    // using H48 = ratio_add<H47, ratio<1, 48>>;
    // using H49 = ratio_add<H48, ratio<1, 49>>;
    // using H50 = ratio_add<H49, ratio<1, 50>>;
    // using H51 = ratio_add<H50, ratio<1, 51>>;
    // using H52 = ratio_add<H51, ratio<1, 52>>;
    // using H53 = ratio_add<H52, ratio<1, 53>>;
    // using H54 = ratio_add<H53, ratio<1, 54>>;
    // using H55 = ratio_add<H54, ratio<1, 55>>;
    // using H56 = ratio_add<H55, ratio<1, 56>>;
    // using H57 = ratio_add<H56, ratio<1, 57>>;
    // using H58 = ratio_add<H57, ratio<1, 58>>;
    // using H59 = ratio_add<H58, ratio<1, 59>>;
    // using H60 = ratio_add<H59, ratio<1, 60>>;
    // using H61 = ratio_add<H60, ratio<1, 61>>;
    // using H62 = ratio_add<H61, ratio<1, 62>>;
    // using H63 = ratio_add<H62, ratio<1, 63>>;
    // using H64 = ratio_add<H63, ratio<1, 64>>;
    // using H65 = ratio_add<H64, ratio<1, 65>>;
    // using H66 = ratio_add<H65, ratio<1, 66>>;
    // using H67 = ratio_add<H66, ratio<1, 67>>;
    // using H68 = ratio_add<H67, ratio<1, 68>>;
    // using H69 = ratio_add<H68, ratio<1, 69>>;
    // using H70 = ratio_add<H69, ratio<1, 70>>;
    // using H71 = ratio_add<H70, ratio<1, 71>>;
    // using H72 = ratio_add<H71, ratio<1, 72>>;
    // using H73 = ratio_add<H72, ratio<1, 73>>;
    // using H74 = ratio_add<H73, ratio<1, 74>>;
    // using H75 = ratio_add<H74, ratio<1, 75>>;
    // using H76 = ratio_add<H75, ratio<1, 76>>;
    // using H77 = ratio_add<H76, ratio<1, 77>>;
    // using H78 = ratio_add<H77, ratio<1, 78>>;
    // using H79 = ratio_add<H78, ratio<1, 79>>;
    // using H80 = ratio_add<H79, ratio<1, 80>>;
    // using H81 = ratio_add<H80, ratio<1, 81>>;
    // using H82 = ratio_add<H81, ratio<1, 82>>;
    // using H83 = ratio_add<H82, ratio<1, 83>>;
    // using H84 = ratio_add<H83, ratio<1, 84>>;
    // using H85 = ratio_add<H84, ratio<1, 85>>;
    // using H86 = ratio_add<H85, ratio<1, 86>>;
    // using H87 = ratio_add<H86, ratio<1, 87>>;
    // using H88 = ratio_add<H87, ratio<1, 88>>;
    // using H89 = ratio_add<H88, ratio<1, 89>>;
    // using H90 = ratio_add<H89, ratio<1, 90>>;
    // using H91 = ratio_add<H90, ratio<1, 91>>;
    // using H92 = ratio_add<H91, ratio<1, 92>>;
    // using H93 = ratio_add<H92, ratio<1, 93>>;
    // using H94 = ratio_add<H93, ratio<1, 94>>;
    // using H95 = ratio_add<H94, ratio<1, 95>>;
    // using H96 = ratio_add<H95, ratio<1, 96>>;
    // using H97 = ratio_add<H96, ratio<1, 97>>;
    // using H98 = ratio_add<H97, ratio<1, 98>>;
    // using H99 = ratio_add<H98, ratio<1, 99>>;
    // using H100 = ratio_add<H99, ratio<1, 100>>;

}