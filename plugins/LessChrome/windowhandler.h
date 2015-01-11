#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "tabwatcher.h"

#include <boost/unordered_map.hpp>
#include <boost/scoped_ptr.hpp>

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

private:
    // @override QObject. Must be throw()
    virtual bool eventFilter(QObject* const, QEvent* const);

    void captureWidgets();

    QWidget &bookmarksBar() const;
    QWidget &navigationBar() const;

    void toolbarSettingChanged(QWidget &widget, bool enabled);

private slots:
    void slotSettingChanged(const QString &key);
    void slotLoadStarted(); // throw()
    void slotLoadFinished(); // throw()
    void slotTabAdded(WebTab&);
    void slotTabDeleted(WebTab&);

private:
    BrowserWindow &m_window;
    QWidget *m_navigationContainer;

    // map<LocationBar*, WebView*>
    boost::unordered_map<QWidget*, QWidget*> m_tabInfos;

    TabWatcher m_tabWatcher;

    boost::scoped_ptr<Toolbar> m_toolbar;
    boost::scoped_ptr<StatusBar> m_statusBar;
    boost::scoped_ptr<MenuBar> m_menuBar;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
