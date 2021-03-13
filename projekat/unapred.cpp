#include "unapred.h"

Unapred::Unapred(int pom, int k, string sek)
{
    pomeraj = pom;
    sekcija = sek;
    kol = k;
}

int Unapred::dohvPomeraj()
{
    return pomeraj;
}

int Unapred::dohvKol()
{
    return kol;
}

string Unapred::dohvSekciju()
{
    return sekcija;
}