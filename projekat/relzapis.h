#ifndef RELZAPIS_H
#define RELZAPIS_H

#include <string>
#include <iostream>
#include "simbol.h"

using namespace std;

class Relzapis
{
    int offset;
    string tip;
    int redBr;
    Simbol *s;

public:
    Relzapis(int ofs, string t, int rBr, Simbol *simb);
    Relzapis &promRedBr(int red);
    friend ostream &operator<<(ostream &os, const Relzapis z);
    int dohvRedBr();
    Simbol *dohvSimb();
    Relzapis &postaviNull();
    Relzapis &postNotNull(Simbol *s);
    ~Relzapis();
    string dohvTip();
    int dohvOffset();
};

#endif