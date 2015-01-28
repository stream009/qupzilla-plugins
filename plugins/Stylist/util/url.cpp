#include "url.h"

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

} // namespace stylist
