#include "styles.h"

#include "css/stylesheet.h"

#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <QtCore/QDebug>

namespace stylist {

Style::
Style(Style &&rhs) noexcept
    : m_parent { rhs.m_parent },
      m_name { std::move(rhs.m_name) },
      m_path { std::move(rhs.m_path) },
      m_enabled { rhs.m_enabled },
      m_styleSheet { std::move(rhs.m_styleSheet) }
{}

// We have to write this manually, since libstd++ doesn't give noexcept
// move assignment operator to std::string
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58265
Style &Style::
operator=(Style &&rhs) noexcept
{
    m_name = std::move(rhs.m_name);
    m_path = std::move(rhs.m_path);
    m_styleSheet = std::move(rhs.m_styleSheet);
    m_enabled = rhs.m_enabled;

    return *this;
}

void Style::
setEnabled(const bool enabled)
{
    m_enabled = enabled;
    Q_EMIT m_parent.changed();
}

} // namespace stylist

namespace stylist {

std::string Styles::
query(const Url &url) const
{
    struct Filter {
        bool operator()(const Container::value_type &value) const {
            return value.enabled();
        }
    };
    struct Extractor {
        std::string operator()(const Container::value_type &value) const {
            return value.styleFor(*m_url);
        }
        Extractor(const Url &url) : m_url { &url } {}
        const Url *m_url;
    };

    namespace ba = boost::algorithm;
    namespace bad = boost::adaptors;
    return ba::join(m_styles | bad::filtered(Filter {})
                             | bad::transformed(Extractor { url }),
                    "\n");
}

void Styles::
init()
{
    namespace bfs = boost::filesystem;

    bfs::create_directory(m_directory);

    scanDirectory();

    this->connect(&m_dirWatcher, SIGNAL(fileAdded(const Path&)),
                  this,          SLOT(addFile(const Path&)));
    this->connect(&m_dirWatcher, SIGNAL(fileDeleted(const Path&)),
                  this,          SLOT(deleteFile(const Path&)));
    this->connect(&m_dirWatcher, SIGNAL(fileModified(const Path&)),
                  this,          SLOT(slotFileModified(const Path&)));
}

void Styles::
scanDirectory()
{
    namespace bfs = boost::filesystem;

    m_styles.clear();

    struct Filter {
        bool operator()(const bfs::directory_entry &e) const {
            return e.path().extension() == ".css";
        }
    };

    const auto &range = boost::make_iterator_range(
        boost::make_filter_iterator(
            Filter {},
            bfs::directory_iterator { m_directory }),
        {}
    );

    for (const auto &entry: range) {
        const auto &path = entry.path();
        addFile(path);
    }

    //qDebug() << m_styles.size() << "styles are loaded";
}

void Styles::
addFile(const Path &path)
{
    const auto &name = path.filename();
    m_styles.emplace_back(name.c_str(), path, true, *this);
    Q_EMIT changed();
}

void Styles::
deleteFile(const Path &path)
{
    boost::range::remove_erase_if(m_styles,
        [&path] (const decltype(m_styles)::value_type &style) {
            return style.path() == path;
        }
    );
    Q_EMIT changed();
}

void Styles::
slotFileModified(const Path &)
{
    Q_EMIT changed();
}

} // namespace stylist
