#ifndef __Splayboom_H
#define __Splayboom_H

#include "zoekboom.h"




// bottom up splay tree


template <class Sleutel,class Data>
class Splayboom : public Zoekboom<Sleutel,Data>{
	public:
		
		Splayboom();
		Splayboom(Splayboom<Sleutel,Data> &&s);
		Splayboom(Zoekboom<Sleutel,Data> &&z);
		void splay(Zoekknoop<Sleutel,Data>* z);
		Splayboom<Sleutel, Data>* zoek_en_splay(const Sleutel& s);
		bool voeg_toe(const Sleutel& s, const Data& d);
};

template <class Sleutel,class Data>
Splayboom<Sleutel,Data>::Splayboom(){}

template <class Sleutel,class Data>
Splayboom<Sleutel,Data>::Splayboom(Zoekboom<Sleutel,Data> &&z){
	Zoekboom<Sleutel,Data>::operator=(move(z));
}

// Zoekboom<Sleutel, Data>* geefBoomBovenKnoop(Zoekknoop<Sleutel, Data>& knoopptr);

template <class Sleutel,class Data>
void Splayboom<Sleutel,Data>::splay(Zoekknoop<Sleutel,Data>* z){
	if(z){
		std::cout << "hallo ik ben knoop met sleutel" << z->sleutel << std::endl;
		if(z->ouder == nullptr){
			return;
		}
		else {
			bool links = z->ouder->links.get() == z;
			if(z->ouder->ouder == nullptr){
				//zig
				Zoekboom<Sleutel,Data>::rotatie(*z->ouder, links);
			}
			else{
				Zoekknoop<Sleutel,Data> *ouder = z->ouder;
				bool ouderlinks = ouder->ouder->links.get() == ouder;
//				 std::cout << links << ouderlinks << std::endl;
//				std::cout << "ouder is" << (*ouder)->sleutel << std::endl;
//				std::cout << "ouder->ouder is" << (*ouder)->ouder->sleutel << std::endl;
				if(ouderlinks && links || !ouderlinks && !links){
					//zigzig
					Zoekboom<Sleutel,Data>::rotatie(*ouder->ouder, ouderlinks);
//					std::cout << (*z)->ouder << std::endl;
					std::cout << "hallo ik ben knoop met sleutel" << z->sleutel << std::endl;
//					std::cout << "ouder is" << (*ouder)->sleutel << std::endl;
//					std::cout << "ouder->ouder is" << (*ouder)->ouder->sleutel << std::endl;
					Zoekboom<Sleutel,Data>::rotatie(*z->ouder, links);
					std::cout << z->sleutel << std::endl;
					std::cout << z->ouder << std::endl;
//					std::cout << ((*z) == nullptr);
				}
				else{
					//zigzag
					Zoekboom<Sleutel,Data>::rotatie(*z->ouder, links);
					Zoekboom<Sleutel,Data>::rotatie(*ouder->ouder, ouderlinks);
				}
				std::cout << (ouder == nullptr);
				std::cout << (z == nullptr);
				std::cout << "ik stop, ben knoop met sleutel" << z->sleutel << std::endl;
				//splay();
				
			}
		}
	}
	
}

template <class Sleutel,class Data>
Splayboom<Sleutel, Data>* Splayboom<Sleutel,Data>::zoek_en_splay(const Sleutel& s){
	Zoekknoop<Sleutel, Data>* ouderptr;
	Zoekboom<Sleutel, Data>* boomptr;
	Zoekboom<Sleutel,Data>::zoek(s, ouderptr, boomptr);
	
	//nu nog splayen
	splay(boomptr->get());
	
	return (Splayboom<Sleutel,Data> *)boomptr;
}

template <class Sleutel,class Data>
bool Splayboom<Sleutel,Data>::voeg_toe(const Sleutel& s, const Data& d){
	Zoekboom<Sleutel,Data>::voegtoe(s, d, false);
	
	// splayen
	splay(zoek(s));
	
	return true;
	
}

#endif
