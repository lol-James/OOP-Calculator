#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <bits/stdc++.h>
#include "SymbolTable.h"

using namespace std;

class Storage : public Serializable
{
private:
    vector<double> values; // Store values of the corresponding position symbols
    vector<bool> inits;    // shether the corresponding position symbols has been initialized

public:
    Storage(SymbolTable &table) { add_const(table); }
    void clear();
    bool is_initialized(const unsigned int id) const { return (id < values.size() && inits[id]); }
    void add_const(SymbolTable &table);
    double get_value(const unsigned int id) const;
    void set_value(const unsigned int id, double value);
    void add_value(const unsigned int id, double value);
    virtual void Serialize(Serializer &out) const;
    virtual void Deserialize(DeSerializer &in);
};

void Storage::clear()
{
    values.clear();
    inits.clear();
}

void Storage::add_const(SymbolTable &table)
{
    unsigned int id;

    cout << "built-in constant list: " << endl;

    id = table.add_new_symbol("e");
    add_value(id, exp(1));
    cout << "e = " << exp(1) << endl;

    id = table.add_new_symbol("pi");
    add_value(id, 2 * acos(0));
    cout << "pi = " << 2 * acos(0) << endl;
    cout << endl;
}

double Storage::get_value(const unsigned int id) const
{
    assert(id < values.size());

    return values[id];
}

void Storage::set_value(const unsigned int id, double value)
{
    if (id < values.size())
    {
        values[id] = value;
        inits[id] = true;
    }
    else
        add_value(id, value);
}

void Storage::add_value(const unsigned int id, double value)
{
    values.resize(id + 1);
    inits.resize(id + 1);

    values[id] = value;
    inits[id] = true;
}

void Storage::Serialize(Serializer &out) const
{
    out << values.size();
    for (unsigned int i = 0; i < values.size(); i++)
        out << values[i] << inits[i];
}

void Storage::Deserialize(DeSerializer &in)
{
    values.clear();
    inits.clear();

    unsigned int size;
    in >> size;
    values.resize(size);
    inits.resize(size);

    for (unsigned int i = 0; i < size; i++)
    {
        double value;
        bool init;
        in >> value;
        in >> init;
        values[i] = value;
        inits[i] = init;
    }
}

#endif // _STORAGE_H_
