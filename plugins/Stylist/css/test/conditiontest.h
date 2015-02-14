#include <QtCore/QObject>

namespace stylist {
namespace css {

class ConditionTest : public QObject
{
    Q_OBJECT
private slots:
    void testUrlCondition();
    void testUrlPrefixCondition();
    void testDomainCondition();
    void testRegExCondition();
};

}} // namespace stylist::css
