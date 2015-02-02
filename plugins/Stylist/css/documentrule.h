#ifndef STYLIST_DOCUMENT_RULE_H
#define STYLIST_DOCUMENT_RULE_H

#include "condition.h"

#include "core/utility.h"

#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <boost/make_unique.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>

namespace stylist {
namespace css {

class DocumentRule
{
public:
    template<typename Str>
    DocumentRule(const Str &header, const Str &body);
    ~DocumentRule();

    DocumentRule(DocumentRule&&) noexcept = default;

    bool match(const Url &url) const;
    const std::string &body() const { return m_body; }

    template<typename InputRange, typename OutputIt>
    static void extract(const InputRange &source, OutputIt dest);

private:
    static const boost::regex &pattern();

private:
    std::string m_body;
    std::vector<std::unique_ptr<Condition>> m_conditions;
};

template<typename It, char Open = '{', char Close = '}'>
inline It
searchMatchingBracket(It begin, It end)
{
    assert(*begin == Open);

    auto level = 0;
    It it = begin;
    for (; it != end; ++it) {
        if (*it == Open) {
            ++level;
        }
        else if (*it == Close) {
            if (--level == 0) break;
        }
    }
    assert(level >= 0);

    return it;
}

template<typename Str>
DocumentRule::
DocumentRule(const Str &header, const Str &body)
    : m_body { std::begin(body), std::end(body) }
{
    auto it = std::back_inserter(m_conditions);

    UrlCondition::extract(header, it);
    UrlPrefixCondition::extract(header, it);
    DomainCondition::extract(header, it);
    RegExCondition::extract(header, it);
}

template<typename BidirectionalRange, typename OutputIt>
inline void DocumentRule::
extract(const BidirectionalRange &source, OutputIt dest)
{
    auto it = std::begin(source);
    boost::match_results<decltype(it)> match;
    while (boost::regex_search(it, std::end(source),
                                   match, pattern()))
    {
        const auto &suffix = match.suffix();
        assert(*(suffix.first-1) == '{');

        const auto closeBracket =
            searchMatchingBracket(suffix.first-1, suffix.second);

        dest = DocumentRule {
            boost::make_iterator_range(match[0].first, match[0].second),
            boost::make_iterator_range(match[0].second, closeBracket)
        };

        it = closeBracket + 1;
    }
}

} // namespace css
} // namespace stylist

#endif // STYLIST_DOCUMENT_RULE_H
