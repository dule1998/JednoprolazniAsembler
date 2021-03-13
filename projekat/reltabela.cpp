#include "reltabela.h"

void Reltabela::dodajZapis(Relzapis z)
{
    zapisi.push_back(z);
}

Reltabela::Reltabela(string sek)
{
    sekcija = sek;
}

string Reltabela::dohvSekciju()
{
    return sekcija;
}

ostream &operator<<(ostream &os, Reltabela &r)
{
    os << "#.ret" << r.sekcija << endl;
    os << "#offset"
       << "\t"
       << "tip"
       << "\t"
       << "\t"
       << "vred" << endl;
    for (list<Relzapis>::iterator it = r.zapisi.begin(); it != r.zapisi.end(); ++it)
    {
        os << *it << endl;
    }
    return os;
}

void Reltabela::postaviNull(string s)
{
    for (list<Relzapis>::iterator it = zapisi.begin(); it != zapisi.end(); ++it)
    {
        if (((*it).dohvSimb())->dohvNaziv() == s)
        {
            (*it).postaviNull();
        }
    }
}

void Reltabela::postNotNull(Simbol *s)
{
    for (list<Relzapis>::iterator it = zapisi.begin(); it != zapisi.end(); ++it)
    {
        if ((*it).dohvSimb() == nullptr)
        {
            (*it).postNotNull(s);
        }
    }
}

void Reltabela::azurPok()
{
    for (list<Relzapis>::iterator it = zapisi.begin(); it != zapisi.end(); ++it)
    {
        if (((*it).dohvSimb())->dohvOpseg() == "global")
        {
            (*it).promRedBr(((*it).dohvSimb())->dohvRedBr());
        }
        else
        {
            (*it).promRedBr(((*it).dohvSimb())->dohvSekciju());
        }
    }
}

void Reltabela::obrisiZapise(string s)
{
    for (list<Relzapis>::iterator it = zapisi.begin(); it != zapisi.end(); ++it)
    {
        if ((*it).dohvTip() == "R_386_PC32" && ((*it).dohvSimb())->dohvNaziv() == s)
        {
            it = zapisi.erase(it);
        }
    }
}

bool Reltabela::tipPC32(int pom)
{
    for (list<Relzapis>::iterator it = zapisi.begin(); it != zapisi.end(); ++it)
    {
        if ((*it).dohvTip() == "R_386_PC32" && pom == (*it).dohvOffset())
        {
            return true;
        }
    }
    return false;
}