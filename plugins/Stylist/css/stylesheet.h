#ifndef STYLIST_STYLESHEET_H
#define STYLIST_STYLESHEET_H

#include "documentrule.h"
#include "../utility.h"

#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/any_range.hpp>

namespace stylist {
namespace css {

class StyleSheet
{
private:
    using Path = boost::filesystem::path;

public:
    explicit StyleSheet(const Path &path);

    StyleSheet(StyleSheet &&) = default;

    bool hasStyleFor(const Url &) const;
    std::string styleFor(const Url &) const;

private:
    Path m_filePath;
    std::vector<DocumentRule> m_documentRules;
};

} // namespace css
} // namespace stylist

#endif // STYLIST_STYLESHEET_H
