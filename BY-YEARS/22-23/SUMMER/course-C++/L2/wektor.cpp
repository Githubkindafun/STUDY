#include "wektor.hpp"

double wektor::wspx() {
	return x;
}

void wektor::set_x(double new_x){
	x = new_x;
}

double wektor::wspy() {
	return y;
}

void wektor::set_y(double new_y){
	y = new_y;
}

wektor::wektor(double px, double py) : x(px), y(py) {}
