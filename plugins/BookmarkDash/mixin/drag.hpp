#ifndef QWIDGET_MIXIN_DRAG_HPP
#define QWIDGET_MIXIN_DRAG_HPP

#include <QtCore/QPoint>

class QDrag;
class QMouseEvent;
class QWidget;

namespace mixin {

template<typename Base>
class Drag : public Base
{
    enum class State { None, Start, InProgress };
public:
    Drag(QWidget* const parent);

protected:
    // @override QWidget
    void mousePressEvent(QMouseEvent* const) override;
    void mouseReleaseEvent(QMouseEvent* const) override;
    void mouseMoveEvent(QMouseEvent* const) override;

private:
    virtual bool canDrag(const QPoint &) { return true; }
    virtual void prepareDrag(QDrag &, const QPoint &) = 0;
    virtual void startDrag(const QPoint &) {}
    virtual void onDragFinished(const QDrag &, const Qt::DropAction) {}

private:
    State m_state = State::None;
    QPoint m_dragStartPosition;
};

} // namespace mixin

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>

namespace mixin {

template<typename Base>
inline Drag<Base>::
Drag(QWidget* const parent)
    : Base { parent }
{}

template<typename Base>
inline void Drag<Base>::
mousePressEvent(QMouseEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    const auto &pos = event->pos();
    if (event->button() == Qt::LeftButton && canDrag(pos)) {
        m_dragStartPosition = pos;
        m_state = State::Start;
    }

    Base::mousePressEvent(event);
}

template<typename Base>
inline void Drag<Base>::
mouseReleaseEvent(QMouseEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    Base::mouseReleaseEvent(event);
    m_state = State::None;
}

template<typename Base>
inline void Drag<Base>::
mouseMoveEvent(QMouseEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event << event->buttons();
    Base::mouseMoveEvent(event);

    if (m_state != State::Start) return;
    if (event->buttons() != Qt::LeftButton) return;

    const auto &pos = event->pos();
    const auto distance =
                (pos - m_dragStartPosition).manhattanLength();
    if (distance < QApplication::startDragDistance()) return;

    if (!canDrag(pos)) return; //TODO should it be the m_dragStartPosition?

    // Qt will take take of the deletion of drag and data.
    auto* const drag = new QDrag { this };
    assert(drag);
    QMimeData *data = new QMimeData;
    assert(data);
    drag->setMimeData(data);

    prepareDrag(*drag, pos);

    m_state = State::InProgress;
    startDrag(pos);
    const auto dropAction = drag->exec();
    m_state = State::None;

    onDragFinished(*drag, dropAction);

    this->update(); // clean up widget
}

} // namespace mixin

#endif // QWIDGET_MIXIN_DRAG_HPP
