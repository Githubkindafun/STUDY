#include <iostream>
#include "wektor.hpp"
#include "punkt.hpp"
#include "odcinek.hpp"
#include "trojkat.hpp"


using namespace std;


bool czy_rownolegle(odcinek o1, odcinek o2) {
    //beda sluzyc sa wspolczynniki
    double k1 = (o1.p2.wspy() - o1.p1.wspy()) / (o1.p2.wspx() - o1.p1.wspx());
    double k2 = (o2.p2.wspy() - o2.p1.wspy()) / (o2.p2.wspx() - o2.p1.wspx());

    if(k1 == k2) {
        return true;
    }
    return false;
}

bool czy_prostopadle(odcinek o1, odcinek o2) {
    //beda sluzyc sa wspolczynniki
    double k1 = (o1.p2.wspy() - o1.p1.wspy()) / (o1.p2.wspx() - o1.p1.wspx());
    double k2 = (o2.p2.wspy() - o2.p1.wspy()) / (o2.p2.wspx() - o2.p1.wspx());

    if(k1 * k2 == -1) {
        return true;
    }
    return false;
}

bool czy_rozlaczne(trojkat t1, trojkat t2) {
    //chce sprawdzic czy wsp trojakta jezeli jest
    //w 2 trojakce znajduja sie w "rzucie na osie"
    //drugiegio trojkata jezeli sa poza
    //to sprawdzam czy jakas wsp jest w trojakcie
    //jezeli tak to nie rozlaczne
    //jezeli nie no to rozlaczne
    //min max x dla t1
    double t1_min_x = min(min(t1.p1.wspx(), t1.p2.wspx()), t1.p3.wspx());
    double t1_max_x = max(max(t1.p1.wspx(), t1.p2.wspx()), t1.p3.wspx());
    //min max y dla t1
    double t1_min_y = min(min(t1.p1.wspy(), t1.p2.wspy()), t1.p3.wspy());
    double t1_max_y = max(max(t1.p1.wspy(), t1.p2.wspy()), t1.p3.wspy());
    //min max x dla t2
    double t2_min_x = min(min(t2.p1.wspx(), t2.p2.wspx()), t2.p3.wspx());
    double t2_max_x = max(max(t2.p1.wspx(), t2.p2.wspx()), t2.p3.wspx());
    //min max y dla t2
    double t2_min_y = min(min(t2.p1.wspy(), t2.p2.wspy()), t2.p3.wspy());
    double t2_max_y = max(max(t2.p1.wspy(), t2.p2.wspy()), t2.p3.wspy());

    if(t1_min_x > t2_min_x && t1_max_x < t2_max_x && t1_min_y > t2_min_y && t1_max_y < t2_max_y) {
        return true;
    }
    else if(t1.czy_w_trojkat(t2.p1) or t1.czy_w_trojkat(t2.p2) or t1.czy_w_trojkat(t2.p3) or t2.czy_w_trojkat(t1.p1) or t2.czy_w_trojkat(t1.p2) or t2.czy_w_trojkat(t1.p3)) {
            return false;
        }
        return true;
    }


bool czy_jeden_w_drugim(trojkat t1, trojkat t2) {
    double t1_min_x = min(min(t1.p1.wspx(), t1.p2.wspx()), t1.p3.wspx());
    double t1_max_x = max(max(t1.p1.wspx(), t1.p2.wspx()), t1.p3.wspx());
    //min max y dla t1
    double t1_min_y = min(min(t1.p1.wspy(), t1.p2.wspy()), t1.p3.wspy());
    double t1_max_y = max(max(t1.p1.wspy(), t1.p2.wspy()), t1.p3.wspy());
    //min max x dla t2
    double t2_min_x = min(min(t2.p1.wspx(), t2.p2.wspx()), t2.p3.wspx());
    double t2_max_x = max(max(t2.p1.wspx(), t2.p2.wspx()), t2.p3.wspx());
    //min max y dla t2
    double t2_min_y = min(min(t2.p1.wspy(), t2.p2.wspy()), t2.p3.wspy());
    double t2_max_y = max(max(t2.p1.wspy(), t2.p2.wspy()), t2.p3.wspy());

     if(t1_min_x > t2_min_x && t1_max_x < t2_max_x && t1_min_y > t2_min_y && t1_max_y < t2_max_y) {
        return true;
    }
    return false;
}

int main() {
	wektor w1 = wektor(1, 2);
	
	punkt p1 = punkt(2, 4);
	punkt p2 = punkt(4, 2);
	punkt p3 = punkt(4, 6);
	punkt p4 = punkt(3, 3);
	//punkty specjalnie pod równoległość
	punkt p5 = punkt(3, 2);
	punkt p6 = punkt(2, 3);
	//p7 razem z p6 pod prostopadłosć
	punkt p7 = punkt(3, 4);
	punkt p8 = punkt(1,1);
	punkt p9 = punkt(10,1);//te 3 pod rozlacznaosc
	punkt p10 = punkt(1,10);
	punkt p11 = punkt(12,1);
	punkt p12 = punkt(11,150);
	
	odcinek o1 = odcinek(p1, p2);
	odcinek o2 = odcinek(p2, p3);
	odcinek o3 = odcinek(p3, p4);
	odcinek o4 = odcinek(p1, p4);
	//odcinek o5 = odcinek(p2, p2); // error case
	odcinek o6 = odcinek(p5, p6); // odcinek pod rownoległość
	odcinek o7 = odcinek(p6, p7); // odcinek pod prostopadłość

	trojkat t1 = trojkat(p1, p2, p3);
	trojkat t2 = trojkat(p2, p3, p4);
	trojkat t3 = trojkat(p3, p4, p1);
	trojkat t4 = trojkat(p4, p3, p1);
	//trojkat t5 = trojkat(p3, p3, p3); // error case 
	trojkat t6 = trojkat(p8, p9, p10);
	trojkat t7 = trojkat(p9, p11, p12);

	
	cout << "Wektor w1: " << w1.wspx() << " " << w1.wspy() << endl;

	cout << endl;
	
	cout << "Punkt p1: " << p1.wspx() << " " << p1.wspy() <<endl;
	cout << "Punkt p2: " << p2.wspx() << " " << p2.wspy() <<endl;
	cout << "Punkt p3: " << p3.wspx() << " " << p3.wspy() <<endl;
	cout << "Punkt p4: " << p4.wspx() << " " << p4.wspy() <<endl;

	cout << endl;
	
	cout << "Odcinek o1: " << o1.p1.wspx() << " " << o1.p1.wspy() << "   " << o1.p2.wspx() << " " << o1.p2.wspy() << endl;
	cout << "Odcinek o2: " << o2.p1.wspx() << " " << o2.p1.wspy() << "   " << o2.p2.wspx() << " " << o2.p2.wspy() << endl;
	cout << "Odcinek o3: " << o3.p1.wspx() << " " << o3.p1.wspy() << "   " << o3.p2.wspx() << " " << o3.p2.wspy() << endl;
	cout << "Odcinek o4: " << o4.p1.wspx() << " " << o4.p1.wspy() << "   " << o4.p2.wspx() << " " << o4.p2.wspy() << endl;

	cout << endl;
	
	cout << "Trojkat t1: " << t1.p1.wspx() << " " << t1.p1.wspy() << "   " << t1.p2.wspx() << " " << t1.p2.wspy() << "   " << t1.p3.wspx() << " " << t1.p3.wspy() <<endl;
	cout << "Trojkat t2: " << t2.p1.wspx() << " " << t2.p1.wspy() << "   " << t2.p2.wspx() << " " << t2.p2.wspy() << "   " << t2.p3.wspx() << " " << t2.p3.wspy() <<endl;
	cout << "Trojkat t3: " << t3.p1.wspx() << " " << t3.p1.wspy() << "   " << t3.p2.wspx() << " " << t3.p2.wspy() << "   " << t3.p3.wspx() << " " << t3.p3.wspy() <<endl;
	cout << "Trojkat t4: " << t4.p1.wspx() << " " << t4.p1.wspy() << "   " << t4.p2.wspx() << " " << t4.p2.wspy() << "   " << t4.p3.wspx() << " " << t4.p3.wspy() <<endl;

	cout << endl;
	
	p1.translacja(w1);
	cout << "Translacja punktu p1 przez wektor w1: " << p1.wspx() << " " << p1.wspy() <<endl;
	p2.obrot_punkt(p3, 90);
	cout << "Obrot punktu p2 względem punktu p3 oraz 90 stopni: " << p2.wspx() << " " << p2.wspy() <<endl;
	p3.symetria_punkt(p4);
	cout << "Symetria punktu p3 względem punkt p4: " << p2.wspx() << " " << p2.wspy() <<endl;
	cout << "Odległość między p3 a p4 wynosi: " << dwa_pkt_dlugosc(p3, p4) << endl;

	cout << endl;

	cout << "Długość odc o2 wynosi: " << o2.odc_dlugosc() << endl;
	cout << "Czy punkt p4 w odc o1: " << o1.punkt_w(p4) << endl;
	cout << "Czy punkt p2 w odc o1: " << o1.punkt_w(p2) << endl;
	o3.translacja(w1);
	cout << "Translacja odcinka o3 przez wektor w1: " << o3.p1.wspx() << " " << o3.p1.wspy() << "   " << o3.p2.wspx() << " " << o3.p2.wspy() << endl;
	o4.symetria_punkt(p2);
	
	cout << "Symetria odcinka o4 względem punkt p2: " << o4.p1.wspx() << " " << o4.p1.wspy() << "   " << o4.p2.wspx() << " " << o4.p2.wspy() << endl;
	o1.obrot_punkt(p3, 90);
	cout << "Obrot odcinka o1 względem punktu p3 oraz 90 stopni: " << o1.p1.wspx() << " " << o1.p1.wspy() << "   " << o1.p2.wspx() << " " << o1.p2.wspy() << endl;
	cout << "Czy odcinek o1 oraz o6 są równoległe: " << czy_rownolegle(o1, o6) << endl;
	cout << "Czy odcinek o1 oraz o2 są równoległe: " << czy_rownolegle(o1, o2) << endl;
	cout << "Czy odcinek o4 oraz o7 są prostopadle: " << czy_prostopadle(o4, o7) << endl;
	cout << "Czy odcinek o2 oraz o4 są prostopadle: " << czy_prostopadle(o2, o4) << endl;
	cout << endl;

	cout << endl;

	cout << "Obwod trojkata t1: " << t1.obwod() << endl;
	cout << "Pole trojkata t2: " << t2.pole() << endl;
	cout << "Czy w trojkacie t1 jest punkt p7: " << t1.czy_w_trojkat(p7) << endl;
	cout << "Czy w trojkacie t3 jest punkt p2: " << t3.czy_w_trojkat(p2) << endl;
	t4.obrot_punkt(p4, 90);
	cout << "Obrot trojkata t4 względem punktu p4 oraz 90 stopni: " << t4.p1.wspx() << " " << t4.p1.wspy() << "   " << t4.p2.wspx() << " " << t4.p2.wspy() << "   " << t4.p3.wspx() << " " << t4.p3.wspy() <<endl;
	t1.symetria_punkt(p6);
	cout << "Symetria trojkata t1 względem punkt p6: " << t1.p1.wspx() << " " << t1.p1.wspy() << "   " << t1.p2.wspx() << " " << t1.p2.wspy() << "   " << t1.p3.wspx() << " " << t1.p3.wspy() <<endl;
	cout << "Czy trojkaty t1 oraz t2 sa rozlaczne: " << czy_rozlaczne(t1, t2) << endl; //nie wiem co tu nie działa
	cout << "Czy trojkaty t1 oraz t6 sa rozlaczne: " << czy_rozlaczne(t1, t6) << endl;
	cout << "Czy trojkaty t1 oraz t7 sa rozlaczne: " << czy_rozlaczne(t1, t7) << endl;
	cout << "Czy  t2 w t6 : " << czy_jeden_w_drugim(t2, t6) << endl;
}