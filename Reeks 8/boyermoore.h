#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H

#include <iostream>
#include <queue>
#include <string>
using std::string;
typedef unsigned int uint;
typedef unsigned char uchar;

class BoyerMoore{
public:
    BoyerMoore(const string&  _naald);
//de zoekfunctie geeft in teller het aantal karaktervergelijkingen terug
    std::queue<int> zoek(const string&  hooiberg,long int& teller);
	std::queue<int> zoekNaief(const string&  hooiberg,long int& teller);
	std::queue<int> zoekNaiever(const string&  hooiberg,long int& teller);
private:
	int rechtse_pos[256];
	string P;
	int grootte_P;
};
#endif
    
BoyerMoore::BoyerMoore(const string& P){
	this->P = P;
	int grootte = P.size();
	grootte_P = grootte;
	for(int i = 0; i < 256; i++){
		rechtse_pos[i] = -1;
	}
	
	for(int i = 0; i < grootte; i++){
		rechtse_pos[(uchar)P[i]] = i;
	}
//	for(int i = 0; i < 256; i++){
//		if(rechtse_pos[i] != grootte){
//			std::cout << i <<  " " << rechtse_pos[i] << std::endl;
//		}
//	}
}

std::queue<int> BoyerMoore::zoek(const string& T, long int& vgl){
	std::queue<int> res;
	int t_pos, p_pos, t_stop;
	p_pos = t_pos = grootte_P - 1;
	t_stop = T.size();
	while(t_pos < t_stop){
		vgl++;
		if(P[p_pos] == T[t_pos - grootte_P + 1 + p_pos]){
			//std::cout << P[p_pos] << "bij " << p_pos << " en " << t_pos - grootte_P + 1 + p_pos << std::endl;
			if(p_pos == 0){
				res.push(t_pos - grootte_P + 1 + p_pos);
				p_pos = grootte_P - 1;
				t_pos++;
			}
			else{
				p_pos--;
			}
		}
		else{
			int hoeveel_opschuiven = p_pos - rechtse_pos[(uchar)T[t_pos - grootte_P + 1 + p_pos]];
			if(hoeveel_opschuiven > 0){
				t_pos += hoeveel_opschuiven;
			}
			else{
				t_pos += 1;
			}
			p_pos = grootte_P - 1;
		}
	}
	
	return res;
}

std::queue<int> BoyerMoore::zoekNaief(const string& T, long int &vgl){
	std::queue<int> res;
	int t_pos, p_pos, t_stop;
	p_pos = t_pos = grootte_P - 1;
	t_stop = T.size();
	while(t_pos < t_stop){
		vgl++;
		if(P[p_pos] == T[t_pos - grootte_P + 1 + p_pos]){
			// std::cout << P[p_pos] << "bij " << p_pos << " en " << t_pos - grootte_P + 1 + p_pos << std::endl;
			if(p_pos == 0){
				res.push(t_pos - grootte_P + 1 + p_pos);
				p_pos = grootte_P - 1;
				t_pos++;
			}
			else{
				p_pos--;
			}
		}
		else{
			t_pos++;
			p_pos = grootte_P - 1;
		}
	}
	
	return res;
}

std::queue<int> BoyerMoore::zoekNaiever(const string&  T,long int& vgl){
	std::queue<int> res;
	int t_pos, p_pos, t_stop;
	t_pos = p_pos = 0;
	t_stop = T.size() - grootte_P + 1;
	while(t_pos < t_stop){
		vgl++;
		if(P[p_pos] == T[t_pos + p_pos]){
			if(p_pos == grootte_P - 1){
				res.push(t_pos);
				p_pos = 0;
				t_pos++;
			}
			p_pos++;
		}
		else{
			p_pos = 0;
			t_pos++;
		}
	}
	return res;
}


