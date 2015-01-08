#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <boost/container/flat_map.hpp>

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

    void mouseMove(const QPoint &);

public slots:
    void show();
    void slotSettingChanged(const QString &key);

protected:
    explicit FloatingBar(BrowserWindow &parent, const Position = Top);

    void enter();
    void leave();
    void updatePositionAndSize();
    BrowserWindow &window() const { return m_window; }

private:
    // @override QWidget
    virtual void wheelEvent(QWheelEvent* const);

private:
    BrowserWindow &m_window;
    QTimer m_timer;
    bool m_entered;
    Position m_position;
};


//TODO stop using timer
class Toolbar : public FloatingBar
{
private:
    struct LayoutInfo {
        QBoxLayout *layout;
        int index;
        Qt::Alignment alignment;
        int stretch;
    };

    typedef boost::container::flat_map<QWidget*, LayoutInfo> WidgetMap;
    typedef WidgetMap::value_type WidgetInfo;

public:
    explicit Toolbar(BrowserWindow &parent);
    virtual ~Toolbar();

    bool empty() const { return this->layout()->count() == 0; }

    void capture(QWidget&);
    void restore(QWidget&);

private:
    void restoreAll();
    void saveOriginalState();

private:
    WidgetMap m_originalStates;
};


class StatusBar : public FloatingBar
{
public:
    explicit StatusBar(BrowserWindow &window);
    virtual ~StatusBar();

private:
    QStatusBar* m_statusBar;
    bool m_wasVisible;
};

} // namespace lesschrome

#endif // TOOLBAR_H
