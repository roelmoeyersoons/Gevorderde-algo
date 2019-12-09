#ifndef __Zoekboom_H
#define __Zoekboom_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <cassert>

using std::cerr;
using std::endl;
using std::max;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::string;
using std::swap;
using std::unique_ptr;
/**********************************************************************

   Class: Zoekboom
   
   beschrijving: Binaire Zoekboom waarin duplicaatsleutels wel of niet zijn toegestaan.
   
***************************************************************************/
template <class Sleutel, class Data>
class zoekKnoop;

template <class Sleutel, class Data>
class Zoekboom : public unique_ptr<zoekKnoop<Sleutel, Data>>
{
    //....move en copy. Noot: als er geen copy nodig is, zet hem beste op delete.
public:
    int count = 0;
    void inorder(std::function<void(const zoekKnoop<Sleutel, Data> &)> bezoek) const;
    void schrijf(ostream &os) const;
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);

    Zoekboom() = default;
    //Copy constructor
    Zoekboom(unique_ptr<zoekKnoop<Sleutel, Data>> knoop)
    {
        this->swap(knoop);
    };

    //te implementeren
    bool repOK() const;
    int geefDiepte();
    // geefBoomBovenKnoop: gegeven een knooppointer, wele boom wijst naar de knoop
    // preconditie: knoop moet een naar een geldige knoop wijzen.
    Zoekboom<Sleutel, Data> *geefBoomBovenKnoop(zoekKnoop<Sleutel, Data> &knoopptr);
    void voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan = false);
    void roteer(Sleutel sleutel);
    void maakOnevenwichtig();
    void maakOnevenwichtig_func(const zoekKnoop<Sleutel, Data> &);
    void maakEvenwichtig();
void zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats);
protected:
    //zoekfunctie zoekt sleutel en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
    //ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
    //noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
    
};

template <class Sleutel, class Data>
class zoekKnoop
{
    friend class Zoekboom<Sleutel, Data>;

public:
    zoekKnoop() : ouder(0) {}
    zoekKnoop(const Sleutel &sl, const Data &d) : sleutel{sl}, data(d), ouder(0){};
    zoekKnoop(Sleutel &&sl, Data &&d) : sleutel{move(sl)}, data(move(d)), ouder(0){};
    Zoekboom<Sleutel, Data> &geefKind(bool links);
    Sleutel sleutel;
    Data data;
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> links, rechts;
};

/*****************************************************************************

    Implementatie
    
*****************************************************************************/

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::inorder(std::function<void(const zoekKnoop<Sleutel, Data> &)> bezoek) const
{
    if (*this)
    {
        (*this)->links.inorder(bezoek);
        bezoek(**this);
        (*this)->rechts.inorder(bezoek);
    };
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::schrijf(ostream &os) const
{
    inorder([&os](const zoekKnoop<Sleutel, Data> &knoop) {
        os << "(" << knoop.sleutel << " -> " << knoop.data << ")";
        os << "\n  Linkerkind: ";
        if (knoop.links)
            os << knoop.links->sleutel;
        else
            os << "-----";
        os << "\n  Rechterkind: ";
        if (knoop.rechts)
            os << knoop.rechts->sleutel;
        else
            os << "-----";
        os << "\n";
    });
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

template <class Sleutel, class Data>
string Zoekboom<Sleutel, Data>::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << ":" << (*this)->data << "\"]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrec(uit, knoopteller);
        string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data> *Zoekboom<Sleutel, Data>::geefBoomBovenKnoop(zoekKnoop<Sleutel, Data> &knoop)
{
    if (knoop.ouder == 0)
        return this;
    else if (knoop.ouder->links.get() == &knoop)
        return &(knoop.ouder->links);
    else
        return &(knoop.ouder->rechts);
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan)
{
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> *plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
    if (dubbelsToestaan)
        while (*plaats)
            (*plaats)->geefKind(rand() % 2).zoek(sleutel, ouder, plaats);
    if (!*plaats)
    {
        Zoekboom<Sleutel, Data> nieuw =
            std::make_unique<zoekKnoop<Sleutel, Data>>(sleutel, data);
        nieuw->ouder = ouder;
        *plaats = move(nieuw);
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel &sleutel, zoekKnoop<Sleutel, Data> *&ouder, Zoekboom<Sleutel, Data> *&plaats)
{
    plaats = this;
    ouder = 0;
    while (*plaats && (*plaats)->sleutel != sleutel)
    {
        ouder = plaats->get();
        if ((*plaats)->sleutel < sleutel)
            plaats = &(*plaats)->rechts;
        else
            plaats = &(*plaats)->links;
    };
}

template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::geefDiepte()
{
    int hoogte = 0;
    // Diepte Eerst
    if (this)
    {
        int linkseDiepte = (*this)->links ? (*this)->links.geefDiepte() : 0;
        int rechtsteDiepte = (*this)->rechts ? (*this)->rechts.geefDiepte() : 0;

        return 1 + max(linkseDiepte, rechtsteDiepte);
    }

    return 0;
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteer(Sleutel sleutel)
{
    this->count++;
    // zoek naar de knoop die je wilt roteren
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> *plaats;

    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
    Zoekboom<Sleutel, Data> *ouderBoom = this->geefBoomBovenKnoop(*ouder);

    if (ouder)
    {
        if ((*plaats)->sleutel > ouder->sleutel)
        {
            // rotatie links
            ouderBoom->swap((*plaats)->links);
            ouderBoom->swap(*plaats);

            //parents aanpassen
            (*ouderBoom)->ouder = (*ouderBoom)->links->ouder;
            (*ouderBoom)->links->ouder = (*ouderBoom).get();
            if ((*ouderBoom)->links->rechts)
                (*ouderBoom)->links->rechts->ouder = (*ouderBoom)->links.get();
        }
        else
        {
            // rotatie rechts
            ouderBoom->swap((*plaats)->rechts);
            ouderBoom->swap(*plaats);

            //parents aanpassen
            (*ouderBoom)->ouder = (*ouderBoom)->rechts->ouder;
            (*ouderBoom)->rechts->ouder = (*ouderBoom).get();
            if ((*ouderBoom)->rechts->links)
                (*ouderBoom)->rechts->links->ouder = (*ouderBoom)->rechts.get();
        }
    }
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data> &zoekKnoop<Sleutel, Data>::geefKind(bool linkerkind)
{
    if (linkerkind)
        return links;
    else
        return rechts;
};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakOnevenwichtig_func(const zoekKnoop<Sleutel, Data> &knoop)
{
    if (knoop.rechts)
    {
        Zoekboom<Sleutel, Data> *boom = Zoekboom<Sleutel, Data>::geefBoomBovenKnoop(knoop);
        boom->roteerLinks(knoop.sleutel);
    }
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakOnevenwichtig()
{
    if ((*this))
    {
        std::cerr << "test" << std::endl;
        while ((*this)->rechts)
        {
            this->roteer((*this)->rechts->sleutel);
        }
        (*this)->links.maakOnevenwichtig();
    };
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakEvenwichtig()
{
    //Beter in 2 aparte methodes
    //...

    //De boom onevenwichtig maken zodat knopen langs 1 kant staan
    this->maakOnevenwichtig();

    //Roteren zolang de boom onevenwichtig is
    int diepte = this->geefDiepte();
    for (int i = 0; i < diepte / 2; i++)
    {
        //Checken indien nullptr
        //...
        this->roteer((*this)->links->sleutel);
    }

    //Dit recursief toepassen
    //Hoeft niet?
}

#endif
