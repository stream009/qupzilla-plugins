#include "../setting_dialog.hpp"

#include "core/settings.hpp"
#include "core/adblock_dash.hpp"

#include <boost/filesystem.hpp>

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QtNetwork/QNetworkAccessManager>

namespace adblock_dash {

static const char*
prepareDataDirectory()
{
    namespace bfs = boost::filesystem;

    static const auto &dirName = "data";
#if 0
    bfs::remove_all(dirName);
    bfs::create_directory(dirName);
#endif
    assert(bfs::exists(dirName));

    return dirName;
}

} // namespace adblock_dash

int main(int argc, char *argv[])
{
    QApplication app { argc, argv };

    using namespace adblock_dash;
    Settings settings { prepareDataDirectory() };

#if 0
    auto &subscriptions = settings.subscriptions();

    QUrl homePage { "http://www.google.com" };
    QUrl location { "http://www.adblock.org" };
    const auto &updated = QDateTime::currentDateTime();
    subscriptions.emplace_back("First" , "data/easylist.txt", homePage, location, updated, false);
    subscriptions.emplace_back("Second", "data/customlist.txt", homePage, location, updated, false);
    subscriptions.emplace_back("Third" , "data/fanboy.txt", homePage, location, updated, false);
#endif

    QWidget parent;
    SettingDialog dialog { settings, parent };

    return dialog.exec();
}
