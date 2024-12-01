#ifndef _SCANNER_H_ // 防止標頭檔案被重複引入
#define _SCANNER_H_

#include <bits/stdc++.h>

using namespace std;

enum EToken
{
    TOKEN_COMMAND,
    TOKEN_END,
    TOKEN_ERROR,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPARENTHESIS,
    TOKEN_RPARENTHESIS,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN
};

class Scanner
{
private:
    EToken token;
    double number;
    istream &in;
    int cur_ch;
    string symbol;
    bool test_empty;

public:
    explicit Scanner(istream &in);
    void accept();
    void accept_cmd_filename();
    double get_number() const { return this->number; }
    string get_symbol() const { return this->symbol; }
    EToken get_token() const { return this->token; }
    void read_char();
    bool is_empty() const { return test_empty; }
    bool is_done() const { return (token == TOKEN_END); }
    bool is_command() const { return (token == TOKEN_COMMAND); }
};

Scanner::Scanner(istream &in) : in(in)
{
    accept();
    test_empty = (token == TOKEN_END);
}

void Scanner::accept()
{
    read_char();
    switch (cur_ch)
    {
    case '!':
        token = TOKEN_COMMAND;
        break;
    case '+':
        token = TOKEN_PLUS;
        break;
    case '-':
        token = TOKEN_MINUS;
        break;
    case '*':
        token = TOKEN_MULTIPLY;
        break;
    case '/':
        token = TOKEN_DIVIDE;
        break;
    case '(':
        token = TOKEN_LPARENTHESIS;
        break;
    case ')':
        token = TOKEN_RPARENTHESIS;
        break;
    case '=':
        token = TOKEN_ASSIGN;
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
        token = TOKEN_NUMBER;
        in.putback(cur_ch); // 把cur_ch歸還給in，number才會是完整的
        in >> number;
        break;
    case '\0':
    case '\n':
    case '\r':
    case EOF:
        token = TOKEN_END;
        break;
    default:
        if (isalpha(cur_ch) || cur_ch == '_')
        {
            token = TOKEN_IDENTIFIER;
            symbol.erase(); // 把symbol內舊內容erase掉

            do
            {
                symbol += cur_ch;
                cur_ch = in.get();
            } while (isalpha(cur_ch) || cur_ch == '_');

            in.putback(cur_ch); // 跳出loop時還多吃了一個char，要還回去給in
        }
        else
            token = TOKEN_ERROR;
        break;
    }
}

void Scanner::accept_cmd_filename()
{
    read_char();    // 去空白字元
    symbol.erase(); // 把symbol內舊內容erase掉

    do
    {
        symbol += cur_ch;
        cur_ch = in.get();
    } while (!isspace(cur_ch));
}

void Scanner::read_char()
{
    cur_ch = in.get(); // get(): get and return a character

    while (cur_ch == ' ' || cur_ch == '\t')
        cur_ch = in.get();
}

#endif // _SCANNER_H_
