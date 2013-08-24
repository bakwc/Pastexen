#include "uexception.h"

UException::UException(const std::string& message) throw()
    : Message(message)
{
}

UException::UException(const char* message) throw()
    : Message(message)
{
}

UException::~UException() throw () {
}

const char* UException::what() const throw () {
    return Message.data();
}
