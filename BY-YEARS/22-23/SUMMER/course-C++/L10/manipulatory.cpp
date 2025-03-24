#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "manipulatory.hpp"

using namespace std;

istream& clearline(istream &is) {

	string input;
	getline(is, input, '\n'); // bierzemy cały input :D
	//cout << input;
	return is;
}


istream& operator >> (istream &is, const miss& val)  {

	for(int i = 0; i < val.amount; i++) {
		char c;
		is.get(c);			//EOF == end of file
		if(c == '\n' || c == EOF) return is;
	}
	return is;
}
	
miss::miss(int x) : amount(x) {}


ostream& comma(ostream &os) {
	
	string output = ",  ";
	os.clear();
	os << output;

	return os;
}

ostream& colon(ostream &os) {
	
	string output = ":  ";
	os.clear();
	os << output;

	return os;
}


ostream& operator << (ostream &os, const index& val)  {

	string res = "";
	res.append("[");
	for(int i = 0; i < val.space; i++) {
		res.append(" ");
	}
	//cout << endl << res << endl;
	string str_num;
	str_num= to_string(val.num);
	res.append(str_num);
	res.append("]");

	os << res;
	return os;
	}


index::index(int x, int w) : num(x), space(w) {}
	

