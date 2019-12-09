#ifndef __Zoekboom_H
#define __Zoekboom_H
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>

using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::unique_ptr;
using std::cerr;
using std::pair;
using std::string;
/**********************************************************************

   Class: Zoekboom
   
   beschrijving: Binaire Zoekboom waarin duplicaatsleutels wel of niet zijn toegestaan.
   
***************************************************************************/

template <class Sleutel,class Data>
class Zoekknoop;

template <class Sleutel,class Data>
class Zoekboom : public unique_ptr<Zoekknoop<Sleutel,Data>>{
//....move en copy. Noot: als er geen copy nodig is, zet hem beste op delete.
public:
    void inorder(std::function<void(const Zoekknoop<Sleutel,Data>&)> bezoek) const;
    void schrijf(ostream& os) const;
    void schrijf_cout() const;
    void teken(const char * bestandsnaam);
    string tekenrec(ostream& uit,int&knoopteller);
	// geefBoomBovenKnoop: gegeven een knooppointer, wele boom wijst naar de knoop
    // preconditie: knoop moet een naar een geldige knoop wijzen.
	Zoekboom<Sleutel, Data>* geefBoomBovenKnoop(Zoekknoop<Sleutel, Data>& knoopptr);
	void voegtoe(const Sleutel& sleutel,const Data& data,bool dubbelsToestaan=false);
	
	Zoekboom();
	Zoekboom(Zoekboom<Sleutel,Data>&& z);
	Zoekboom(const Zoekboom<Sleutel,Data>& z);
	Zoekboom(unique_ptr<Zoekknoop<Sleutel,Data>>&& z);
	Zoekboom<Sleutel,Data>& operator=(Zoekboom<Sleutel,Data>&& z);
	Zoekboom<Sleutel,Data>& operator=(const Zoekboom<Sleutel,Data>& z);

    void rotatie(Zoekknoop<Sleutel, Data> &knoop, bool links);
	void maakOnevenwichtig();
	void maakEvenwichtig();
	
    //te implementeren
    bool repOK() const;
    int geefDiepte();
   

protected:
//zoekfunctie zoekt sleutel en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
    void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel,Data>*& ouder, Zoekboom<Sleutel,Data>*& plaats);
private:
	void copy(Zoekboom<Sleutel,Data>& ik, const Zoekboom<Sleutel,Data>& andere);
	void overloop(const Zoekboom<Sleutel,Data>& huidige, int meegegevven, int &diepte);
    void onevenwichtigHulp(const Zoekknoop<Sleutel, Data> &s);
    void onevPostorder(Zoekknoop<Sleutel, Data> &knoop);
    //void maakEvRec(Zoekknoop<Sleutel,Data> &knoop, bool links);
	void maakEvRec(Zoekboom<Sleutel,Data> *boom);
	bool repOKRec(const Sleutel &s, bool links) const;
};




template <class Sleutel,class Data>
class Zoekknoop {
    friend class Zoekboom<Sleutel,Data>;
    public:
        Zoekknoop():ouder(0){}
        Zoekknoop(const Sleutel& sl, const Data& d):sleutel{sl},data(d),ouder(0){};
        Zoekknoop(Sleutel&& sl, Data&& d):sleutel{std::move(sl)},data(std::move(d)),ouder(0){};
        Zoekboom<Sleutel,Data>& geefKind(bool links);
        Sleutel sleutel;
        Data data;
        Zoekknoop<Sleutel,Data>* ouder;
        Zoekboom<Sleutel,Data> links,rechts;
};


/*****************************************************************************

    Implementatie
    
*****************************************************************************/


template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::inorder(std::function<void(const Zoekknoop<Sleutel,Data>&)> bezoek) const{
    if (*this){
        (*this)->links.inorder(bezoek);
        bezoek(**this);
        (*this)->rechts.inorder(bezoek);
    };
}


template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::schrijf_cout() const{
	std::function<void(const Zoekknoop<Sleutel,Data>&)> f = [](const Zoekknoop<Sleutel,Data>& b) {
		std::cout << b.sleutel << std::endl;
	};
	inorder(f);
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::schrijf(ostream& os) const{
    inorder([&os](const Zoekknoop<Sleutel,Data>& knoop){
        os<<"("<<knoop.sleutel<<" -> "<<knoop.data<<")";
        os<<"\n  Linkerkind: ";
        if (knoop.links)
            os<<knoop.links->sleutel;
        else
            os<<"-----";
        os<<"\n  Rechterkind: ";
        if (knoop.rechts)
            os<<knoop.rechts->sleutel;
        else
            os<<"-----";
        os<<"\n";
    });
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::teken(const char * bestandsnaam){
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller=0;//knopen moeten een eigen nummer krijgen.
    uit<<"digraph {\n";
    this->tekenrec(uit,knoopteller);
    uit<<"}";
}

template <class Sleutel,class Data>
string Zoekboom<Sleutel,Data>::tekenrec(ostream& uit,int&knoopteller ){
    ostringstream wortelstring;
    wortelstring<<'"'<<++knoopteller<<'"';
    if (!*this){
        uit<<wortelstring.str()<<" [shape=point];\n";
    }
    else{
        uit<<wortelstring.str()<<"[label=\""<<(*this)->sleutel<<":"<<(*this)->data<<"\"]";
        uit<<";\n";
        string linkskind=(*this)->links.tekenrec(uit,knoopteller);
        string rechtskind=(*this)->rechts.tekenrec(uit,knoopteller);
        uit<<wortelstring.str()<<" -> "<<linkskind<<";\n";
        uit<<wortelstring.str()<<" -> "<<rechtskind<<";\n";
    };
    return wortelstring.str();
}

template <class Sleutel,class Data>
Zoekboom<Sleutel, Data>* Zoekboom<Sleutel,Data>::geefBoomBovenKnoop(Zoekknoop<Sleutel ,Data>& knoop){
    if (knoop.ouder==0)
        return this;
    else
        if (knoop.ouder->links.get()==&knoop)
            return &(knoop.ouder->links);
        else
            return &(knoop.ouder->rechts);
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::voegtoe(const Sleutel& sleutel,const Data& data,bool dubbelsToestaan){
    Zoekknoop<Sleutel, Data>* ouder;
    Zoekboom<Sleutel, Data>* plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel,ouder,plaats);
    if (dubbelsToestaan)
        while (*plaats)
            (*plaats)->geefKind((rand()%2)).zoek(sleutel,ouder,plaats);
    if (!*plaats){
        Zoekboom<Sleutel, Data> nieuw=
                std::make_unique<Zoekknoop<Sleutel, Data> >(sleutel,data);
        nieuw->ouder=ouder;
        *plaats=move(nieuw);
    }
}


template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel,Data>*& ouder, Zoekboom<Sleutel,Data>*& plaats){
    plaats=this;
    ouder=0;
    while (*plaats && (*plaats)->sleutel !=sleutel){
        ouder=plaats->get();
        if ((*plaats)->sleutel < sleutel)
            plaats=&(*plaats)->rechts;
        else
            plaats=&(*plaats)->links;
    };
}

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>& Zoekknoop<Sleutel,Data>::geefKind(bool linkerkind){
    if (linkerkind)
        return links;
    else
        return rechts;
}


/*****************************************************************************

    EIGEN TOEVOEGINGEN
    
*****************************************************************************/

template <class Sleutel,class Data>
int Zoekboom<Sleutel,Data>::geefDiepte(){
	int diepte = 0;
	overloop(*this, 0, diepte);
	return diepte;
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::overloop(const Zoekboom<Sleutel,Data>& huidige, int meegegeven, int &diepte){
	if(huidige){
		int eigendiepte = ++meegegeven;
		if(eigendiepte > diepte)
				diepte = eigendiepte;
		overloop(huidige->links, eigendiepte, diepte);
		overloop(huidige->rechts, eigendiepte, diepte);
	}	
}


		// OUD
// template <class Sleutel,class Data>
// void Zoekboom<Sleutel,Data>::rotatie(Zoekboom<Sleutel, Data> *boom, bool links){
//     Zoekboom<Sleutel, Data> *kind = links? &(*boom)->links : &(*boom)->rechts;
//     std::swap(*boom, *kind);
//     //*kind hangt nu aan de unique pointer en boom bevat de kinderen van kind
//     if(links)
//         swap((*boom)->rechts, (*kind)->links);
//     else if(!links)
//         swap ((*boom)->links, (*kind)->rechts);
//     //ouderptrs vervangen
//     Zoekboom<Sleutel,Data> *grootouder = &(*kind);
//     (*boom)->ouder = (*boom)->ouder->ouder;
//     (links ? (*boom)->links : (*boom)->rechts)->ouder = (*boom).get();

// }
	
	
	//LENNART
	// FOUT, OOIT EENS DEBUGGEN
/*template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::rotatie(Zoekknoop<Sleutel, Data> &knoop, bool links){
    Zoekboom<Sleutel, Data> *boom = geefBoomBovenKnoop(knoop);
	std::cout << "ik roteer " << knoop.sleutel << links << std::endl;
    if((links? (*boom)->links : (*boom)->rechts) != nullptr){
        Zoekboom<Sleutel, Data> *kind = links? &(*boom)->links : &(*boom)->rechts;
        std::swap(*boom, *kind);
        //*kind hangt nu aan de unique pointer en boom bevat de kinderen van kind
        if(links)
            swap((*boom)->rechts, (*kind)->links);
        else if(!links)
            swap ((*boom)->links, (*kind)->rechts);
        //ouderptrs vervangen
        Zoekboom<Sleutel,Data> *grootouder = &(*kind);
        (*boom)->ouder = (*boom)->ouder->ouder;
        (links ? (*boom)->links : (*boom)->rechts)->ouder = (*boom).get();
    }
}*/

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::rotatie(Zoekknoop<Sleutel, Data> &knoop, bool links){
	Zoekboom<Sleutel,Data> *boom = geefBoomBovenKnoop(knoop);
	if(links && (*boom)->links != nullptr){
		Zoekboom<Sleutel,Data> *kind = &(*boom)->links;
		//eerst ouders aanpassen = ez
		(*kind)->ouder = (*boom)->ouder;
		(*boom)->ouder = (*kind).get();
		
		//dan het rechterkind van kind verplaatsen naar boom ? nee want kind zit nog in links van boom
		// dus eerst roteren ~kind naar boven halen
		
		// dus in *boom moet *kind komen, en in *kind moet *boom komen? want *kind is de linker unique ptr
		// nu zal boom naar het kind wijzen, en wijst de linkerknoopptr van nieuwe kind naar de eigen knoop
		swap(*boom, (*kind));
		
		
		// boom wijst naar de kindknoop, daarvan moet het rechterkind aan de linkerkant van de nieuwe ouder gehangen worden
		// nadenken over hier kind-> links vs kind hierboven (hier: DE KNOOP WAAR KIND NAAR WIJST NODIG KIND NODIG)
		swap((*boom)->rechts, (*kind)->links);
		
		
		
	}
	else if(!links && (*boom)->rechts != nullptr){
		Zoekboom<Sleutel,Data> *kind = &(*boom)->rechts;
		
		(*kind)->ouder = (*boom)->ouder;
		(*boom)->ouder = (*kind).get();
		
		swap(*boom, (*kind));
		
		swap((*boom)->links, (*kind)->rechts);
		
	}
	
}

/*template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::maakOnevenwichtig(){
    onevPostorder(**this);
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::onevPostorder(Zoekknoop<Sleutel, Data> &knoop){
    if(knoop.links){
        onevPostorder(*(knoop.links));
    }
    if(knoop.rechts){
        onevPostorder(*(knoop.rechts));
    }
    std::cout << "ik bewerk knoop " << knoop.sleutel << std::endl;
    rotatie(knoop, true);

}*/

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::maakOnevenwichtig(){
   	Zoekboom<Sleutel,Data> *huidige = this;
   	while((*huidige)->links != nullptr){
		rotatie(**huidige, true);
	}
	//std::cout << "opschuiven\n";
	while((*huidige)->rechts != nullptr){
		huidige = &((*huidige)->rechts);
		while((*huidige)->links != nullptr){
			rotatie(**huidige, true);
		}
	}
}

/*template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::maakEvenwichtig(){
		//ifs toevoegen
		maakEvRec(*(*this)->links, true);
		maakEvRec(*(*this)->rechts, false);
}


template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::maakEvRec(Zoekknoop<Sleutel,Data> &knoop, bool links){
	if(knoop.links)
		maakEvRec(*knoop.links, true);
	if(knoop.rechts)
		maakEvRec(*knoop.rechts, false);
	if(links){
		int diepteBroer = knoop.ouder->rechts ? (knoop.ouder->rechts).geefDiepte() : 0;
		int diepteLinks = (*geefBoomBovenKnoop(knoop))->links ? (*geefBoomBovenKnoop(knoop))->links.geefDiepte() + 1 : 1;
		std::cout << diepteBroer << " " << diepteLinks << std::endl;
		if(diepteLinks > diepteBroer + 1){
			rotatie(*(knoop.ouder), true);
		}
	}
}*/


//concept: diepte berekenen en dan /2 keer roteren, dan recursief beide kinderen opvragen en hetzelfde doen
// implementatie: knoop moet wetne of kidneren links of rechts zitten = bool meegeven? of zelf uitvissen?
template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::maakEvenwichtig(){
	if(*this){
		maakOnevenwichtig();
		maakEvRec(this);
	}
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::maakEvRec(Zoekboom<Sleutel,Data> *boom){
	//std::cout << boom->geefDiepte() << std::endl;
	int diepte;
	if((diepte = boom->geefDiepte()) > 1){
		//std::cout << "dieper gaan" << std::endl;
		
		if((*boom)->links != nullptr){
			for(int i= 0; i < diepte/2; i++)
				rotatie(**boom, true);
		}
		else if((*boom)->rechts != nullptr){
			for(int i= 0; i < diepte/2; i++)
				rotatie(**boom, false);
		}
		
		maakEvRec(&(*boom)->links);
		maakEvRec(&(*boom)->rechts);
	}	
}

template <class Sleutel,class Data>
bool Zoekboom<Sleutel,Data>::repOK() const{
	if(*this == nullptr){
		return true;
	}
	return (*this)->links.repOKRec((*this)->sleutel, true) 
				&& (*this)->rechts.repOKRec((*this)->sleutel, false);
	
}

template <class Sleutel,class Data>
bool Zoekboom<Sleutel,Data>::repOKRec(const Sleutel &s, bool links) const{
	if(*this == nullptr){
		return true;
	}
	bool binboomOK = links? (*this)->sleutel < s : (*this)->sleutel > s;
	if (!binboomOK || (*this)->ouder == nullptr)
		return false;
	return (*this)->links.repOKRec((*this)->sleutel, true) 
				&& (*this)->rechts.repOKRec((*this)->sleutel, false);
		
}

/*****************************************************************************

    Constr + ops
    
*****************************************************************************/

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>::Zoekboom() {
}


template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>::Zoekboom(Zoekboom<Sleutel,Data>&& z){
	operator=(std::move(z));
}

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>::Zoekboom(const Zoekboom<Sleutel,Data>& z){
	operator=(z);
}

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>::Zoekboom(unique_ptr<Zoekknoop<Sleutel,Data>>&& z){
	unique_ptr<Zoekknoop<Sleutel,Data>>::operator=(move(z));
}
	
template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>& Zoekboom<Sleutel,Data>::operator=(Zoekboom<Sleutel,Data>&& z){
	unique_ptr<Zoekknoop<Sleutel,Data>>::operator=(move(z));
	return *this;
}
	
template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>& Zoekboom<Sleutel,Data>::operator=(const Zoekboom<Sleutel,Data>& z){
	std::cout << "start kopie \n";
	this->reset();
	if(z){
		Zoekboom<Sleutel,Data> ik = *this; // geeft toch geen errors um
		//std::cout << "ik kopieer sleutel " << z->sleutel << std::endl;
		*this = std::make_unique<Zoekknoop<Sleutel,Data>>(z->sleutel, z->data);
		copy(*this, z);
		
	}
	return *this;
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::copy(Zoekboom<Sleutel,Data>& ik, const Zoekboom<Sleutel,Data>& andere){
	/*ik->links = nullptr;
	ik->rechts = nullptr;*/
	if(andere->links){
		ik->links = std::make_unique<Zoekknoop<Sleutel,Data>>(andere->links->sleutel, andere->links->data);
		ik->links->ouder = ik.get();
		copy(ik->links, andere->links);
	}
	if(andere->rechts){
		ik->rechts = std::make_unique<Zoekknoop<Sleutel,Data>>(andere->rechts->sleutel, andere->rechts->data);
		ik->rechts->ouder = ik.get();
		copy(ik->rechts, andere->rechts);
	}
	
}



#endif
