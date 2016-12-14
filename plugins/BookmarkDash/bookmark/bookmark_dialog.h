#ifndef BOOKMARK_DASH_BOOKMARK_DIALOG_H
#define BOOKMARK_DASH_BOOKMARK_DIALOG_H

#include "ui_bookmark_dialog.h"

#include <QtWidgets/QDialog>

class BookmarkItem;
class QString;
class QWidget;

namespace bookmark_dash {

class BookmarkDialog : public QDialog
{
    Q_OBJECT
    using Base = QDialog;
public:
    BookmarkDialog(BookmarkItem &, QWidget &parent);

    // @overload QDialog
    int exec();

private:
    QString title() const;
    void setTitle(const QString&);

    QString url() const;
    void setUrl(const QString&);

    QString keyword() const;
    void setKeyword(const QString&);

    QString description() const;
    void setDescription(const QString&);

private Q_SLOTS:
    void validateUrl(const QString&);
    void commitChange();
    void removeBookmark();

private:
    Ui::BookmarkDialog m_ui;
    BookmarkItem &m_item;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_BOOKMARK_DIALOG_H
