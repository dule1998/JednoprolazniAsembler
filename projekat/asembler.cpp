#include "asembler.h"

void Asembler::prolaz(ifstream &ul, ofstream &iz)
{
    Token t = Token(ul);
    tokeni = t.dohvTokene();
    bool zavrsio = false;
    for (list<list<string>>::iterator it = tokeni.begin(); it != tokeni.end(); ++it)
    {
        bool lab = false;
        string str = (*it).front();
        if (regex_match(str, Regeksi::labela))
        {
            lab = true;
            (*it).pop_front();
            if (trSekcija == 0)
            {
                throw runtime_error("***Labela ne moze da se nalazi van sekcija***");
            }
            else
            {
                str = str.substr(0, str.size() - 1);
                if (tabSimb.pronSimbol(str) && tabSimb.pronDefSimbol(str))
                {
                    string s = "***Labela" + str + "se pojavljuje vise puta***";
                    throw runtime_error(s);
                }
                else if (!(tabSimb.pronSimbol(str)))
                {
                    Simbol simb = Simbol(str, trSekcija, lc, 0, "local", trRedBr, 0, true);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    tabSimb.equObrada(sekcije, reltabele, kodovi);
                }
                else if (tabSimb.pronSimbol(str) && tabSimb.dohvExt(str))
                {
                    throw runtime_error("***Labela deklarisana u .extern direktivi je kasnije definisana***");
                }
                else if (tabSimb.pronSimbol(str) && !(tabSimb.pronDefSimbol(str)))
                {
                    tabSimb.postDef(str);
                    tabSimb.postSekciju(trSekcija, str);
                    tabSimb.postPomeraj(lc, str);
                    list<Unapred> l = tabSimb.dohvUnapred(str);
                    for (list<Unapred>::iterator it1 = l.begin(); it1 != l.end(); ++it1)
                    {
                        for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                        {
                            if ((*it2).dohvSekciju() == (*it1).dohvSekciju())
                            {
                                list<Reltabela>::iterator it3;
                                for (it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                                {
                                    if (trSekcijaS == (*it3).dohvSekciju())
                                    {
                                        break;
                                    }
                                }
                                (*it2).promeniKod((*it1).dohvPomeraj(), (*it1).dohvKol(), str, tabSimb, (it3 != reltabele.end() ? &(*it3) : nullptr));
                            }
                        }
                    }
                    for (list<Reltabela>::iterator it1 = reltabele.begin(); it1 != reltabele.end(); ++it1)
                    {
                        if (tabSimb.dohvSekciju(str) == trSekcija && trSekcijaS == (*it1).dohvSekciju())
                        {
                            (*it1).obrisiZapise(str);
                            break;
                        }
                    }
                    l.clear();
                    tabSimb.equObrada(sekcije, reltabele, kodovi);
                }
            }

            if ((*it).empty())
            {
                continue;
            }
            else
            {
                str = (*it).front();
            }
        }
        if (str == ".global")
        {
            if ((*it).size() == 1)
            {
                throw runtime_error("***Nije naveden nijedan simbol u okviru .global direktive***");
            }
            int br = 0;
            for (list<string>::iterator it1 = ++((*it).begin()); it1 != (*it).end(); ++it1)
            {
                string str1 = *it1;
                if (br % 2 == 0 && regex_match(str1, Regeksi::simbol))
                {
                    if (tabSimb.localDef(str1))
                    {
                        throw runtime_error("***Simbol se prvo navodi u .global direktivi***");
                    }
                    else
                    {
                        if (tabSimb.pronSimbol(str1) == false)
                        {
                            Simbol simb = Simbol(str1, trSekcija, lc, 0, "global", trRedBr, 0, false);
                            trRedBr++;
                            tabSimb.dodajSimbol(simb);
                            br++;
                        }
                        else
                        {
                            throw runtime_error("***Simbol je naveden vise puta u okviru .global direktive***");
                        }
                    }
                }
                else if (br % 2 == 0 && str1 == ",")
                {
                    throw runtime_error("***Ocekivao se simbol umesto zareza u .global direktivi***");
                }
                else if (br % 1 == 0 && regex_match(str1, Regeksi::simbol))
                {
                    throw runtime_error("***Ocekivao se zarez umesto simbola u .global direktivi***");
                }
                else if (br % 2 == 1 && str1 == ",")
                {
                    br++;
                    if ((++it1) == (*it).end())
                    {
                        throw runtime_error("***Direktiva .global se zavrsila zarezom***");
                    }
                    --it1;
                }
                else
                {
                    throw runtime_error("***Greska kod .global direktive***");
                }
            }
        }
        if (str == ".extern")
        {
            if ((*it).size() == 1)
            {
                throw runtime_error("***Nije naveden nijedan simbol u okviru .extern direktive***");
            }
            int br = 0;
            for (list<string>::iterator it1 = ++((*it).begin()); it1 != (*it).end(); ++it1)
            {
                string str1 = *it1;
                if (br % 2 == 0 && regex_match(str1, Regeksi::simbol))
                {
                    if (tabSimb.pronSimbol(str1))
                    {
                        throw runtime_error("***Simbol je vec definisan (proveri .extern)***");
                    }
                    Simbol simb = Simbol(str1, 0, 0, 0, "global", trRedBr, 0, false);
                    trRedBr++;
                    simb.postExt();
                    tabSimb.dodajSimbol(simb);
                    br++;
                }
                else if (br % 2 == 0 && str1 == ",")
                {
                    throw runtime_error("***Ocekivao se simbol umesto zareza u .extern direktivi***");
                }
                else if (br % 1 == 0 && regex_match(str1, Regeksi::simbol))
                {
                    throw runtime_error("***Ocekivao se zarez umesto simbola u .extern direktivi***");
                }
                else if (br % 2 == 1 && str1 == ",")
                {
                    br++;
                    if ((++it1) == (*it).end())
                    {
                        throw runtime_error("***Direktiva .extern se zavrsila zarezom***");
                    }
                    --it1;
                }
                else
                {
                    throw runtime_error("***Greska kod .extern direktive***");
                }
            }
        }
        if (str == ".section")
        {
            if ((*it).size() > 2)
            {
                throw runtime_error("***Naziv sekcije mora da se sastoji od jedne reci***");
            }
            else if (!(regex_match(*(++((*it).begin())), Regeksi::sekcija)))
            {
                throw runtime_error("***Pogresan naziv sekcije***");
            }

            else
            {
                int flag = 0;
                if (*(++((*it).begin())) == ".text")
                {
                    flag = 0xD;
                }
                else if (*(++((*it).begin())) == ".data")
                {
                    flag = 0xB;
                }
                else if (*(++((*it).begin())) == ".bss")
                {
                    flag = 0x3;
                }
                else
                {
                    throw runtime_error("***Pogresan naziv sekcije***");
                }
                for (list<string>::iterator it1 = sekcije.begin(); it1 != sekcije.end(); ++it1)
                {
                    if (*(++((*it).begin())) == *it1)
                    {
                        throw runtime_error("***Postoje dve sekcije sa istim nazivom***");
                    }
                }

                sekcije.push_back(*(++((*it).begin())));
                list<Simbol> &lista = tabSimb.dohvSimbole();
                for (list<Simbol>::iterator it1 = lista.begin(); it1 != lista.end(); ++it1)
                {
                    if ((trSekcija == (*it1).dohvSekciju()) && ((*it1).dohvNaziv() == trSekcijaS))
                    {
                        (*it1).postVred(lc);
                        break;
                    }
                }

                bool b = false;
                if (tabSimb.pronSimbol(*(++((*it).begin()))))
                {
                    list<Unapred> l = tabSimb.dohvUnapred(*(++((*it).begin())));
                    for (list<Unapred>::iterator it1 = l.begin(); it1 != l.end(); ++it1)
                    {
                        for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                        {
                            if ((*it2).dohvSekciju() == (*it1).dohvSekciju())
                            {
                                list<Reltabela>::iterator it3;
                                for (it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                                {
                                    if (trSekcijaS == (*it3).dohvSekciju())
                                    {
                                        break;
                                    }
                                }
                                (*it2).promeniKod((*it1).dohvPomeraj(), (*it1).dohvKol(), *(++((*it).begin())), tabSimb, (it3 != reltabele.end() ? &(*it3) : nullptr));
                            }
                        }
                    }
                    l.clear();
                    for (list<Reltabela>::iterator it1 = reltabele.begin(); it1 != reltabele.end(); ++it1)
                    {
                        (*it1).postaviNull(*(++((*it).begin())));
                    }
                    tabSimb.obrisiSimbol(*(++((*it).begin())));
                    tabSimb.azurRedBr();
                    b = true;
                }
                trSekcijaS = *(++((*it).begin()));
                trSekcija++;
                lc = 0;
                Simbol simb = Simbol(*(++((*it).begin())), trSekcija, 0, 0, "local", trSekcija, flag, true);
                trRedBr++;
                tabSimb.dodajSimbol(simb);

                for (list<Reltabela>::iterator it1 = reltabele.begin(); it1 != reltabele.end(); ++it1)
                {
                    if (b)
                    {
                        (*it1).postNotNull(tabSimb.dohvAdrSimb(*(++((*it).begin()))));
                    }
                    (*it1).azurPok();
                }
            }
        }
        if (str == ".skip")
        {
            if ((*it).size() > 2)
            {
                throw runtime_error("***Direktiva .skip mora da ima samo jedan parametar koji je broj***");
            }
            else if (trSekcija == 0)
            {
                throw runtime_error("***Direktiva .skip je van bilo koje sekcije***");
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::broj))
            {
                bool b = true;
                int count = stoi(*(++((*it).begin())));
                for (list<Maskod>::iterator it1 = kodovi.begin(); it1 != kodovi.end(); ++it1)
                {
                    if (trSekcijaS == (*it1).dohvSekciju())
                    {
                        if (trSekcijaS != ".bss")
                        {
                            (*it1).skip(count);
                        }
                        b = false;
                    }
                }

                if (b && (trSekcijaS != ".bss"))
                {
                    Maskod m = Maskod(trSekcijaS);
                    m.skip(count);
                    kodovi.push_back(m);
                }

                lc += count;
            }
        }
        if (str == ".byte")
        {
            if ((*it).size() < 2)
            {
                throw runtime_error("***Direktiva .byte mora da ima jedan ili vise argumenata, razdvojenih zarezom***");
            }
            else if (trSekcijaS == ".bss")
            {
                throw runtime_error("***Direktiva .byte se ne moze nalaziti u .bss sekciji***");
            }
            else if (trSekcija == 0)
            {
                throw runtime_error("***Direktiva .byte se ne moze nalaziti van sekcija***");
            }
            else
            {
                int br = 0;
                for (list<string>::iterator it1 = ++((*it).begin()); it1 != (*it).end(); ++it1)
                {
                    string str1 = *it1;
                    if (br % 2 == 0)
                    {
                        if (regex_match(str1, Regeksi::simbol) || regex_match(str1, Regeksi::sekcija))
                        {
                            if (tabSimb.pronDefSimbol(str1))
                            {
                                bool b = true;
                                int num = 0;
                                int rb = tabSimb.dohvRedBr(str1);
                                if ((tabSimb.localDef(str1)))
                                {
                                    num = tabSimb.dohvPomeraj(str1);
                                    rb = tabSimb.dohvSekciju(str1);
                                }
                                if (num < -128 || num > 127)
                                {
                                    num &= 0xFF;
                                }
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajBajt((char)num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajBajt((char)num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 1;
                                br++;
                            }
                            else if (tabSimb.globalDef(str1))
                            {
                                bool b = true;
                                char num = 0;
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajBajt(num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajBajt(num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 1;
                                br++;
                            }
                            else if (tabSimb.pronSimbol(str1))
                            {
                                Unapred u = Unapred(lc, 1, trSekcijaS);
                                tabSimb.dodajOffset(u, str1);
                                bool b = true;
                                char num = 0;
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajBajt(num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajBajt(num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 1;
                                br++;
                            }
                            else if (!(tabSimb.pronSimbol(str1)))
                            {
                                Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                                trRedBr++;
                                tabSimb.dodajSimbol(simb);
                                Unapred u = Unapred(lc, 1, trSekcijaS);
                                tabSimb.dodajOffset(u, str1);
                                bool b = true;
                                char num = 0;
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajBajt(num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajBajt(num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 1;
                                br++;
                            }
                            else
                            {
                                throw runtime_error("***Greska kod .byte direktive (kod koriscenja simbola)***");
                            }
                        }
                        else if (regex_match(str1, Regeksi::broj))
                        {
                            bool b = true;
                            int num = stoi(str1);
                            if (num < -128 || num > 127)
                            {
                                throw runtime_error("***Broj u .byte direktivi nije u dozvoljenom opsegu***");
                            }
                            for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                            {
                                if (trSekcijaS == (*it2).dohvSekciju())
                                {
                                    (*it2).dodajBajt((char)num);
                                    b = false;
                                }
                            }

                            if (b)
                            {
                                Maskod m = Maskod(trSekcijaS);
                                m.dodajBajt((char)num);
                                kodovi.push_back(m);
                            }

                            lc += 1;
                            br++;
                        }
                        else
                        {
                            throw runtime_error("***Ocekivao se simbol ili decimalan broj kod .byte direktive***");
                        }
                    }
                    else if (br % 1 == 0 && (regex_match(str1, Regeksi::simbol) || regex_match(str1, Regeksi::broj)))
                    {
                        throw runtime_error("***Ocekivao se zarez umesto simbola u .byte direktivi***");
                    }
                    else if (br % 2 == 1 && str1 == ",")
                    {
                        br++;
                        if ((++it1) == (*it).end())
                        {
                            throw runtime_error("***Direktiva .byte se zavrsila zarezom***");
                        }
                        --it1;
                    }
                    else
                    {
                        throw runtime_error("***Greska kod .byte direktive***");
                    }
                }
            }
        }
        if (str == ".word")
        {
            if ((*it).size() < 2)
            {
                throw runtime_error("***Direktiva .word mora da ima jedan ili vise argumenata, razdvojenih zarezom***");
            }
            else if (trSekcijaS == ".bss")
            {
                throw runtime_error("***Direktiva .word se ne moze nalaziti u .bss sekciji***");
            }
            else if (trSekcija == 0)
            {
                throw runtime_error("***Direktiva .word se ne moze nalaziti van sekcija***");
            }
            else
            {
                int br = 0;
                for (list<string>::iterator it1 = ++((*it).begin()); it1 != (*it).end(); ++it1)
                {
                    string str1 = *it1;
                    if (br % 2 == 0)
                    {
                        if (regex_match(str1, Regeksi::simbol) || regex_match(str1, Regeksi::sekcija))
                        {
                            if (tabSimb.pronDefSimbol(str1))
                            {
                                bool b = true;
                                int num = 0;
                                int rb = tabSimb.dohvRedBr(str1);
                                if ((tabSimb.localDef(str1)))
                                {
                                    num = tabSimb.dohvPomeraj(str1);
                                    rb = tabSimb.dohvSekciju(str1);
                                }
                                if (num < -32768 || num > 32767)
                                {
                                    num &= 0xFFFF;
                                }
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        if ((lc % 2) != 0)
                                        {
                                            (*it2).dodajBajt(0);
                                            lc += 1;
                                        }
                                        (*it2).dodajRec((short)num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajRec((short)num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 2;
                                br++;
                            }
                            else if (tabSimb.globalDef(str1))
                            {
                                bool b = true;
                                short num = 0;
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        if ((lc % 2) != 0)
                                        {
                                            (*it2).dodajBajt(0);
                                            lc += 1;
                                        }
                                        (*it2).dodajRec(num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajRec(num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 2;
                                br++;
                            }
                            else if (tabSimb.pronSimbol(str1))
                            {
                                Unapred u = Unapred(lc, 2, trSekcijaS);
                                tabSimb.dodajOffset(u, str1);
                                bool b = true;
                                short num = 0;
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        if ((lc % 2) != 0)
                                        {
                                            (*it2).dodajBajt(0);
                                            lc += 1;
                                        }
                                        (*it2).dodajRec(num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajRec(num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 2;
                                br++;
                            }
                            else if (!(tabSimb.pronSimbol(str1)))
                            {
                                Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                                trRedBr++;
                                tabSimb.dodajSimbol(simb);
                                Unapred u = Unapred(lc, 2, trSekcijaS);
                                tabSimb.dodajOffset(u, str1);
                                bool b = true;
                                short num = 0;
                                for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        if ((lc % 2) != 0)
                                        {
                                            (*it2).dodajBajt(0);
                                            lc += 1;
                                        }
                                        (*it2).dodajRec(num);
                                        b = false;
                                    }
                                }

                                if (b)
                                {
                                    Maskod m = Maskod(trSekcijaS);
                                    m.dodajRec(num);
                                    kodovi.push_back(m);
                                }

                                Relzapis z = Relzapis(lc, "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                                bool b1 = true;
                                for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                {
                                    if (trSekcijaS == (*it2).dohvSekciju())
                                    {
                                        (*it2).dodajZapis(z);
                                        b1 = false;
                                    }
                                }

                                if (b1)
                                {
                                    Reltabela r = Reltabela(trSekcijaS);
                                    r.dodajZapis(z);
                                    reltabele.push_back(r);
                                }

                                lc += 2;
                                br++;
                            }
                            else
                            {
                                throw runtime_error("***Greska kod .word direktive (kod koriscenja simbola)***");
                            }
                        }
                        else if (regex_match(str1, Regeksi::broj))
                        {
                            bool b = true;
                            int num = stoi(str1);
                            if (num < -32768 || num > 32767)
                            {
                                throw runtime_error("***Broj u .word direktivi nije u dozvoljenom opsegu***");
                            }
                            for (list<Maskod>::iterator it2 = kodovi.begin(); it2 != kodovi.end(); ++it2)
                            {
                                if (trSekcijaS == (*it2).dohvSekciju())
                                {
                                    if ((lc % 2) != 0)
                                    {
                                        (*it2).dodajBajt(0);
                                        lc += 1;
                                    }
                                    (*it2).dodajRec((short)num);
                                    b = false;
                                }
                            }

                            if (b)
                            {
                                Maskod m = Maskod(trSekcijaS);
                                m.dodajRec((short)num);
                                kodovi.push_back(m);
                            }

                            lc += 2;
                            br++;
                        }
                        else
                        {
                            throw runtime_error("***Ocekivao se simbol ili decimalan broj kod .word direktive***");
                        }
                    }
                    else if (br % 1 == 0 && (regex_match(str1, Regeksi::simbol) || regex_match(str1, Regeksi::broj)))
                    {
                        throw runtime_error("***Ocekivao se zarez umesto simbola u .word direktivi***");
                    }
                    else if (br % 2 == 1 && str1 == ",")
                    {
                        br++;
                        if ((++it1) == (*it).end())
                        {
                            throw runtime_error("***Direktiva .word se zavrsila zarezom***");
                        }
                        --it1;
                    }
                    else
                    {
                        throw runtime_error("***Greska kod .word direktive***");
                    }
                }
            }
        }
        if (regex_match(str, Regeksi::instrukcija))
        {
            map<string, char>::iterator it1 = (Instrukcije::brOpr).find(str);
            map<string, char>::iterator it2 = (Instrukcije::oc).find(str);

            if ((it1->second != ((*it).size() - 1) && (it1->second == 0 || it1->second == 1)) || (it1->second != ((*it).size() - 2) && it1->second == 2))
            {
                throw runtime_error("***Los broj parametara kod instrukcije***");
            }
            if ((*it).size() == 4 && it1->second == 2)
            {
                if (*(++(++((*it).begin()))) != ",")
                {
                    throw runtime_error("***Ocekivao se zarez kod instrukcije***");
                }
            }
            if (trSekcijaS != ".text")
            {
                throw runtime_error("***Instrukcije se moraju nalaziti iskljucivo u .text sekciji***");
            }
            if (regex_match(str, Regeksi::instrukcija1))
            {
                throw runtime_error("***Instrukcija sa nepotrebnim sufiksom***");
            }

            char c = it2->second << 3;
            c |= 0x4;
            if ((str[str.size() - 1] == 'b') || (str == "subb"))
            {
                c = it2->second << 3;
            }
            bool b = true;
            for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
            {
                if (trSekcijaS == (*it3).dohvSekciju())
                {
                    (*it3).dodajBajt(c);
                    b = false;
                }
            }

            if (b)
            {
                Maskod m = Maskod(trSekcijaS);
                m.dodajBajt(c);
                kodovi.push_back(m);
            }

            lc += 1;

            if ((*it).size() == 1 && regex_match(str, Regeksi::instrukcija3))
            {
                continue;
            }
            else if ((*it).size() == 1)
            {
                throw runtime_error("***Instrukcija treba da ima argumente***");
            }

            smatch sm;

            if (regex_match(*(++((*it).begin())), sm, Regeksi::regdir) && ((*it).size() == 4 || *((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw"))
            {
                if (((str[str.size() - 1] == 'b') && (str != "sub")) && (sm[sm.size() - 1] != 'h' && sm[sm.size() - 1] != 'l'))
                {
                    throw runtime_error("***Instrukcija se ne slaze sa operandima***");
                }
                else if ((str[str.size() - 1] == 'w' || regex_match(str, Regeksi::instrukcija2)) && (sm[sm.size() - 1] == 'l' || sm[sm.size() - 1] == 'h'))
                {
                    throw runtime_error("***Instrukcija se ne slaze sa operandima***");
                }

                char c1 = 1;
                c1 <<= 5;
                int i = (*(++((*it).begin())))[2] - '0';
                i <<= 1;
                c1 |= i;
                if (sm[sm.size() - 1] != 'l' && sm[sm.size() - 1] != 'h')
                {
                    c1 |= 0x1;
                }

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;
            }
            else if (regex_match(*(++((*it).begin())), sm, Regeksi::regind) && ((*it).size() == 4 || *((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw"))
            {
                char c1 = 2;
                c1 <<= 5;
                int i = (*(++((*it).begin())))[3] - '0';
                i <<= 1;
                c1 |= i;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;
            }
            else if (regex_match(*(++((*it).begin())), sm, Regeksi::regindpomb) && ((*it).size() == 4 || *((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw"))
            {
                int i1 = stoi((*(++((*it).begin()))).substr(0, (*(++((*it).begin()))).size() - 5));
                int i2 = (*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0';
                if (i1 > 32767 && i1 < -32768)
                {
                    throw runtime_error("***Preveliki pomeraj kod registarskog indirektnog pomeraja***");
                }
                char c1 = 3;
                c1 <<= 5;
                i2 <<= 1;
                c1 |= i2;
                short s = i2 && 0xFFFF;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                        (*it3).dodajRec(s);
                    }
                }

                lc += 3;
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::simbol1) && ((*it).size() == 4 || *((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw"))
            {
                string str1 = (*(++((*it).begin()))).substr(1, (*(++((*it).begin()))).size() - 1);

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(0);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                num &= 0xFF;
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                num &= 0xFFFF;
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else if (tabSimb.globalDef(str1))
                {
                    int num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, (str[str.size() - 1] == 'b' ? 1 : 2), trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    int num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, (str[str.size() - 1] == 'b' ? 1 : 2), trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    int num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else
                {
                    throw runtime_error("***Greska prilikom memorijskog adresiranja***");
                }
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::broj1) && ((*it).size() == 4 || *((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw"))
            {
                int num = stoi((*(++((*it).begin()))).substr(1, (*(++((*it).begin()))).size() - 1));
                if ((num < -128 || num > 127) && (str[str.size() - 1] == 'b' && str != "sub"))
                {
                    throw runtime_error("***Preveliki broj zadat kao neposredni operand***");
                }
                if ((num < -32768 || num > 32767) && (str[str.size() - 1] == 'w' || regex_match(str, Regeksi::instrukcija2)))
                {
                    throw runtime_error("***Preveliki broj zadat kao neposredni operand***");
                }

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(0);
                    }
                }

                lc += 1;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        if ((num >= -128 && num <= 127) && (str[str.size() - 1] == 'b' && str != "sub"))
                        {
                            (*it3).dodajBajt((char)num);
                            lc += 1;
                        }
                        else if ((num >= -32768 && num <= 32767 && num < -128 && num > 127) && (str[str.size() - 1] == 'w' || regex_match(str, Regeksi::instrukcija2)))
                        {
                            (*it3).dodajRec((short)num);
                            lc += 2;
                        }
                    }
                }
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::regindpoms) && ((*it).size() == 4 || *((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw"))
            {
                string str1 = (*(++((*it).begin()))).substr(0, (*(++((*it).begin()))).size() - 5);
                if (!(regex_match(str1, Regeksi::simbol)))
                {
                    throw runtime_error("***Nije naveden simbol kod registarskog indirektnog adresiranja sa pomerajem***");
                }
                int i2 = (*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0';
                char c1 = 3;
                c1 <<= 5;
                i2 <<= 1;
                c1 |= i2;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    if ((*((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw") && ((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (regex_match(*(++(++(++((*it).begin())))), Regeksi::regdir) || regex_match(*(++(++(++((*it).begin())))), Regeksi::regind)))
                    {
                        num = -3;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (str[str.size() - 1] == 'b' && str != "sub") && (regex_match(*(++(++(++((*it).begin())))), Regeksi::broj1) || regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol1)))
                    {
                        num = -4;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -5;
                    }
                    int b2 = true;

                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                        if ((*((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw") && ((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                        {
                            num = num - (lc + 2);
                        }
                        else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (regex_match(*(++(++(++((*it).begin())))), Regeksi::regdir) || regex_match(*(++(++(++((*it).begin())))), Regeksi::regind)))
                        {
                            num = num - (lc + 3);
                        }
                        else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (str[str.size() - 1] == 'b' && str != "sub") && (regex_match(*(++(++(++((*it).begin())))), Regeksi::broj1) || regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol1)))
                        {
                            num = num - (lc + 4);
                        }
                        else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                        {
                            num = num - (lc + 5);
                        }
                        b = false;
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec((short)num);
                        }
                    }

                    if (b2)
                    {
                        Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 ? "R_386_PC32" : "R_386_32"), rb, tabSimb.dohvAdrSimb(str1));
                        bool b1 = true;
                        for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                        {
                            if (trSekcijaS == (*it3).dohvSekciju())
                            {
                                (*it3).dodajZapis(z);
                                b1 = false;
                            }
                        }

                        if (b1)
                        {
                            Reltabela r = Reltabela(trSekcijaS);
                            r.dodajZapis(z);
                            reltabele.push_back(r);
                        }
                    }

                    lc += 2;
                }
                else if (tabSimb.globalDef(str1))
                {
                    short num = 0;
                    if ((*((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw") && ((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (regex_match(*(++(++(++((*it).begin())))), Regeksi::regdir) || regex_match(*(++(++(++((*it).begin())))), Regeksi::regind)))
                    {
                        num = -3;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (str[str.size() - 1] == 'b' && str != "sub") && (regex_match(*(++(++(++((*it).begin())))), Regeksi::broj1) || regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol1)))
                    {
                        num = -4;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -5;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 ? "R_386_PC32" : "R_386_32"), tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    if ((*((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw") && ((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (regex_match(*(++(++(++((*it).begin())))), Regeksi::regdir) || regex_match(*(++(++(++((*it).begin())))), Regeksi::regind)))
                    {
                        num = -3;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (str[str.size() - 1] == 'b' && str != "sub") && (regex_match(*(++(++(++((*it).begin())))), Regeksi::broj1) || regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol1)))
                    {
                        num = -4;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -5;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 ? "R_386_PC32" : "R_386_32"), tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    if ((*((*it).begin()) == "push" || *((*it).begin()) == "pushb" || *((*it).begin()) == "pushw" || *((*it).begin()) == "pop" || *((*it).begin()) == "popb" || *((*it).begin()) == "popw") && ((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (regex_match(*(++(++(++((*it).begin())))), Regeksi::regdir) || regex_match(*(++(++(++((*it).begin())))), Regeksi::regind)))
                    {
                        num = -3;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 && (str[str.size() - 1] == 'b' && str != "sub") && (regex_match(*(++(++(++((*it).begin())))), Regeksi::broj1) || regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol1)))
                    {
                        num = -4;
                    }
                    else if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -5;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7 ? "R_386_PC32" : "R_386_32"), trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else
                {
                    throw runtime_error("***Greska prilikom registarskog indirektnog adresiranja sa pomerajem***");
                }
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::simbol))
            {
                string str1 = *(++((*it).begin()));
                char c1 = 4;
                if (regex_match(str, Regeksi::instrukcija4))
                {
                    c1 = 0;
                }
                c1 <<= 5;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec((short)num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.globalDef(str1))
                {
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else
                {
                    throw runtime_error("***Greska prilikom memorijskog adresiranja***");
                }
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::broj) && str != "call")
            {
                int num = stoi(*(++((*it).begin())));
                if (num < -32768 || num > 32767)
                {
                    throw runtime_error("***Preveliki broj zadat kao neposredni operand***");
                }

                char c1 = 4;
                if (regex_match(str, Regeksi::instrukcija4))
                {
                    c1 = 0;
                }
                c1 <<= 5;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(0);
                    }
                }

                lc += 1;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajRec((short)num);
                    }
                }

                lc += 2;
            }
            else if (regex_match(*(++((*it).begin())), sm, Regeksi::regdskok) && regex_match(str, Regeksi::instrukcija4) && str != "call")
            {
                char c1 = 1;
                c1 <<= 5;
                int i = (*(++((*it).begin())))[3] - '0';
                i <<= 1;
                c1 |= i;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;
            }
            else if (regex_match(*(++((*it).begin())), sm, Regeksi::regiskok) && regex_match(str, Regeksi::instrukcija4) && str != "call")
            {
                char c1 = 2;
                c1 <<= 5;
                int i = (*(++((*it).begin())))[4] - '0';
                i <<= 1;
                c1 |= i;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;
            }
            else if (regex_match(*(++((*it).begin())), sm, Regeksi::regindpombs) && regex_match(str, Regeksi::instrukcija4) && str != "call")
            {
                int i1 = stoi((*(++((*it).begin()))).substr(1, (*(++((*it).begin()))).size() - 6));
                int i2 = (*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0';
                if (i1 > 32767 && i1 < -32768)
                {
                    throw runtime_error("***Preveliki pomeraj kod registarskog indirektnog pomeraja***");
                }
                char c1 = 3;
                c1 <<= 5;
                i2 <<= 1;
                c1 |= i2;
                short s = i2 && 0xFFFF;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                        (*it3).dodajRec(s);
                    }
                }

                lc += 3;
            }
            else if (regex_match(*(++((*it).begin())), sm, Regeksi::regindpomss) && regex_match(str, Regeksi::instrukcija4) && str != "call")
            {
                string str1 = (*(++((*it).begin()))).substr(1, (*(++((*it).begin()))).size() - 6);
                if (!(regex_match(str1, Regeksi::simbol)))
                {
                    throw runtime_error("***Nije naveden simbol kod registarskog indirektnog adresiranja sa pomerajem***");
                }
                int i2 = (*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0';
                char c1 = 3;
                c1 <<= 5;
                i2 <<= 1;
                c1 |= i2;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    bool b2 = true;
                    if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }

                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                        if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                        {
                            num = num - (lc + 2);
                            b = false;
                        }
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec((short)num);
                        }
                    }

                    if (b2)
                    {
                        Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                        bool b1 = true;
                        for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                        {
                            if (trSekcijaS == (*it3).dohvSekciju())
                            {
                                (*it3).dodajZapis(z);
                                b1 = false;
                            }
                        }

                        if (b1)
                        {
                            Reltabela r = Reltabela(trSekcijaS);
                            r.dodajZapis(z);
                            reltabele.push_back(r);
                        }
                    }

                    lc += 2;
                }
                else if (tabSimb.globalDef(str1))
                {
                    short num = 0;
                    if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    if (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, (((*(++((*it).begin())))[(*(++((*it).begin()))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else
                {
                    throw runtime_error("***Greska prilikom registarskog indirektnog adresiranja sa pomerajem***");
                }
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::simbol2) && regex_match(str, Regeksi::instrukcija4) && str != "call")
            {
                string str1 = (*(++((*it).begin()))).substr(1, (*(++((*it).begin()))).size() - 1);
                char c1 = 4;
                c1 <<= 5;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec((short)num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.globalDef(str1))
                {
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else
                {
                    throw runtime_error("***Greska prilikom memorijskog adresiranja***");
                }
            }
            else if (regex_match(*(++((*it).begin())), Regeksi::broj2) && regex_match(str, Regeksi::instrukcija4) && str != "call")
            {
                int num = stoi((*(++((*it).begin()))).substr(1, (*(++((*it).begin()))).size() - 1));
                if (num < -32768 || num > 32767)
                {
                    throw runtime_error("***Preveliki broj zadat kao neposredni operand***");
                }

                char c1 = 4;
                c1 <<= 5;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajRec((short)num);
                    }
                }

                lc += 2;
            }
            else
            {
                throw runtime_error("***Greska kod prvog operanda instrukcije***");
            }

            if ((*it).size() == 2)
            {
                continue;
            }

            if (regex_match(*(++(++(++((*it).begin())))), sm, Regeksi::regdir))
            {
                if (((str[str.size() - 1] == 'b') && (str != "sub")) && (sm[sm.size() - 1] != 'h' && sm[sm.size() - 1] != 'l'))
                {
                    throw runtime_error("***Instrukcija se ne slaze sa operandima***");
                }
                else if ((str[str.size() - 1] == 'w' || regex_match(str, Regeksi::instrukcija2)) && (sm[sm.size() - 1] == 'l' || sm[sm.size() - 1] == 'h'))
                {
                    throw runtime_error("***Instrukcija se ne slaze sa operandima***");
                }

                char c1 = 1;
                c1 <<= 5;
                int i = (*(++(++(++((*it).begin())))))[2] - '0';
                i <<= 1;
                c1 |= i;
                if (sm[sm.size() - 1] != 'l' && sm[sm.size() - 1] != 'h')
                {
                    c1 |= 0x1;
                }

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;
            }
            else if (regex_match(*(++(++(++((*it).begin())))), sm, Regeksi::regind))
            {
                char c1 = 2;
                c1 <<= 5;
                int i = (*(++(++(++((*it).begin())))))[3] - '0';
                i <<= 1;
                c1 |= i;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;
            }
            else if (regex_match(*(++(++(++((*it).begin())))), sm, Regeksi::regindpomb))
            {
                int i1 = stoi((*(++(++(++((*it).begin()))))).substr(0, (*(++(++(++((*it).begin()))))).size() - 5));
                int i2 = stoi((*(++(++(++((*it).begin()))))).substr((*(++(++(++((*it).begin()))))).size() - 5, 5));
                if (i1 > 32767 && i1 < -32768)
                {
                    throw runtime_error("***Preveliki pomeraj kod registarskog indirektnog pomeraja***");
                }
                char c1 = 3;
                c1 <<= 5;
                i2 <<= 1;
                c1 |= i2;
                short s = i2 && 0xFFFF;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                        (*it3).dodajRec(s);
                    }
                }

                lc += 3;
            }
            else if (regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol1))
            {
                string str1 = (*(++(++(++((*it).begin()))))).substr(1, (*(++(++(++((*it).begin()))))).size() - 1);

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(0);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                num &= 0xFF;
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                num &= 0xFFFF;
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else if (tabSimb.globalDef(str1))
                {
                    int num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                num &= 0xFF;
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                num &= 0xFFFF;
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    int num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                num &= 0xFF;
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                num &= 0xFFFF;
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    int num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            if (str[str.size() - 1] == 'b')
                            {
                                num &= 0xFF;
                                (*it3).dodajBajt((char)num);
                                lc += 1;
                            }
                            else
                            {
                                num &= 0xFFFF;
                                (*it3).dodajRec((short)num);
                                lc += 2;
                            }
                        }
                    }

                    Relzapis z = Relzapis((str[str.size() - 1] == 'b' ? (lc - 1) : (lc - 2)), "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }
                }
                else
                {
                    throw runtime_error("***Greska prilikom memorijskog adresiranja***");
                }
            }
            else if (regex_match(*(++(++(++((*it).begin())))), Regeksi::regindpoms))
            {
                string str1 = (*(++(++(++((*it).begin()))))).substr(0, (*(++(++(++((*it).begin()))))).size() - 5);
                if (!(regex_match(str1, Regeksi::simbol)))
                {
                    throw runtime_error("***Nije naveden simbol kod registarskog indirektnog adresiranja sa pomerajem***");
                }
                int i2 = (*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0';
                char c1 = 3;
                c1 <<= 5;
                i2 <<= 1;
                c1 |= i2;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    if (((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                        if (((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7)
                        {
                            num = num - (lc + 2);
                        }
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec((short)num);
                        }
                    }

                    Relzapis z = Relzapis(lc, ((((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32"), rb, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.globalDef(str1))
                {
                    short num = 0;
                    if (((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, ((((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32"), tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    if (((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, ((((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32"), tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    if (((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7)
                    {
                        num = -2;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, ((((*(++(++(++((*it).begin())))))[(*(++(++(++((*it).begin()))))).size() - 2] - '0') == 7) ? "R_386_PC32" : "R_386_32"), trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else
                {
                    throw runtime_error("***Greska prilikom registarskog indirektnog adresiranja sa pomerajem***");
                }
            }
            else if (regex_match(*(++(++(++((*it).begin())))), Regeksi::simbol))
            {
                string str1 = *(++(++(++((*it).begin()))));
                char c1 = 4;
                c1 <<= 5;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(c1);
                    }
                }

                lc += 1;

                if (tabSimb.pronDefSimbol(str1))
                {
                    int num = 0;
                    int rb = tabSimb.dohvRedBr(str1);
                    if ((tabSimb.localDef(str1)))
                    {
                        num = tabSimb.dohvPomeraj(str1);
                        rb = tabSimb.dohvSekciju(str1);
                    }
                    if (num < -32768 || num > 32767)
                    {
                        num &= 0xFFFF;
                    }
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec((short)num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", rb, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.globalDef(str1))
                {
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvRedBr(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (tabSimb.pronSimbol(str1))
                {
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", tabSimb.dohvSekciju(str1), tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else if (!(tabSimb.pronSimbol(str1)))
                {
                    Simbol simb = Simbol(str1, trSekcija, 0, 0, "local", trRedBr, 0, false);
                    trRedBr++;
                    tabSimb.dodajSimbol(simb);
                    Unapred u = Unapred(lc, 2, trSekcijaS);
                    tabSimb.dodajOffset(u, str1);
                    short num = 0;
                    for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajRec(num);
                        }
                    }

                    Relzapis z = Relzapis(lc, "R_386_32", trSekcija, tabSimb.dohvAdrSimb(str1));
                    bool b1 = true;
                    for (list<Reltabela>::iterator it3 = reltabele.begin(); it3 != reltabele.end(); ++it3)
                    {
                        if (trSekcijaS == (*it3).dohvSekciju())
                        {
                            (*it3).dodajZapis(z);
                            b1 = false;
                        }
                    }

                    if (b1)
                    {
                        Reltabela r = Reltabela(trSekcijaS);
                        r.dodajZapis(z);
                        reltabele.push_back(r);
                    }

                    lc += 2;
                }
                else
                {
                    throw runtime_error("***Greska prilikom memorijskog adresiranja***");
                }
            }
            else if (regex_match(*(++(++(++((*it).begin())))), Regeksi::broj))
            {
                int num = stoi(*(++(++(++((*it).begin())))));
                if (num < -32768 || num > 32767)
                {
                    throw runtime_error("***Preveliki broj zadat kao neposredni operand***");
                }

                char c1 = 4;
                c1 <<= 5;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajBajt(0);
                    }
                }

                lc += 1;

                for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                {
                    if (trSekcijaS == (*it3).dohvSekciju())
                    {
                        (*it3).dodajRec((short)num);
                    }
                }

                lc += 2;
            }
            else
            {
                throw runtime_error("***Greska kod drugog operanda instrukcije***");
            }
        }
        if (str == ".equ")
        {
            if ((*it).size() < 4)
            {
                throw runtime_error("***Direktiva .equ nije definisana kako treba***");
            }
            if (regex_match(*(++(*it).begin()), Regeksi::simbol))
            {
                if (tabSimb.pronSimbol(*(++(*it).begin())) && tabSimb.pronDefSimbol(*(++(*it).begin())))
                {
                    throw runtime_error("***Simbol definisan .equ direktivom vec postoji***");
                }
            }
            else
            {
                throw runtime_error("***Ocekivao se simbol u .equ direktivi***");
            }
            if (*(++(++(*it).begin())) != ",")
            {
                throw runtime_error("***Ocekivao se zarez u .equ direktivi***");
            }
            if (!(tabSimb.pronSimbol(*(++(*it).begin()))))
            {
                Simbol simb = Simbol(*(++(*it).begin()), trSekcija, 0, 0, "local", trRedBr, 0, false);
                trRedBr++;
                tabSimb.dodajSimbol(simb);
            }
            int br = 0;
            if (*(++(++(++(*it).begin()))) == "-" || *(++(++(++(*it).begin()))) == "+")
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
            bool b = true;
            for (list<string>::iterator it1 = ++(++(++(*it).begin())); it1 != (*it).end(); ++it1)
            {
                if ((br % 2) == 0 && (regex_match(*it1, Regeksi::simbol) || regex_match(*it1, Regeksi::broj)))
                {
                    if (b)
                    {
                        if (regex_match(*it1, Regeksi::simbol))
                        {
                            if (!(tabSimb.pronDefSimbol(*it1)) || !(tabSimb.pronSimbol(*it1)))
                            {
                                list<string> l;
                                for (list<string>::iterator it2 = ++(++(++(*it).begin())); it2 != (*it).end(); ++it2)
                                {
                                    l.push_back(*it2);
                                }
                                tabSimb.postEqu(*(++(*it).begin()), l);
                                br++;
                                b = false;
                                continue;
                            }
                        }
                        int i1 = 0;
                        if (regex_match(*it1, Regeksi::simbol))
                        {
                            i1 = tabSimb.dohvPomeraj(*it1);
                        }
                        else
                        {
                            i1 = stoi(*it1);
                        }

                        if (it1 != ++(++(++(*it).begin())))
                        {
                            if (*(--it1) == "-")
                            {
                                i1 = -i1;
                                n += i1;
                                if (regex_match(*(++it1), Regeksi::simbol))
                                {
                                    if (tabSimb.localDef(*it1))
                                    {
                                        (niz1[0])--;
                                        (niz2[0])++;
                                    }
                                    else if (tabSimb.globalDef(*it1))
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
                                    if (tabSimb.localDef(*it1))
                                    {
                                        (niz1[0])++;
                                        (niz2[0])++;
                                    }
                                    else if (tabSimb.globalDef(*it1))
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
                            if (regex_match(*it1, Regeksi::simbol))
                            {
                                if (tabSimb.localDef(*it1))
                                {
                                    (niz1[0])++;
                                    (niz2[0])++;
                                }
                                else if (tabSimb.globalDef(*it1))
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
                                if (tabSimb.dohvSekciju(*it1) == (i2 + 1))
                                {
                                    if (it1 != ++(++(++(*it).begin())))
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

                        if ((++it1) == (*it).end())
                        {
                            if (niz1[0] > 0 && niz1[0] > niz1[1])
                            {
                                tabSimb.postOpseg(*(++(*it).begin()), "local");
                            }
                            else if (niz1[1] > 0 && niz1[1] > niz1[0])
                            {
                                tabSimb.postOpseg(*(++(*it).begin()), "global");
                            }
                            else if (niz1[0] == niz1[1] && niz2[0] > niz2[1])
                            {
                                tabSimb.postOpseg(*(++(*it).begin()), "local");
                            }
                            else if (niz1[0] == niz1[1] && niz2[1] > niz2[0])
                            {
                                tabSimb.postOpseg(*(++(*it).begin()), "global");
                            }
                            else if (niz1[0] == niz1[1] && niz2[1] == niz2[0])
                            {
                                tabSimb.postOpseg(*(++(*it).begin()), "local");
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
                                    tabSimb.postSekciju((i + 1), *(++(*it).begin()));
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
                                tabSimb.postPomeraj(n, *(++(*it).begin()));
                                tabSimb.postDef(*(++(*it).begin()));
                                list<Unapred> l = tabSimb.dohvUnapred(*(++(*it).begin()));
                                if (tabSimb.localDef(*(++(*it).begin())))
                                {
                                    for (list<Unapred>::iterator it2 = l.begin(); it2 != l.end(); ++it2)
                                    {
                                        for (list<Maskod>::iterator it3 = kodovi.begin(); it3 != kodovi.end(); ++it3)
                                        {
                                            if ((*it2).dohvSekciju() == (*it2).dohvSekciju())
                                            {
                                                list<Reltabela>::iterator it4;
                                                for (it4 = reltabele.begin(); it4 != reltabele.end(); ++it4)
                                                {
                                                    if (tabSimb.dohvSekciju(*(++(*it).begin())) == tabSimb.dohvSekciju((*it4).dohvSekciju()))
                                                    {
                                                        break;
                                                    }
                                                }
                                                (*it3).promeniKod((*it2).dohvPomeraj(), (*it2).dohvKol(), *(++(*it).begin()), tabSimb, (it4 != reltabele.end() ? &(*it4) : nullptr));
                                            }
                                        }
                                    }
                                    for (list<Reltabela>::iterator it2 = reltabele.begin(); it2 != reltabele.end(); ++it2)
                                    {
                                        if (tabSimb.dohvSekciju(*(++(*it).begin())) == tabSimb.dohvSekciju((*it2).dohvSekciju()))
                                        {
                                            (*it2).obrisiZapise(*(++(*it).begin()));
                                            break;
                                        }
                                    }
                                }
                                l.clear();
                            }
                        }
                        it1--;
                    }
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
                    if (it1 == (*it).end())
                    {
                        throw runtime_error("***Direktiva .equ se ne moze zavrsiti operatorom***");
                    }
                    it1--;
                }
                else if ((br % 2) == 1 && (*it1 != "+" && *it1 != "-"))
                {
                    throw runtime_error("***Ocekivao se operator u .equ direktivi (1)***");
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
        if (str == ".end")
        {
            if (lab)
            {
                throw runtime_error("***Labela ne moze da bude u istom redu sa .equ direktivom***");
            }

            if (trSekcija != 0)
            {
                tabSimb.postVred(trSekcijaS, lc);
            }

            if (!(tabSimb.provera()))
            {
                throw runtime_error("***Nisu svi simboli definisani***");
            }

            for (list<Reltabela>::iterator it1 = reltabele.begin(); it1 != reltabele.end(); ++it1)
            {
                (*it1).azurPok();
            }

            zavrsio = true;
        }
    }
    if (zavrsio)
    {
        cout << tabSimb;
        for (list<Maskod>::iterator it = kodovi.begin(); it != kodovi.end(); ++it)
        {
            cout << *it << endl;
        }
        for (list<Reltabela>::iterator it = reltabele.begin(); it != reltabele.end(); ++it)
        {
            cout << *it;
        }

        iz << tabSimb;
        for (list<Maskod>::iterator it = kodovi.begin(); it != kodovi.end(); ++it)
        {
            iz << *it << endl;
        }
        for (list<Reltabela>::iterator it = reltabele.begin(); it != reltabele.end(); ++it)
        {
            iz << *it;
        }
        iz.flush();
    }
    else
    {
        throw runtime_error("***Program se bezuspesno zavrsio***");
    }
}

Asembler::~Asembler()
{
    for (list<list<string>>::iterator it = tokeni.begin(); it != tokeni.end(); ++it)
    {
        (*it).clear();
    }
    tokeni.clear();
    kodovi.clear();
    sekcije.clear();
    reltabele.clear();
    tabSimb.~Tabela();
}

Asembler::Asembler()
{
    tabSimb = Tabela();
    trRedBr = 1;
    trSekcija = 0;
    lc = 0;
    trSekcijaS = "";
}