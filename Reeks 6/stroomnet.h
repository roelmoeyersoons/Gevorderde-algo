#ifndef __STROOMNETOPGELOST_H
#define __STROOMNETOPGELOST_H
#include "graaf.h"
#include <cassert>
#include <iostream>
#include <functional>
#include <queue>
using std::vector;
using std::function;
using std::ostream;
using std::cerr;
using std::queue;
using std::stack;
template< class T>
class Stroomnetwerk;


template< class T >
class Pad:public std::vector< int >{
public:
    T geefCapaciteit() const;
    void zetCapaciteit(const T& _capaciteit);
protected:
    T capaciteit;
};
template< class T >
T Pad<T>::geefCapaciteit() const{
    return capaciteit;
}

template< class T >
void Pad<T>::zetCapaciteit(const T& _capaciteit){
    capaciteit=_capaciteit;
}

template< class T >
ostream& operator<<(ostream& os,const Pad<T>& p){
    os<<"Capaciteit= "<<p.geefCapaciteit()<<" :: ";
    if (p.size() > 0){
        os<<p[0];
    }
    for (int i=1; i<p.size(); i++ ){
        os<<"->"<<p[i];
    }
    os<<"\n";
}

/**********************************************************************

   Class:Vergrotendpadzoeker
   
   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 een vergrotend pad teruggeeft.
                 Vermits ze hoort bij Ford-Fulkerson zal een object van deze klasse
                 elke keer een ander pad geven als het stroomnetwerk opgegeven
                 in de constructor verandert.

   
***************************************************************************/
template <class T>
class VergrotendpadzoekerTemplate{
	public:
		virtual Pad<T> geefVergrotendPad() = 0;
};

template <class T>
class Vergrotendpadzoeker : VergrotendpadzoekerTemplate<T> {
public:
    Vergrotendpadzoeker(const Stroomnetwerk<T>& stroomnetwerk):
                    q(stroomnetwerk),v(stroomnetwerk.van),v2(stroomnetwerk.naar),
                    l(q.aantalKnopen()), m(q.aantalKnopen(),false){};
    Pad<T> geefVergrotendPad();
protected:
    virtual void foo(int t, int x,Pad<T>& p);
    const Stroomnetwerk<T>& q;
    vector<int> l;
    vector<bool> m;
    int v,v2;
};


template <class T>
Pad<T> Vergrotendpadzoeker<T>::geefVergrotendPad(){
    for (int i=0; i<m.size(); i++ ){
        m[i]=false;
    }
    Pad<T> p;
    assert(v != v2);
    foo(v,0,p);
    assert(p.size()!=1);
    if (p.size() > 1){
        T g=*q.geefTakdata(p[0],p[1]);
        for (int i=2; i<p.size(); i++ ){
            T ychg=*q.geefTakdata(p[i-1],p[i]);
            if (ychg<g)
                g=ychg;
        }
        p.zetCapaciteit(g);
    }
    return p;
}

template <class T>
void Vergrotendpadzoeker<T>::foo(int t,int x, Pad<T>& p){
    m[t]=true;
    const typename GraafMetTakdata<GERICHT,T>::Burenlijst& a=q[t];
    int ychx=x+1;
    for (typename GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it=a.begin();
                it!=a.end();it++){
        int u=it->first;
        if (*q.geefTakdata(t,u)> 0){
            if (it->first==v2 && ychx+1 > p.size()){
                l[v2]=t;
                p.resize(ychx+1);
                int ychf=v2;
                int i=ychx;
                while (ychf!=v){
                    p[i--]=ychf;
                    ychf=l[ychf];
                }
                p[0]=ychf;
            }
            else if(u!=v2 && !m[u]){
                l[u]=t;
                foo(u,ychx,p);
            }
        }
    }
}

template <class T>
class Kortstepadzoeker : VergrotendpadzoekerTemplate<T> {
	public:
		Kortstepadzoeker(const Stroomnetwerk<T>& stroomnetwerk):
                    netwerk(stroomnetwerk),van(stroomnetwerk.van),naar(stroomnetwerk.naar),
                    grootte(stroomnetwerk.aantalKnopen()){};
		Pad<T> geefVergrotendPad();
	protected:
		const Stroomnetwerk<T>& netwerk;
		int van, naar, grootte;
};


template<class T>
Pad<T> Kortstepadzoeker<T>::geefVergrotendPad(){
	
//	vector<pair<int, int>> queue;
//	int teller = 0;
//	
//	// ik push altijd <vectorindex ouder, mijn knoopnr>
//	for(GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it = netwerk[nu].begin(); it != netwerk[nu].end(); it++){
//		pair<int,int> p(0, it->first);
//		queue.push_back(p);
//	}
//	bool gevonden = false;
//	while(!gevonden || !queue.empty()){
//		pair<int, int>& huidig = queue[teller];
//		if(huidig.second == naar){
//			gevonden = true;
//		}
//		else{
//			for(GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it = netwerk[pair->second].begin(); it != netwerk[nu].end(); it++){
//				pair<int,int> p(teller, it->first);
//				queue.push_back(p);
//			}
//		}
//		teller++;
//	}
//	if(gevonden)
//		
//	else{
//		
//	}


	Pad<T> res;
	vector<bool> bezocht(grootte, false);
	vector<int> ouders(grootte, 0);
	
	//initieel instellen
	ouders[van] = -1;
	bezocht[van] = true;
	
	queue<int> BEZ;
	BEZ.push(van);
	bool gevonden = false;
	
	while(!gevonden || !BEZ.empty()){
		int huidig = BEZ.front();
		BEZ.pop();
		if(huidig == naar){
			gevonden = true;
		}
		else{
			for(typename GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it = netwerk[huidig].begin(); it != netwerk[huidig].end(); it++){
				if(!bezocht[it->first]){
					bezocht[it->first] = true;
					ouders[it->first] = huidig;
					BEZ.push(it->first);
				}				
			}
		}
	}
	std::cout << "gevonden:" << gevonden << std::endl;
	if(gevonden){
		int index = naar;
		stack<int> juisteVolgorde;
		while (index != -1){
			std::cout << "ik: " << index << " ouder" << ouders[index] << std::endl;
			juisteVolgorde.push(index);
			index = ouders[index];
		}
		
		int ouder = juisteVolgorde.top();
		juisteVolgorde.pop();
		
		res.push_back(ouder);
		res.zetCapaciteit(999999999);
		while(!juisteVolgorde.empty()){
			int kind = juisteVolgorde.top();
			juisteVolgorde.pop();
			
			res.push_back(kind);
			const T * capaciteit = netwerk.geefTakdata(ouder, kind);
			if(*capaciteit < res.geefCapaciteit()){
				res.zetCapaciteit(*capaciteit);
			}
			ouder = kind;
		}
	}
	for(auto ele: res){
		std::cout << ele << "  ";
	}
	std::cout << std::endl;
	return res;
}

/**********************************************************************

   Class: Stroomnetwerk
   
   beschrijving: Een stroomnetwerk gaat uit van een gewogen gerichte graaf
                 die in de constructor wordt opgegeven
   
***************************************************************************/

template <class T>//T = takdata
class Stroomnetwerk:public GraafMetTakdata<GERICHT, T >{
	public:
		//leeg netwerk; alleen aantal knopen en van en naar gegeven.
		Stroomnetwerk(int grootte, int _van, int _naar);
		//Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
		//anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
		//van Graaf op en krijgen we een lege graaf.
		//Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr):
		//                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(0),naar(1){};
		Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar);
		Stroomnetwerk(const Stroomnetwerk<T>& gr);
		Stroomnetwerk(Stroomnetwerk<T>&& gr);
		
		Stroomnetwerk<T> geefStroom();
		T geefCapaciteit();
		
		int van,naar;
		virtual void teken(const char* bestandsnaam) const;
		
		
		Stroomnetwerk<T>& operator-=(const Pad<T>&);
		Stroomnetwerk<T>& operator+=(const Pad<T>&);
	protected:
	    virtual std::string knooplabel(int i) const;
};
template <class T>
Stroomnetwerk<T>::Stroomnetwerk(int grootte,int _van,int _naar):
                    Graaf<GERICHT>(grootte),GraafMetTakdata<GERICHT, T>(grootte),van(_van),naar(_naar){}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const GraafMetTakdata<GERICHT ,T>& gr,int _van,int _naar):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(_van),naar(_naar){}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(const Stroomnetwerk<T>& gr):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr),van(gr.van),naar(gr.naar){}

template <class T>
Stroomnetwerk<T>::Stroomnetwerk(Stroomnetwerk<T>&& gr):Stroomnetwerk(0,0,0){
    swap(this->burenlijsten,gr.burenlijsten);
    swap(this->vrijgekomenVerbindingsnummers,gr.vrijgekomenVerbindingsnummers);
    std::swap(this->hoogsteVerbindingsnummer,gr.hoogsteVerbindingsnummer);
    swap(this->takdatavector,gr.takdatavector);
    std::swap(this->van,gr.van);
    std::swap(this->naar,gr.naar);
}


// EIGEN CODE

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator-=(const Pad<T>& p){
	int oude = p[0];
	for(int i = 1; i < p.size(); i++){
		int huidig = p[i];
		
		int* gewicht = this->geefTakdata(oude, huidig);
		//zou nooit null mogen zijn eig
		*gewicht -= p.geefCapaciteit();
		
		
		int *omgekeerde = this->geefTakdata(huidig, oude);
		if(omgekeerde == nullptr){
			this->voegVerbindingToe(huidig, oude, p.geefCapaciteit());
		}
		else{
			*omgekeerde += p.geefCapaciteit();
		}
		
		oude = huidig;	
	}
	return *this;
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator+=(const Pad<T>& p){
	int oude = p[0];
	for(int i = 1; i < p.size(); i++){
		int huidig = p[i];
		int nettoStroom = p.geefCapaciteit();
		
//		int* gewichtTerug = this->geefTakdata(huidig, oude);
//		if(gewichtTerug != nullptr){
//			if(*gewichtTerug > nettoStroom){
//				*gewichtTerug -= nettoStroom;
//				nettoStroom = 0;
//			}
//			else{
//				nettoStroom -= *gewichtTerug;
//				this->verwijderVerbinding(huidig, oude);
//			}
//		}
		
		if(nettoStroom > 0){
			int *gewicht = this->geefTakdata(oude, huidig);
			if(gewicht == nullptr)
				this->voegVerbindingToe(oude, huidig, nettoStroom);
			else{
				*gewicht += nettoStroom;
			}
		}
		oude = huidig;
	}
	return *this;
}




// END

template <class T>
Stroomnetwerk<T> Stroomnetwerk<T>::geefStroom(){
    Stroomnetwerk<T> oplossing(this->aantalKnopen(),van,naar);
    Stroomnetwerk<T> restnetwerk(*this);
    Kortstepadzoeker<T> vg(restnetwerk);
    Pad<T> vergrotendpad=vg.geefVergrotendPad();
    while(vergrotendpad.size() !=0 ){
        restnetwerk-=vergrotendpad;
        oplossing+=vergrotendpad;
        vergrotendpad=vg.geefVergrotendPad();
    }
    restnetwerk.teken("restnet.dot");
    return oplossing;
}
template <class T>
T Stroomnetwerk<T>::geefCapaciteit(){
    T som=0;
    for (typename GraafMetTakdata<GERICHT,T>::Burenlijst::const_iterator it=this->burenlijsten[van].begin();
                it!=this->burenlijsten[van].end();it++)
        som+=this->takdatavector[it->second];
    return som;
}

template <class T>
void Stroomnetwerk<T>::teken(const char* bestandsnaam) const{
    std::ofstream uit(bestandsnaam);
    assert(uit);
    std::string pijl;
        uit<<"digraph {\n";
        pijl="->";
    uit<<"{ rank=source; "<<knooplabel(van)<<" }\n";
    uit<<"{ rank=sink; "<<knooplabel(naar)<<" }\n";
    for (int k=0; k<this->aantalKnopen(); k++){
        if (this->burenlijsten[k].empty())
            uit<<knooplabel(k)<<";\n";
        else{
        for (auto& p: this->burenlijsten[k])
                uit<<knooplabel(k)<<" "<<pijl<<" "
                   <<knooplabel(p.first)<<this->taklabel(p.second)<<";\n";
        };
    };
    uit<<"}";
    uit.close();
}

template <class T>
std::string Stroomnetwerk<T>::knooplabel(int i) const{
    std::ostringstream uit;
    if (i==van)
        uit<<'"'<<i<<":P\"";
    else if (i==naar)
        uit<<'"'<<i<<":V\"";
    else
        uit<<i;
    return uit.str();
}



#endif
