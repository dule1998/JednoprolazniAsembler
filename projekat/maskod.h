#ifndef MASKOD_H
#define MASKOD_H

#include <list>
#include <string>
#include <iostream>
#include "unapred.h"
#include "tabela.h"
#include "reltabela.h"

using namespace std;

class Maskod
{
    list<char> kod;
    string sekcija;

public:
    Maskod(string sek);
    void dodajBajt(char c);
    void dodajRec(short s);
    void skip(int count);
    string dohvSekciju();
    ~Maskod();
    friend ostream &operator<<(ostream &os, const Maskod &m);
    void promeniKod(int n, int kol, string s, Tabela &t, Reltabela *r);
};

#endif