#include<iostream>
#include<fstream>
#include "compgraaf.h"
#include<stack>
#include<vector>
#include<map>

#define HUIZEN 5000

using namespace std;
void maak_initiele_graaf_en_omgekeerde(Graaf<GERICHT>& g, string fnaam, Graaf<GERICHT>& omgekeerde);
//void maak_omgekeerde_graaf(const Graaf<GERICHT>& g, Graaf<GERICHT>& omgekeerde);
void bouw_postorder_nr(const Graaf<GERICHT>& g, stack<int>& postorder, vector<bool>& overlopen);
void postorder_rec(const Graaf<GERICHT>& g, int start, stack<int>& postorder, vector<bool>& overlopen);
GraafMetKnoopdata<GERICHT, int> maak_comp_graaf(const Graaf<GERICHT>& g, stack<int>& postordernr);
void postorder_rec2(const Graaf<GERICHT>& g, int start, int comp_knoop, vector<bool>& overlopen, GraafMetKnoopdata<GERICHT, int>& compgraaf, map<int,int>& compgraafMapping);

int main(){
	Graaf<GERICHT> gr(HUIZEN);
	Graaf<GERICHT> omgekeerde(HUIZEN);
	maak_initiele_graaf_en_omgekeerde(gr, "RSS.txt", omgekeerde);
	//maak_omgekeerde_graaf(gr, omgekeerde);
	
	stack<int> postordernr;
	vector<bool> v(HUIZEN, false);
	bouw_postorder_nr(gr, postordernr, v);
	
	GraafMetKnoopdata<GERICHT, int> compgraaf = maak_comp_graaf(gr, postordernr);
	compgraaf.teken("compgraaf.txt");
	return 0;
}

void maak_initiele_graaf_en_omgekeerde(Graaf<GERICHT>& g, string fnaam, Graaf<GERICHT>& omgekeerde){
	ifstream inv(fnaam);
	if(inv.is_open()){
		string woord;
		int teller = 0;
		int van, naar;
		while(inv >> woord){
			if(teller%8 == 1){
				van = stoi(woord);
			}		
			else if(teller%8 == 7){
				naar = stoi(woord);
				if(van != 0 && naar != 0){
					//cout << van << " " << naar << endl;
					g.voegVerbindingToe(van, naar);
					omgekeerde.voegVerbindingToe(naar, van);
					van = 0;
					naar = 0;
					//throw "wtf";
				}
			}
			teller++;	
		}
	}	
}

void bouw_postorder_nr(const Graaf<GERICHT>& g, stack<int>& postorder, vector<bool>& overlopen){
	int start = 744;
	postorder_rec(g, start, postorder, overlopen);
	for(int i = 0; i < g.aantalKnopen(); i++){
		if(!overlopen[i])
			postorder_rec(g, i, postorder, overlopen);
	}
}

void postorder_rec(const Graaf<GERICHT>& g, int start, stack<int>& postorder, vector<bool>& overlopen){
	map<int,int> burenlijst = g[start];
	overlopen[start] = true;
	for(map<int,int>::const_iterator it = burenlijst.begin(); it != burenlijst.end(); it++){
		if(!overlopen[it->first]){
			postorder_rec(g, it->first, postorder, overlopen);
			
		}
	}
	postorder.push(start);
}

GraafMetKnoopdata<GERICHT, int> maak_comp_graaf(const Graaf<GERICHT>& g, stack<int>& postordernr){
	GraafMetKnoopdata<GERICHT, int> compgraaf;
	map<int, int> compgraafMapping;
	vector<bool> gebruikt(HUIZEN, false);
	
	while(!postordernr.empty()){
		int huidig = postordernr.top();
		postordernr.pop();
		if(!gebruikt[huidig]){
			gebruikt[huidig] = true;
			int comp_knoop = compgraaf.voegKnoopToe(huidig);
			cout << endl << "nieuwe comp knoop aangemaakt " << comp_knoop << "     " << huidig << endl; 
			compgraafMapping[huidig] = comp_knoop;
			postorder_rec2(g, huidig, comp_knoop, gebruikt, compgraaf, compgraafMapping);
		}
	}
	return compgraaf;
}

void postorder_rec2(const Graaf<GERICHT>& g, int start, int comp_knoop, vector<bool>& overlopen, GraafMetKnoopdata<GERICHT, int>& compgraaf, map<int,int>& compgraafMapping){
	map<int,int> burenlijst = g[start];

	for(map<int,int>::const_iterator it = burenlijst.begin(); it != burenlijst.end(); it++){
		if(!overlopen[it->first]){
			//cout << it->first << "\t";
			overlopen[it->first] = true;
			compgraafMapping[it->first] = comp_knoop;
			postorder_rec2(g, it->first, comp_knoop, overlopen, compgraaf, compgraafMapping);
		}
		else{
			//oude -> huidig
			if(compgraafMapping[start] != compgraafMapping[it->first]){
				if(compgraaf.verbindingsnummer(compgraafMapping[start], compgraafMapping[it->first]) == -1)
					compgraaf.voegVerbindingToe(compgraafMapping[start], compgraafMapping[it->first]);
			}
			
		}
	}
}


void maak_omgekeerde_graaf(const Graaf<GERICHT>& g, Graaf<GERICHT>& omgekeerde){
	//via burenlijsten
}
