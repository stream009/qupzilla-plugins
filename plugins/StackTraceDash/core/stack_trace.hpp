#ifndef STACK_TRACE_DASH_STACK_TRACE_HPP
#define STACK_TRACE_DASH_STACK_TRACE_HPP

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

class QTextStream;

namespace stack_trace_dash {

class StackTrace : public boost::noncopyable
{
private:
    struct Item
    {
        struct ObjectInfo
        {
            void* address;
            void* baseAddress;
            std::string filename;
            boost::optional<std::string> functionName;
        };

        struct SourceInfo
        {
            std::string filename;
            size_t lineNo;
        };

        ObjectInfo object;
        boost::optional<SourceInfo> source;
    };

public:
    using value_type = Item;
    using Items = std::vector<value_type>;
    using size_type = Items::size_type;
    using const_iterator = Items::const_iterator;
    using const_reference = Items::const_reference;

    StackTrace(size_type const size, size_type const ignore);

    const_iterator begin() const { return items().begin(); }
    const_iterator end() const { return items().end(); }

    size_type size() const { return m_numTrace; }
    size_type ignore() const { return m_ignore; }

    const_reference operator[](size_type const i) const
    {
        return items()[i];
    }

private:
    Items &items() const;

private:
    std::vector<void*> m_addrs;
    size_type m_numTrace = 0;
    size_type m_ignore;
    mutable boost::optional<Items> m_items;
};

QTextStream& operator<<(QTextStream&, StackTrace const&);

} // namespace stack_trace_dash

#endif // STACK_TRACE_DASH_STACK_TRACE_HPP
