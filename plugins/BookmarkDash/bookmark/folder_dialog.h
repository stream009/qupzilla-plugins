#ifndef BOOKMARK_DASH_FOLDER_DIALOG_H
#define BOOKMARK_DASH_FOLDER_DIALOG_H

#include "ui_folder_dialog.h"

#include <QtWidgets/QDialog>

class BookmarkItem;

namespace bookmark_dash {

class FolderDialog : public QDialog
{
    Q_OBJECT
    using Base = QDialog;
public:
    FolderDialog(BookmarkItem &, QWidget &parent);

private Q_SLOTS:
    void commitChange() const;

private:
    Ui::FolderDialog m_ui;
    BookmarkItem &m_item;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_FOLDER_DIALOG_H
