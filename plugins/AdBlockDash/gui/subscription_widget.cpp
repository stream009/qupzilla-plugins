#include "subscription_widget.hpp"

#include "subscriptions_view.hpp"
#include "subscriptions_model.hpp"

#include <cassert>

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtGui/QCursor>
#include <QtGui/QTextDocument>
#include <QtWidgets/QToolTip>

namespace adblock_dash { namespace setting_dialog {

class SubscriptionWidget::LinkHandler : public QObject
{
    Q_OBJECT
public:
    LinkHandler(QObject &target)
        : m_target { target }
    {
        this->connect(&target, SIGNAL(linkHovered(const QString&)),
                         this,   SLOT(onLinkHovered(const QString&)));
        m_target.installEventFilter(this);
    }

private Q_SLOTS:
    // @override QObject
    bool eventFilter(QObject* const target, QEvent* const event) override
    {
        assert(target); assert(event);
        if (target != &m_target) return false;

        if (event->type() == QEvent::ToolTip) {
            QToolTip::showText(QCursor::pos(), m_toolTip);
        }

        return false;
    }

    void onLinkHovered(const QString &link)
    {
        m_toolTip = link;
    }

private:
    QObject &m_target;
    QString m_toolTip;
};


SubscriptionWidget::
SubscriptionWidget(QAbstractItemModel &model,
                   const QModelIndex &index, QWidget &parent)
    : Base { &parent }
    , m_title { new QLabel { this } }
    , m_titleEdit { new QLineEdit { this } }
    , m_links { new QLabel { this } }
    , m_updateStatus { new QLabel { this } }
    , m_model { model }
    , m_index { index }
{
    assert(m_title);
    assert(m_titleEdit);
    assert(m_links);
    assert(m_updateStatus);

    m_linkHandler.reset(new LinkHandler { *m_links });

    this->connect(&m_model, SIGNAL(dataChanged(const QModelIndex&,
                                               const QModelIndex&)),
                  this,       SLOT(onDataChanged(const QModelIndex&,
                                                 const QModelIndex&)));
    initWidgets();
    layoutWidgets();
    connectWidgets();

    validate();
}

SubscriptionWidget::~SubscriptionWidget() = default;

void SubscriptionWidget::
editTitle()
{
    QTextDocument doc;
    doc.setHtml(m_title->text());
    m_titleEdit->setText(doc.toPlainText());

    m_title->setVisible(false);
    m_titleEdit->setVisible(true);
    m_titleEdit->setFocus();
    m_titleEdit->selectAll();
}

void SubscriptionWidget::
initWidgets()
{
    m_titleEdit->setVisible(false);
    setValues();
}

void SubscriptionWidget::
layoutWidgets()
{
    auto* const upper = new QHBoxLayout();
    assert(upper);
    upper->setContentsMargins(0, 0, 0, 0);
    upper->addWidget(m_title);
    upper->addWidget(m_titleEdit);
    upper->addWidget(m_links);

    auto* const lower = new QHBoxLayout();
    lower->setContentsMargins(0, 0, 0, 0);
    assert(lower);
    lower->addWidget(m_updateStatus);

    auto* const layout = new QVBoxLayout(this);
    assert(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(upper); // pass ownership
    layout->addLayout(lower); // pass ownership

    assert(upper->parent() == layout);
    assert(lower->parent() == layout);
    assert(layout->parent() == this);
}

void SubscriptionWidget::
connectWidgets()
{
    this->connect(m_titleEdit, SIGNAL(editingFinished()),
                  this,          SLOT(onEditingTitleFinished()));
}

void SubscriptionWidget::
onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (topLeft.row()     < m_index.row()) return;
    if (bottomRight.row() > m_index.row()) return;

    setValues();
}

void SubscriptionWidget::
onEditingTitleFinished()
{
    m_model.setData(m_index, m_titleEdit->text());

    m_title->setVisible(true);
    m_titleEdit->setVisible(false);
}

void SubscriptionWidget::
setTitle(const QString &title)
{
    m_title->setText(tr("<b>%1</b>").arg(title));
}

void SubscriptionWidget::
setLinks(const QUrl &homePage, const QUrl &listLocation)
{
    m_links->setText(tr(R"(
        (<a href="%1">Homepage</a>, <a href="%2">Filter list</a>)
    )").arg(homePage.toString()).arg(listLocation.toString()));
}

void SubscriptionWidget::
setUpdateStatus(const QDateTime &updated, const bool succeed)
{
    m_updateStatus->setText(tr(
        "Last downloaded: %1 (%2)"
    ).arg(updated.toString()).arg(succeed ? tr("Success") : tr("Fail")));
}

void SubscriptionWidget::
setValues()
{
    using Column = SubscriptionsModel::Column;

    const auto &title =
        m_model.index(m_index.row(), static_cast<int>(Column::Name));
    assert(title.isValid());
    setTitle(title.data().toString());

    const auto &homePage =
        m_model.index(m_index.row(), static_cast<int>(Column::HomePage));
    assert(homePage.isValid());
    const auto &location =
        m_model.index(m_index.row(), static_cast<int>(Column::Location));
    assert(location.isValid());
    setLinks(homePage.data().toUrl(), location.data().toUrl());

    const auto &updated =
        m_model.index(m_index.row(), static_cast<int>(Column::Updated));
    assert(updated.isValid());
    const auto &status =
        m_model.index(m_index.row(), static_cast<int>(Column::UpdateStatus));
    assert(status.isValid());
    setUpdateStatus(updated.data().toDateTime(), status.data().toBool());
}

}} // namespace adblock_dash::setting_dialog

#include "subscription_widget.moc"
