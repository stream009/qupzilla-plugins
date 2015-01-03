#include "toolbar.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QWheelEvent>
#include <QtGui/QVBoxLayout>

namespace lesschrome {

Toolbar::
Toolbar(QWidget* const parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout)
{
    assert(parent);
    assert(m_layout);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);
    this->setAutoFillBackground(true);
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

void Toolbar::
wheelEvent(QWheelEvent* const ev)
{
    if (ev->orientation() == Qt::Vertical && ev->delta() > 0) {
        this->setVisible(false);
    }
}

} // namespace lesschrome
