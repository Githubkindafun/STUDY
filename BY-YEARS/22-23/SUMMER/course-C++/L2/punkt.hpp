#pragma once //jezeli usłyszysz ze masz mnie dodac kilka razy dodaj mnie tylko raz
#include "wektor.hpp"

class punkt{
public:
    double x;
    double y;

    double wspx() const;

    void set_x(double new_x);

    double wspy() const;

    void set_y(double new_y);

    punkt(double px, double py);

	punkt(const punkt& p1);

    void translacja(wektor w1);

    void obrot_punkt(punkt p, double alpha);
    
    void symetria_punkt(punkt p) ;
    
};