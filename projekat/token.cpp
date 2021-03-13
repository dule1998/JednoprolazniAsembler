#include "token.h"

Token::Token(ifstream &ul)
{
    string rec = "";
    list<string> token;
    char karakter;
    ul.get(karakter);
    while (!(ul.eof()))
    {
        bool provera = true;

        if (karakter == '\n')
        {
            string str = rec;
            if (rec != "")
            {
                token.push_back(rec);
                rec = "";
            }
            if (token.empty() == false)
            {
                tokeni.push_back(token);
                token.erase(token.begin(), token.end());
                if (str == ".end")
                {
                    break;
                }
            }
            ul.get(karakter);
            continue;
        }

        if (karakter == ',' || karakter == '+' || karakter == '-')
        {
            if (rec != "")
            {
                token.push_back(rec);
                if (rec == ".end")
                {
                    tokeni.push_back(token);
                    token.erase(token.begin(), token.end());
                    rec = "";
                    break;
                }
                rec = "";
            }
            string str;
            str = karakter;
            token.push_back(str);
            ul.get(karakter);
            continue;
        }

        if (karakter == ' ' || karakter == '\t')
        {
            if (rec != "")
            {
                token.push_back(rec);
                if (rec == ".end")
                {
                    tokeni.push_back(token);
                    token.erase(token.begin(), token.end());
                    rec = "";
                    break;
                }
                rec = "";
            }
            ul.get(karakter);
            while (karakter == ' ' || karakter == '\t')
            {
                ul.get(karakter);
            }
            provera = false;
        }

        if (karakter == '\n' && provera == false)
        {
            if (token.empty() == false)
            {
                tokeni.push_back(token);
                token.erase(token.begin(), token.end());
            }
            ul.get(karakter);
            continue;
        }

        if (karakter == ',' || karakter == '+' || karakter == '-')
        {
            string str;
            str = karakter;
            token.push_back(str);
            ul.get(karakter);
            continue;
        }

        if (provera)
        {
            rec += karakter;
            ul.get(karakter);
        }
    }

    if (rec != "")
    {
        token.push_back(rec);
        rec = "";
        tokeni.push_back(token);
        token.erase(token.begin(), token.end());
    }
}

list<list<string>> &Token::dohvTokene()
{
    return tokeni;
}

Token::~Token()
{
    for (list<list<string>>::iterator it = tokeni.begin(); it != tokeni.end(); ++it)
    {
        (*it).clear();
    }
    tokeni.clear();
}