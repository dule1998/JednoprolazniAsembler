#include "regeksi.h"

regex Regeksi::labela = regex("^([a-zA-Z]+[0-9]*|_[a-zA-Z][a-zA-Z0-9]*):$");
regex Regeksi::simbol = regex("^([a-zA-Z]+[0-9]*|_[a-zA-Z][a-zA-Z0-9]*)$");
regex Regeksi::broj = regex("^[0-9]{1,16}$");
regex Regeksi::simbol1 = regex("^(\\$)([a-zA-Z]+[0-9]*|_[a-zA-Z][a-zA-Z0-9]*)$");
regex Regeksi::broj1 = regex("^(\\$)[0-9]{1,16}$");
regex Regeksi::simbol2 = regex("^(\\*)([a-zA-Z]+[0-9]*|_[a-zA-Z][a-zA-Z0-9]*)$");
regex Regeksi::broj2 = regex("^(\\*)[0-9]{1,16}$");
regex Regeksi::sekcija = regex("^(\\.)[a-z]+$");
regex Regeksi::instrukcija = regex("^(halt|iret|ret|int|call|jmp|jeq|jne|jgt|push|pop|xchg|mov|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr)(w|b)?$");
regex Regeksi::instrukcija1 = regex("^(haltb|iretb|retb|intb|callb|jmpb|jeqb|jneb|jgtb|haltw|iretw|retw|intw|callw|jmpw|jeqw|jnew|jgtw)$");
regex Regeksi::instrukcija2 = regex("^(halt|iret|ret|int|call|jmp|jeq|jne|jgt|push|pop|xchg|mov|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr)$");
regex Regeksi::instrukcija3 = regex("^(halt|iret|ret|int)$");
regex Regeksi::instrukcija4 = regex("^(call|jmp|jeq|jne|jgt)$");
regex Regeksi::regdir = regex("^%r[0-7](h|l)?$");
regex Regeksi::regind = regex("^\\(%r[0-7]\\)$");
regex Regeksi::regdskok = regex("^\\*%r[0-7]$");
regex Regeksi::regiskok = regex("^\\*\\(%r[0-7]\\)$");
regex Regeksi::regindpomb = regex("^[0-9]{1,16}\\(%r[0-7]\\)$");
regex Regeksi::regindpoms = regex("^([a-zA-Z]+[0-9]*|_[a-zA-Z][a-zA-Z0-9]*)?\\(%r[0-7]\\)$");
regex Regeksi::regindpombs = regex("^(\\*)[0-9]{1,16}\\(%r[0-7]\\)$");
regex Regeksi::regindpomss = regex("^(\\*)([a-zA-Z]+[0-9]*|_[a-zA-Z][a-zA-Z0-9]*)?\\(%r[0-7]\\)$");
