#ifndef STYLIST_STYLES_H
#define STYLIST_STYLES_H

#include "css/stylesheet.h"

#include <string>

#include <boost/filesystem.hpp>

namespace stylist {

class Condition;
class Url;
class Styles;

class Style
{
    using Path = boost::filesystem::path;
public:
    template<typename Str, typename P>
    Style(Str &&name, P &&path, const bool enabled, const Styles &parent)
        : m_parent { parent },
          m_name { std::forward<Str>(name) },
          m_path { std::forward<P>(path) },
          m_enabled { enabled },
          m_styleSheet { m_path }
    {}

    Style(Style &&) noexcept; // required by vector
    Style &operator=(Style &&) noexcept; // required by remove

    const std::string &name() const { return m_name; }
    const Path &path() const { return m_path; }

    bool enabled() const { return m_enabled; }
    void setEnabled(const bool enabled);

    std::string styleFor(const Url &url) const
    {
        return m_styleSheet.styleFor(url);
    }

private:
    const Styles &m_parent;
    std::string m_name;
    Path m_path;
    bool m_enabled;
    css::StyleSheet m_styleSheet;

    friend class StylesTest;

    template<typename Archive>
    friend void boost::serialization::
    save_construct_data(Archive&, const Style*,
                                  const unsigned int version);
    template<typename Archive>
    friend void boost::serialization::
    load_construct_data(Archive&, Style*,
                                  const unsigned int version);
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

    void scanDirectory();

Q_SIGNALS:
    void changed() const;

private:
    void init();

private Q_SLOTS:
    void addFile(const Path&);
    void deleteFile(const Path&);
    void slotFileModified(const Path&);

private:
    Path m_directory;
    BufferedDirectoryWatcher m_dirWatcher;
    Container m_styles;

    friend class StylesTest;
    friend void Style::setEnabled(const bool);

    template<typename Archive>
    friend void boost::serialization::
    save_construct_data(Archive &, const Styles*,
                                   const unsigned int version);
    template<typename Archive>
    friend void boost::serialization::
    load_construct_data(Archive &, Styles*,
                                   const unsigned int version);
};

} // namespace stylist

#endif // STYLIST_STYLES_H
