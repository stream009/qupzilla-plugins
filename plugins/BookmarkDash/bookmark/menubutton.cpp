#include "menubutton.h"

#include <QtGui/QWidget>

namespace bookmark_dash {

MenuButton::
MenuButton(QWidget* const parent/*= 0*/)
    : Base { parent }
{
    this->setObjectName("navigation-button-bookmarks-menu");
    this->setPopupMode(QToolButton::InstantPopup);
    this->setToolbarButtonLook(true);
    this->setToolTip(tr("Bookmarks Menu"));
    this->setAutoRaise(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setShowMenuInside(true);
}

} // namespace bookmark_dash
