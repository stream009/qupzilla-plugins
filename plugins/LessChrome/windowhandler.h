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
//TODO once hide toolbar with wheel, make sure it doesnt come back
//TODO status bar support
class WindowHandler : public QObject
{
    Q_OBJECT
public:
    WindowHandler(QWidget* const window);
    ~WindowHandler();

    void mouseMove(QMouseEvent* const event);

private:
    // @overrides QObject
    virtual bool eventFilter(QObject* const, QEvent* const event);

    void captureWidgets();
    void resizeToolBars();

private slots:
    void showToolbar();
    void hideToolbar();

private:
    enum { showTimeout = 1000 };

    QWidget *m_window;
    QWidget *m_navigationContainer;
    QWidget *m_tabBar;
    QWidget *m_webView;

    Toolbar *m_container;
    QTimer m_timer;
};

} // namespace lesschrome

#endif // WINDOWHANDLER_H
