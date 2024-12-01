#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_

#include <bits/stdc++.h>
#include "parser_and_calc.h"

using namespace std;

class CommandParser
{
private:
    enum ECommand
    {
        CMD_HELP,
        CMD_QUIT,
        CMD_VAR,
        CMD_FUN,
        CMD_LOAD,
        CMD_SAVE,
        CMD_ERROR,
    };
    Scanner &scanner;
    Calc &calc;
    ECommand cmd_name;
    void help() const;
    STATUS quit();
    void print_var_list() const;
    void print_func_list() const;
    STATUS load_var(const string &file_name);
    STATUS save_var(const string &file_name) const;

public:
    CommandParser(Scanner &scanner, Calc &calc);
    STATUS execute_command();
};

CommandParser::CommandParser(Scanner &scanner, Calc &cal) : scanner(scanner), calc(cal)
{
    assert(scanner.is_command());
    scanner.accept();
    string command_string = scanner.get_symbol();

    switch (command_string[0])
    {
    case 'c':
        cmd_name = CMD_HELP;
        break;
    case 'q':
        cmd_name = CMD_QUIT;
        break;
    case 'v':
        cmd_name = CMD_VAR;
        break;
    case 'f':
        cmd_name = CMD_FUN;
        break;
    case 'l':
        cmd_name = CMD_LOAD;
        break;
    case 's':
        cmd_name = CMD_SAVE;
        break;
    default:
        cmd_name = CMD_ERROR;
        break;
    }
}

STATUS CommandParser::execute_command()
{
    STATUS status = STATUS_OK;

    switch (cmd_name)
    {
    case CMD_HELP:
        scanner.read_char();
        help();
        break;
    case CMD_QUIT:
        scanner.read_char();
        status = quit();
        break;
    case CMD_VAR:
        scanner.read_char();
        print_var_list();
        break;
    case CMD_FUN:
        scanner.read_char();
        print_func_list();
        break;
    case CMD_LOAD:
    { // 加上{} 防止swatich case略過file_name的初始化的報錯
        scanner.accept_cmd_filename();
        string file_name = scanner.get_symbol();
        status = load_var(file_name);
        break;
    }
    case CMD_SAVE:
    {
        scanner.accept_cmd_filename();
        string file_name = scanner.get_symbol();
        status = save_var(file_name);
        break;
    }
    default:
        cout << "Error: Unknown ! command " << endl;
        status = STATUS_ERROR;
        break;
    }

    return status;
}

void CommandParser::help() const
{
    cout << "Recongnized commands: " << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "!c          : check all ! recongnized commends" << endl;
    cout << "!q          : quit" << endl;
    cout << "!v          : print all variables and constants saved" << endl;
    cout << "!f          : print all built-in funstions" << endl;
    cout << "!l fileName : load variables from fileName" << endl;
    cout << "!s fileName : save variables in fileName" << endl;
    cout << "-------------------------------------------------------" << endl;
}

STATUS CommandParser::quit()
{
    cout << "Thank you for your using this calculator." << endl;
    cout << "Good bye!" << endl;
    cout << endl;

    return STATUS_QUIT;
}

void CommandParser::print_var_list() const
{
    cout << "variable and constants list: " << endl;
    calc.print_var_list();
}

void CommandParser::print_func_list() const
{
    cout << "function list: " << endl;
    calc.print_func_list();
}

const unsigned int version = 1;

STATUS CommandParser::load_var(const string &file_name)
{
    STATUS status = STATUS_OK;

    cout << "load variables from \"" << file_name << "\"" << endl;
    try
    {
        DeSerializer in(file_name);
        unsigned int ver;

        in >> ver;
        if (ver != version)
            throw Exception("Version error: Missing match version");
        calc.Deserialize(in);

        cout << "Save successfully" << endl;
    }
    catch (FileStreamError &fse)
    {
        cout << fse.what() << endl;
        status = STATUS_ERROR;
    }
    catch (Exception &e)
    {
        cout << e.what() << endl;
        status = STATUS_ERROR;
    }

    return status;
}

STATUS CommandParser::save_var(const string &file_name) const
{
    STATUS status = STATUS_OK;

    cout << "save variables in \"" << file_name << "\"" << endl;
    try
    {
        Serializer out(file_name);

        out << version;
        calc.Serialize(out);

        cout << "Load successfully" << endl;
    }
    catch (FileStreamError &fse)
    {
        cout << fse.what() << endl;
        status = STATUS_ERROR;
    }

    return status;
}

#endif // _COMMAND_PARSER_H_
