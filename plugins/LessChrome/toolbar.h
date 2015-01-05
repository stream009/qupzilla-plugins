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

class FloatingBar : public QWidget
{
    Q_OBJECT
public:
    enum Position { Top, Bottom };

    void enter();
    void leave();
    bool isEntered() { return m_entered; }

public slots:
    void show();

protected:
    FloatingBar(BrowserWindow* const parent, const Position = Top);

    void updatePositionAndSize();
    BrowserWindow* window() { return m_window; }

private:
    // @override QWidget
    virtual void wheelEvent(QWheelEvent* const);

private:
    enum { showTimeout = 1000 };

    BrowserWindow* m_window;
    QTimer m_timer;
    bool m_entered;
    Position m_position;
};

class Toolbar : public FloatingBar
{
public:
    explicit Toolbar(BrowserWindow* const parent);
    virtual ~Toolbar();

    void capture(QWidget* const);

private:
    struct LayoutInfo {
        QBoxLayout *layout;
        int index;
        Qt::Alignment alignment;
        int stretch;
    };

    typedef std::pair<QWidget*, LayoutInfo> WidgetInfo;

    std::vector<WidgetInfo> m_widgets;
};

class StatusBar : public FloatingBar
{
public:
    explicit StatusBar(BrowserWindow* const window);
    virtual ~StatusBar();

private:
    QStatusBar* m_statusBar;
    bool m_wasVisible;
};

} // namespace lesschrome

#endif // TOOLBAR_H
