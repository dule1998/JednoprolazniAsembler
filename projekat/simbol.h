#ifndef SIMBOL_H
#define SIMBOL_H

#include <string>
#include <list>
#include <iostream>
#include "unapred.h"

using namespace std;

class Simbol
{
    string naziv;
    int sekcija;
    int pomeraj;
    int vrednost;
    string opseg;
    int redBr;
    bool def;
    int flag; //0-R, 1-W, 2-X, 3-P
    bool ext;
    list<Unapred> unapred;
    list<string> equ;

public:
    Simbol(string naz, int sek, int pom, int vred, string ops, int rBr, int f, bool d);
    ~Simbol();
    string dohvNaziv();
    int dohvSekciju();
    int dohvPomeraj();
    string dohvOpseg();
    int dohvRedBr();
    bool dohvDef();
    Simbol &uvecajRedBr();
    Simbol &smanjiRedBr();
    friend ostream &operator<<(ostream &os, const Simbol s);
    Simbol &postExt();
    Simbol &postVred(int vred);
    bool dohvExt();
    Simbol &dodajOffset(Unapred u);
    Simbol &postDef();
    list<Unapred> &dohvUnapred();
    Simbol &postSekciju(int sek);
    Simbol &postPomeraj(int pom);
    Simbol &postRedBr(int redBr);
    Simbol *dohvAdrSimb();
    Simbol &postOpseg(string opseg);
    Simbol &postEqu(list<string> &l);
    list<string> &dohvEqu();
    int dohvVred();
};

#endif