#include "view.h"

#include "bookmark_dialog.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtGui/QDrag>
#include <QtGui/QFontMetrics>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>

#include <bookmarkitem.h>
#include <bookmarksmodel.h>
#include <bookmarkstools.h>
#include <browserwindow.h>

namespace bookmark_dash {

template<typename BaseT>
View<BaseT>::
View(BrowserWindow &window, QWidget*const parent)
    : Base { parent },
      m_window { window },
      m_urlDropHandler { *this }
{
    auto* const contextMenu =
                new bookmark_dash::ContextMenu { *this, nullptr };
    assert(contextMenu);
    this->setContextMenu(*contextMenu);
}

template<typename BaseT>
inline void View<BaseT>::
openFolderInTabs(BookmarkItem &item)
{
    BookmarksTools::openFolderInTabs(&m_window, &item);
}

template<typename BaseT>
inline BookmarkItem *View<BaseT>::
currentItem() const
{
    if (this->currentIndex().isValid()) {
        return &item(this->currentIndex());
    }
    else {
        return nullptr;
    }
}

template<typename BaseT>
inline BookmarkItem &View<BaseT>::
parentItem() const
{
    auto &rootIndex = this->rootIndex();
    assert(rootIndex.isValid());

    return item(rootIndex);
}

template<typename BaseT>
inline QAction &View<BaseT>::
createItemAction(const QModelIndex &index)
{
    assert(index.isValid());
    assert(index.model() == &this->model());
    assert(index.parent() == this->rootIndex());

    auto &item = this->item(index);

    auto &result = Base::createItemAction(index);
    if (item.isSeparator()) {
        result.setSeparator(true);
    }

    const auto &title = item.title();
    const QFontMetrics fm { result.font() };
    const auto &elidedText = fm.elidedText(title, Qt::ElideRight, 250);
    result.setText(elidedText);

    const auto &icon = item.icon();
    result.setIcon(icon);

    return result;
}

template<typename BaseT>
inline bool View<BaseT>::
isMenuItem(const QModelIndex &index)
{
    assert(index.isValid());

    auto &item = this->item(index);
    return item.type() == BookmarkItem::Folder;
}

template<typename BaseT>
inline void View<BaseT>::
prepareDrag(QDrag &drag, const QPoint &pos)
{
    Base::prepareDrag(drag, pos);

    auto* const action = this->actionAt(pos);
    assert(action);

    const auto &index = this->index(*action);
    assert(index.isValid());

    auto &item = this->item(index);

    auto* const mimeData = drag.mimeData();
    assert(mimeData);

    QList<QUrl> urls;
    urls << item.url();
    mimeData->setUrls(urls);

    if (!item.title().isEmpty()) {
        mimeData->setText(item.title());
    }
}

template<typename BaseT>
inline bool View<BaseT>::
canDrop(const QDragMoveEvent &event)
{
    if (Base::canDrop(event)) return true;

    auto* const mimeData = event.mimeData();
    assert(mimeData);

    auto* const action = this->actionAt(event.pos());
    if (!action) return false;
    const auto &index = this->index(*action);
    if (!index.isValid()) return false;

    return mimeData->hasUrls();
}

template<typename BaseT>
inline bool View<BaseT>::
canDrop(const QMimeData &mimeData)
{
    if (Base::canDrop(mimeData)) return true;

    return mimeData.hasUrls();
}

template<typename BaseT>
void View<BaseT>::
onDrop(QDropEvent &event)
{
    auto* const mimeData = event.mimeData();
    assert(mimeData);

    if (Base::canDrop(*mimeData)) return Base::onDrop(event);

    assert(mimeData->hasUrls());
    const auto &urls = mimeData->urls();
    assert(!urls.empty());
    const auto &url = urls.front();

    auto* const action = this->actionAt(event.pos());
    assert(action);
    const auto &index = this->index(*action);

    // This function call will be back asynchronously.
    // We have to do this indirection because until drag & drop
    // operation has finished, mouse cursor won't be back to
    // the nomal state.
    m_urlDropHandler.drop(mimeData->text(), url, index);
}

template<typename BaseT>
inline void View<BaseT>::
onUrlDropped(const QString &title, const QUrl &url, const QModelIndex &before)
{
    assert(url.isValid());
    // before can be invalid.

    auto* const newItem = new BookmarkItem { BookmarkItem::Url };
    assert(newItem);
    newItem->setUrl(url);
    if (!title.isEmpty()) {
        newItem->setTitle(title);
    }
    BookmarkDialog dialog { *newItem, m_window };
    dialog.setWindowTitle(QObject::tr("New Bookmark"));

    const auto result = dialog.exec();
    if (result == QDialog::Accepted) {
        assert(mApp);
        auto* const bookmarks = mApp->bookmarks();
        assert(bookmarks);

        auto &parent = this->item(this->rootIndex());

        if (before.isValid()) {
            assert(before.parent() == this->rootIndex());
            // BookmarkModel will take ownership of the item.
            bookmarks->insertBookmark(&parent, before.row(), newItem);
        }
        else {
            // BookmarkModel will take ownership of the item.
            bookmarks->addBookmark(&parent, newItem);
        }
    }
    else {
        delete newItem;
    }
}

template<typename BaseT>
inline BookmarksModel &View<BaseT>::
model() const
{
    return dynamic_cast<BookmarksModel&>(Base::model());
}

template<typename BaseT>
inline BookmarkItem &View<BaseT>::
item(const QModelIndex &index) const
{
    assert(index.isValid());

    auto* const result = model().item(index);
    assert(result);

    return *result;
}

} // namespace bookmark_dash

