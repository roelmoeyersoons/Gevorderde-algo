#ifndef __Splayboom_H
#define __Splayboom_H

#include "zoekboom1.h"

template <class Sleutel, class Data>
class Splayboom : public Zoekboom<Sleutel, Data>
{
public:
    friend class Zoekboom<Sleutel, Data>;
    //Splay operatie - Zig, Zig Zig, Zig Zag
    //https://en.wikipedia.org/wiki/Splay_tree
    void splay(zoekKnoop<Sleutel, Data> *knoop);
    //----Zoeken
    //Item zoals normaal zoeken
    //Splay operatie uitvoeren
    zoekKnoop<Sleutel, Data> *zoek(Sleutel sleutel);
    //----Toevoegen
    //Item zoals normaal toevoegen
    //Splay operatie uitvoeren
    void voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan = false);
};

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::splay(zoekKnoop<Sleutel, Data> *knoop)
{
    if (knoop->ouder)
    {
        //C staat links
        if (knoop->ouder->ouder == nullptr)
        {
            //Zig
            this->roteer(knoop->sleutel);
        }
        else if ((knoop == knoop->ouder->links.get() && knoop->sleutel < knoop->ouder->sleutel) || (knoop == knoop->ouder->rechts.get() && knoop->sleutel > knoop->ouder->sleutel))
        {
            //Zig-Zig
            this->roteer(knoop->ouder->sleutel);
            this->roteer(knoop->sleutel);
            this->splay(knoop);
        }
        else
        {
            //Zig-zag
            this->roteer(knoop->sleutel);
            this->roteer(knoop->sleutel);
            this->splay(knoop);
        }
    }
}

template <class Sleutel, class Data>
zoekKnoop<Sleutel, Data> *Splayboom<Sleutel, Data>::zoek(Sleutel sleutel)
{
    this->count = 0;
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> *plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
    if(plaats->get() != nullptr){
		this->splay(plaats->get());
		return (*this).get();
	}
	return nullptr;
    	
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan)
{
    Zoekboom<Sleutel, Data>::voegtoe(sleutel, data, dubbelsToestaan);
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> *plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
    this->splay(plaats->get());
}

#endif
