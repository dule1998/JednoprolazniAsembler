#include "relzapis.h"

Relzapis::Relzapis(int ofs, string t, int rBr, Simbol *simb)
{
    offset = ofs;
    tip = t;
    redBr = rBr;
    s = simb;
}

Relzapis &Relzapis::promRedBr(int red)
{
    (*this).redBr = red;
    return *this;
}

ostream &operator<<(ostream &os, const Relzapis z)
{
    os << hex << z.offset << "\t" << z.tip << "\t" << z.redBr;
    return os;
}

int Relzapis::dohvRedBr()
{
    return redBr;
}

Simbol *Relzapis::dohvSimb()
{
    return s;
}

Relzapis &Relzapis::postaviNull()
{
    (*this).s = nullptr;
    return *this;
}

Relzapis &Relzapis::postNotNull(Simbol *s)
{
    (*this).s = s;
    return *this;
}

Relzapis::~Relzapis()
{
    s = nullptr;
}

string Relzapis::dohvTip()
{
    return tip;
}

int Relzapis::dohvOffset()
{
    return offset;
}