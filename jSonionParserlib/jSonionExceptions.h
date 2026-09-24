#ifndef EXCEPTSONS
#define EXCEPTSONS

#include <stdexcept>
#include <string>

namespace jSONion {
    class JsonSyntaxError : public std::runtime_error
    {
    public:
        JsonSyntaxError(const std::string& message) throw();
        virtual char const* what() const throw();
    };

    class JsonKeyError : public std::runtime_error
    {
    public:
        JsonKeyError(const std::string& message) throw();
        virtual char const* what() const throw();
    };

    class JsonRecursionError : public std::runtime_error
    {
    public:
        JsonRecursionError(const std::string& message) throw();
        virtual char const* what() const throw();
    };

    class JsonValueError : public std::runtime_error
    {
    public:
        JsonValueError(const std::string& message) throw();
        virtual char const* what() const throw();
    };

    class JsonTypeError : public std::runtime_error
    {
    public:
        JsonTypeError(const std::string& message) throw();
        virtual char const* what() const throw();
    };
};

#endif // !EXCEPTSONS