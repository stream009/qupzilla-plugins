#ifndef GTEST_UTILITY_HPP
#define GTEST_UTILITY_HPP

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

#endif // GTEST_UTILITY_HPP
