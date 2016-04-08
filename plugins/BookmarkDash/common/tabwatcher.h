#ifndef COMMON_TABWATCHER_H
#define COMMON_TABWATCHER_H

#include "tabwidget.h"

#include <QtCore/QObject>
#include <QtCore/QList>

class BrowserWindow;
class TabWidget;
class WebTab;

namespace bookmark_dash {

class TabWatcher : public QObject
{
    Q_OBJECT
public:
    explicit TabWatcher(BrowserWindow &window);

Q_SIGNALS:
    void tabAdded(WebTab&) const;
    void tabDeleted(WebTab&) const;

private:
    // @override QObject
    void connectNotify(const QMetaMethod &signal) override; // throw()

private Q_SLOTS:
    void slotTabChanged(); // throw()

private:
    TabWidget* m_tabWidget = nullptr;
    QList<WebTab*> m_tabs;
};

} // namespace bookmark_dash

#endif // COMMON_TABWATCHER_H
