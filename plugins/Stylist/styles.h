#ifndef STYLIST_STYLES_H
#define STYLIST_STYLES_H

#include "css/stylesheet.h"

#include <string>

namespace stylist {

class Condition;
class Url;
class Styles;

class Style
{
public:
    template<typename Str, typename Path>
    Style(Str &&name, Path &&path, const bool enabled, const Styles &parent)
        : m_parent { parent },
          m_name { std::forward<Str>(name) },
          m_styleSheet { std::forward<Path>(path) },
          m_enabled { enabled }
    {}

    Style(Style &&) noexcept = default; // required by vector
    Style &operator=(Style &&) noexcept; // required by remove

    const std::string &name() const { return m_name; }
    const css::StyleSheet &styleSheet() const { return m_styleSheet; }

    bool enabled() const { return m_enabled; }
    void setEnabled(const bool enabled);

private:
    const Styles &m_parent;
    std::string m_name;
    css::StyleSheet m_styleSheet;
    bool m_enabled;
};

} // namespace stylist

#include "util/buffereddirectorywatcher.h"

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class Styles : public QObject
{
    Q_OBJECT
private:
    using Path = boost::filesystem::path;
    using Container = std::vector<Style>;
    using ConstIterator = Container::const_iterator;

public:
    template<typename P>
    explicit Styles(P &&path)
        : m_directory { std::forward<P>(path) },
          m_dirWatcher { m_directory }
    { init(); }

    std::string query(const Url &url) const;

    bool empty() const { return m_styles.empty(); }
    Container::size_type size() const { return m_styles.size(); }
    Container::reference at(size_t pos) { return m_styles.at(pos); }

Q_SIGNALS:
    void changed() const;

private:
    void init();
    void scanDirectory();

private Q_SLOTS:
    void addFile(const Path&);
    void deleteFile(const Path&);
    void slotFileModified(const Path&);

private:
    Path m_directory;
    BufferedDirectoryWatcher m_dirWatcher;
    Container m_styles;

    friend void Style::setEnabled(const bool);
};

} // namespace stylist

#endif // STYLIST_STYLES_H
