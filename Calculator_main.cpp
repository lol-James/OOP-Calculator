#include <bits/stdc++.h>

#include "parser_and_calc.h"
#include "Exception.h"
#include "CommandParser.h"

using namespace std;

int main()
{
    Calc calc;
    STATUS status = STATUS_OK;

    // 把已經輸出的文字清掉，然後再把游標移回到第一行最前面的位置。
    system("cls");

    /* intro */
    cout << "\n Welcome to use the calculator~~~\n\n";
    cout << "This calculator supports the following functions:\n";
    cout << "1. Arithmetic operations" << endl;
    cout << "2. Common math functions" << endl;
    cout << "3. Custom variables" << endl;
    cout << "4. ! commands" << endl;
    cout << "\n~~~Enter !c to ckeck all ! recongnized commands~~~" << endl;
    cout << "\nPS. When you enter an expression but there is not any output, it means that your expression may be wrong." << endl;
    cout << "    Please re-enter the correct expression again.\n";
    cout << endl;

    do
    {
        cout << ">> ";
        Scanner scanner(cin); // 直接從istream獲取expression

        if (!scanner.is_empty())
        {
            if (scanner.is_command())
            {
                CommandParser c_parser(scanner, calc);
                status = c_parser.execute_command();
            }
            else
            {
                Parser parser(scanner, calc);

                try
                {
                    status = parser.Parse();
                    if (status == STATUS_OK)
                        cout << parser.Calculate_value() << endl;
                }
                catch (SyntaxError &s_error)
                {
                    cout << s_error.what() << endl;
                }
                catch (Exception &error)
                {
                    cout << error.what() << endl;
                }
                catch (...)
                {
                    cout << "Internal error" << endl;
                }
            }
        }
        else
        {
            cout << "Expression is empty!" << endl;
        }

        cin.clear();
        cin.sync(); // 清除輸入緩衝區的所有內容
    } while (status != STATUS_QUIT);

    system("pause");
    return 0;
}
