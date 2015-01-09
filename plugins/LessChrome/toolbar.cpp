#include "toolbar.h"

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
      m_position(position)
{
    QVBoxLayout *layout = new QVBoxLayout(this); // this take ownership
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setAutoFillBackground(true);
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
    saveOriginalState();
}

Toolbar::
~Toolbar()
{
    //qDebug() << __FUNCTION__;
    restoreAll();
}

void Toolbar::
capture(QWidget& widget)
{
    assert(this->layout()->indexOf(&widget) == -1); //TODO better

    this->layout()->addWidget(&widget);
    updatePositionAndSize();
}

void Toolbar::
restore(QWidget &widget)
{
    const LayoutInfo &layoutInfo = m_originalStates.at(&widget); //TODO out_of_range

    QBoxLayout* layout = layoutInfo.layout;
    if (layout->parentWidget() == NULL || !layout->isEnabled()) {
        //TODO better
        qDebug() << "layout is invalid:" << &widget << layout;
    }

    //qDebug() << __FUNCTION__ << &widget << layoutInfo.index;
    layout->insertWidget(layoutInfo.index,
                &widget, layoutInfo.stretch, layoutInfo.alignment);

    updatePositionAndSize();
}

void Toolbar::
restoreAll()
{
    QLayout* const layout = this->layout();
    while (layout->count()) {
        QLayoutItem* const item = layout->itemAt(0);
        assert(item);

        QWidget* const widget = item->widget();
        assert(widget);

        restore(*widget);
    }
}

void Toolbar::
saveOriginalState()
{
    QWidget* const navigationBar = this->window().navigationBar();
    assert(navigationBar); //TODO better

    QWidget* const container = navigationBar->parentWidget();
    assert(container); //TODO better

    QBoxLayout* const layout = qobject_cast<QBoxLayout*>(container->layout());
    assert(layout);
    assert(layout->count());
    for (size_t index = 0u, len = layout->count(); index < len; ++index) {
        QLayoutItem* const item = layout->itemAt(index);
        assert(item);

        QWidget* const widget = item->widget();
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
    assert(m_statusBar);
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
