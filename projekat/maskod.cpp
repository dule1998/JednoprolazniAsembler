#include "maskod.h"

void Maskod::dodajBajt(char c)
{
    kod.push_back(c);
}

void Maskod::dodajRec(short s)
{
    char c = s & 0xFF;
    kod.push_back(c);
    s >>= 8;
    c = s & 0xFF;
    kod.push_back(c);
}

void Maskod::skip(int count)
{
    for (int i = 0; i < count; i++)
    {
        kod.push_back((char)0);
    }
}

Maskod::Maskod(string sek)
{
    sekcija = sek;
}

string Maskod::dohvSekciju()
{
    return sekcija;
}

Maskod::~Maskod()
{
    kod.clear();
}

ostream &operator<<(ostream &os, const Maskod &m)
{
    os << "#" << m.sekcija << endl;
    for (auto const &c : m.kod)
    {
        os << hex << (c < 16 ? (c < 0 ? "" : "0") : "") << +(c & 0xFF) << " ";
    }

    return os;
}

void Maskod::promeniKod(int n, int kol, string s, Tabela &t, Reltabela *r)
{
    int m = n;
    for (list<char>::iterator it = kod.begin(); it != kod.end(); ++it, n--)
    {
        if (n == 0)
        {
            if (kol == 1)
            {
                (*it) = (char)(t.dohvPomeraj(s));
            }
            else
            {
                int i = t.dohvPomeraj(s);
                int c1 = *it;
                int c2 = *(++it);
                it--;
                c2 <<= 8;
                c1 |= c2;
                i = i + c1;
                if (t.dohvSekciju(s) == t.dohvSekciju(sekcija) && r != nullptr)
                {
                    if (r->tipPC32(m))
                    {
                        i = i - m;
                    }
                }
                char c = i & 0xFF;
                (*it) = c;
                i >>= 8;
                c = i & 0xFF;
                it++;
                (*it) = c;
            }
            break;
        }
    }
}