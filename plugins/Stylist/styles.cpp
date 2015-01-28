#include "styles.h"

#include "css/stylesheet.h"

#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <QtCore/QDebug>

namespace stylist {

void Style::
setEnabled(const bool enabled)
{
    m_enabled = enabled;
    Q_EMIT m_parent.changed();
}

// We have to write this manually, since libstd++ doesn't give noexcept
// move assignment operator to std::string
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58265
Style &Style::
operator=(Style &&rhs) noexcept
{
    m_name = std::move(rhs.m_name);
    m_styleSheet = std::move(rhs.m_styleSheet);
    m_enabled = rhs.m_enabled;

    return *this;
}

bool operator<(const Style &lhs, const Style &rhs)
{
    return lhs.m_styleSheet.path() < rhs.m_styleSheet.path();
}


Styles::
Styles(const Path &path)
    : m_directory { path }
{
    namespace bfs = boost::filesystem;

    bfs::create_directory(path);

    scanDirectory();
}

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
            return value.styleSheet().styleFor(*m_url);
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
scanDirectory()
{
    namespace bfs = boost::filesystem;

    struct Predicate {
        bool operator()(const bfs::directory_entry &e) const {
            return e.path().extension() == ".css";
        }
    };

    const auto &range = boost::make_iterator_range(
        boost::make_filter_iterator(
            Predicate {},
            bfs::directory_iterator { m_directory }),
        {}
    );

    for (const auto &entry: range) {
        const auto &path = entry.path();
        const auto &name = path.filename().c_str();
        m_styles.emplace_back(name, path, true, *this);
    }
    std::sort(m_styles.begin(), m_styles.end());

    qDebug() << m_styles.size() << "styles are loaded"; //TODO remove
}

} // namespace stylist
