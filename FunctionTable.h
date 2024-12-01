#ifndef _FUNCTIONTABLE_H_
#define _FUNCTIONTABLE_H_

#include <bits/stdc++.h>
#include "SymbolTable.h"

using namespace std;

class SymbolTable;

/*
    定義一個函式指標

    function block也占有記憶體空間，
    因此可利用函式指標(function pointer)指到函式的記憶體位址以調用該函數，
    fptr_A = func1;     // 函式指標(fptr_A)指向函式(func1)
    則 (*fptr_A)可當func1用

    Syntax: typedef 函式回傳型態 (*函式指標的型別名稱)(參數1型態, 參數2型態, ...)
*/
typedef double (*function_ptr)(double);

/*額外自己增加的函數*/
double cot(double x);
double sec(double x);
double csc(double x);
double coth(double x);
double sech(double x);
double csch(double x);

struct function_entry
{
    function_ptr ptr;
    string function_name;
};

function_entry entrys[] = {
    log, "ln",
    log10, "log",
    exp, "exp",
    sqrt, "sqrt",
    abs, "abs",
    sin, "sin",
    cos, "cos",
    tan, "tan",
    cot, "cot",
    sec, "sec",
    csc, "csc",
    sinh, "sinh",
    cosh, "cosh",
    tanh, "tanh",
    coth, "coth",
    sech, "sech",
    csch, "csch",
    asin, "asin",
    acos, "acos",
    atan, "atan"};

/* FunctionTable用來存放數學函數符號並調用相應的函數 */
class FunctionTable
{
private:
    function_ptr *fptrs; // 為指標的指標
    unsigned int size;

public:
    FunctionTable(SymbolTable &table) : size(sizeof(entrys) / sizeof(entrys[0])) { initialize(table); }
    ~FunctionTable() { delete[] fptrs; }
    void initialize(SymbolTable &table);
    unsigned int get_size() const { return this->size; }
    function_ptr get_function(unsigned int id) const { return fptrs[id]; }
};

void FunctionTable::initialize(SymbolTable &table)
{
    fptrs = new function_ptr[size];

    cout << "function list: " << endl;
    for (unsigned int i = 0; i < size; i++)
    {
        fptrs[i] = entrys[i].ptr;
        unsigned int j = table.add_new_symbol(entrys[i].function_name);
        assert(i == j);

        // setw(10) :set width = 10   setiosflags(left) : 向左對齊
        cout << setw(10) << setiosflags(ios::left) << entrys[i].function_name + "()";
        if (i % 2)
            cout << endl;
    }
    cout << "\n\n";
}

double cot(double x) { return (1 / tan(x)); }
double sec(double x) { return (1 / cos(x)); }
double csc(double x) { return (1 / sin(x)); }
double coth(double x) { return (1 / tanh(x)); }
double sech(double x) { return (1 / cosh(x)); }
double csch(double x) { return (1 / sinh(x)); }

#endif // _FUNCTIONTABLE_H_
