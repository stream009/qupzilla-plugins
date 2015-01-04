#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "tabwatcher.h"
#include "toolbar.h"

#include <cassert>
#include <utility>
#include <vector>

#include <boost/unordered_set.hpp>

#include <QtCore/QTimer>

class QWidget;
class QMouseEvent;

namespace lesschrome {

//TODO save initial visibility of target widgets
//TODO detect visibility change of menubar, toolbar
//TODO status bar support
class WindowHandler : public QObject
{
    Q_OBJECT
public:
    WindowHandler(BrowserWindow* const window);
    ~WindowHandler();

    void mouseMove(QMouseEvent* const event);

private:
    // @override QObject
    virtual bool eventFilter(QObject* const, QEvent* const event);

    void captureWidgets();
    void resizeToolBars();

private slots:
    void slotTabAdded(WebTab*);
    void slotTabDeleted(WebTab*);

private:
    QWidget *m_window;
    QWidget *m_navigationContainer;
    QWidget *m_webView;
    QWidget *m_tabBar;
    TabWatcher m_tabWatcher;
    boost::unordered_set<QWidget*> m_locationBars;

    Toolbar m_container;
    QPoint m_mousePos;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
