#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <utility>
#include <vector>

#include <QtCore/QTimer>
#include <QtGui/QWidget>

class QWheelEvent;
class QBoxLayout;
class QVBoxLayout;

namespace lesschrome {

class Toolbar : public QWidget
{
    Q_OBJECT
public:
    Toolbar(QWidget* const parent);
    ~Toolbar();

    void capture(QWidget* const);

    void enter();
    void leave();
    bool isEntered() { return m_entered; }

public slots:
    void show();
    void hide();

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

    QVBoxLayout *m_layout;
    std::vector<WidgetInfo> m_widgets;
    QTimer m_timer;
    bool m_entered;
};

} // namespace lesschrome

#endif // TOOLBAR_H
