#include "knoop.h"
#include <functional>
#include <iostream>
#include <vector>
#include "pugixml.cpp"
using namespace pugi;
using namespace std;
const char* node_types[] =
    {
        "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
    };

void maakVector(pugi::xml_document doc) {
    vector<Knoop> resultVector;
    for (pugi::xml_node_iterator it = doc.begin(); it != doc.end(); ++it) {
        if(it->name() == "knooppunt") {
            cout << "knooppunt" << endl;
        }

        // for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait) {
        //     std::cout << " " << ait->name() << "=" << ait->value();
        // }

        // std::cout << std::endl;
    }
}

int main() {
    pugi::xml_document doc;
    pugi::xml_parse_result file = doc.load_file("kerstnet.xml");

    cout << file.description();

    maakVector(doc);

    return 0;
}
