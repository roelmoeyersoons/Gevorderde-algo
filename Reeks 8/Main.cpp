#include<iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include "boyermoore.h"
#include "woordstroom.h"
#include <map>

using namespace std;

void test();
void opgave();

int main(){
	//test();
	
	opgave();
	return 0;
}

void opgave(){
	ifstream t("bijbel.txt");
	stringstream buffer;
	buffer << t.rdbuf();
	const string &bijbel = buffer.str();
	
	Woordstroom w("kapitaal.txt");
	string huidig_woord;
	
	map<string,int> reeds_gelezen;
	
	w >> huidig_woord;
	while(!w.fail()){
		if(!reeds_gelezen.count(huidig_woord)){
			BoyerMoore b(huidig_woord);
			BoyerMoore b_naief(huidig_woord);
			
			long int aantal, aantal_naief;
			aantal = aantal_naief = 0;
			
			queue<int> q = b.zoek(bijbel,aantal);
			if(!q.empty()){
				queue<int> q_naief = b.zoekNaief(bijbel,aantal_naief);
				cout << huidig_woord << ": " << q.size() << " == "  << q_naief.size() << ", " << aantal << " vs " << aantal_naief << endl;	
			}
			reeds_gelezen[huidig_woord] = q.size();
			
			
		}
		w >> huidig_woord;
	}
	
	
}





void test(){
	BoyerMoore b("en ");
	long int hoeveel = 0;
	queue<int> res = b.zoek("een teksten balen stoppen", hoeveel); //of zoekNaief/ver
	while(!res.empty()){
		int pos = res.front();
		res.pop();
		cout << pos << " " << endl;
	}
	std::cout << "aantal testen:" << hoeveel << endl;
}
