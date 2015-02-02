#ifndef STYLIST_CSS_CONDITION_H
#define STYLIST_CSS_CONDITION_H

#include "common/utility.h"
#include "common/url.h"

#include <string>

#include <boost/make_unique.hpp>
#include <boost/regex.hpp>

namespace stylist {
namespace css {

class Condition
{
public:
    virtual ~Condition() = default;

    virtual bool match(const Url &url) const = 0;

    template<typename T, typename BidirectionalRange, typename OutputIt>
    static void extract(const BidirectionalRange &source, OutputIt dest)
    {
        assert(T::pattern().mark_count() == 1);

        auto it = source.begin();
        boost::match_results<decltype(it)> match;
        while (boost::regex_search(it, source.end(), match, T::pattern())) {
            dest = boost::make_unique<T>(match[1].str());

            it = match[0].second;
        }
    }
};


class UrlCondition : public Condition
{
public:
    template<typename Str>
    UrlCondition(Str &&value)
        : m_url { std::forward<Str>(value) }
    {}

    // @override Condition
    bool match(const Url &url) const override;

    template<typename BidirectionalRange, typename OutputIt>
    static void extract(const BidirectionalRange &source, OutputIt dest)
    {
        Condition::extract<UrlCondition>(source, dest);
    }

    static const boost::regex &pattern();

private:
    Url m_url;
};


class UrlPrefixCondition : public Condition
{
public:
    template<typename Str>
    UrlPrefixCondition(Str &&value)
        : m_prefix { std::forward<Str>(value) }
    {}

    // @override Condition
    bool match(const Url &url) const override;

    template<typename BidirectionalRange, typename OutputIt>
    static void extract(const BidirectionalRange &source, OutputIt dest)
    {
        Condition::extract<UrlPrefixCondition>(source, dest);
    }

    static const boost::regex &pattern();

private:
    std::string m_prefix;
};


class DomainCondition : public Condition
{
public:
    template<typename Str>
    DomainCondition(Str &&value)
        : m_domain { std::forward<Str>(value) }
    {}

    // @override Condition
    bool match(const Url &url) const override;

    template<typename BidirectionalRange, typename OutputIt>
    static void extract(const BidirectionalRange &source, OutputIt dest)
    {
        Condition::extract<DomainCondition>(source, dest);
    }

    static const boost::regex &pattern();

private:
    std::string m_domain;
};


class RegExCondition : public Condition
{
public:
    template<typename Str>
    RegExCondition(Str &&value)
        : m_regEx { std::forward<Str>(value) }
    {
        //TODO valdiate regex
    }

    // @override Condition
    bool match(const Url &url) const override;

    template<typename BidirectionalRange, typename OutputIt>
    static void extract(const BidirectionalRange &source, OutputIt dest)
    {
        Condition::extract<RegExCondition>(source, dest);
    }

    static const boost::regex &pattern();

private:
    boost::regex m_regEx;
};

} // namespace css
} // namespace stylist

#endif // STYLIST_CSS_CONDITION_H
