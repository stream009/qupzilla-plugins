#ifndef SUBSCRIPTION_WIDGET_HPP
#define SUBSCRIPTION_WIDGET_HPP

#include <cassert>
#include <memory>

#include <QtCore/QPersistentModelIndex>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class QDateTime;
class QModelIndex;
class QString;
class QUrl;

namespace adblock_dash { namespace setting_dialog {

class SubscriptionView;

class SubscriptionWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    //constructor
    SubscriptionWidget(QAbstractItemModel&,
                       const QModelIndex&, QWidget &parent);
    ~SubscriptionWidget() override;

public Q_SLOTS:
    void editTitle();

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();

private Q_SLOTS:
    void onDataChanged(const QModelIndex&, const QModelIndex&);
    void onEditingTitleFinished();

private:
    void setValues();
    void setTitle(const QString &title);
    void setLinks(const QUrl &homePage, const QUrl &listLocation);
    void setUpdateStatus(const QDateTime &updated, const bool succeed);

    void validate() const
    {
        assert(m_title && m_title->parent() == this);
        assert(m_titleEdit && m_titleEdit->parent() == this);
        assert(m_links && m_links->parent() == this);
        assert(m_updateStatus && m_updateStatus->parent() == this);
        assert(m_index.isValid());
        assert(m_linkHandler);
    }

private:
    class LinkHandler;

    QLabel    *m_title;
    QLineEdit *m_titleEdit;
    QLabel    *m_links;
    QLabel    *m_updateStatus;

    QAbstractItemModel &m_model;
    QPersistentModelIndex m_index;
    std::unique_ptr<LinkHandler> m_linkHandler;
};

}} // namespace adblock_dash::setting_dialog

#endif // SUBSCRIPTION_WIDGET_HPP
