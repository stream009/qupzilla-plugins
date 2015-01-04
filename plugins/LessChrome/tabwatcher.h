#ifndef TABWATCHER_H
#define TABWATCHER_H

#include "tabwidget.h"

#include <boost/noncopyable.hpp>

#include <QtCore/QObject>
#include <QtCore/QList>

class BrowserWindow;
class TabWidget;
class WebTab;

namespace lesschrome {

class TabWatcher : public QObject, boost::noncopyable
{
    Q_OBJECT
public:
    explicit TabWatcher(BrowserWindow* const window);

signals:
    void tabAdded(WebTab*) const;
    void tabDeleted(WebTab*) const;

private:
    // @override QObject
    virtual void connectNotify(const char* const signal);

    void notifyAdded(WebTab* const tab) const;
    void notifyDeleted(WebTab* const tab) const;

private slots:
    void slotTabChanged();

private:
    TabWidget* m_tabWidget;
    QList<WebTab*> m_tabs;
};

} // namespace lesschrome

#endif // TABWATCHER_H
