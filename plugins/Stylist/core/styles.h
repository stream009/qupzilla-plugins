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
    template<typename Str>
    void setName(Str &&name)
    {
        m_name = std::forward<Str>(name);
    }


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

#include "core/buffereddirectorywatcher.h"

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class Styles : public QObject
{
    Q_OBJECT
private:
    friend class boost::serialization::access;

    using Path = boost::filesystem::path;
    using Container = std::vector<Style>; //TODO set would be more appropriate
    using ConstIterator = Container::const_iterator;

public:
    static Styles &instance();
    ~Styles() override;

    std::string query(const Url &url) const;

    bool empty() const { return m_styles.empty(); }
    Container::size_type size() const { return m_styles.size(); }
    Container::reference at(size_t pos) { return m_styles.at(pos); }
    Container::reference at(const Path &path);

    void import(const Path &path);
    void remove(const Path &path);

    static const Path &directory();
    static void setDirectory(const Path &);

Q_SIGNALS:
    void changed() const;

private:
    Styles();

    template<typename C> // for deserialization
    Styles(C &&container)
        : m_dirWatcher { m_directory },
          m_styles { std::forward<C>(container) }
    { init(); }

    static std::unique_ptr<Styles> create();
    void init();
    void scanDirectory();

private Q_SLOTS:
    void addFile(const Path&);
    void deleteFile(const Path&);
    void slotFileModified(const Path&);

private:
    static Path m_directory;
    BufferedDirectoryWatcher m_dirWatcher;
    Container m_styles;

    friend class StylesTest;
    friend void Style::setEnabled(const bool);

#ifdef __GNUC__
    template<typename Archive>
    friend void boost::serialization::
    save_construct_data(Archive &ar, const stylist::Styles *styles,
                                     const unsigned int /*version*/)
    {
        //qDebug() << "Styles" << __func__;
        const auto &container = styles->m_styles;

        // ar << container.size();  //TODO why the f--- does this fail?
        const auto count = container.size();
        ar << count;

        for (const auto &item: container) {
            //ar << &item; //TODO ditto
            const stylist::Styles::Container::value_type *ptr = &item;
            ar << ptr;
        }
    }

    template<typename Archive>
    friend void boost::serialization::
    load_construct_data(Archive &ar, stylist::Styles *styles,
                                     const unsigned int /*version*/)
    {
        //qDebug() << "Styles" << __func__;
        using stylist::Styles;

        Styles::Container container; //ar >> container;
        size_t count; ar >> count;
        for (auto i = 0u; i < count; ++i) {
            Styles::Container::value_type *style;
            try {
                ar >> style;
            }
            catch (const std::exception &) { //TODO more specific exception
                continue;
            }
            container.push_back(std::move(*style));
        }

        ::new(styles) Styles { std::move(container) };
    }
#endif // __GNUC__
    template<typename Archive>
    friend void boost::serialization::
    save_construct_data(Archive&, const Styles*,
                                   const unsigned int version);
    template<typename Archive>
    friend void boost::serialization::
    load_construct_data(Archive&, Styles*,
                                   const unsigned int version);
};

} // namespace stylist

#endif // STYLIST_STYLES_H
