#ifndef _UTNA_H
#define _UTNA_H
#include"thompsonna.h"
#include<vector>
#include<set>
#include<stack>

typedef std::set<int> Staat;

class UitbreidingTNA: public ThompsonNA {
	public: 
		UitbreidingTNA(const Regexp& r);
		
		Staat bepaalESluiting(const Staat& bits) const;
		Staat s(const Staat& bits, uchar ch);
		
	private:
		vector<vector<vector<Verbinding>>> index; 
		//map alle bits op alle mogelijke chars op alle verbindingen daarvoor
};

UitbreidingTNA::UitbreidingTNA(const Regexp& r)	: ThompsonNA(r) {
	//ThompsonNA::verbind(0,0);
	for(int i = 1; i < 255; i++){
		ThompsonNA::verbind(0,0,i);
	}
	
	//maak index
	index.resize(ThompsonNA::geefAantalStatenbits());
	for(int i = 0; i < index.size(); i++){
		index[i].resize(256);
	}
	for(const Verbinding& v : overgang){
		index[v.geefBron()][v.geefKarakter()].push_back(v);
	}
}

Staat UitbreidingTNA::bepaalESluiting(const Staat& bits)const {
	Staat res;
	std::stack<int> DEZstack;
	for(auto it = bits.begin(); it != bits.end(); it++){
		res.insert(*it);
		DEZstack.push(*it);
	}
	while(!DEZstack.empty()){
		int bit = DEZstack.top();
		DEZstack.pop();
		for(const Verbinding& verbinding : index[bit][epsilon]){
			std::cout << "olo"<< std::endl;
			res.insert(verbinding.geefDoel());
			DEZstack.push(verbinding.geefDoel());
		}
	}
	return res;
}

Staat s(const Staat& bits, uchar ch){
	Staat res;
	for(int i : bits){
		for(const Verbinding& v : index[i][ch]){
			res.insert(v.geefDoel());
		}
	}
	return res;
}
#endif
