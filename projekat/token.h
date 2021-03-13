#ifndef TOKEN_H
#define TOKEN_H

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include "regeksi.h"
using namespace std;

class Token
{
    list<list<string>> tokeni;

public:
    Token(ifstream &ul);
    list<list<string>> &dohvTokene();
    ~Token();
};

#endif