#include <iostream>
#include <cstdlib>
#include <string>
#include "token.h"
#include "asembler.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc > 5)
    {
        cout << "***Uneli ste vise od cetiri argumenata***" << endl;
        exit(-1);
    }
    else if (argc < 5)
    {
        cout << "***Uneli ste manje od cetiri argumenata***" << endl;
        exit(-1);
    }

    string prvi = argv[1];
    string drugi = argv[2];
    string treci = argv[3];
    string cetvrti = argv[4];

    bool greska = false;

    if (prvi != "asembler")
    {
        cout << "***Uneli ste pogresan prvi argument***" << endl;
        greska = true;
    }

    if (drugi != "-o")
    {
        cout << "***Uneli ste pogresan drugi argument***" << endl;
        greska = true;
    }

    if (treci[treci.length() - 1] != 'o' || treci[treci.length() - 2] != '.')
    {
        cout << "***Uneli ste pogresnu ekstenziju objektnog fajla***" << endl;
        greska = true;
    }

    if (cetvrti[cetvrti.length() - 1] != 's' || cetvrti[cetvrti.length() - 2] != '.')
    {
        cout << "***Uneli ste pogresnu ekstenziju asemblerskog fajla***" << endl;
        greska = true;
    }

    if (greska)
    {
        exit(-1);
    }

    cout << "***Uspesno pokrenut program***" << endl;

    treci = treci.substr(0, treci.size() - 1);
    treci += "txt";

    cetvrti = cetvrti.substr(0, cetvrti.size() - 1);
    cetvrti += "txt";

    ifstream ul;
    ul.open(cetvrti);
    ofstream iz;
    iz.open(treci);

    Asembler asembler = Asembler();
    try
    {
        asembler.prolaz(ul, iz);
        cout << "***Uspesno zavrsen program***" << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    if (ul.is_open())
    {
        ul.close();
    }
    if (iz.is_open())
    {
        iz.close();
    }

    return 0;
}