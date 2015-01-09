#ifndef LESSCHROME_ERROR_H
#define LESSCHROME_ERROR_H

#include <exception>
#include <string>

namespace lesschrome {

class Error : public std::exception
{
public:
    Error(const char* const message)
        : m_message(message)
    {}

    virtual ~Error() throw() {}

    const char* what() const throw() { return m_message.c_str(); }

private:
    std::string m_message;
};

class RuntimeError : public Error
{
public:
    RuntimeError(const char* const message)
        : Error(message) {}
};

class InternalError : public Error
{
public:
    InternalError(const char* const message)
        : Error(message) {}
};

} // namespace lesschrome

#endif // LESSCHROME_ERROR_H
