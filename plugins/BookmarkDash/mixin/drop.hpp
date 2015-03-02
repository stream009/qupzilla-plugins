#ifndef QWIDGET_MIXIN_DROP_HPP
#define QWIDGET_MIXIN_DROP_HPP

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class QMimeData;
class QMouseEvent;
class QPaintEvent;
class QPoint;
class QWidget;

namespace mixin {

template<typename Base>
class Drop : public Base
{
    enum class State { None, Accept };
public:
    Drop(QWidget* const parent);

protected:
    // @override QWidget
    void dragEnterEvent(QDragEnterEvent* const) override;
    void dragMoveEvent(QDragMoveEvent* const) override;
    void dropEvent(QDropEvent* const) override;
    void mouseReleaseEvent(QMouseEvent* const) override;
    void paintEvent(QPaintEvent* const) override;

private:
    virtual bool canDrop(const QPoint &) { return true; }
    virtual bool canDrop(const QMimeData &) = 0;
    virtual void paintDropMarker(QPaintEvent &) {}
    virtual void onDrop(QDropEvent&) = 0;

private:
    State m_state = State::None;
};

} // namespace mixin

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QWidget>

namespace mixin {

template<typename Base>
inline Drop<Base>::
Drop(QWidget* const parent)
    : Base { parent }
{
    this->setAcceptDrops(true);
}

template<typename Base>
inline void Drop<Base>::
dragEnterEvent(QDragEnterEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    auto* const mimeData = event->mimeData();
    assert(mimeData);
    if (canDrop(*mimeData)) {
        event->accept();
    }
    else {
        qDebug() << "unsupported drag:" << event->mimeData()->formats();
    }
}

template<typename Base>
inline void Drop<Base>::
dragMoveEvent(QDragMoveEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    if (canDrop(event->pos())) {
        m_state = State::Accept;
        event->accept();
    }
    else {
        m_state = State::None;
        event->ignore();
    }
    this->update();
}

template<typename Base>
inline void Drop<Base>::
dropEvent(QDropEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    assert(event);
    event->acceptProposedAction();
    onDrop(*event);
    m_state = State::None;
}

template<typename Base>
inline void Drop<Base>::
mouseReleaseEvent(QMouseEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    Base::mouseReleaseEvent(event);
    m_state = State::None;
}

template<typename Base>
inline void Drop<Base>::
paintEvent(QPaintEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    assert(event);
    Base::paintEvent(event);

    if (m_state != State::Accept) return;

    paintDropMarker(*event);
}

} // namespace mixin

#endif // QWIDGET_MIXIN_DROP_HPP
