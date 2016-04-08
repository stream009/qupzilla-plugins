#include "folder_dialog.h"

#include <bookmarkitem.h>

#include <cassert>

#include <QtWidgets/QWidget>

namespace bookmark_dash {

FolderDialog::
FolderDialog(BookmarkItem &item, QWidget &parent)
    : Base { &parent },
      m_item { item }
{
    assert(item.type() == BookmarkItem::Folder);

    m_ui.setupUi(this);
    assert(m_ui.name);
    assert(m_ui.description);

    m_ui.name->setText(item.title());
    m_ui.description->setPlainText(item.description());

    this->connect(this, SIGNAL(accepted()),
                  this,   SLOT(commitChange()));
}

void FolderDialog::
commitChange() const
{
    m_item.setTitle(m_ui.name->text());
    m_item.setDescription(m_ui.description->toPlainText());
}

} // namespace bookmark_dash
