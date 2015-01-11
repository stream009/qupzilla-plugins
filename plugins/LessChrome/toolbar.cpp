#include "toolbar.h"

#include "error.h"

#include "browserwindow.h"
#include "navigationbar.h"
#include "plugin.h"
#include "settings.h"
#include "tabbedwebview.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWheelEvent>

namespace lesschrome {

FloatingBar::
FloatingBar(BrowserWindow &window, const Position position)
    : QWidget(&window),
      m_window(window),
      m_position(position),
      m_locked(false),
      m_hideRequested(false)
{
    QVBoxLayout *layout = new QVBoxLayout(this); // this takes ownership
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setAutoFillBackground(true);
}

void FloatingBar::
handleWebViewEvent(const QEvent &event)
{
    switch (event.type()) {
    case QEvent::Enter:
    case QEvent::FocusIn:
        // Delay hiding so that signals will be processed first.
        // This is required by lock/unlock mechanism.
        QTimer::singleShot(0, this, SLOT(hide()));
        break;
    default:
        break;
    }
}

void FloatingBar::
lock()
{
    if (m_locked) return;

    m_locked = true;
    if (!this->isVisible()) {
        m_hideRequested = true;
        show();
    }
}

void FloatingBar::
unlock()
{
    if (!m_locked) return;

    m_locked = false;
    if (m_hideRequested) {
        m_hideRequested = false;
        hide();
    }
}

void FloatingBar::
show()
{
    //qDebug() << __FUNCTION__;
    try {
        QWidget::show();
        updatePositionAndSize();
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void FloatingBar::
hide()
{
    //qDebug() << __FUNCTION__ << this << m_locked;
    try {
        if (!m_locked) {
            QWidget::hide();
        }
        else {
            m_hideRequested = true;
        }
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void FloatingBar::
updatePositionAndSize()
{
    QWidget* const webView = m_window.weView();
    if (!webView) {
        throw RuntimeError("Fail to obtain WebView.");
    }

    const QRect &childrenRect = this->childrenRect();
    QPoint topLeft;
    switch (m_position) {
    case Top:
        topLeft = webView->mapTo(this->parentWidget(), QPoint(0, 0));
        break;
    case Bottom:
        topLeft = webView->mapTo(this->parentWidget(),
            QPoint(0, webView->height() - childrenRect.height()));
        break;
    default:
        assert(false && "Unknown Position value.");
        break;
    }

    this->setGeometry(
        topLeft.x(), topLeft.y(),
        webView->width(), childrenRect.height());
}

void FloatingBar::
wheelEvent(QWheelEvent* const) // throw()
{
    try {
        hide();
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}


Toolbar::
Toolbar(BrowserWindow &parent)
    : FloatingBar(parent)
{
    //qDebug() << __FUNCTION__;
    saveOriginalState();
}

Toolbar::
~Toolbar()
{
    //qDebug() << __FUNCTION__;
    restoreAll();
}

void Toolbar::
handleLocationBarEvent(const QEvent &event)
{
    if (event.type() == QEvent::FocusIn) {
        show();
    }
}

void Toolbar::
handleNavigationContainerEvent(const QEvent &event)
{
    if (event.type() == QEvent::Enter) {
        show();
    }
}

void Toolbar::
capture(QWidget& widget)
{
    QLayout* const layout = this->layout();
    assert(layout);
    if (layout->indexOf(&widget) != -1) {
        throw InternalError("Tried to capture widget twice.");
    }

    layout->addWidget(&widget);
    updatePositionAndSize();
}

void Toolbar::
restore(QWidget &widget)
try {
    const LayoutInfo &layoutInfo = m_originalStates.at(&widget);

    QBoxLayout* layout = layoutInfo.layout;
    if (layout->parentWidget() == NULL || !layout->isEnabled()) {
        throw InternalError("Invalid layout object");
    }

    //qDebug() << __FUNCTION__ << &widget << layoutInfo.index;
    layout->insertWidget(layoutInfo.index,
                &widget, layoutInfo.stretch, layoutInfo.alignment);

    updatePositionAndSize();
}
catch (const std::out_of_range &e) {
    throw InternalError("Try to restore widget that hadn't captured.");
}

void Toolbar::
restoreAll()
{
    QLayout* const layout = this->layout();
    assert(layout);
    while (layout->count()) {
        QLayoutItem* const item = layout->itemAt(0);
        if (!item) {
            throw RuntimeError("Invalid layout item");
        }

        QWidget* const widget = item->widget();
        if (!widget) {
            throw RuntimeError("Got invalid widget from layout.");
        }

        restore(*widget);
    }
}

void Toolbar::
saveOriginalState()
{
    QWidget* const navigationBar = this->window().navigationBar();
    if (!navigationBar) {
        throw RuntimeError("Fail to obtain navigation bar.");
    }

    QWidget* const container = navigationBar->parentWidget();
    if (!container) {
        throw RuntimeError("Fail to obtain navigation container");
    }

    QBoxLayout* const layout = qobject_cast<QBoxLayout*>(container->layout());
    if (!layout) {
        throw RuntimeError(
            "Fail to obtain QBoxLayout from navigation container.");
    }

    for (size_t index = 0u, len = layout->count(); index < len; ++index) {
        QLayoutItem* const item = layout->itemAt(index);
        if (!item) {
            throw RuntimeError(
                "Got invalid layout item from navigation container.");
        }

        QWidget* const widget = item->widget();
        if (!widget) {
            throw RuntimeError("Layout item doesn't have widget to layout.");
        }
        const Qt::Alignment alignment = item->alignment();
        const int stretch = layout->stretch(index);

        assert(widget);
        assert(alignment >= 0);
        assert(stretch >= 0);

        LayoutInfo info = { layout, index, alignment, stretch };
        m_originalStates.emplace(widget, info);
    }
}


StatusBar::
StatusBar(BrowserWindow &window)
    : FloatingBar(window, Toolbar::Bottom),
      m_statusBar(NULL),
      m_wasVisible(true),
      m_entered(false)
{
    m_statusBar = window.statusBar();
    // Empty status bar will be returned if status bar hasn't set.
    m_wasVisible = m_statusBar->isVisible();

    this->layout()->addWidget(m_statusBar);

    window.setStatusBar(NULL);
    window.statusBar()->hide();

    m_statusBar->adjustSize();
    m_statusBar->show();

    updatePositionAndSize();

    const Settings &settings = Plugin::settings();
    m_timer.setSingleShot(true);
    m_timer.setInterval(settings.waitTimer);

    this->connect(&m_timer, SIGNAL(timeout()),
                  this,     SLOT(show()));
    this->connect(&settings, SIGNAL(change(QString)),
                  this,      SLOT(slotSettingChanged(const QString&)));

    assert(m_statusBar);
}

StatusBar::
~StatusBar()
{
    this->window().setStatusBar(m_statusBar);
    m_statusBar->setVisible(m_wasVisible);
}

void StatusBar::
handleWindowEvent(const QEvent &event)
{
    if (event.type() == QEvent::HoverMove) {
        const QHoverEvent &ev = static_cast<const QHoverEvent&>(event);
        if (ev.pos() == ev.oldPos()) return; // Somethis it happens.

        if (this->geometry().contains(ev.pos())) {
            enter();
        }
        else {
            leave();
        }
    }
}

void StatusBar::
enter()
{
    //qDebug() << __FUNCTION__;
    if (m_entered) return;
    m_entered = true;

    if (!m_timer.isActive()) {
        m_timer.start();
    }
}

void StatusBar::
leave()
{
    //qDebug() << __FUNCTION__;
    if (!m_entered) return;
    m_entered = false;

    if (m_timer.isActive()) {
        m_timer.stop();
    }
    hide();
}

void StatusBar::
slotSettingChanged(const QString &key)
{
    if (key == Settings::keyWaitTimer) {
        m_timer.setInterval(Plugin::settings().waitTimer);
    }
}

} // namespace lesschrome
