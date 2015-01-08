#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "tabwatcher.h"

#include <boost/unordered_set.hpp>
#include <boost/scoped_ptr.hpp>

#include <QtCore/QPoint>

class BrowserWindow;
class QEvent;
class QMouseEvent;
class QWidget;

namespace lesschrome {

class MenuBar;
class StatusBar;
class Toolbar;

class WindowHandler : public QObject
{
    Q_OBJECT
public:
    WindowHandler(BrowserWindow &window);
    virtual ~WindowHandler();

    void mouseMove(const QMouseEvent &event);

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
    BrowserWindow &m_window;
    QWidget *m_navigationContainer;
    boost::unordered_set<QWidget*> m_locationBars;

    TabWatcher m_tabWatcher;

    boost::scoped_ptr<Toolbar> m_toolbar;
    boost::scoped_ptr<StatusBar> m_statusBar;
    boost::scoped_ptr<MenuBar> m_menuBar;

    QPoint m_mousePos;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
