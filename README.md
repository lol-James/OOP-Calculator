# OOP Scientific Calculator

## Overview
Welcome to the OOP Scientific Calculator! This calculator is designed with object-oriented programming principles and supports a wide range of mathematical operations and functionalities. Whether you need to perform basic arithmetic, use common math functions, or handle custom variables, this calculator is versatile and user-friendly.

## Features
The calculator supports the following functionalities:

### 1. **Arithmetic Operations**
   - Perform basic operations such as addition, subtraction, multiplication, and division.

### 2. **Common Math Functions**
   - Use built-in functions like square root, sine, cosine, tangent, and other standard mathematical operations.

### 3. **Custom Variables**
   - Create and use custom variables for storing values. This allows for more complex calculations and efficient use of previously computed results.

### 4. **Command-Line Operations**
   - Utilize `!` commands to access advanced features and control the calculator environment.

## Command List

### Recognized Commands:
- **`!c`**: Check all recognized commands.
- **`!q`**: Quit the calculator.
- **`!v`**: Print all variables and constants that are currently stored.
- **`!f`**: Print all built-in functions available in the calculator.
- **`!l fileName`**: Load variables from a specified file.
- **`!s fileName`**: Save all current variables to a specified file.

### Example Usage:
- **Basic Arithmetic**: `5 + 3`, `12 / 4`, `2 * 8`
- **Math Functions**: `sin(45)`, `cos(90)`, `sqrt(16)`
- **Variable Creation**: `x = 10`, `y = 25`
- **Command-Line**: `!v` to list all stored variables, `!c` to check all available commands.

### Sample Session:
```
 Welcome to use the calculator~~~

This calculator supports the following functions:
1. Arithmetic operations
2. Common math functions
3. Custom variables
4. ! commands

~~~Enter !c to ckeck all ! recongnized commands~~~

PS. When you enter an expression but there is not any output, it means that your expression may be wrong.
    Please re-enter the correct expression again.

>> 1+1
2
>> 2*(3/2)
3
>> a = 2
2
>> b = 2
2
>> a + b = 4
>> sin(30)
-0.988032
>> !
Error: Unknown ! command 
>> !c
Recongnized commands: 
-------------------------------------------------------
!c          : check all ! recongnized commends
!q          : quit
!v          : print all variables and constants saved
!f          : print all built-in funstions
!l fileName : load variables from fileName
!s fileName : save variables in fileName
-------------------------------------------------------
>> !v
variable and constants list: 
e = 2.71828
pi = 3.14159
a = 2
b = 2

>> !f
function list: 
ln()      log()
exp()     sqrt()
abs()     sin()
cos()     tan()
cot()     sec()
csc()     sinh()
cosh()    tanh()
coth()    sech()
csch()    asin()
acos()    atan()

```
