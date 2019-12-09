#include "splayboom.h"
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;
void maak_boom(Zoekboom<int, string>&z, int i);
void maak_hardcoded_boom(Zoekboom<int, string> &z);
void maak_boom_volgorde(Zoekboom<int, string> &z, int n);
void reeks1();
void reeks2();
void shakespeare();

int main(){
	//reeks1();
	//reeks2();
	shakespeare();
	return 0;
}

void shakespeare(){
	Splayboom<string, int> s;
	ifstream inv("Shakespeare.txt");
	if(inv.is_open()){
		string text;
		while(inv >> text){
			//cout << "zoeken";
			zoekKnoop<string, int>* knoop = s.zoek(text);
			//cout << "gezocht";
			if(knoop != nullptr){
				knoop->data++;
			}
			else{
				s.voegtoe(text, 1, false);
			}
		}
		s.zoek("www.gutenberg.org");
		//s.teken("splayshakespeare.txt");
	}
	zoekKnoop<string, int>* knoop = s.zoek("www.gutenberg.org");
	cout << knoop->data;
	
	
}

void reeks2(){
	Splayboom<int,string> s;
	maak_boom_volgorde(s, 100);
	s.teken("splayvoor.txt");
	cout << "hallo";
	zoekKnoop<int ,string> *z = s.zoek(50);
	
	cout << z->sleutel;
	s.teken("splayna.txt");
}

void reeks1(){
	std::srand(std::time(nullptr));
	Zoekboom<int,string> z;
	maak_hardcoded_boom(z);
	//maak_boom(z,514);
	
	//z.schrijf(cout);
	//z.teken("graph.txt");
	//z.schrijf_cout();
	cout << "\n\n--------------------------------------\n\n" << endl;
	//z.rotatie(*z, false);
	//Zoekboom<int,string> x = z;
	//x.schrijf(cout);*/
	
	
	Zoekboom<int, string> * boomptr;
	zoekKnoop<int, string>* ouder;
	z.zoek(10, ouder, boomptr);	
	
//	std::cout << z.geefDiepte() << std::endl;
//	z.maakOnevenwichtig();
//	std::cout << z.geefDiepte() << std::endl;
	//z.maakEvenwichtig();
	//std::cout << z.geefDiepte() << std::endl;
	//std::cout << "wegschrijven naar graph 2" << std::endl;
	z.teken("graph2.txt");

}

void maak_boom(Zoekboom<int, string> &z, int n){
	for(int i = 0; i < n; i++){
		z.voegtoe(std::rand() % 100000, "data", false);
	}
}
void maak_boom_volgorde(Zoekboom<int, string> &z, int n){
	for(int i = 0; i < n; i++){
		z.voegtoe(i, "data", false);
	}
}

void maak_hardcoded_boom(Zoekboom<int, string> &z){
	z.voegtoe(4, "hallo4", true);
	z.voegtoe(2, "hallo2", true);
	z.voegtoe(1, "hallo1", true);
	z.voegtoe(3, "hallo3", true);
	z.voegtoe(5, "hallo5", true);
	z.voegtoe(7, "hallo6", true);
	for(int i = 0; i < 7; i++){
		z.voegtoe(i+10, "hallo", true);
	}
}

