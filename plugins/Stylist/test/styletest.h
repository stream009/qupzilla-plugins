#include <string>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class StylesTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testConstructor();
    void testQuery();

private:
    const boost::filesystem::path m_directory { "styles" };
};

} // namespace stylist
