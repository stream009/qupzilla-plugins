#ifndef BOOKMARK_DASH_ICON_CHANGE_WATCHER_H
#define BOOKMARK_DASH_ICON_CHANGE_WATCHER_H

#include "common/tabwatcher.h"

#include <QtCore/QObject>

class Bookmarks;
class WebTab;

namespace bookmark_dash {

class IconChangeWatcher : public QObject
{
    Q_OBJECT
public:
    IconChangeWatcher(BrowserWindow&);

private Q_SLOTS:
    void onTabAdded(WebTab&);
    void onIconChanged();

private:
    TabWatcher m_tabWatcher;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_ICON_CHANGE_WATCHER_H
