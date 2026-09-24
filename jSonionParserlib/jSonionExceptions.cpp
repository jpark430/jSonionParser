#include "jSonionExceptions.h"

using namespace jSONion;

jSONion::JsonSyntaxError::JsonSyntaxError(const std::string& message) throw()
    : std::runtime_error("JSONION SYNTAX ERROR: " + message)
{

}

char const* jSONion::JsonSyntaxError::what() const throw()
{
    return std::exception::what();
}

jSONion::JsonKeyError::JsonKeyError(const std::string& message) throw()
    : std::runtime_error("JSONION KEY ERROR: " + message)
{

}

char const* jSONion::JsonKeyError::what() const throw()
{
    return std::exception::what();
}

jSONion::JsonRecursionError::JsonRecursionError(const std::string& message) throw()
    : std::runtime_error("JSONION RECURSION ERROR: " + message)
{

}

char const* jSONion::JsonRecursionError::what() const throw()
{
    return std::exception::what();
}

jSONion::JsonValueError::JsonValueError(const std::string& message) throw()
    : std::runtime_error("JSONION VALUE ERROR: " + message)
{

}

char const* jSONion::JsonValueError::what() const throw()
{
    return std::exception::what();
}

jSONion::JsonTypeError::JsonTypeError(const std::string& message) throw()
    : std::runtime_error("JSONION TYPE ERROR: " + message)
{

}

char const* jSONion::JsonTypeError::what() const throw()
{
    return std::exception::what();
}