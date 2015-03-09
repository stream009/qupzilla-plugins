#include "bookmark_dialog.h"

#include <bookmarkitem.h>
#include <bookmarksmodel.h>

#include <cassert>

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

namespace bookmark_dash {

BookmarkDialog::
BookmarkDialog(BookmarkItem &item, QWidget &parent)
    : Base { &parent },
      m_item { item }
{
    assert(item.type() == BookmarkItem::Url);
    m_ui.setupUi(this);

    assert(m_ui.title);
    setTitle(item.title());
    assert(m_ui.url);
    setUrl(item.url().toString());
    assert(m_ui.keyword);
    setKeyword(item.keyword());
    assert(m_ui.description);
    setDescription(item.description());

    this->connect(m_ui.url, SIGNAL(textEdited(const QString&)),
                  this,       SLOT(validateUrl(const QString &)));
    this->connect(this, SIGNAL(accepted()),
                  this,   SLOT(commitChange()));
}

int BookmarkDialog::
exec()
{
    validateUrl(url());
    return Base::exec();
}

QString BookmarkDialog::
title() const
{
    return m_ui.title->text();
}

void BookmarkDialog::
setTitle(const QString &title)
{
    m_ui.title->setText(title);
}

QString BookmarkDialog::
url() const
{
    return m_ui.url->text();
}

void BookmarkDialog::
setUrl(const QString &title)
{
    m_ui.url->setText(title);
}

QString BookmarkDialog::
keyword() const
{
    return m_ui.keyword->text();
}

void BookmarkDialog::
setKeyword(const QString &title)
{
    m_ui.keyword->setText(title);
}

QString BookmarkDialog::
description() const
{
    return m_ui.description->toPlainText();
}

void BookmarkDialog::
setDescription(const QString &title)
{
    m_ui.description->setPlainText(title);
}

void BookmarkDialog::
validateUrl(const QString &text)
{
    auto* const okButton = m_ui.buttonBox->button(QDialogButtonBox::Ok);
    assert(okButton);

    QUrl url { text };
    okButton->setEnabled(url.isValid());
}

void BookmarkDialog::
commitChange()
{
    m_item.setTitle(title());
    m_item.setUrl(url());
    m_item.setKeyword(keyword());
    m_item.setDescription(description());
}

} // namespace bookmark_dash
