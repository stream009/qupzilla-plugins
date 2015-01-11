#ifndef LESSCHROME_ERROR_H
#define LESSCHROME_ERROR_H

#if defined(Q_OS_LINUX) || defined(__GLIBC__)
#include "stacktrace.h"
#endif

#include <cstdlib>
#include <exception>
#include <string>

namespace lesschrome {

class Error : public std::exception
{
public:
    Error(const char* const message)
        : m_message(message)
    {
#ifdef HAVE_STACK_TRACE
        m_message += "\nStack trace:\n" + m_stackTrace.str();
#endif
    }

    virtual ~Error() throw() {}

    const char *what() const throw()
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
#ifdef HAVE_STACK_TRACE
    StackTrace m_stackTrace;
#endif
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

#define DEFAULT_EXCEPTION_HANDLER(exception) \
    defaultExceptionHandler(__FILE__, __LINE__, __FUNCTION__, exception)

void defaultExceptionHandler(const char* const fileName,
                             const size_t lineNo,
                             const char* const functionName,
                             const std::exception &e);

} // namespace lesschrome

#endif // LESSCHROME_ERROR_H
