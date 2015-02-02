#ifndef STYLE_TEST_H
#define STYLE_TEST_H

#include <string>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class StylesTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testConstructor() const;
    void testQuery() const;

    void testAddFile() const;
    void testDeleteFile() const;
    void testModifyFile() const;
    void testRenameFile() const;

    void testSerialize() const;

private:
    const boost::filesystem::path m_directory { "styles" };
};

} // namespace stylist

#endif // STYLE_TEST_H
