#include "toolbar.h"

#include "browserwindow.h"
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
      m_entered(false),
      m_position(position)
{
    QVBoxLayout *layout = new QVBoxLayout(this); // this take ownership
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setAutoFillBackground(true);

    const Settings &settings = Plugin::settings();
    m_timer.setSingleShot(true);
    m_timer.setInterval(settings.waitTimer);

    this->connect(&m_timer, SIGNAL(timeout()),
                  this,     SLOT(show()));
    this->connect(&settings, SIGNAL(change(QString)),
                  this,      SLOT(slotSettingChanged(const QString&)));
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
slotSettingChanged(const QString &key)
{
    if (key == Settings::keyWaitTimer) {
        m_timer.setInterval(Plugin::settings().waitTimer);
    }
}

void FloatingBar::
enter()
{
    //qDebug() << __FUNCTION__;
    m_entered = true;

    if (!m_timer.isActive()) {
        m_timer.start();
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
updatePositionAndSize()
{
    QWidget* const webView = m_window.weView();
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
Toolbar(BrowserWindow &parent)
    : FloatingBar(parent)
{
    //qDebug() << __FUNCTION__;
}

Toolbar::
~Toolbar()
{
    //qDebug() << __FUNCTION__;
    foreach (const WidgetInfo &item, m_widgets) {
        restore(item);
    }

    assert(m_widgets.empty());
}

void Toolbar::
capture(QWidget& widget)
{
    assert(!m_widgets.count(&widget)); //TODO better

    QBoxLayout* const layout =
        qobject_cast<QBoxLayout*>(widget.parentWidget()->layout());
    if (layout == NULL) {
        qDebug() << "layout";
        throw "layout"; //TODO
    }

    const int index = layout->indexOf(&widget);
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

    m_widgets.emplace(&widget, info);

    this->layout()->addWidget(&widget);
    updatePositionAndSize();
}

void Toolbar::
restore(QWidget &widget)
{
    const WidgetMap::const_iterator it = m_widgets.find(&widget);
    assert(it != m_widgets.end()); //TODO better

    restore(*it);
}

void Toolbar::
restore(const WidgetInfo &info)
{
    QWidget* const widget = info.first;
    const LayoutInfo &layoutInfo = info.second;

    QBoxLayout* layout = layoutInfo.layout;
    if (layout->parentWidget() == NULL || !layout->isEnabled()) {
        //TODO better
        qDebug() << "layout is invalid:" << widget << layout;
    }

    //qDebug() << __FUNCTION__ << widget << layoutInfo.index;
    layout->insertWidget(layoutInfo.index,
                widget, layoutInfo.stretch, layoutInfo.alignment);

    m_widgets.erase(widget);
    updatePositionAndSize();
}


StatusBar::
StatusBar(BrowserWindow &window)
    : FloatingBar(window, Toolbar::Bottom),
      m_statusBar(NULL),
      m_wasVisible(true)
{
    m_statusBar = window.statusBar();
    assert(m_statusBar);
    m_wasVisible = m_statusBar->isVisible();

    this->layout()->addWidget(m_statusBar);

    window.setStatusBar(NULL);
    window.statusBar()->hide();

    m_statusBar->adjustSize();
    m_statusBar->show();

    updatePositionAndSize();

    assert(m_statusBar);
}

StatusBar::
~StatusBar()
{
    this->window().setStatusBar(m_statusBar);
    m_statusBar->setVisible(m_wasVisible);
}

} // namespace lesschrome
