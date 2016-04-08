#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <boost/container/flat_map.hpp>

#include <QtCore/QTimer>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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

    void handleWebViewEvent(const QEvent &);

    void lock();
    void unlock();

public slots:
    // @override QWidget. throw()
    void show();
    void hide();

protected:
    explicit FloatingBar(BrowserWindow &parent, const Position = Top);

    void updatePositionAndSize();
    BrowserWindow &window() const { return m_window; }

private:
    // @override QWidget. Must be throw()
    virtual void wheelEvent(QWheelEvent* const);

private:
    BrowserWindow &m_window;
    Position m_position;
    bool m_locked;
    bool m_hideRequested;
};


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

    void handleLocationBarEvent(const QEvent &);
    void handleNavigationContainerEvent(const QEvent &);

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
    Q_OBJECT
public:
    explicit StatusBar(BrowserWindow &window);
    virtual ~StatusBar();

    // @overload FloatingBar
    void handleWindowEvent(const QEvent &);

private:
    void enter();
    void leave();

public slots:
    void slotSettingChanged(const QString &key);

private:
    QStatusBar* m_statusBar;
    bool m_wasVisible;
    bool m_entered;
    QTimer m_timer;
};

} // namespace lesschrome

#endif // TOOLBAR_H
