#ifndef UNAPRED_H
#define UNAPRED_H

#include <string>

using namespace std;

class Unapred
{
    int pomeraj;
    int kol;
    string sekcija;

public:
    Unapred(int pom, int k, string sek);
    int dohvPomeraj();
    int dohvKol();
    string dohvSekciju();
};

#endif