#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "knoop.h"
#include "pugixml.cpp"
#include "padSimAnnealing.h"

using namespace pugi;
using namespace std;
const char* node_types[] =
    {
        "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"};

vector<unique_ptr<Knoop>> maakVector(pugi::xml_node doc) {
    vector<unique_ptr<Knoop>> resultVector;
    pugi::xml_node start = doc.child("netwerk");
    for (pugi::xml_node knooppunt = start.child("knooppunt"); knooppunt; knooppunt = knooppunt.next_sibling("knooppunt")) {
        resultVector.push_back(make_unique<Knoop>(knooppunt));
    }
    return resultVector;
}

int main() {
    pugi::xml_document doc;
    if (!doc.load_file("kerstnet.xml")) return -1;
    vector<unique_ptr<Knoop>> vect = maakVector(doc);
    
    PadSimAnnealing psa = PadSimAnnealing(move(vect));
    std::vector<Knoop*> resultaat = psa.oplossing();
    
    ofstream output("weg.txt", ios::app);
    std::cout << "Totale gewicht: " << psa.gewicht() << std::endl;
    if(output.is_open()){
        output << std::endl << "Totale gewicht: " << psa.gewicht() << std::endl; 
    }
    
    for( Knoop* k : resultaat){
        k->print(cout);
        k->print(output);
    }
    
    
    
    return 0;
}
