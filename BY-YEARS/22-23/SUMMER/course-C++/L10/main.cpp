#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "manipulatory.hpp"

using namespace std;

/*
istream& clearline(istream &is) {

	string input;
	getline(is, input, '\n'); // bierzemy cały input :D
	//cout << input;
	return is;
}

struct miss {

	int amount;
	
	friend istream& operator>>(istream &is, const miss& val)  {
	
		for(int i = 0; i < val.amount; i++) {
			char c;
			is.get(c);			//EOF == end of file
			if(c == '\n' || c == EOF) return is;
		}
		return is;
	}
	public:
		miss(int x) : amount(x) {}
};

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

struct index {

	int num;
	int space;
	
	friend ostream& operator << (ostream &os, const index& val)  {

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

	public:
		index(int x, int w) : num(x), space(w) {}
	
};
*/

int main() {
	ifstream inputFile("test.txt");
    if (!inputFile.is_open()) {
        cerr << "Failed to open the input file." << std::endl;
        return 1;
    }
	
    string data;
    inputFile >> clearline;
	inputFile >> miss(5);
	//getline(inputFile, data, {});
	//cout << data;
	//cout << '\n';
	cout << comma << "test \n" << colon  << "test \n";

	vector<string> container;
	string tmp = "";
	
	while(getline(inputFile, data)) {
		container.push_back(data);
	}
	
	for(int i = 0; i < container.size(); i++) {
		cout << index(i, 0) << container[i] << '\n';
	}
	
    inputFile.close();
}