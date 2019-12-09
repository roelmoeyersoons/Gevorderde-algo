#ifndef eiers
#define eiers


#include<vector>
#include<iomanip>
#include<iostream>
#include "zoekboom.h"

using namespace std;

class Eieren{
	public:
		Eieren(int v, int e);
		void print_kost();
		void print_wortels();
		int geef_kost(int v, int e);
		Zoekboom<int, int> geef_boom(int v, int e);
	private:
		int eieren(int v, int e);
		void init_kost();
		void init_wortels();
		void voeg_kinderen_toe(Zoekboom<int,int>& ouder, bool links, int v, int e, int extra);
		vector<vector<int>> kost;
		vector<vector<int>> wortels;
		int e, v;
		
		
};

Eieren::Eieren(int v, int e){
	this->e = e;
	this->v = v;
	init_kost();
	init_wortels();
	//start
	
	for(int v_huidig = 2; v_huidig < v + 1; v_huidig++){
		for(int e_huidig = 2; e_huidig < e + 1; e_huidig++){
			int min = 99999999;
			int minwortel = 9999999;
			for(int wortel = 1; wortel < v_huidig; wortel++){
				
				// bepaal maxdiepte van links en rechts
				
				//links: #v daar < wortel en #e = e_huidig - 1				
				int links = wortel == 1? 1 : kost[wortel - 1][e_huidig - 1];
				//rechts: #v daar = v_max - wortel denk ik, e blijft
				int rechts = wortel == v_huidig - 1? 1 : kost[v_huidig - wortel][e_huidig];
				
				if(max(links, rechts) < min){
					min = max(links, rechts);
					minwortel = wortel;
				}
			}
			kost[v_huidig][e_huidig] = min + 1;
			wortels[v_huidig][e_huidig] = minwortel;
		}
	}
	
	//print_tabel(kost);
	//return kost[v][e];
}

int Eieren::geef_kost(int v, int e){
	return kost[v][e];
}


void Eieren::init_wortels(){
	wortels = vector<vector<int>>(v + 1);
	for(int i = 1; i < v + 1; i++){
		wortels[i] = vector<int>(e+1);
	}
	for(int i = 1; i < wortels[1].size(); i++){
		wortels[1][i] = 1;
	}
	for(int i = 1; i < wortels.size(); i++){
		wortels[i][1] = 1;
	}
}


void Eieren::init_kost(){
	kost = vector<vector<int>>(v + 1);
	for(int i = 1; i < v + 1; i++){
		kost[i] = vector<int>(e+1);
	}
	for(int i = 1; i < kost[1].size(); i++){
		kost[1][i] = 1;
	}
	for(int i = 1; i < kost.size(); i++){
		kost[i][1] = i;
	}
}

void Eieren::print_kost(){
	for(int i = 0; i < kost[1].size(); i++){
		cout << setw(4) << i;
	}
	cout << endl;
	for(int i = 1; i < kost.size(); i++){
		cout << setw(4) << i;
		for (int j = 1; j < kost[1].size(); j++){
			cout << setw(4) << kost[i][j];
		}
		cout << endl;
	}
}

void Eieren::print_wortels(){
	for(int i = 0; i < wortels[1].size(); i++){
		cout << setw(4) << i;
	}
	cout << endl;
	for(int i = 1; i < wortels.size(); i++){
		cout << setw(4) << i;
		for (int j = 1; j < wortels[1].size(); j++){
			cout << setw(4) << wortels[i][j];
		}
		cout << endl;
	}
}

Zoekboom<int, int> Eieren::geef_boom(int v, int e){
	if(v <= this->v && e <= this->e){
		
		Zoekboom<int,int> boom = make_unique<Zoekknoop<int,int>>(wortels[v][e],1);
		//links: er zijn dan wortels[v][e] -1 verdiepingen kleiner en e -1 eieren
		//rechts: er zijn v - wortels[v][e] verdiepingen groter en e eieren
		//		MAAR rechts: + wortel toevoegen (relatieve deelboom)
		
		//boom, links?, #v, #e, extra factor indien rechts
		voeg_kinderen_toe(boom, true, wortels[v][e] - 1, e - 1, 0);
		voeg_kinderen_toe(boom, true, v - wortels[v][e], e, wortels[v][e]);
		
		return std::move(boom);
	}
	throw "v of e te groot";
}

void Eieren::voeg_kinderen_toe(Zoekboom<int,int>& ouder, bool links, int v, int e, int extra){
	Zoekboom<int, int>* kind;
	if(links){
		ouder->links = make_unique<Zoekknoop<int,int>>();
		kind = &ouder->links;
	}
}

#endif
