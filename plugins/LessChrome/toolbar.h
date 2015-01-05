#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <utility>
#include <vector>

#include <QtCore/QTimer>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class BrowserWindow;
class QWheelEvent;
class QBoxLayout;
class QStatusBar;

namespace lesschrome {

//TODO extract FloatingToolbar from Toolbar
class Toolbar : public QWidget
{
    Q_OBJECT
public:
    enum Position { Top, Bottom };

    Toolbar(BrowserWindow* const parent, const Position = Top);
    ~Toolbar();

    void capture(QWidget* const);

    void enter();
    void leave();
    bool isEntered() { return m_entered; }

public slots:
    void show();

protected:
    void updatePositionAndSize();
    BrowserWindow* window() { return m_window; }

private:
    // @override QWidget
    virtual void wheelEvent(QWheelEvent* const);

    void restore();

private:
    enum { showTimeout = 1000 };

    struct LayoutInfo {
        QBoxLayout *layout;
        int index;
        Qt::Alignment alignment;
        int stretch;
    };

    typedef std::pair<QWidget*, LayoutInfo> WidgetInfo;

    BrowserWindow* m_window;
    std::vector<WidgetInfo> m_widgets;
    QTimer m_timer;
    bool m_entered;
    Position m_position;
};

class StatusBar : public Toolbar
{
public:
    StatusBar(BrowserWindow* const window);
    ~StatusBar();

private:
    QStatusBar* m_statusBar;
    bool m_wasVisible;
};

} // namespace lesschrome

#endif // TOOLBAR_H
