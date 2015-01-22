#ifndef STYLIST_UTILITY_H
#define STYLIST_UTILITY_H

#include <cassert>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/utility/string_ref.hpp>

#include <QtCore/QDebug>
#include <QtCore/QUrl>

namespace stylist {

class QuString : public QString
{
public:
    QuString(const std::string &str) // implicit
        : QString { QString::fromUtf8(str.c_str()) }
    {}

    QuString(const boost::string_ref &ref) // implicit
        : QString { QString::fromUtf8(ref.data(), ref.size()) }
    {}

    ~QuString() = default; // Caution! Not virtual

    using ::QString::QString;
};


class Url : public QUrl
{
public:
    Url(const char* const url); //implicit
    Url(const QUrl &url); // implicit
    Url(const QuString &url); // implicit
    ~Url() = default; // Caution! Not virtual

    std::string str() const;
};


QDebug &operator<<(QDebug dbg, const std::string &str);
QDebug &operator<<(QDebug dbg, const boost::string_ref &ref);
QDebug &operator<<(QDebug dbg, const boost::filesystem::path &path);

} // namespace stylist

#endif // STYLIST_UTILITY_H
