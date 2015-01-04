#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "tabwatcher.h"
#include "toolbar.h"

#include <boost/unordered_set.hpp>

#include <QtCore/QPoint>

class QWidget;
class QMouseEvent;

namespace lesschrome {

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

private slots:
    void slotTabAdded(WebTab*);
    void slotTabDeleted(WebTab*);

private:
    QWidget *m_window;
    QWidget *m_tabBar;
    boost::unordered_set<QWidget*> m_locationBars;

    TabWatcher m_tabWatcher;
    Toolbar m_container;
    QPoint m_mousePos;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
