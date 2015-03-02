#ifndef BOOKMARK_DASH_MENUDASH_H
#define BOOKMARK_DASH_MENUDASH_H

#include "mixin/context_menu.h"
#include "mixin/drag.hpp"
//#include "mixin/drop.hpp"

#include <bookmarksmenu.h>

class QDrag;
class QPoint;

namespace bookmark_dash {

using MenuDashBase = mixin::Drag<
                     mixin::ContextMenu<BookmarksMenu>>;

class MenuDash : public MenuDashBase
{
    using Base = MenuDashBase;
public:
    MenuDash(QWidget*const parent);

private:
    // @override Drag
    bool canDrag(const QPoint &) override;
    void prepareDrag(QDrag &, const QPoint &) override;
    void onDragFinished(const QDrag &, const Qt::DropAction) override;

private:
    bool isFixed(QAction &) const;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_MENUDASH_H
