#include "tabela.h"
#include "maskod.h"

Tabela::Tabela()
{
    Simbol s = Simbol("UND", 0, 0, 0, "local", 0, 0, true);
    dodajSimbol(s);
}

void Tabela::dodajSimbol(Simbol s)
{
    string naziv = s.dohvNaziv();
    if (naziv != ".text" && naziv != ".data" && naziv != ".bss")
    {
        simboli.push_back(s);
    }
    else if ((naziv == ".text" || naziv == ".data" || naziv == ".bss") && (dohvSekciju(naziv) != dohvRedBr(naziv)))
    {
        simboli.push_back(s);
    }
    else
    {
        bool b = true;
        for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
        {
            if ((*it).dohvSekciju() != (*it).dohvRedBr())
            {
                b = false;
                simboli.insert(it, s);
                for (list<Simbol>::iterator it1 = it; it1 != simboli.end(); ++it1)
                {
                    (*it1).uvecajRedBr();
                }
                break;
            }
        }
        if (b)
        {
            simboli.push_back(s);
        }
    }
}

Tabela::~Tabela()
{
    simboli.clear();
}

bool Tabela::pronSimbol(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return true;
        }
    }
    return false;
}

bool Tabela::pronDefSimbol(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if (((*it).dohvNaziv() == s) && (*it).dohvDef())
        {
            return true;
        }
    }
    return false;
}

ostream &operator<<(ostream &os, Tabela &t)
{
    os << "#tabela simbola" << endl;
    os << "#naziv"
       << "\t"
       << "sekcija"
       << "\t"
       << "pomeraj"
       << "\t"
       << "vred"
       << "\t"
       << "opseg"
       << "\t"
       << "red.br"
       << "\t"
       << "def"
       << "\t"
       << "flag" << endl;
    for (list<Simbol>::iterator it = t.simboli.begin(); it != t.simboli.end(); ++it)
    {
        os << *it << endl;
    }
    return os;
}

bool Tabela::localDef(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s && (*it).dohvOpseg() == "local")
        {
            return true;
        }
    }
    return false;
}

bool Tabela::globalDef(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s && (*it).dohvOpseg() == "global")
        {
            return true;
        }
    }
    return false;
}

list<Simbol> &Tabela::dohvSimbole()
{
    return simboli;
}

int Tabela::dohvPomeraj(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return (*it).dohvPomeraj();
        }
    }
    return 0;
}

void Tabela::dodajOffset(Unapred u, string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).dodajOffset(u);
        }
    }
}

bool Tabela::dohvExt(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return (*it).dohvExt();
        }
    }
    return false;
}

void Tabela::postDef(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).postDef();
        }
    }
}

list<Unapred> Tabela::dohvUnapred(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return (*it).dohvUnapred();
        }
    }
    return list<Unapred>();
}

void Tabela::postSekciju(int sek, string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).postSekciju(sek);
        }
    }
}

void Tabela::postPomeraj(int pom, string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).postPomeraj(pom);
        }
    }
}

void Tabela::postRedBr(int redBr, string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).postRedBr(redBr);
        }
    }
}

void Tabela::obrisiSimbol(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            it = simboli.erase(it);
        }
    }
}

void Tabela::azurRedBr()
{
    int i = 0;
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        (*it).postRedBr(i++);
    }
}

int Tabela::dohvRedBr(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return (*it).dohvRedBr();
        }
    }
    return 0;
}

int Tabela::dohvSekciju(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return (*it).dohvSekciju();
        }
    }
    return 0;
}

Simbol *Tabela::dohvAdrSimb(string s)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            return (*it).dohvAdrSimb();
        }
    }
    return nullptr;
}

void Tabela::postOpseg(string s, string opseg)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).postOpseg(opseg);
        }
    }
}

void Tabela::postEqu(string s, list<string> &l)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == s)
        {
            (*it).postEqu(l);
        }
    }
}

void Tabela::equObrada(list<string> &sekcije, list<Reltabela> &reltabele, list<Maskod> &kodovi)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        list<string> l = (*it).dohvEqu();
        if (!(l.empty()) && !((*it).dohvDef()))
        {
            int br = 0;
            if (*(l.begin()) == "-" || *(l.begin()) == "+")
            {
                br++;
            }
            int n = 0;
            int *niz = new int[sekcije.size()];
            for (int i = 0; i < sekcije.size(); i++)
            {
                niz[i] = 0;
            }
            int *niz1 = new int[2];
            for (int i = 0; i < 2; i++)
            {
                niz1[i] = 0;
            }
            int *niz2 = new int[2];
            for (int i = 0; i < 2; i++)
            {
                niz2[i] = 0;
            }
            for (list<string>::iterator it1 = l.begin(); it1 != l.end(); ++it1)
            {
                if ((br % 2) == 0 && (regex_match(*it1, Regeksi::simbol) || regex_match(*it1, Regeksi::broj)))
                {
                    if (regex_match(*it1, Regeksi::simbol))
                    {
                        if (!(pronDefSimbol(*it1)) || !(pronSimbol(*it1)))
                        {
                            break;
                        }
                    }
                    int i1 = 0;
                    if (regex_match(*it1, Regeksi::simbol))
                    {
                        i1 = dohvPomeraj(*it1);
                    }
                    else
                    {
                        i1 = stoi(*it1);
                    }

                    if (it1 != l.begin())
                    {
                        if (*(--it1) == "-")
                        {
                            i1 = -i1;
                            n += i1;
                            if (regex_match(*(++it1), Regeksi::simbol))
                            {
                                if (localDef(*it1))
                                {
                                    (niz1[0])--;
                                    (niz2[0])++;
                                }
                                else if (globalDef(*it1))
                                {
                                    (niz1[1])--;
                                    (niz2[1])++;
                                }
                            }
                        }
                        else
                        {
                            n += i1;
                            if (regex_match(*(++it1), Regeksi::simbol))
                            {
                                if (localDef(*it1))
                                {
                                    (niz1[0])++;
                                    (niz2[0])++;
                                }
                                else if (globalDef(*it1))
                                {
                                    (niz1[1])++;
                                    (niz2[1])++;
                                }
                            }
                        }
                    }
                    else
                    {
                        n += i1;
                        if (regex_match(*(it1), Regeksi::simbol))
                        {
                            if (localDef(*it1))
                            {
                                (niz1[0])++;
                                (niz2[0])++;
                            }
                            else if (globalDef(*it1))
                            {
                                (niz1[1])++;
                                (niz2[1])++;
                            }
                        }
                    }

                    if (regex_match(*it1, Regeksi::simbol))
                    {
                        int i2 = 0;
                        for (list<string>::iterator it2 = sekcije.begin(); it2 != sekcije.end(); ++it2, i2++)
                        {
                            if (dohvSekciju(*it1) == (i2 + 1))
                            {
                                if (it1 != l.begin())
                                {
                                    if (*(--it1) == "-")
                                    {
                                        (niz[i2])--;
                                    }
                                    else
                                    {
                                        (niz[i2])++;
                                    }
                                    it1++;
                                }
                                else
                                {
                                    (niz[i2])++;
                                }
                                break;
                            }
                        }
                    }

                    if ((++it1) == l.end())
                    {
                        if (niz1[0] > 0 && niz1[0] > niz1[1])
                        {
                            (*it).postOpseg("local");
                        }
                        else if (niz1[1] > 0 && niz1[1] > niz1[0])
                        {
                            (*it).postOpseg("global");
                        }
                        else if (niz1[0] == niz1[1] && niz2[0] > niz2[1])
                        {
                            (*it).postOpseg("local");
                        }
                        else if (niz1[0] == niz1[1] && niz2[1] > niz2[0])
                        {
                            (*it).postOpseg("global");
                        }
                        else if (niz1[0] == niz1[1] && niz2[1] == niz2[0])
                        {
                            (*it).postOpseg("local");
                        }
                        else
                        {
                            throw runtime_error("***Greska kod .equ direktive (1)***");
                        }

                        int i3 = 0;

                        for (int i = 0; i < sekcije.size(); i++)
                        {
                            if (niz[i] > 1 && niz[i] < -1)
                            {
                                throw runtime_error("***Greska kod .equ direktive (2)***");
                            }
                            else if (niz[i] == 1)
                            {
                                (*it).postSekciju(i + 1);
                                i3++;
                            }
                        }

                        if (i3 > 1)
                        {
                            throw runtime_error("***Greska kod .equ direktive (3)***");
                        }
                        else
                        {
                            if (n < 0)
                            {
                                throw runtime_error("***Izraz u .equ direktivi ne sme biti negativan***");
                            }
                            (*it).postPomeraj(n);
                            (*it).postDef();
                            list<Unapred> li = (*it).dohvUnapred();
                            if (localDef((*it).dohvNaziv()))
                            {
                                for (list<Unapred>::iterator it2 = li.begin(); it2 != li.end(); ++it2)
                                {
                                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                                    {
                                        if ((*it2).dohvSekciju() == (*it2).dohvSekciju())
                                        {
                                            list<Reltabela>::iterator it4;
                                            for (it4 = reltabele.begin(); it4 != reltabele.end(); ++it4)
                                            {
                                                if ((*it).dohvSekciju() == dohvSekciju((*it4).dohvSekciju()))
                                                {
                                                    break;
                                                }
                                            }
                                            (*it3).promeniKod((*it2).dohvPomeraj(), (*it2).dohvKol(), (*it).dohvNaziv(), *this, (it4 != reltabele.end() ? &(*it4) : nullptr));
                                        }
                                    }
                                }
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (dohvSekciju((*it).dohvNaziv()) == dohvSekciju((*it2).dohvSekciju()))
                                    {
                                        (*it2).obrisiZapise((*it).dohvNaziv());
                                        break;
                                    }
                                }
                            }
                            li.clear();
                        }
                    }
                    it1--;
                    br++;
                }
                else if ((br % 2) == 0 && !(regex_match(*it1, Regeksi::simbol)))
                {
                    throw runtime_error("***Ocekivao se simbol u .equ direktivi***");
                }
                else if ((br % 2) == 1 && (*it1 == "+" || *it1 == "-"))
                {
                    br++;
                    it1++;
                    if (it1 == l.end())
                    {
                        throw runtime_error("***Direktiva .equ se ne moze zavrsiti operatorom***");
                    }
                    it1--;
                }
                else if ((br % 2) == 1 && (*it1 != "+" && *it1 != "-"))
                {
                    throw runtime_error("***Ocekivao se operator u .equ direktivi (2)***");
                }
                else
                {
                    throw runtime_error("***Greska kod .equ direktive***");
                }
            }
            delete niz;
            delete niz1;
            delete niz2;
        }
    }
}

void Tabela::postVred(string sekcija, int vred)
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if ((*it).dohvNaziv() == sekcija)
        {
            (*it).postVred(vred);
            break;
        }
    }
}

bool Tabela::provera()
{
    for (list<Simbol>::iterator it = simboli.begin(); it != simboli.end(); ++it)
    {
        if (!((*it).dohvDef()))
        {
            if (!((*it).dohvExt()))
            {
                return false;
            }
        }

        if ((*it).dohvSekciju() != (*it).dohvRedBr())
        {
            for (list<Simbol>::iterator it1 = simboli.begin(); it1 != simboli.end(); ++it1)
            {
                if ((*it1).dohvSekciju() == (*it1).dohvRedBr() && (*it).dohvSekciju() == (*it1).dohvSekciju())
                {
                    if (((*it).dohvPomeraj() > (*it1).dohvVred()) || ((*it).dohvPomeraj() < 0))
                    {
                        return false;
                    }
                }
                else if ((*it1).dohvSekciju() != (*it1).dohvRedBr())
                {
                    break;
                }
            }
        }
    }
    return true;
}