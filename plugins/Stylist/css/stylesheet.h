#ifndef STYLIST_STYLESHEET_H
#define STYLIST_STYLESHEET_H

#include "../utility.h"

#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/any_range.hpp>

namespace stylist {
namespace css {

class DocumentRule;

class StyleSheet
{
private:
    using Path = boost::filesystem::path;
    using StyleRange = boost::any_range<
                            const std::string &,
                            boost::bidirectional_traversal_tag,
                            const std::string &,
                            std::ptrdiff_t>;
public:
    explicit StyleSheet(const Path &path);
    ~StyleSheet();

    StyleSheet(StyleSheet &&) = default;

    bool hasStyleFor(const Url &) const; //TODO could be removed
    StyleRange styleFor(const Url &) const; //TODO could be removed

    std::vector<DocumentRule> &documentRules() { return m_documentRules; }

private:
    Path m_filePath;
    std::vector<DocumentRule> m_documentRules;
};

} // namespace css
} // namespace stylist

#endif // STYLIST_STYLESHEET_H
