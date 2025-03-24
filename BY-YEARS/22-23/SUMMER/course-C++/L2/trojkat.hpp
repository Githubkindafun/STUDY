#pragma once 
#include "wektor.hpp"
#include "punkt.hpp"
#include "odcinek.hpp"

double dwa_pkt_dlugosc(punkt p1, punkt p2);

class trojkat{
public:
    punkt p1;
    punkt p2;
    punkt p3;

    double obwod();

    double pole();

    bool czy_w_trojkat(punkt p4);

    trojkat(punkt p, punkt q, punkt r);

	trojkat(const trojkat& t1);
    
	void translacja(wektor w1);

    void obrot_punkt(punkt p, double alpha);

    void symetria_punkt(punkt p);
};