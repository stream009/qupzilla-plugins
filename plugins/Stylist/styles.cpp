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
        css::StyleSheet css { entry.path() };
        auto &rules = css.documentRules();
        std::move(rules.begin(), rules.end(), std::back_inserter(m_styles));
    }

    qDebug() << m_styles.size() << "styles are loaded"; //TODO remove
}

std::string Styles::
query(const Url &url) const
{
    // Range adaptors require default copy constructable functor.
    // A closure created by lambda expression does not satisfy that.
    struct Filter {
        bool operator()(const css::DocumentRule &rule) const {
            return rule.match(m_url);
        }
        // Copy is mandatory since filter might outlive the given url.
        Filter(const Url &url) : m_url { url } {}
        Url m_url;
    };

    struct Extractor {
        const std::string &operator()(const css::DocumentRule &rule) const {
            return rule.body();
        }
    };

    namespace ba = boost::algorithm;
    namespace bad = boost::adaptors;
    return ba::join(m_styles | bad::filtered(Filter { url })
                             | bad::transformed(Extractor {}),
                    "\n");
}

} // namespace stylist
