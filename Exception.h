#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <bits/stdc++.h>

using namespace std;

class Exception : public exception
{
private:
    string message;
    string stack_trace;

public:
    explicit Exception(const char *message) : message(message) {}
    virtual ~Exception() throw() {}
    const char *what() const throw() { return message.c_str(); }
};

class SyntaxError : public Exception
{
public:
    explicit SyntaxError(const char *message) : Exception(message) {}
    virtual ~SyntaxError() throw() {}
};

class FileStreamError : public Exception
{
public:
    explicit FileStreamError(const char *message) : Exception(message) {}
    virtual ~FileStreamError() throw() {}
};

#endif // _EXCEPTION_H_
