#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <bits/stdc++.h>
#include "Exception.h"
#include "Serial.h"

using namespace std;

/* SymbolTable 用來存放pi / e、函數符號與自訂義變數等 */
class SymbolTable : public Serializable
{
private:
    map<string, int> dictionary;
    unsigned int current_id;

public:
    enum
    {
        IDNOFOUND = 0xffffffff
    };
    SymbolTable() : current_id(0) {}
    unsigned int add_new_symbol(const string &str);
    unsigned int find_symbol(const string &str) const;
    void clear();
    string get_symbol(const unsigned int id) const;
    unsigned int get_current_id() const { return this->current_id; }
    virtual void Serialize(Serializer &out) const;
    virtual void Deserialize(DeSerializer &in);
};

unsigned int SymbolTable::add_new_symbol(const string &str)
{
    this->dictionary[str] = this->current_id;

    return this->current_id++; // After a new symbol, current_id should be +1
}

unsigned int SymbolTable::find_symbol(const string &str) const
{
    map<string, int>::const_iterator it = dictionary.find(str);

    if (it != dictionary.end())
        return it->second;
    else
        return IDNOFOUND;
}

void SymbolTable::clear()
{
    dictionary.clear();
    this->current_id = 0;
}

string SymbolTable::get_symbol(const unsigned int id) const
{
    map<string, int>::const_iterator it;

    if (0 <= id && id <= this->current_id)
        for (it = dictionary.begin(); it != dictionary.end(); ++it)
            if (it->second == id)
                return it->first;

    throw Exception("Internal error: Missing entry in symbol table");
}

void SymbolTable::Serialize(Serializer &out) const
{
    out << dictionary.size();
    for (map<string, int>::const_iterator it = dictionary.begin(); it != dictionary.end(); it++)
        out << it->first << it->second;
    out << current_id;
}

void SymbolTable::Deserialize(DeSerializer &in)
{
    dictionary.clear();

    unsigned int size;
    unsigned int id;
    string str;

    in >> size;
    for (unsigned int i = 0; i < size; ++i)
    {
        in >> str >> id;
        dictionary[str] = id;
    }
    in >> current_id;
}

#endif // _SYMBOLTABLE_H_
