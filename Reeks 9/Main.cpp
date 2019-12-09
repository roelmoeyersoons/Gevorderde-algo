#include "regexp11.h"
#include "uitbreidingtna.h"
//#include "da.h"
#include <string>
#include<set>
using namespace std;

int main(){
	Regexp r("a|b");
//	ThompsonNA na(r);
	UitbreidingTNA na(r);
	
//	na.teken("na.txt");
//	una.teken("una.txt");
	std::set<int> s;
	s.insert(0);
	na.bepaalESluiting(s);
	//DAfromNA da(na);
	
//	vector<int> huidigeStatussen;
//	string tekst = ...;
//	int i = 0;
//	bool gevonden = false;
//	while(i < test.size() && gevonden == false){
//		for(int j = 0; i < huidigeStatussen.size(); j++){
//			int volgende = DA.check(v[j], tekst[i]);
//			if(volgende != -1){
//				v[j] = volgende;
//			}
//			else{
//				//verwijder v[j] uit tabel maar is traag, gelinkte lijst beter?
//				//of maak nieuwe lijst aan en overschrijf
//			}
//			if(da.isEindtoestand(volgende)){
//				gevonden = true;
//			}
//			v.push_back(DA.begin(tekst[i]));
//		}	
//	}
//	if(gevonden){
//		cout << tekst;
//	}
	
	
	//na.teken("na.txt");
	return 0;
}
