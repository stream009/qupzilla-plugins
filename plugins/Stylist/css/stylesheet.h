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
    template<typename Path>
    explicit StyleSheet(Path &&path)
        : m_filePath { std::forward<Path>(path) }
    {
        init();
    }

    StyleSheet(StyleSheet &&) noexcept;

    bool hasStyleFor(const Url &) const;
    std::string styleFor(const Url &) const;

    const Path &path() const { return m_filePath; }

private:
    void init();

private:
    Path m_filePath;
    std::vector<DocumentRule> m_documentRules;
};

} // namespace css
} // namespace stylist

#endif // STYLIST_STYLESHEET_H
