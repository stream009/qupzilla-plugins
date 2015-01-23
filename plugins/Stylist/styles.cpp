#include "styles.h"

#include "plugin.h"
#include "css/stylesheet.h"

#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace stylist {

void Style::
setEnabled(const bool enabled)
{
    m_enabled = enabled;
    Q_EMIT Plugin::styles().changed();
}

Styles::
Styles(const Path &path)
    : m_directory { path }
{
    namespace bfs = boost::filesystem;

    struct Predicate {
        bool operator()(const bfs::directory_entry &e) const {
            return e.path().extension() == ".css";
        }
    };

    bfs::create_directory(path);
    const auto &range = boost::make_iterator_range(
        boost::make_filter_iterator(
            Predicate {},
            bfs::directory_iterator { path }),
        {}
    );

    for (const auto &entry: range) {
        auto name = entry.path().filename().string();
        m_styles.emplace_back(
            entry.path().filename().c_str(), entry.path(), true);
    }

    qDebug() << m_styles.size() << "styles are loaded"; //TODO remove
}

std::string Styles::
query(const Url &url) const
{
    struct Filter {
        bool operator()(const Style &style) const {
            return style.enabled();
        }
    };
    struct Extractor {
        std::string operator()(const Style &style) const {
            return style.styleSheet().styleFor(*m_url);
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

} // namespace stylist
