#ifndef KNOOP_H
#define KNOOP_H

#include "pugixml.cpp"
#include <iostream>
#include <fstream>

class Knoop {
    public:
        Knoop(pugi::xml_node node); 
        int index;
        double x;
        double y;
        void print(std::ostream&);
};

Knoop::Knoop(pugi::xml_node node) : 
    x(std::stod(node.attribute("xcoordinaat").value())),
    y(std::stod(node.attribute("ycoordinaat").value())),
    index(std::stoi(node.attribute("nummer").value()))
    {}

void Knoop::print(std::ostream& os){
    os << index << ":\tKnoop(" << x << "," << y << ")" << std::endl;
}


#endif