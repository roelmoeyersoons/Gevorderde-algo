#ifndef __PRQUADTREE_H__
#define __PRQUADTREE_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stack>
using std::unique_ptr;

/*****************************************************************************
    Bevat de headers en code voor een PRQuadtree
    en voor de abstracte klasse PRKnoop met deelklassen Blad en Nietblad
    
*****************************************************************************/


class PRKnoop;
typedef unique_ptr<PRKnoop> Knoopptr;
class PRQuadtree;



class PRQuadtree: public Knoopptr{
public:
    using Knoopptr::unique_ptr;
    PRQuadtree(int a):maxcoordinaat{a}{};
    PRQuadtree( Knoopptr&& a):Knoopptr(move(a)){};
    PRQuadtree& operator=(Knoopptr&& a){
        Knoopptr::operator=(move(a));
        return *this;
    };
    void preEnPostOrder(std::function<void(PRKnoop*)>& bezoekPre,std::function<void(PRKnoop*)>& bezoekPost) const;
    
    //te implementeren
    void voegToe(int x, int y);
    int geefDiepte();
	//de PRquadtree kan alleen punten bevatten met
	//-maxcoordinaat <= x < maxcoordinaat
	//-maxcoordinaat <= y < maxcoordinaat
    int maxcoordinaat;
};

//Opmerking: om de functies specifiek aan een deelklasse te kunnen gebruiken moet je soms een
//static_cast doen, zoals in
//  PRKnoop* knoopptr=...;
//  if (knoopptr!=nullptr && knoopptr->isBlad() && static_cast<PRBlad*>(knoopptr)->x == 5)
class PRKnoop{
public:
    virtual bool isBlad()=0;
    virtual int geefDiepte()=0;
};

class PRBlad:public PRKnoop{
public:
    PRBlad(int x,int y):x{x},y{y}{};
    virtual bool isBlad(){ return true;}
    int x,y;//co"ordinaten punt
    virtual int geefDiepte(){
        return 1;
    };
};
class PRNietblad:public PRKnoop{
public:
    virtual bool isBlad(){ return false;}
    int geefAantalKinderen(){
         int aantal=0;
         for (int i=0; i<4; i++ ){
             if (kind[i])
                ++aantal;
         }
         return aantal;
    };
    //xc, yc: co"ordinaten van het centrum van het gebied
    Knoopptr* geefKind(int x, int y, int xc, int yc){
        int xindex=(x<xc? WEST : OOST);
        int yindex=(y<yc? NOORD : ZUID);
        return &kind[xindex+yindex];
    };
    static const int OOST, WEST, NOORD, ZUID;
    Knoopptr kind[4];//indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
                       //met x en y kleiner dan grenswaarde)
                       //leeg gebied: nulpointer
    virtual int geefDiepte();
};

void PRQuadtree::voegToe(int x, int y){
	if(x > this->maxcoordinaat || y > maxcoordinaat){
		throw "geef iets kleiner mee dan de maxcoordinaat";
	}
	
	if(*this == nullptr){
		*this = (Knoopptr)std::make_unique<PRNietblad>();
	
	}
//static_cast<PRBlad*>(knoopptr)->x == 5
	int xc = 0;
	int yc = 0;
	int halve_breedte = maxcoordinaat;
	
	Knoopptr* ouder = this;
	PRNietblad* ouderKnoop = static_cast<PRNietblad*>(ouder->get());
	Knoopptr* knoopptr = ouderKnoop->geefKind(x, y, 0, 0);
	
	while(*knoopptr != nullptr && halve_breedte > 0 ){
		std::cout << halve_breedte << std::endl;
		if((*knoopptr)->isBlad()){			
			Knoopptr nieuw = std::make_unique<PRNietblad>();
			std::cout << "conflict aan " << xc << " " << yc << std::endl;
			PRNietblad* ouderKnoop = static_cast<PRNietblad*>(ouder->get());

			// hang nieuw aan waar de kindknoop hangt EERST voor deling 			
			// verplaats de kindknoop van ouder naar nieuw en hou rekening met volgende deling
			
			int xindex=(x<xc? PRNietblad::WEST : PRNietblad::OOST);
        	int yindex=(y<yc? PRNietblad::NOORD : PRNietblad::ZUID);
        	
        	halve_breedte /= 2;
        	int xcoud = xc;
        	int ycoud = yc;
			xc = (x > xc) ? xc + halve_breedte : xc - halve_breedte;
			yc = (y > yc) ? yc + halve_breedte : yc - halve_breedte;
        	
        	int xindexkind=(x<xc? PRNietblad::WEST : PRNietblad::OOST);
        	int yindexkind=(y<yc? PRNietblad::NOORD : PRNietblad::ZUID);
        	
        	PRNietblad* nieuwKnoop = static_cast<PRNietblad*>(nieuw.get());
			nieuwKnoop->kind[xindexkind + yindexkind] = std::move(ouderKnoop->kind[xindex + yindex]);
			ouderKnoop->kind[xindex + yindex] = std::move(nieuw);
			//ouderKnoop->geefKind(x, y, xc, yc)->move(nieuw);
				
			//ouderKnoop->geefKind(x, y, xc, yc); mag niet denk ik want foute xc yc
			ouder = &ouderKnoop->kind[xindex + yindex];
			ouderKnoop = static_cast<PRNietblad*>(ouder->get());
			//oude? over nadenken, met xc en yc 
			knoopptr = ouderKnoop->geefKind(x, y, xcoud, ycoud);
			
		}
		else{
			std::cout << "geenblad" << std::endl;
			halve_breedte /= 2;
			xc = (x > xc) ? xc + halve_breedte : xc - halve_breedte;
			yc = (y > yc) ? yc + halve_breedte : yc - halve_breedte;
		
			PRNietblad* knoopptrKnoop = static_cast<PRNietblad*>(knoopptr->get());
			ouder = knoopptr;
			knoopptr = knoopptrKnoop->geefKind(x, y, xc, yc);
		}
	}
	std::cout << "toevoegen " << x << " " << y << "boven het nietblad" << xc << " " << yc << std::endl;
	*knoopptr = std::make_unique<PRBlad>(x, y);		
}

int PRNietblad::geefDiepte(){
	//return 1 + max(kind[0].geefDiepte(), kind[1].geefDiepte(), kind[2].geefDiepte(), kind[3].geefDiepte());
	return 1;
}

void PRQuadtree::preEnPostOrder(std::function<void(PRKnoop*)>& bezoekPre,std::function<void(PRKnoop*)>& bezoekPost) const{
        enum staat {pre,post};
        std::stack<std::pair<PRKnoop*,staat>> DEstack;//bevat alleen niet-nulpointers
        if (*this)
            DEstack.emplace(this->get(),pre);
        while (!DEstack.empty()){
            auto p = DEstack.top();
            auto nuknoop = p.first;
            auto nustaat = p.second;
			if (nustaat==pre){
                bezoekPre(nuknoop);
                DEstack.top().second=post;
                if (!nuknoop->isBlad()){
                    for (int i=0; i<4; i++ ){
                        Knoopptr* kind=&(static_cast<PRNietblad*>(nuknoop)->kind[i]);
                        if (*kind)
                            DEstack.emplace(kind->get(),pre);
                    };
                };
            } else{
                bezoekPost(nuknoop);
                DEstack.pop();
            };
        };

    };

const int PRNietblad::OOST=0;
const int PRNietblad::WEST=1;
const int PRNietblad::NOORD=0;
const int PRNietblad::ZUID=2;
#endif


