#ifndef STYLIST_STYLESHEETTEST_H
#define STYLIST_STYLESHEETTEST_H

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {
namespace css {

class StyleSheetTest : public QObject
{
    Q_OBJECT
    using Path = boost::filesystem::path;

private Q_SLOTS:
    void init();
    void cleanupTestCase();

    void testConstructor();
    void testHasStyleFor();
    void testStyleFor();

private:
    const Path m_path { "test.css" };
};

} // namespace css
} // namespace stylist

#endif // STYLIST_STYLESHEETTEST_H
