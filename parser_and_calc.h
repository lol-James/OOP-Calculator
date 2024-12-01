#ifndef _PARSER_AND_CALC_H_
#define _PARSER_AND_CALC_H_

#include <bits/stdc++.h>
#include "node.h"
#include "scanner.h"
#include "Exception.h"

using namespace std;

enum STATUS
{
    STATUS_OK,
    STATUS_ERROR,
    STATUS_QUIT
};

class Calc : public Serializable
{
public:
    friend class Parser;
    Calc() : function_table(symbol_table), storage(symbol_table) {}
    void print_func_list() const;
    void print_var_list() const;
    virtual void Serialize(Serializer &out) const;
    virtual void Deserialize(DeSerializer &in);

private:
    Storage &GetStorage() { return storage; }
    bool is_function(unsigned int id) const { return (id < function_table.get_size()); }
    unsigned int AddSymbol(const string &str) { return symbol_table.add_new_symbol(str); }
    unsigned int FindSymbol(const string &str) const { return symbol_table.find_symbol(str); }
    SymbolTable symbol_table;
    FunctionTable function_table;
    Storage storage;
    function_ptr get_function(unsigned int id) const { return function_table.get_function(id); }
    bool get_variable_value(unsigned int id, double &value) const;
};

/* Parser : 解析並生成expession tree */
class Parser
{
private:
    Scanner &scanner;
    Node *tree;
    STATUS status;
    Calc &calc;

public:
    Parser(Scanner &scanner, Calc &calc) : scanner(scanner), calc(calc), tree(0), status(STATUS_OK) {}
    STATUS Parse();
    Node *Expression();
    Node *Term();
    Node *Factor();
    double Calculate_value() const;
};

/* Calc */
void Calc::print_func_list() const
{
    for (unsigned int i = 0; i < function_table.get_size(); ++i)
    {
        // setw(10) :set width = 10   setiosflags(left) : 向左對齊
        cout << setw(10) << setiosflags(ios::left) << entrys[i].function_name + "()";
        if (i % 2)
            cout << endl;
    }
    cout << endl;
    cout << endl;
}

void Calc::print_var_list() const
{
    string name;
    double value;

    for (unsigned int i = function_table.get_size(); i < symbol_table.get_current_id(); ++i)
    {
        name = symbol_table.get_symbol(i);

        if (get_variable_value(i, value))
            cout << name << " = " << value << endl;
        else
            cout << name << " = ???" << endl;
    }
    cout << endl;
}

void Calc::Serialize(Serializer &out) const
{
    symbol_table.Serialize(out);
    storage.Serialize(out);
}

void Calc::Deserialize(DeSerializer &in)
{
    symbol_table.Deserialize(in);
    storage.Deserialize(in);
}

bool Calc::get_variable_value(unsigned int id, double &value) const
{
    if (storage.is_initialized(id) == true)
    {
        value = storage.get_value(id);
        return true;
    }

    return false;
}

/* Parser */
STATUS Parser::Parse()
{
    this->tree = Expression();
    if (!scanner.is_done())
        status = STATUS_ERROR;

    return status;
}

double Parser::Calculate_value() const
{
    assert(tree != 0); // 運用assert() 確保tree不為0(NULL)
    return tree->Calculate();
}

Node *Parser::Expression()
{
    Node *node = Term(); // Expression至少為Term
    EToken token = (this->scanner).get_token();

    /*
        Expression(表達式) is
        Term(項) +or- Term
        or Term = Expression
        or just a Term
    */
    if (token == TOKEN_PLUS || token == TOKEN_MINUS)
    {
        MultipleNode *multipleNode = new SumNode(node);

        do
        {
            (this->scanner).accept();
            Node *nextNode = Term(); // 產生另一個recursion分支
            multipleNode->AppendChild(nextNode, (token == TOKEN_PLUS));
            /*
                (token == TOKEN_PLUS) will return a boolean value
                若(token == TOKEN_PLUS)成立, 則第二個參數傳入true，即為加法
                若(token == TOKEN_PLUS)不成立，則第二個參數傳入false，即為減法
            */
            token = (this->scanner).get_token();
        } while (token == TOKEN_PLUS || token == TOKEN_MINUS);

        node = multipleNode;
    }
    else if (token == TOKEN_ASSIGN)
    {
        scanner.accept();
        Node *node_right = Expression();

        if (node->is_Lvalue())
            node = new AssignNode(node, node_right);
        else
        {
            status = STATUS_ERROR;
            throw SyntaxError("Syntax error: The left side of an assignment must be a variable.");
        }
    }

    return node;
}

Node *Parser::Term()
{
    Node *node = Factor(); // Term至少為Factor
    EToken token = (this->scanner).get_token();

    /*
        Term is
        Factor *or/ Factor
        or just a Factor
    */
    if (token == TOKEN_MULTIPLY || token == TOKEN_DIVIDE)
    {
        MultipleNode *multipleNode = new ProductNode(node);

        do
        {
            (this->scanner).accept();
            Node *nextNode = Factor();
            multipleNode->AppendChild(nextNode, (token == TOKEN_MULTIPLY));
            token = (this->scanner).get_token();
        } while (token == TOKEN_MULTIPLY || token == TOKEN_DIVIDE);

        node = multipleNode;
    }

    return node;
}

Node *Parser::Factor()
{
    Node *node;
    EToken token = (this->scanner).get_token();

    /*
        Factor is
        Number
        or Identifier
        or Identifier(Expression)
        or -Factor
        or (Expression)
    */
    if (token == TOKEN_LPARENTHESIS) // accept '('
    {
        (this->scanner).accept();
        node = Expression();

        if ((this->scanner).get_token() == TOKEN_RPARENTHESIS) // accept ')'
            (this->scanner).accept();
        else // 無與之搭配的')'
        {
            this->status = STATUS_ERROR;
            node = 0;
            throw SyntaxError("Syntex Error: Missing ')'");
        }
    }
    else if (token == TOKEN_NUMBER)
    {
        node = new NumberNode((this->scanner).get_number());
        (this->scanner).accept();
    }
    else if (token == TOKEN_IDENTIFIER)
    {
        string symbol = scanner.get_symbol();
        unsigned int id = calc.FindSymbol(symbol);
        scanner.accept();

        if (scanner.get_token() == TOKEN_LPARENTHESIS) // function call
        {
            scanner.accept(); // accept '('
            node = Expression();
            if ((this->scanner).get_token() == TOKEN_RPARENTHESIS)
            {
                scanner.accept(); // accept ')'
                if (id != SymbolTable::IDNOFOUND && calc.is_function(id) == true)
                    node = new FunctionaNode(node, calc.get_function(id));
                else
                {
                    status = STATUS_ERROR;
                    node = 0;
                    char error_output[256];
                    sprintf(error_output, "SyntaxError: Unknown function \" %s \"", symbol.c_str());
                    // sprintf(char*, const char*); 可用來製造一個格式化字串
                    throw SyntaxError(error_output);
                }
            }
            else
            {
                status = STATUS_ERROR;
                throw SyntaxError("Syntex Error: Missing ')' in a function call");
            }
        }
        else
        {
            if (id == SymbolTable::IDNOFOUND)
                id = calc.AddSymbol(symbol);
            node = new VariableNode(id, calc.GetStorage());
        }
    }
    else if (token == TOKEN_MINUS)
    {
        (this->scanner).accept();
        node = new UMinusNode(Factor());
    }
    else
    {
        this->status = STATUS_ERROR;
        node = 0;
        throw SyntaxError("Syntex Error: Invalid Expression");
    }

    return node;
}

#endif // _PARSER_AND_CALC_H_
