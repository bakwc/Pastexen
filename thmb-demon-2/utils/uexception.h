#pragma once

#include <exception>
#include <string>
#include <vector>

class UException : public std::exception
{
public:
    UException(const std::string& message) throw();
    UException(const char* message) throw();
    const char* what() const throw();
    ~UException() throw ();
private:
    std::string Message;
};
