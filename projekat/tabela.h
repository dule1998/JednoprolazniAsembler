#ifndef TABELA_H
#define TABELA_H

#include <list>
#include "simbol.h"
#include "regeksi.h"
#include "reltabela.h"

using namespace std;

class Maskod;

class Tabela
{
    list<Simbol> simboli;

public:
    Tabela();
    void dodajSimbol(Simbol s);
    ~Tabela();
    bool pronSimbol(string s);
    bool localDef(string s);
    bool globalDef(string s);
    friend ostream &operator<<(ostream &os, Tabela &t);
    list<Simbol> &dohvSimbole();
    bool pronDefSimbol(string s);
    int dohvPomeraj(string s);
    void dodajSimbDef(string s);
    void dodajOffset(Unapred u, string s);
    bool dohvExt(string s);
    void postDef(string s);
    list<Unapred> dohvUnapred(string s);
    void postSekciju(int sek, string s);
    void postPomeraj(int pom, string s);
    void postRedBr(int redBr, string s);
    void obrisiSimbol(string s);
    void azurRedBr();
    int dohvRedBr(string s);
    int dohvSekciju(string s);
    Simbol *dohvAdrSimb(string s);
    void postOpseg(string s, string opseg);
    void postEqu(string s, list<string> &l);
    void equObrada(list<string> &sekcije, list<Reltabela> &reltabele, list<Maskod> &kodovi);
    void postVred(string sekcija, int vred);
    bool provera();
};

#endif