#ifndef STYLIST_STACK_H
#define STYLIST_STACK_H

#include <sstream>
#include <string>

#include <execinfo.h>

#define HAVE_STACK_TRACE 1

namespace stylist {

class StackTrace
{
public:
    StackTrace()
        : m_numTraces(::backtrace(m_addrs, sizeof(m_addrs)))
    {}

    std::string str() const
    {
        std::ostringstream oss;
        char** const symbols = ::backtrace_symbols(m_addrs, m_numTraces);
        for (size_t i = 0u; i < m_numTraces; ++i) {
            oss << "#" << i << ": " << symbols[i] << "\n";
        }
        return oss.str();
    }

private:
    void *m_addrs[50];
    size_t m_numTraces;
};

} // namespace stylist

#endif // STYLIST_STACK_H
