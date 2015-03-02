#include "menudash.h"

#include "action_mime_data.h"

#include <QtCore/QPoint>
#include <QtGui/QDrag>
#include <QtGui/QMenu>

namespace bookmark_dash {

MenuDash::
MenuDash(QWidget*const parent)
    : Base { parent }
{
    this->addContextAction(new QAction { tr("foo"), this });
}

bool MenuDash::
canDrag(const QPoint &pos)
{
    //qDebug() << __func__;
    auto* const target = this->actionAt(pos);
    return target && !isFixed(*target);
}

void MenuDash::
prepareDrag(QDrag &drag, const QPoint &pos)
{
    //qDebug() << __func__;
    auto* const action = this->actionAt(pos);
    assert(action);

    auto* const data = new ActionMimeData(*action);
    assert(data);
    data->setText(action->text());
    drag.setMimeData(data);

    const auto &rect = this->actionGeometry(action);
    drag.setPixmap(QPixmap::grabWidget(this, rect));
}

void MenuDash::
onDragFinished(const QDrag &drag, const Qt::DropAction dropAction)
{
    if (dropAction == Qt::MoveAction && drag.target() != this) {
        auto* const data = dynamic_cast<ActionMimeData*>(drag.mimeData());
        assert(data);
        this->removeAction(&data->action());
    }
}

bool MenuDash::
isFixed(QAction &target) const
{
    return this->actions().indexOf(&target) <= 5;
}

} // namespace bookmark_dash
