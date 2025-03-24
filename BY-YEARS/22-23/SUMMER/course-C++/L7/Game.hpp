#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h> //for rand


namespace g {
	using namespace std;
	
	const string convert[9] = { "A1", "A2", "A3", "B1", "B2", "B3", "C1", "C2", "C3" };
	
		class Game  {
		    char grid[3][3];
		 
		    void Generate(); 
		
		    void Win(); 
		
		public:
		    Game(); 
		    
	};
}





