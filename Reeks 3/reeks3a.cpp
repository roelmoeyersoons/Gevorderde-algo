
#include<iostream>
#include<vector>
#include<iomanip>

#include "eieren.h"

using namespace std;

int main(){
	Eieren eieren(50,20);
	
	cout << eieren.geef_kost(30,12) << endl;
	
	eieren.print_kost();
	//Zoekboom<int, int> test = eieren.geef_boom(10,15);
	
	//cout << test->sleutel;
	
	return 0;
}


