#include "styles.h"

#include "css/stylesheet.h"

#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace stylist {

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
        m_styles.emplace_back(entry.path());
    }

    qDebug() << m_styles.size() << "styles are loaded"; //TODO remove
}

std::string Styles::
query(const Url &url) const
{
    struct Extractor {
        std::string operator()(const css::StyleSheet &css) const {
            return css.styleFor(*m_url);
        }
        Extractor(const Url &url) : m_url { &url } {}
        const Url *m_url;
    };

    namespace ba = boost::algorithm;
    namespace bad = boost::adaptors;
    return ba::join(m_styles | bad::transformed(Extractor { url }),
                    "\n");
}

} // namespace stylist
