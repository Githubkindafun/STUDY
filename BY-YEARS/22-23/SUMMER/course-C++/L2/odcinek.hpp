#pragma once //jezeli usłyszysz ze masz mnie dodac kilka razy dodaj mnie tylko raz
#include "wektor.hpp"
#include "punkt.hpp"

class odcinek{
public:
    punkt p1;
    punkt p2;

    double odc_dlugosc();

    bool punkt_w(punkt p3);

    odcinek(punkt p, punkt q);

	odcinek(const odcinek& o1 );
    
    void translacja(wektor w1);

    void obrot_punkt(punkt p, double alpha);
    
    void symetria_punkt(punkt p);

};