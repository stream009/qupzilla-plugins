#ifndef ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTERS_MODEL_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTERS_MODEL_HPP

#include "core/settings.hpp"

#include <QtCore/QAbstractItemModel>

class QModelIndex;
class QVariant;

namespace adblock_dash { namespace setting_dialog {

class CustomFilterSetsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum class Column {
        Name = 0, Path, HomePage, Location, Updated, UpdateStatus
    };
public:
    CustomFilterSetsModel(Settings::FilterSets&);

    // accessor
    // @override QAbstractItemModel
    QVariant data(const QModelIndex&,
                  const int role = Qt::DisplayRole) const override;
    QVariant headerData(const int section, const Qt::Orientation,
                        const int role = Qt::DisplayRole) const override;
    // query
    // @overide QAbstractItemModel
    Qt::ItemFlags flags(const QModelIndex&) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    QModelIndex index(const int row, const int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex&) const override;

    // modifier
    // @overide QAbstractItemModel
    bool setData(const QModelIndex&, const QVariant&,
                                     const int role = Qt::EditRole) override;

private Q_SLOTS:
    void onAppended(const FilterSet&);
    void onRemoved(const FilterSet&, const size_t index);
    void onMoved(const FilterSet&, const size_t from, const size_t to);

private:
    Settings::FilterSets &m_filterSets;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTERS_MODEL_HPP
