#include "simbol.h"

Simbol::Simbol(string naz, int sek, int pom, int vred, string ops, int rBr, int f, bool d)
{
    sekcija = sek;
    pomeraj = pom;
    naziv = naz;
    vrednost = vred;
    def = d;
    opseg = ops;
    redBr = rBr;
    flag = f;
    ext = false;
}

string Simbol::dohvNaziv()
{
    return naziv;
}

int Simbol::dohvSekciju()
{
    return sekcija;
}

int Simbol::dohvRedBr()
{
    return redBr;
}

int Simbol::dohvPomeraj()
{
    return pomeraj;
}

string Simbol::dohvOpseg()
{
    return opseg;
}

Simbol &Simbol::uvecajRedBr()
{
    (*this).redBr++;
    return *this;
}

Simbol &Simbol::smanjiRedBr()
{
    (*this).redBr--;
    return *this;
}

bool Simbol::dohvDef()
{
    return def;
}

ostream &operator<<(ostream &os, const Simbol s)
{
    string pom = "";
    if ((s.flag & 0x1) == 0x1)
    {
        pom += "R";
    }
    if ((s.flag & 0x2) == 0x2)
    {
        pom += "W";
    }
    if ((s.flag & 0x4) == 0x4)
    {
        pom += "X";
    }
    if ((s.flag & 0x8) == 0x8)
    {
        pom += "P";
    }
    os << s.naziv << "\t" << s.sekcija << "\t" << hex << s.pomeraj << "\t" << s.vrednost << "\t" << s.opseg << "\t" << s.redBr << "\t" << dec << s.def << "\t" << pom;
    return os;
}

Simbol &Simbol::postExt()
{
    (*this).ext = true;
    return *this;
}

Simbol &Simbol::postVred(int vred)
{
    (*this).vrednost = vred;
    return *this;
}

bool Simbol::dohvExt()
{
    return ext;
}

Simbol &Simbol::dodajOffset(Unapred u)
{
    ((*this).unapred).push_back(u);
    return *this;
}

Simbol &Simbol::postDef()
{
    (*this).def = true;
    return *this;
}

list<Unapred> &Simbol::dohvUnapred()
{
    return unapred;
}

Simbol &Simbol::postSekciju(int sek)
{
    (*this).sekcija = sek;
    return *this;
}

Simbol &Simbol::postPomeraj(int pom)
{
    (*this).pomeraj = pom;
    return *this;
}

Simbol::~Simbol()
{
    unapred.clear();
    equ.clear();
}

Simbol &Simbol::postRedBr(int redBr)
{
    (*this).redBr = redBr;
    return *this;
}

Simbol *Simbol::dohvAdrSimb()
{
    return this;
}

Simbol &Simbol::postOpseg(string opseg)
{
    (*this).opseg = opseg;
    return *this;
}

Simbol &Simbol::postEqu(list<string> &l)
{
    (*this).equ = l;
    return *this;
}

list<string> &Simbol::dohvEqu()
{
    return equ;
}

int Simbol::dohvVred()
{
    return vrednost;
}