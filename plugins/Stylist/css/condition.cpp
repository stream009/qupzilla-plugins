#include "condition.h"

#include <boost/algorithm/string/predicate.hpp>

namespace stylist {
namespace css {

const boost::regex &UrlCondition::
pattern()
{
    //TODO matching quote mark
    static const boost::regex result {
        R"(url\(["']?(.*?)["']?\))"
        //R"(url\((["']?)(.*?)(?:\1)?\))"
    };
    return result;
}

bool UrlCondition::
match(const Url &url) const
{
    return url == m_url;
}


const boost::regex &UrlPrefixCondition::
pattern()
{
    static const boost::regex result {
        R"(url-prefix\(["']?(.*?)["']?\))"
    };
    return result;
}

bool UrlPrefixCondition::
match(const Url &url) const
{
    namespace ba = boost::algorithm;
    return ba::starts_with(url.str(), m_prefix);
}


const boost::regex &DomainCondition::
pattern()
{
    static const boost::regex result {
        R"(domain\(["']?(.*?)["']?\))"
    };
    return result;
}

bool DomainCondition::
match(const Url &url) const
{
    namespace ba = boost::algorithm;
    return ba::ends_with(url.host(), m_domain);
}


const boost::regex &RegExCondition::
pattern()
{
    static const boost::regex result {
        R"(regex\(["'](.*?)["']\))"
    };
    return result;
}

bool RegExCondition::
match(const Url &url) const
{
    const auto &url_ = url.str();
    return boost::regex_match(url_.begin(), url_.end(), m_regEx);
}

} // namespace css
} // namespace stylist
