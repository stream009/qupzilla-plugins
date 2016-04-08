#ifndef ADBLOCK_DASH_SUBSCRIPTIONS_MODEL_HPP
#define ADBLOCK_DASH_SUBSCRIPTIONS_MODEL_HPP

#include "core/settings.hpp"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QVariant>

class QModelIndex;

namespace adblock_dash {

class Subscription;

class SubscriptionsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum class Column {
        Name = 0, Path, HomePage, Location, Updated, UpdateStatus
    };
public:
    // constructor
    SubscriptionsModel(Settings::Subscriptions&);

    // accessor
    // @override QAbstractItemModel
    QVariant data(const QModelIndex&,
                  const int role = Qt::DisplayRole) const override;
    QVariant headerData(const int section, const Qt::Orientation,
                        const int role = Qt::DisplayRole) const override;
    // query
    // @overide QAbstractItemModel
    Qt::ItemFlags flags(const QModelIndex&) const override;
    int rowCount(const QModelIndex &parent = QModelIndex {}) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(const int row, const int column,
                      const QModelIndex &parent = QModelIndex {}) const override;
    QModelIndex parent(const QModelIndex&) const override;

    // modifier
    // @overide QAbstractItemModel
    bool setData(const QModelIndex&, const QVariant&,
                                     const int role = Qt::EditRole) override;

private Q_SLOTS:
    void onAppended(const Subscription&);
    void onRemoved(const Subscription&, const size_t index);
    void onMoved(const Subscription&, const size_t from, const size_t to);

private:
    Settings::Subscriptions &m_subscriptions;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_SUBSCRIPTIONS_MODEL_HPP
