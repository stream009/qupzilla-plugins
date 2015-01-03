#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <utility>
#include <vector>

#include <QtGui/QWidget>

class QWheelEvent;
class QBoxLayout;
class QVBoxLayout;

namespace lesschrome {

class Toolbar : public QWidget
{
public:
    Toolbar(QWidget* const parent);
    ~Toolbar();

    void capture(QWidget* const);

private:
    void restore(); // needs to be noexcept

    // @override QWidget
    virtual void wheelEvent(QWheelEvent* const);

private:
    struct LayoutInfo {
        QBoxLayout *layout;
        int index;
        Qt::Alignment alignment;
        int stretch;
    };
    typedef std::pair<QWidget*, LayoutInfo> WidgetInfo;

    QVBoxLayout *m_layout;
    std::vector<WidgetInfo> m_widgets;
};

} // namespace lesschrome

#endif // TOOLBAR_H
