#include "splayboom.h"
#include <ctime>
#include <cstdlib>

using namespace std;
void maak_boom(Zoekboom<int, string>&z, int i);
void maak_hardcoded_boom(Zoekboom<int, string> &z);
void maak_boom_volgorde(Zoekboom<int, string> &z, int n);
void reeks1();
void reeks2();

int main(){
	//reeks1();
	reeks2();
	return 0;
}

void reeks2(){
	Splayboom<int,string> s;
	maak_hardcoded_boom(s);
	s.teken("splaygraphvoor.txt");
	
	Zoekboom<int, string> * boomptr;
	Zoekknoop<int, string>* ouder;
	s.zoek(10, ouder, boomptr);
	
	s.splay(boomptr->get());
	s.teken("splaygraph.txt");
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
	Zoekknoop<int, string>* ouder;
	z.zoek(10, ouder, boomptr);
	z.rotatie(**boomptr, false);	
	cout << z.repOK() << endl;
	
//	std::cout << z.geefDiepte() << std::endl;
//	z.maakOnevenwichtig();
//	std::cout << z.geefDiepte() << std::endl;
	//z.maakEvenwichtig();
	//std::cout << z.geefDiepte() << std::endl;
	//std::cout << "wegschrijven naar graph 2" << std::endl;
	z.teken("graph2.txt");
	
	//z.schrijf_cout();
	cout << z.repOK();
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

