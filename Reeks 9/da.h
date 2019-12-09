#include "thompsonna.h"
#include<vector>
#include<stack>
#include<set>
#include<map>

typedef std::set<int> Staat;
 
class DAfromNA{
	public:
		DAfromNA(const UitbreidingTNA&);
	private:
		vector<vector<int>> overgangstabel;
		//vector<int> toestanden;
};

DAfromNA::DAfromNA(const UitbreidingTNA& na){
	Staat x;
	begin.insert(0);
	Staat begin = na.bepaalESluiting(begin);
	
	int volgnr = 0;
	std::stack<Staat> DEZstaten;
	std::map<Staat, int> gevondenStaten;
	gevondenStaten.insert(std::pair<Staat,int>(sluiting, volgnr++));
	DEZstaten.push(gevondenStaten);
	
	while(!DEZstaten.empty()){
		set<int> staat = DEZstaten.top();
		DEZstaten.pop();
		for(int i = 1; i < 256; i++){
			Staat volgendeStaat = na.bepaalESluiting(na.s(staat,i))
			if(!gevondenStaten.count(volgendeStaat)){
				gevondenStaten.insert(std::pair<Staat,int>(volgendeStaat, volgnr++))
				DEZstaten.push(volgendeStaat);
			}
			overgangstabel[gevondenStaten[staat], i] = gevondenStaten[volgendeStaat];
		}
	}
	
}


