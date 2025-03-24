#include "trojkat.hpp"
#include <cmath>
#define PI 3.14159
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

double dwa_pkt_dlugosc(punkt p1, punkt p2) {
	double res = 0;
	res = sqrt(pow(p2.wspx() - p1.wspx(), 2) + pow(p2.wspy() - p1.wspy(), 2));
	return res;
}

double trojkat::obwod() {
	double res = odcinek(p1, p2).odc_dlugosc() + odcinek(p2, p3).odc_dlugosc() + odcinek(p1, p3).odc_dlugosc();
	return res;
}

double trojkat::pole() {
	//double p = trojkat(p1, p2, p3).obwod() / 2;
	double res = 0.5 * abs(((p2.wspx() - p1.wspx()) * (p3.wspy() - p1.wspy())) - ((p2.wspy() - p1.wspy()) * (p3.wspx() - p1.wspx())) );
	return res;
}

bool trojkat::czy_w_trojkat(punkt p4) {
	//zmienne odpowiadajace polom 4 kolejnych trojkatow
	//p1 -> A, p2 -> B, p3 -> C
	double ABC = abs((p1.wspx() * (p2.wspy() - p3.wspy()) + p2.wspx() * (p3.wspy() - p1.wspy()) + p3.wspx() * (p1.wspy() - p2.wspy()))/2);
	// p4 -> S
	double SBC = abs((p4.wspx() * (p2.wspy() - p3.wspy()) + p2.wspx() * (p3.wspy() - p4.wspy()) + p3.wspx() * (p4.wspy() - p2.wspy()))/2);
	double ASC = abs((p1.wspx() * (p4.wspy() - p3.wspy()) + p4.wspx() * (p3.wspy() - p1.wspy()) + p3.wspx() * (p1.wspy() - p4.wspy()))/2);
	double ABS = abs((p1.wspx() * (p2.wspy() - p4.wspy()) + p2.wspx() * (p4.wspy() - p1.wspy()) + p4.wspx() * (p1.wspy() - p2.wspy()))/2);;
	//jezeli suma wszystkich 3 rowna orginalnej to punkt w trojaktcie
	return (ABC == SBC + ASC + ABS);
	
}

trojkat::trojkat(punkt p, punkt q, punkt r) : p1(p), p2(q), p3(r) {
	if (p1.wspx() == p2.wspx() and p1.wspx() == p3.wspx() and p2.wspx()== p3.wspx()) throw invalid_argument("z takich wspolrzednych nie mozna zbudowac trojkata (wsp x jednakowa w 3 punktach a moze byc max w 2");
	if (p1.wspy() == p2.wspy() and p1.wspy() == p3.wspy() and p2.wspy()== p3.wspy()) throw invalid_argument("z takich wspolrzednych nie mozna zbudowac trojkata (wsp y jednakowa w 3 punktach a moze byc max w 2");
	if (dwa_pkt_dlugosc(p1, p2) + dwa_pkt_dlugosc(p1, p3) < dwa_pkt_dlugosc(p2, p3) ) throw invalid_argument("z takich wspolrzednych nie mozna zbudowac trojkata (suma 2 bokow musi byc wieksza od 3");
	if (dwa_pkt_dlugosc(p2, p3) + dwa_pkt_dlugosc(p2, p1) < dwa_pkt_dlugosc(p3, p1) ) throw invalid_argument("z takich wspolrzednych nie mozna zbudowac trojkata (suma 2 bokow musi byc wieksza od 3");
	if (dwa_pkt_dlugosc(p1, p3) + dwa_pkt_dlugosc(p2, p3) < dwa_pkt_dlugosc(p1, p2) ) throw invalid_argument("z takich wspolrzednych nie mozna zbudowac trojkata (suma 2 bokow musi byc wieksza od 3");
}

trojkat::trojkat(const trojkat& t1) : p1(t1.p1), p2(t1.p2), p3(t1.p3) {}

void trojkat::translacja(wektor w1) {
	double wx = w1.wspx();
	double wy = w1.wspy();
	p1.set_x(wx + p1.wspx());
	p1.set_y(wy + p1.wspy());
	p2.set_x(wx + p2.wspx());
	p2.set_y(wy + p2.wspy());
	p3.set_x(wx + p3.wspx());
	p3.set_y(wy + p3.wspy());
}

void trojkat::obrot_punkt(punkt p, double alpha) {
	p1.obrot_punkt(p, alpha);
	p2.obrot_punkt(p, alpha);
	p3.obrot_punkt(p, alpha);
}

void trojkat::symetria_punkt(punkt p) {
	p1.symetria_punkt(p);
	p2.symetria_punkt(p);
	p3.symetria_punkt(p);
}


