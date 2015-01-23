#ifndef STYLIST_STYLES_H
#define STYLIST_STYLES_H

#include "css/stylesheet.h"

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class Condition;
class Url;

class Style
{
public:
    template<typename Str, typename Path>
    Style(Str &&name, Path &&path, const bool enabled)
        : m_name { std::forward<Str>(name) },
          m_styleSheet { std::forward<Path>(path) },
          m_enabled { enabled }
    {}

    Style(Style &&) noexcept = default;

    const std::string &name() const { return m_name; }
    const css::StyleSheet &styleSheet() const { return m_styleSheet; }

    bool enabled() const { return m_enabled; }
    void setEnabled(const bool enabled);

private:
    std::string m_name;
    css::StyleSheet m_styleSheet;
    bool m_enabled;
};

class Styles : public QObject
{
    Q_OBJECT
private:
    using Path = boost::filesystem::path;
    using Container = std::vector<Style>;
    using ConstIterator = Container::const_iterator;

public:
    Styles(const Path &path);

    std::string query(const Url &url) const;

    Container::const_iterator begin() const { return m_styles.begin(); }
    Container::const_iterator end() const { return m_styles.end(); }
    Container::size_type size() const { return m_styles.size(); }

    Container::const_reference at(size_t pos) const
    {
        return m_styles.at(pos);
    }
    Container::reference at(size_t pos) { return m_styles.at(pos); }

Q_SIGNALS:
    void changed();

private:
    Path m_directory;
    Container m_styles;

    friend void Style::setEnabled(const bool);
};

} // namespace stylist

#endif // STYLIST_STYLES_H
