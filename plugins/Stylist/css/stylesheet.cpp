#include "stylesheet.h"

#include "documentrule.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/size.hpp>

namespace stylist {
namespace css {

StyleSheet::
StyleSheet(const Path &path)
    : m_filePath { path }
{
    // mmap give us random access iterator (pointer to raw data)
    // as opposed to fstream which give us only forward traversal
    // iterator. To execute regular expression we need better iterator
    // than forward traversal iterator.
    // Alternative design would be reading file into the buffer first.
    const boost::iostreams::mapped_file_source file { path };
    const auto &contents = boost::make_iterator_range(
        file.data(), file.data() + file.size()
    );

    DocumentRule::extract(contents, std::back_inserter(m_documentRules));
}

bool StyleSheet::
hasStyleFor(const Url &url) const
{
    return std::any_of(m_documentRules.begin(), m_documentRules.end(),
        [&url] (const DocumentRule &rule) {
            return rule.match(url);
        }
    );
}

std::string StyleSheet::
styleFor(const Url &url) const
{
    // Range adaptors require default copy constructable functor.
    // A closure created by lambda expression does not satisfy that.
    struct Filter {
        bool operator()(const DocumentRule &rule) const {
            return rule.match(m_url);
        }
        // Copy is mandatory since filter might outlive the given url.
        Filter(const Url &url) : m_url { url } {}
        Url m_url;
    };

    struct Extractor {
        const std::string &operator()(const DocumentRule &rule) const {
            return rule.body();
        }
    };

    namespace ba = boost::algorithm;
    namespace bad = boost::adaptors;
    return ba::join(m_documentRules | bad::filtered(Filter { url })
                                    | bad::transformed(Extractor {}),
                    "\n");
}

} // namespace css
} // namespace stylist
