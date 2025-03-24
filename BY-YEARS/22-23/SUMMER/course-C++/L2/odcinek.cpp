#include "odcinek.hpp"
#include <cmath>
#define PI 3.14159
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

    double odcinek::odc_dlugosc() {
        double res = 0;
        res = sqrt(pow(p2.wspx() - p1.wspx(), 2) + pow(p2.wspy() - p1.wspy(), 2));
        return res;
    }

    bool odcinek::punkt_w(punkt p3) {
        if(p2.wspx()*p3.wspy() + p1.wspx()*p2.wspy() + p3.wspx()*p1.wspy() - p1.wspx()*p3.wspy() - p2.wspx()*p1.wspy() - p3.wspx()*p2.wspy() == 0) {
            if(p3.wspx() >= min(p1.wspx(), p2.wspx()) && p3.wspx() <= max(p1.wspx(), p2.wspx())) {
                if(p3.wspy() >= min(p1.wspy(), p2.wspy()) && p3.wspy() <= max(p1.wspy(), p2.wspy())) {
                    return true;
                }
            }
        }
        return false;
    }

    odcinek::odcinek(punkt p, punkt q) : p1(p), p2(q) {
    if (p1.wspx() == p2.wspx() and p1.wspy() == p2.wspy()) throw invalid_argument("nie mozna utworzyć odcinka o długości 0");
    }

	odcinek::odcinek(const odcinek& o1 ) : p1(o1.p1), p2(o1.p2) {}

    void odcinek::translacja(wektor w1) {
        double wx = w1.wspx();
        double wy = w1.wspy();
        p1.set_x(wx + p1.wspx());
        p1.set_y(wy + p1.wspy());
        p2.set_x(wx + p2.wspx());
        p2.set_y(wy + p2.wspy());
    }

    void odcinek::obrot_punkt(punkt p, double alpha) {
        p1.obrot_punkt(p, alpha);
        p2.obrot_punkt(p, alpha);
    }
    
    void odcinek::symetria_punkt(punkt p) {
        p1.symetria_punkt(p);
        p2.symetria_punkt(p);
    }

