#include "toolbar.h"

#include "browserwindow.h"
#include "tabbedwebview.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWheelEvent>

namespace lesschrome {

FloatingBar::
FloatingBar(BrowserWindow* const window, const Position position)
    : QWidget(window),
      m_window(window),
      m_entered(false),
      m_position(position)
{
    assert(m_window);

    QVBoxLayout *layout = new QVBoxLayout(this); // this take ownership
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setAutoFillBackground(true);

    m_timer.setSingleShot(true);
    this->connect(&m_timer, SIGNAL(timeout()),
                  this,     SLOT(show()));
}

void FloatingBar::
enter()
{
    //qDebug() << __FUNCTION__;
    m_entered = true;

    if (!m_timer.isActive()) {
        m_timer.start(Toolbar::showTimeout);
    }
}

void FloatingBar::
leave()
{
    //qDebug() << __FUNCTION__;
    m_entered = false;

    if (m_timer.isActive()) {
        m_timer.stop();
    }
    hide();
}

void FloatingBar::
mouseMove(const QPoint &pos)
{
    if (this->geometry().contains(pos)) {
        if (!m_entered) {
            enter();
        }
    }
    else {
        if (m_entered) {
            leave();
        }
        else if (this->isVisible()){
            this->hide();
        }
    }
}

void FloatingBar::
show()
{
    //qDebug() << __FUNCTION__;
    QWidget::show();
    updatePositionAndSize();
}

void FloatingBar::
updatePositionAndSize()
{
    assert(m_window);
    QWidget* const webView = m_window->weView();
    assert(webView);

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
        //TODO error handling
        break;
    }

    this->setGeometry(
        topLeft.x(), topLeft.y(),
        webView->width(), childrenRect.height());
}

void FloatingBar::
wheelEvent(QWheelEvent* const)
{
    hide();
}

Toolbar::
Toolbar(BrowserWindow* const parent)
    : FloatingBar(parent)
{}

Toolbar::
~Toolbar()
{
    foreach (const WidgetInfo &info, m_widgets) {
        QWidget* const widget = info.first;
        const LayoutInfo &layoutInfo = info.second;

        QBoxLayout* layout = layoutInfo.layout;
        if (layout->parentWidget() == NULL ||
            !layout->isEnabled())
        {
            //TODO do better
            qDebug() << "layout is invalid:" << widget << layout;
        }

        layout->insertWidget(layoutInfo.index,
                    widget, layoutInfo.stretch, layoutInfo.alignment);
    }
    m_widgets.clear();

    assert(m_widgets.empty());
}

void Toolbar::
capture(QWidget* const widget)
{
    assert(widget);

    QBoxLayout* const layout =
        qobject_cast<QBoxLayout*>(widget->parentWidget()->layout());
    if (layout == NULL) {
        qDebug() << "layout";
        throw "layout"; //TODO
    }

    const int index = layout->indexOf(widget);
    if (index == -1) {
        qDebug() << "index";
        throw "index"; //TODO
    }

    const Qt::Alignment alignment = layout->itemAt(index)->alignment();
    const int stretch = layout->stretch(index);

    assert(layout);
    assert(index >= 0);
    assert(alignment >= 0);
    assert(stretch >= 0);

    LayoutInfo info = { layout, index, alignment, stretch };

    m_widgets.push_back(std::make_pair(widget, info));

    this->layout()->addWidget(widget);
    updatePositionAndSize();
}

StatusBar::
StatusBar(BrowserWindow* const window)
    : FloatingBar(window, Toolbar::Bottom),
      m_statusBar(NULL),
      m_wasVisible(true)
{
    assert(window);

    m_statusBar = window->statusBar();
    assert(m_statusBar);
    m_wasVisible = m_statusBar->isVisible();

    this->layout()->addWidget(m_statusBar);

    window->setStatusBar(NULL);
    window->statusBar()->hide();

    m_statusBar->adjustSize();
    m_statusBar->show();

    updatePositionAndSize();

    assert(m_statusBar);
}

StatusBar::
~StatusBar()
{
    this->window()->setStatusBar(m_statusBar);
    m_statusBar->setVisible(m_wasVisible);
}

} // namespace lesschrome
