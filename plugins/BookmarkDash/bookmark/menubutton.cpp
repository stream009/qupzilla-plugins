#include "menubutton.h"

#include <algorithm>

#include <QtCore/QMimeData>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QWidget>

#include <bookmarks.h>
#include <bookmarksmodel.h>
#include <mainapplication.h>

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
    this->setAcceptDrops(true);
}

void MenuButton::
dragEnterEvent(QDragEnterEvent* const event)
{
    assert(event);
    auto* const mimeData = event->mimeData();
    assert(mimeData);

    if (isSupported(*mimeData)) {
        // Dirty hack though, since ToolButton hides QToolButton::showMenu()
        QMetaObject::invokeMethod(this, "showMenu");
        event->accept();
    }
}

bool MenuButton::
isSupported(const QMimeData &mimeData) const
{
    if (mimeData.hasUrls()) return true;

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);
    auto* const model = bookmarks->model();
    assert(model);

    const auto &mimeTypes = model->mimeTypes();
    return std::any_of(mimeTypes.begin(), mimeTypes.end(),
        [&mimeData] (const QString &mimeType) {
            return mimeData.hasFormat(mimeType);
        }
    );
}

} // namespace bookmark_dash
