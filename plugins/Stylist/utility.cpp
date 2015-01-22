#include "utility.h"

#include <boost/algorithm/string/find.hpp>

namespace stylist {

Url::
Url(const char* const url) // implicit
    : QUrl { QuString { url } }
{
    assert(this->isValid()); //TODO better
}

Url::
Url(const QUrl &url) // implicit
    : QUrl { url }
{
    assert(this->isValid()); //TODO better
}

Url::
Url(const QuString &url) // implicit
    : QUrl { url }
{
    assert(this->isValid()); //TODO better
}

std::string Url::
str() const
{
    const auto &buf = this->toString().toUtf8();
    return std::string { buf.begin(), buf.end() };
}


QDebug &operator<<(QDebug dbg, const std::string &str)
{
    return dbg << str.c_str();
}

QDebug &operator<<(QDebug dbg, const boost::string_ref &ref)
{
    return dbg << ref.to_string().c_str();
}

QDebug &operator<<(QDebug dbg, const boost::filesystem::path &path)
{
    return dbg << path.c_str();
}

} // namespace stylist
