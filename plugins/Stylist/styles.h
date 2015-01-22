#ifndef STYLIST_STYLES_H
#define STYLIST_STYLES_H

#include "css/documentrule.h"

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

namespace stylist {

class Condition;
class Url;

class Styles
{
private:
    using Path = boost::filesystem::path;
    using Container = std::vector<css::DocumentRule>;

public:
    Styles(const Path &path);

    std::string query(const Url &url) const;

private:
    Path m_directory;
    Container m_styles;
};

} // namespace stylist

#endif // STYLIST_STYLES_H
