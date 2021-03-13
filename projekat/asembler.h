#ifndef ASEMBLER_H
#define ASEMBLER_H

#include <string>
#include <list>
#include <iostream>
#include "token.h"
#include "simbol.h"
#include "tabela.h"
#include "maskod.h"
#include "reltabela.h"
#include "instrukcije.h"

using namespace std;

class Asembler
{
    list<list<string>> tokeni;
    int trSekcija;
    int lc;
    int trRedBr;
    string trSekcijaS;
    Tabela tabSimb;
    list<Maskod> kodovi;
    list<string> sekcije;
    list<Reltabela> reltabele;

public:
    Asembler();
    void prolaz(ifstream &ul, ofstream &il);
    ~Asembler();
};

#endif