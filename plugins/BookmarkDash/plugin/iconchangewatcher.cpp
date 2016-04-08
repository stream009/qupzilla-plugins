#include "iconchangewatcher.h"

#include <QtWebKitWidgets/QWebView>

#include <bookmarkitem.h>
#include <browserwindow.h>
#include <tabbedwebview.h>
#include <webtab.h>

// Ideally, monitoring iocn change should be done by IconProvider
// then notify via BookmarksModel::dataChanged()
namespace bookmark_dash {

IconChangeWatcher::
IconChangeWatcher(BrowserWindow &window)
    : m_tabWatcher { window }
{
    this->connect(&m_tabWatcher, SIGNAL(tabAdded(WebTab&)),
                  this,            SLOT(onTabAdded(WebTab&)));
}

void IconChangeWatcher::
onTabAdded(WebTab &tab)
{
    //qDebug() << __func__;
    auto* const webView = tab.webView();
    assert(webView);

    this->connect(webView, SIGNAL(iconChanged()),
                  this,      SLOT(onIconChanged()));
}

void IconChangeWatcher::
onIconChanged()
{
    auto* const sender = this->sender();
    assert(sender);
    auto &webView = dynamic_cast<QWebView&>(*sender);

    const auto &url = webView.url();
    //qDebug() << __func__ << url;

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);

    const auto &items = bookmarks->searchBookmarks(url);
    for (auto* const item: items) {
        assert(item);
        bookmarks->changeBookmark(item);
    }
}

} // namespace bookmark_dash
