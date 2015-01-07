#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "tabwatcher.h"
#include "toolbar.h"

#include <boost/unordered_set.hpp>
#include <boost/scoped_ptr.hpp>

#include <QtCore/QPoint>

class BrowserWindow;
class QWidget;
class QMouseEvent;

namespace lesschrome {

//TODO setting
class WindowHandler : public QObject
{
    Q_OBJECT
public:
    WindowHandler(BrowserWindow* const window);

    void mouseMove(QMouseEvent* const event);

private:
    // @override QObject
    virtual bool eventFilter(QObject* const, QEvent* const event);

    void captureWidgets();

    QWidget &bookmarksBar() const;
    QWidget &navigationBar() const;

    void toolbarSettingChanged(QWidget &widget, bool enabled);

private slots:
    void slotSettingChanged(const QString &key);
    void slotTabAdded(WebTab*);
    void slotTabDeleted(WebTab*);

private:
    BrowserWindow *m_window;
    QWidget *m_tabBar;
    boost::unordered_set<QWidget*> m_locationBars;

    TabWatcher m_tabWatcher;
    boost::scoped_ptr<Toolbar> m_toolbar;
    boost::scoped_ptr<StatusBar> m_statusBar;
    QPoint m_mousePos;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
