#include <QtCore/QObject>

namespace stylist {
namespace css {

class DocumentRuleTest : public QObject
{
    Q_OBJECT
private slots:
    void testExtractZeroRule();
    void testExtractOneRule();
    void testExtractMultipleRules();
};

} // namespace css
} // namespace stylist
