#ifndef __BINAIRETRIE_H__
#define __BINAIRETRIE_H__
#include "bincode.h"
#include <functional>
#include <memory>
using std::cerr;
/**********************************************************************

   Class: BinaireTrie, met hulpklassen van knopen
   
   beschrijving: Huffmantries, geschikt voor byte-alfabet
   letters zijn 0-255, plus afsluitingsteken 256.
   Een lege Huffmantrie is zinloos, ook als deeltrie
   Alleen bij opbouw zijn er zo'n tries mogelijk.
   Vandaar dat de parameterloze constructor protected is
   
***************************************************************************/
using std::binary_function;
using std::cout;
class Trieknoop;

typedef std::unique_ptr<Trieknoop> upKnoop;


class BinaireTrie: public upKnoop{
friend class TrieNietblad;
public:
    BinaireTrie(const Bincode&);
    BinaireTrie(BinaireTrie&& b){move(b);};
    BinaireTrie& operator=(upKnoop&& a){
        upKnoop::operator=(move(a));
        return *this;
    };
    BinaireTrie& operator=(BinaireTrie&& a){
        upKnoop::operator=(move(a));
        return *this;
    };
protected:
    BinaireTrie():upKnoop(){};
    BinaireTrie( upKnoop&& a):upKnoop(move(a)){};
//hulpfunctie voor constructor uitgaande van Bincode
    void voegToe(const Varbitpatroon& vb,int letter);
    
};

class Trieknoop{
public:
    virtual bool isBlad()=0;
};

class TrieBlad:public Trieknoop{
public:
    TrieBlad(int letter):letter{letter}{};
    virtual bool isBlad(){ return true;}
    int letter;
};

class TrieNietblad:public Trieknoop{
public:
    TrieNietblad(){};
    TrieNietblad(BinaireTrie&& l,BinaireTrie&&r):links(move(l)),rechts(move (r)){};
    virtual bool isBlad(){ return false;}
    BinaireTrie& geefKind(bool rechterkind){
        return (rechterkind?
                    rechts:
                    links);
    };
    BinaireTrie links, rechts;
};

//	EIGEN CODE

BinaireTrie::BinaireTrie(const Bincode& b){
	std::cout << "ik begin" << std::endl;
	*this = std::make_unique<TrieNietblad>();
	
	for(int i = 0; i < 257; i++){
		voegToe(b[i], i);
	}
	std::cout << "gedaan";
}

void BinaireTrie::voegToe(const Varbitpatroon& vb,int letter){
	std::cout << "ik voeg letter " << letter << "toe " << std::endl;
	BinaireTrie *afdalen, *kind;
	int bit_checken = -1;
	
	afdalen = this;
	kind = &static_cast<TrieNietblad*>(afdalen->get())->geefKind(vb.geefbit(++bit_checken));
	
	while(*kind != nullptr && !(*kind)->isBlad()){
		
		afdalen = kind;
		kind = &static_cast<TrieNietblad*>(kind->get())->geefKind(vb.geefbit(++bit_checken));
	}
	
	
	//if(*kind != nullptr){
		while(*kind != nullptr && (*kind)->isBlad()){
			//tussen NietKnoop aanmaken
			//en dan weer toevoegen? beter: via while lus
			
			BinaireTrie hulp = std::move(*kind);
			//(*afdalen)->geefKind(vb.geefbit(bit_checken)) niet nodig door ptrs
			*kind = std::make_unique<TrieNietblad>();
			cout << "aaaa";
			BinaireTrie* volgende = &static_cast<TrieNietblad*>(kind->get())->geefKind(vb.geefbit(++bit_checken));
			*volgende = std::move(hulp);
			afdalen = kind;
			kind = volgende;
	
		}
	//}
		//toevoegen
	if(*kind == nullptr){
		*kind = std::make_unique<TrieBlad>(letter);
		std::cout << "letter " << letter << "toegevoegd " << std::endl;
	}	
}


#endif
