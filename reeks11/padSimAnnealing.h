#include "simulanneal11.h"
#include "knoop.h"
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iostream>

using std::vector;

class PadSimAnnealing : public SimulatedAnnealing<vector<Knoop*>> {
    public:
        PadSimAnnealing(vector<std::unique_ptr<Knoop>>&&);
        double gewicht();
        void printOplossing();
    protected:
        double T0() override;
        bool terminateCond() override;
        double f(const vector<Knoop*>& v) override;
        vector<Knoop*> initialSolution() override;
        vector<Knoop*> pickAtRandom(const vector<Knoop*>&) override;
        double p(double T, double deltaf) override;
        void updateT() override;
    private:
        vector<std::unique_ptr<Knoop>> knopen;
        int grootte;
        int tellerBewerkingen;
        double startwaarden[7] = {100, 50, 25, 12.5, 6.25, 3.125, 1};
        int startwaardenteller = 0;
};

PadSimAnnealing::PadSimAnnealing(vector<std::unique_ptr<Knoop>>&& v) 
            : knopen(std::move(v)), tellerBewerkingen(0) {
    
    grootte = knopen.size();
    srand(time(NULL));
}
vector<Knoop*> PadSimAnnealing::initialSolution() {
    vector<Knoop*> res(grootte);
    for(int i = 0; i < grootte; i++){
        res[i] = knopen[i].get();
    }

    for(int i = 1; i < grootte; i++){
        int kleinere = rand()%i;
        Knoop* hulp = res[i];
        res[i] = res[kleinere];
        res[kleinere] = hulp;
    }

    return res;
}

double PadSimAnnealing::f(const vector<Knoop*>& v){
    double kost = 0;
    for(int i = 1 ; i < grootte; i++){
        kost += sqrt((v[i-1]->x - v[i]->x)*(v[i-1]->x - v[i]->x) + (v[i-1]->y - v[i]->y)*(v[i-1]->y - v[i]->y)); 
    }
    return kost;
}

bool PadSimAnnealing::terminateCond(){
    tellerBewerkingen++;
    if(tellerBewerkingen%100000 == 0)
        std::cout << "gewicht nu: " << gewicht() << std::endl;
    return tellerBewerkingen > 10000000;
}

double PadSimAnnealing::p(double T, double deltaf){
    return exp(-deltaf/T);
}

double PadSimAnnealing::T0(){
    return startwaarden[startwaardenteller++];
}

vector<Knoop*> PadSimAnnealing::pickAtRandom(const vector<Knoop*>& v){
    int pos1 = rand()%grootte;
    int pos2 = rand()%grootte;
    while(pos2 == pos1){
        pos2 = rand()%grootte;
    }
    vector<Knoop*> res = v;

    Knoop* hulp = res[pos1];
    res[pos1] = res[pos2];
    res[pos2] = hulp;

    return res;
}

void PadSimAnnealing::updateT(){
    if(T > 0.0005)
        T *=999999.0/1000000;
    else {
        if (startwaardenteller < 7){
            T = startwaarden[startwaardenteller++];
        }
        else{
            T = startwaarden[4];
        }
    }
}

double PadSimAnnealing::gewicht(){
    return f(s);
}

void PadSimAnnealing::printOplossing(){
    for(Knoop* k : s){
        std::cout << k->index << ", ";
    }
    std::cout << std::endl;
}
