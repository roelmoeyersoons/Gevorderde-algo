#ifndef KNOOP_H
#define KNOOP_H

#include "pugixml.cpp"


class Knoop {
    public:
        Knoop(pugi::xml_node node); 
        int index;
        double x;
        double y;

};

Knoop::Knoop(pugi::xml_node node) : 
    x(std::stod(node.attribute("xcoordinaat").value())),
    y(std::stod(node.attribute("ycoordinaat").value())),
    index(std::stoi(node.attribute("nummer").value()))
    {}



#endif