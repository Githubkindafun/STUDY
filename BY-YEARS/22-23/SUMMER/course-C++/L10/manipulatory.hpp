#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

istream& clearline(istream &is);

struct miss {

	int amount;
	
	friend istream& operator>>(istream &is, const miss& val);
	public:
		miss(int x);
};

ostream& comma(ostream &os);

ostream& colon(ostream &os);

struct index {

	int num;
	int space;
	
	friend ostream& operator << (ostream &os, const index& val);
	public:
		index(int x, int w);
};
