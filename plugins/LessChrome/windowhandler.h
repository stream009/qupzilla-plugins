#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <cassert>
#include <utility>
#include <vector>

#include <QtCore/QTimer>

class QWidget;
class QMouseEvent;

namespace lesschrome {

class Toolbar;

//TODO save initial visibility of target widgets
//TODO show when location bar got focus
//TODO detect visibility change of menubar, toolbar
//TODO status bar support
class WindowHandler : public QObject
{
public:
    WindowHandler(QWidget* const window);
    ~WindowHandler();

    void mouseMove(QMouseEvent* const event);

private:
    // @override QObject
    virtual bool eventFilter(QObject* const, QEvent* const event);

    void captureWidgets();
    void resizeToolBars();

private:
    QWidget *m_window;
    QWidget *m_navigationContainer;
    QWidget *m_tabBar;
    QWidget *m_webView;

    Toolbar *m_container;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
