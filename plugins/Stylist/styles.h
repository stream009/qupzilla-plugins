#ifndef STYLIST_STYLES_H
#define STYLIST_STYLES_H

#include "css/stylesheet.h"

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

public:
    Styles(const Path &path);

    std::string query(const Url &url) const;

private:
    Path m_directory;
    std::vector<css::StyleSheet> m_styles;
};

} // namespace stylist

#endif // STYLIST_STYLES_H
