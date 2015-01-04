#include "toolbar.h"

#include "browserwindow.h"
#include "tabbedwebview.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtGui/QWheelEvent>
#include <QtGui/QVBoxLayout>

namespace lesschrome {

Toolbar::
Toolbar(BrowserWindow* const window)
    : QWidget(window),
      m_window(window),
      m_layout(new QVBoxLayout), //TODO remove new
      m_entered(false)
{
    assert(m_window);
    assert(m_layout);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);
    this->setAutoFillBackground(true);

    m_timer.setSingleShot(true);
    this->connect(&m_timer, SIGNAL(timeout()),
                  this,     SLOT(show()));
}

Toolbar::
~Toolbar()
{
    restore();
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

    m_layout->addWidget(widget);
    this->adjustSize();
}

void Toolbar::
enter()
{
    //qDebug() << __FUNCTION__;
    m_entered = true;

    if (!m_timer.isActive()) {
        m_timer.start(Toolbar::showTimeout);
    }
}

void Toolbar::
leave()
{
    //qDebug() << __FUNCTION__;

    m_entered = false;

    if (m_timer.isActive()) {
        m_timer.stop();
    }
    hide();
}

void Toolbar::
show()
{
    //qDebug() << __FUNCTION__;
    assert(m_window);

    this->setVisible(true);

    updatePositionAndSize();
}

void Toolbar::
hide()
{
    //qDebug() << __FUNCTION__;
    this->setVisible(false);
}

void Toolbar::
updatePositionAndSize()
{
    QWidget* const webView = m_window->weView();
    assert(webView);

    const QPoint &topLeft =
        webView->mapTo(this->parentWidget(), QPoint(0, 0));
    this->setGeometry(
        topLeft.x(), topLeft.y(),
        webView->width(), this->childrenRect().height());
}

void Toolbar::
wheelEvent(QWheelEvent* const)
{
    hide();
}

void Toolbar::
restore() // needs to be noexcept
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

} // namespace lesschrome
