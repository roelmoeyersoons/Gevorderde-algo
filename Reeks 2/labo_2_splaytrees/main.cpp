#include <iostream>
#include "splayboom.h"
#include <string>
#include <fstream>

int main()
{
    //1-100
    Splayboom<int, string> boom;
    for (int i = 1; i <= 100; i++)
    {
        boom.voegtoe(i, "data", false);
    }
    boom.teken("stap1");
    boom.zoek(1);
    boom.teken("stap2");

    //1-100 vanaf 50 beginnen en dan 51,49,52,48,...
    Splayboom<int, string> boom2;
    boom2.voegtoe(50, "data", false);
    for (int i = 1; i < 50; i++)
    {
        boom2.voegtoe(50 + i, "data", false);
        boom2.voegtoe(50 - i, "data", false);
    }
    boom2.voegtoe(100, "data", false);
    boom2.teken("stap3");
    boom2.zoek(50);
    boom2.teken("stap4");

    //Shakespear bestand
    // Splayboom<string, int> shakespearboom;
    // std::ifstream in{"Shakespeare.txt"};
    // if (!in)
    // {
    //     std::cerr << "Error while reading file" << std::endl;
    // }

    // std::string woord;
    // shakespearboom.count = 0;
    // while (in >> woord)
    // {
    //     std::cerr << woord << std::endl;
    //     zoekKnoop<string, int> *knoop = shakespearboom.zoek(woord);
    //     shakespearboom.teken("lol");
    //     if (knoop == nullptr)
    //     {
    //         shakespearboom.voegtoe(woord, 1);
    //     }
    //     else
    //     {
    //         shakespearboom.voegtoe(woord, knoop->data + 1);
    //     }
    // }

    //std::cout << "Aantal rotaties voor elementen: " << shakespearboom.count << std::endl;
    //shakespearboom.teken("stap5");

    std::cout << "Finished" << std::endl;

    return 0;
}