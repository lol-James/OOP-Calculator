#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <bits/stdc++.h>
#include "Exception.h"

using namespace std;

const int TRUE = 0xffffffff;
const int FALSE = 0x00000000;

// 可以執行Serializer(保存) and DeSerializer(還原)的類，稱為可序列化類別
// 在此如: calc, storage, SymbolTable

// Serializer類負責將變數與常數表寫入檔案，以便下次直接使用
class Serializer
{
private:
    ofstream file;

public:
    Serializer(const string &file_name);
    Serializer &put(int x);
    Serializer &put(double x);
    Serializer &put(unsigned int x);
    Serializer &put(long int x);
    Serializer &put(unsigned long int x);
    Serializer &put(const string &x);
    Serializer &put(bool x);
    Serializer &operator<<(int x) { return put(x); }
    Serializer &operator<<(double x) { return put(x); }
    Serializer &operator<<(unsigned int x) { return put(x); }
    Serializer &operator<<(long int x) { return put(x); }
    Serializer &operator<<(unsigned long int x) { return put(x); }
    Serializer &operator<<(const string &x) { return put(x); }
    Serializer &operator<<(bool x) { return put(x); }
};

// deserializer類負責加載檔案內的變數與常數表
class DeSerializer
{
private:
    ifstream file;

public:
    DeSerializer(const string &file_name);
    DeSerializer &get(int &x);
    DeSerializer &get(double &x);
    DeSerializer &get(unsigned int &x);
    DeSerializer &get(long int &x);
    DeSerializer &get(unsigned long int &x);
    DeSerializer &get(string &x);
    DeSerializer &get(bool &x);
    DeSerializer &operator>>(int &x) { return get(x); }
    DeSerializer &operator>>(double &x) { return get(x); }
    DeSerializer &operator>>(unsigned int &x) { return get(x); }
    DeSerializer &operator>>(long int &x) { return get(x); }
    DeSerializer &operator>>(unsigned long int &x) { return get(x); }
    DeSerializer &operator>>(string &x) { return get(x); }
    DeSerializer &operator>>(bool &x) { return get(x); }
};

// Serializable整合Serialzer and DeSerializer
class Serializable
{
public:
    virtual void Serialize(Serializer &out) const = 0; // 寫檔本身output不會變，所以加const
    virtual void Deserialize(DeSerializer &in) = 0;
};

/* Serializer */
Serializer::Serializer(const string &file_name)
{
    file.open(file_name.c_str(), ios::out | ios::binary);

    if (!file.is_open())
        throw FileStreamError("File error: Couldn't open file");
}

// PS. 寫檔需用二進制方式寫
Serializer &Serializer::put(int x)
{
    file.write(reinterpret_cast<char *>(&x), sizeof(int)); // reinterpret_cast : 用於將一種型態的指標轉換為另一種型態的指標

    if (file.bad())
        throw FileStreamError("File error: Write file failed");

    return *this;
}

Serializer &Serializer::put(unsigned int x)
{
    file.write(reinterpret_cast<char *>(&x), sizeof(unsigned int));

    if (file.bad())
        throw FileStreamError("File error: Write file failed");

    return *this;
}

Serializer &Serializer::put(double x)
{
    file.write(reinterpret_cast<char *>(&x), sizeof(double));

    if (file.bad())
        throw FileStreamError("File error: Write file failed");

    return *this;
}

Serializer &Serializer::put(long int x)
{
    file.write(reinterpret_cast<char *>(&x), sizeof(long int));

    if (file.bad())
        throw FileStreamError("File error: Write file failed");

    return *this;
}

Serializer &Serializer::put(unsigned long int x)
{
    file.write(reinterpret_cast<char *>(&x), sizeof(unsigned long int));

    if (file.bad())
        throw FileStreamError("File error: Write file failed");

    return *this;
}

Serializer &Serializer::put(const string &x)
{
    int len = x.length();

    // 用字符長度在前進行分隔
    put(len);
    file.write(x.data(), len);

    if (file.bad())
        throw FileStreamError("File error: Write file failed");

    return *this;
}

Serializer &Serializer::put(bool x)
{
    // 轉換為特殊值來寫檔，以確保皆寫入4 bytes
    int value = (x == true) ? TRUE : FALSE;

    put(value);

    return *this;
}

/* DeSerializer */
DeSerializer::DeSerializer(const string &file_name)
{
    file.open(file_name, ios::in | ios::binary);

    if (!file.is_open())
        throw FileStreamError("File error: Couldn't open file");
}

DeSerializer &DeSerializer::get(int &x)
{
    if (file.eof())
        throw("File error: Unexpected end of file");

    file.read(reinterpret_cast<char *>(&x), sizeof(int));

    if (file.bad())
        throw FileStreamError("File error: Read file failed");

    return *this;
}

DeSerializer &DeSerializer::get(double &x)
{
    if (file.eof())
        throw("File error: Unexpected end of file");

    file.read(reinterpret_cast<char *>(&x), sizeof(double));

    if (file.bad())
        throw FileStreamError("File error: Read file failed");

    return *this;
}

DeSerializer &DeSerializer::get(unsigned int &x)
{
    if (file.eof())
        throw FileStreamError("File error: Unexpected end of file");

    file.read(reinterpret_cast<char *>(&x), sizeof(unsigned int));

    if (file.bad())
        throw FileStreamError("File error: Read file failed");

    return *this;
}

DeSerializer &DeSerializer::get(long int &x)
{
    if (file.eof())
        throw FileStreamError("File error: Unexpected end of file");

    file.read(reinterpret_cast<char *>(&x), sizeof(long int));

    if (file.bad())
        throw FileStreamError("File error: Read file failed");

    return *this;
}

DeSerializer &DeSerializer::get(unsigned long int &x)
{
    if (file.eof())
        throw FileStreamError("File error: Unexpected end of file");

    file.read(reinterpret_cast<char *>(&x), sizeof(unsigned long int));

    if (file.bad())
        throw FileStreamError("File error: Read file failed");

    return *this;
}

DeSerializer &DeSerializer::get(string &x)
{
    if (file.eof())
        throw FileStreamError("File error: Unexpected end of file");

    int len;

    get(len); // 先把前面的length讀出來
    if (file.eof())
        throw FileStreamError("File error: Unexpected end of file");

    x.resize(len);         // 分配x的大小
    file.read(&x[0], len); // 利用內建的read()讀取相應長度的字串

    if (file.bad())
        throw FileStreamError("File error: Read file failed");

    return *this;
}

DeSerializer &DeSerializer::get(bool &x)
{
    int value;

    get(value);

    if (value == TRUE)
        x = true;
    else if (value == FALSE)
        x = false;
    else
        throw FileStreamError("File error:  Data conversion failed");

    return *this;
}

#endif // _SERIALIZER_H_
