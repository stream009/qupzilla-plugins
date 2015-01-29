#ifndef STYLIST_UTILITY_H
#define STYLIST_UTILITY_H

#include <algorithm>
#include <cassert>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/utility/string_ref.hpp>

#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QString>
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


inline QDebug &
operator<<(QDebug dbg, const std::string &str)
{
    return dbg << str.c_str();
}

inline QDebug &
operator<<(QDebug dbg, const boost::string_ref &ref)
{
    return dbg << ref.to_string().c_str();
}

inline QDebug &
operator<<(QDebug dbg, const boost::filesystem::path &path)
{
    return dbg << path.c_str();
}

} // namespace stylist

namespace boost {
template<>
struct hash<QString>
{
    size_t operator()(const QString &str) const {
        return qHash(str);
    }
};

template<>
struct hash<QVariant>
{
    size_t operator()(const QVariant &value) const {
        namespace bfs = boost::filesystem;

        if (value.canConvert<QString>()) {
            return qHash(value.toString());
        }
        else if (value.canConvert<bfs::path>()) {
            const boost::hash<bfs::path> hash {};
            return hash(value.value<bfs::path>());
        }
        else {
            assert(false && "unimplemented type");
        }
    }
};

} // namespace boost

Q_DECLARE_METATYPE(boost::filesystem::path);

#endif // STYLIST_UTILITY_H
