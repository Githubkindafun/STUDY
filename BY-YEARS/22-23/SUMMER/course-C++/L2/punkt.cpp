#include "punkt.hpp"
#include <cmath>
#define PI 3.14159

double punkt::wspx() const{
	return x;
}

void punkt::set_x(double new_x){
	x = new_x;
}

double punkt::wspy() const{ //przeczytałem o const i tak to powinno wyglądac i działa!
	return y;
}

void punkt::set_y(double new_y){
	y = new_y;
}

punkt::punkt(double px, double py) : x(px), y(py) {}

punkt::punkt(const punkt& p1) : x(p1.wspx()), y(p1.wspy()) {}

void punkt::translacja(wektor w1) {
	double wx = w1.wspx();
	double wy = w1.wspy();
	x = x + wx;
	y = y + wy;
}
void punkt::obrot_punkt(punkt p, double alpha) {
	
	alpha = alpha * PI / 180;
	
	double tempX = x - p.wspx();
	double tempY = y - p.wspy();
	
	double newX = tempX * cos(alpha) - tempY * sin(alpha);
	double newY = tempX * sin(alpha) + tempY * cos(alpha);
	
	newX += p.wspx();
	newY += p.wspy();
	
	x = newX;
	y = newY;
}

void punkt::symetria_punkt(punkt p) {
	double tempX = x - p.wspx();
	double tempY = y - p.wspy();
	
	double newX = -tempX;
	double newY = -tempY;
	
	newX += p.wspx();
	newY += p.wspy();
	
	x = newX;
	y = newY; 
}

