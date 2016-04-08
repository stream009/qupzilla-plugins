#ifndef COMMON_TEST_HPP
#define COMMON_TEST_HPP

#include <ostream>

#include <QtCore/QString>

inline void
PrintTo(const QString &str, std::ostream *os)
{
    *os << str.toUtf8().constData();
}

inline std::ostream&
operator<<(std::ostream &os, const QString &str)
{
    return os << str.toUtf8().constData();
}

#endif // COMMON_TEST_HPP
