#include "documentrule.h"

#include "condition.h"

#include <boost/format.hpp>

namespace stylist {
namespace css {

DocumentRule::~DocumentRule() = default;

bool DocumentRule::
match(const Url &url) const
{
    return std::any_of(m_conditions.begin(), m_conditions.end(),
        [&url] (const std::unique_ptr<Condition> &cond) {
            return cond->match(url);
        }
    );
}

const boost::regex &DocumentRule::
pattern()
{
    static const std::string functions =
        str(boost::format { "%s|%s|%s|%s" }
            % UrlCondition::pattern().str()
            % UrlPrefixCondition::pattern().str()
            % DomainCondition::pattern().str()
            % RegExCondition::pattern().str());

    static const char* const rule = R"(@(?:-[a-z]+-)?document)";

    static const boost::regex pattern {
        str(boost::format {
                R"(%s\s+(?:%s)(?:\s*,\s*%s)*\s*{)"
            } % rule % functions % functions
        )
    };

    return pattern;
}

} // namespace css
} // namespace stylist
