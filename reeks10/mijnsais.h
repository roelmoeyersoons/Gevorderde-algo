#ifndef mijnsais
#define mijnsais

#include <vector>
#include "saisclass.h"

using std::string;
using std::vector;
class MijnSAIS : public SAIS {
   private:
    const unsigned char* tekst;
    vector<int> inverseSAIS;
    vector<int> LCP;
    void maakLCP();

   public:
    MijnSAIS(const unsigned char* T, int len);
    void besteVerwijzing(int& verschuiving, int& lengte, int locatie);
    void print();
};

MijnSAIS::MijnSAIS(const unsigned char* T, int len)
    : SAIS(T, len), LCP(len), tekst(T), inverseSAIS(len) {
    for(int i = 0; i < (*this).size(); i++){
        inverseSAIS[(*this)[i]] = i;
    }
    
    maakLCP();
}

void MijnSAIS::maakLCP() {
    int oudeLCP = 0;
    for (int i = 0; i < this->size(); i++) {
        int suffixIndex = inverseSAIS[i];
        int volgendeIndex = suffixIndex + 1 < this->size() ? suffixIndex + 1 : -1;
        if (volgendeIndex == -1) {
            LCP[suffixIndex] = 0;
        } 
        else {
            
            while(tekst[i+ oudeLCP] == tekst[(*this)[volgendeIndex] + oudeLCP]){
                oudeLCP++;
            }
            LCP[suffixIndex] = oudeLCP;
            if(oudeLCP > 0)
                oudeLCP--;
        }
    }
}

void MijnSAIS::besteVerwijzing(int& verschuiving, int& lengte, int locatie){
    int saisIndex = inverseSAIS[locatie];
    int locatieVolgende = (*this)[saisIndex+1];
    //int besteLCP;
    //aanpassen:: zolang locatievolgende < locatie degene daarop bekijken, en lcp = min(lcp's) van alle tussenliggende
    //e nook evt naar boven gaan indien beneden min < 3
    if(LCP[saisIndex] > 3 && locatieVolgende < locatie){
        lengte = LCP[saisIndex];
        verschuiving = locatie;
    }
}

void MijnSAIS::print(){

}

#endif