#include "simulanneal11.h";
#include "knoop.h";
#include <time.h>
#include <stdlib.h>
#include <vector>
using std::vector;

class PadSimAnnealing : public SimulatedAnnealing<vector<Knoop>> {
    public:
        PadSimAnnealing(const vector<Knoop>& v);
    protected:
        vector<Knoop> initialSolution() override;
        double f(const vector<Knoop>& v);
    private:
        vector<Knoop> knopen;

};

PadSimAnnealing::PadSimAnnealing(const vector<Knoop>& v) : knopen(v){
    srand(time(NULL));
}
vector<Knoop> PadSimAnnealing::initialSolution() {
    s = knopen;
    for(int i = 1; i < knopen.size(); i++){
        int kleinere = rand()%i;
        Knoop hulp = s[i];
        s[i] = s[kleinere];
        s[kleinere] = hulp;
    }
}

double PadSimAnnealing::f(const vector<Knoop>& v){
    double kost = 0;
    for(int i = 1 ; i < v.size(); i++){
        kost += (v[i-1].x - v[i].x)*(v[i-1].x - v[i].x) + (v[i-1].y - v[i].y)*(v[i-1].y - v[i].y); 
    }
    return kost;
}