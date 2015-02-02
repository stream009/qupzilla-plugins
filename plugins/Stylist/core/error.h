#ifndef STYLIST_ERROR_H
#define STYLIST_ERROR_H

#if defined(Q_OS_LINUX) || defined(__GLIBC__)
#include "stacktrace.h"
#endif

#include <cstdlib>
#include <exception>
#include <string>

namespace stylist {

class Error : public std::exception
{
public:
    Error(const char* const message)
        : m_message { message }
    {
#ifdef HAVE_STACK_TRACE
        m_message += "\nStack trace:\n" + m_stackTrace.str();
#endif
    }

    virtual ~Error() noexcept {}

    const char *what() const noexcept
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
        : Error { message } {}
};

class InternalError : public Error
{
public:
    InternalError(const char* const message)
        : Error { message } {}
};

#define DEFAULT_EXCEPTION_HANDLER(exception) \
    defaultExceptionHandler(__FILE__, __LINE__, __func__, exception)

void defaultExceptionHandler(const char* const fileName,
                             const size_t lineNo,
                             const char* const functionName,
                             const std::exception &e);

} // namespace stylist

#endif // STYLIST_STYLIST_H
