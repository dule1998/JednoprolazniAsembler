#ifndef RELTABELA_H
#define RELTABELA_H

#include <string>
#include <list>
#include "relzapis.h"

using namespace std;

class Reltabela
{
    string sekcija;
    list<Relzapis> zapisi;

public:
    void dodajZapis(Relzapis z);
    Reltabela(string sek);
    string dohvSekciju();
    friend ostream &operator<<(ostream &os, Reltabela &r);
    void postaviNull(string s);
    void postNotNull(Simbol *s);
    void azurPok();
    void obrisiZapise(string s);
    bool tipPC32(int pom);
};

#endif