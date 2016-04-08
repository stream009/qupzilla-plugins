#include "subscriptions_model.hpp"

#include "core/subscription.hpp"
#include "common/utility.hpp"

namespace adblock_dash {

//
// constructor
//
SubscriptionsModel::
SubscriptionsModel(Settings::Subscriptions &subscriptions)
    : m_subscriptions { subscriptions }
{
    const auto &notifier = subscriptions.notifier();
    this->connect(
        &notifier, SIGNAL(appended(const Subscription&)),
        this,        SLOT(onAppended(const Subscription&))
    );
    this->connect(
        &notifier, SIGNAL(removed(const Subscription&, size_t)),
        this,        SLOT(onRemoved(const Subscription&, size_t))
    );
    this->connect(
        &notifier, SIGNAL(moved(const Subscription&, size_t, size_t)),
        this,        SLOT(onMoved(const Subscription&, size_t, size_t))
    );
}

//
// accessor
//
QVariant SubscriptionsModel::
data(const QModelIndex &index, const int role/* = Qt::DisplayRole*/) const
{
    if (!index.isValid())        return {};

    const auto row = index.row();
    const auto column = index.column();

    if (column < 0 || column >= columnCount()) return {};
    if (row    < 0 || row    >= rowCount())    return {};

    const auto &subscription = m_subscriptions[::toSizeT(row)];

    if (role == Qt::CheckStateRole) {
        return subscription.enabled();
    }
    else if (role == Qt::DisplayRole) {
        switch (column) {
        case static_cast<int>(Column::Name):
            return subscription.name();
        case static_cast<int>(Column::Path):
            return QString { subscription.path().c_str() };
        case static_cast<int>(Column::HomePage):
            return subscription.homePage();
        case static_cast<int>(Column::Location):
            return subscription.location();
        case static_cast<int>(Column::Updated):
            return subscription.updated();
        case static_cast<int>(Column::UpdateStatus):
            return subscription.updateStatus();
        default:
            assert(false);
        }
    }
    else {
        return {};
    }
}

QVariant SubscriptionsModel::
headerData(const int section, const Qt::Orientation orientation,
                         const int role/* = Qt::DisplayRole*/) const
{
    if (role != Qt::DisplayRole) return {};
    if (orientation != Qt::Horizontal) return {};

    switch (section) {
    case static_cast<int>(Column::Name):
        return "Name";
    case static_cast<int>(Column::Path):
        return "Path";
    case static_cast<int>(Column::HomePage):
        return "Home Page";
    case static_cast<int>(Column::Location):
        return "Location";
    case static_cast<int>(Column::Updated):
        return "Updated";
    case static_cast<int>(Column::UpdateStatus):
        return "Update Status";
    default:
        assert(false);
    }
}

//
// query
//
Qt::ItemFlags SubscriptionsModel::
flags(const QModelIndex&) const
{
    return Qt::ItemIsSelectable |
           Qt::ItemIsEnabled |
           Qt::ItemIsEditable;
}

int SubscriptionsModel::
rowCount(const QModelIndex &parent/* = QModelIndex {}*/) const
{
    //assert(!parent.isValid());
    if (parent.isValid()) return 0;

    return ::toSignedInt(m_subscriptions.size());
}

int SubscriptionsModel::
columnCount(const QModelIndex &parent/* = QModelIndex {}*/) const
{
    assert(!parent.isValid());
    return 6;
}

QModelIndex SubscriptionsModel::
index(const int row, const int column,
            const QModelIndex &parent/* = QModelIndex {}*/) const
{
    if (parent.isValid())        return {};
    if (column >= columnCount()) return {};
    if (row >= rowCount())       return {};

    return this->createIndex(row, column, nullptr);
}

QModelIndex SubscriptionsModel::
parent(const QModelIndex&) const
{
    return {};
}

//
// modifier
//
bool SubscriptionsModel::
setData(const QModelIndex &index, const QVariant &value,
                            const int role/* = Qt::EditRole*/)
{
    assert(index.isValid());
    assert(value.isValid());

    const auto row = index.row();
    assert(0 <= row && row < rowCount());

    auto &data = m_subscriptions[::toSizeT(row)];

    if (role == Qt::CheckStateRole) {
        data.setEnabled(value.toInt() == Qt::Checked);
    }
    else if (role == Qt::EditRole) {
        const auto column = index.column();
        switch (column) {
        case static_cast<int>(Column::Name):
            data.setName(value.toString());
            break;
        default:
            assert(false);
        }
    }
    else {
        return false;
    }

    Q_EMIT dataChanged(index, index);

    return true;
}

void SubscriptionsModel::
onAppended(const Subscription &subscription)
{
    const auto &begin = m_subscriptions.begin();
    const auto &end = m_subscriptions.end();
    const auto &it = std::find_if(begin, end,
        [&](const Subscription &item) {
            return &item == &subscription;
        }
    );
    assert(it != end);

    const auto row = std::distance(begin, it);
    assert(0 <= row && row < ::toSignedInt(m_subscriptions.size()));

    this->beginInsertRows({}, row, row);
    this->endInsertRows();
}

void SubscriptionsModel::
onRemoved(const Subscription&, const size_t index)
{
    const auto row = ::toSignedInt(index);
    this->beginRemoveRows({}, row, row);
    this->endRemoveRows();
}

void SubscriptionsModel::
onMoved(const Subscription&, const size_t from, const size_t to)
{
    this->beginRemoveRows({}, ::toSignedInt(from), ::toSignedInt(from));
    this->endRemoveRows();

    this->beginInsertRows({}, ::toSignedInt(to), ::toSignedInt(to));
    this->endInsertRows();
#if 0
    this->beginMoveRows(
            {}, ::toSignedInt(from), ::toSignedInt(from),
            {}, ::toSignedInt(to));
    this->endMoveRows();
#endif
}

} // namespace adblock_dash
