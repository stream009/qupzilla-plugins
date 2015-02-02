#ifndef STYLIST_URL_H
#define STYLIST_URL_H

#include "utility.h"

#include <string>

#include <QtCore/QUrl>

namespace stylist {

class Url : public QUrl
{
public:
    Url(const char* const url); //implicit
    Url(const QUrl &url); // implicit
    Url(const QuString &url); // implicit
    ~Url() = default; // Caution! Not virtual

    std::string str() const;
};

} // namespace stylist

#endif // STYLIST_URL_H
