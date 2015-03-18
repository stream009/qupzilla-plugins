#ifndef BOOKMARK_DASH_STACK_H
#define BOOKMARK_DASH_STACK_H

#include <sstream>
#include <string>

#include <execinfo.h>

#define HAVE_STACK_TRACE 1

namespace bookmark_dash {

class StackTrace
{
public:
    StackTrace()
        : m_numTraces {
            static_cast<size_t>(::backtrace(m_addrs, sizeof(m_addrs))) }
    {
        assert(m_numTraces <= maxTraces);
    }

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
    static const size_t maxTraces = 50u;
    void *m_addrs[maxTraces];
    size_t m_numTraces;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_STACK_H
