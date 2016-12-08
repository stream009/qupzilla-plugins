#include "custom_filter_sets_model.hpp"

#include "common/utility.hpp"
#include "core/filter_set.hpp"

#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace adblock_dash { namespace setting_dialog {

//
// constructor
//
CustomFilterSetsModel::
CustomFilterSetsModel(Settings::FilterSets &filterSets)
    : m_filterSets { filterSets }
{
    const auto &notifier = filterSets.notifier();
    this->connect(
        &notifier, SIGNAL(appended(const FilterSet&)),
        this,        SLOT(onAppended(const FilterSet&))
    );
    this->connect(
        &notifier, SIGNAL(removed(const FilterSet&, size_t)),
        this,        SLOT(onRemoved(const FilterSet&, size_t))
    );
    this->connect(
        &notifier, SIGNAL(moved(const FilterSet&, size_t, size_t)),
        this,        SLOT(onMoved(const FilterSet&, size_t, size_t))
    );
}

//
// accessor
//
QVariant CustomFilterSetsModel::
data(const QModelIndex &index, const int role/* = Qt::DisplayRole*/) const
{
    if (!index.isValid())        return {};

    const auto row = index.row();
    const auto column = index.column();

    if (column < 0 || column >= columnCount()) return {};
    if (row    < 0 || row    >= rowCount())    return {};

    const auto &filterSet = m_filterSets[::toSizeT(row)];

    if (role == Qt::CheckStateRole) {
        return filterSet.enabled();
    }
    else if (role == Qt::DisplayRole) {
        switch (column) {
        case static_cast<int>(Column::Name):
            return filterSet.name();
        case static_cast<int>(Column::Path):
            return QString { filterSet.path().c_str() };
        default:
            assert(false);
        }
    }
    else {
        return {};
    }
}

QVariant CustomFilterSetsModel::
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
    default:
        assert(false);
    }
}

//
// query
//
Qt::ItemFlags CustomFilterSetsModel::
flags(const QModelIndex&) const
{
    return Qt::ItemIsSelectable |
           Qt::ItemIsEnabled |
           Qt::ItemIsEditable;
}

int CustomFilterSetsModel::
rowCount(const QModelIndex &parent/* = QModelIndex {}*/) const
{
    if (parent.isValid()) return 0;

    return ::toSignedInt(m_filterSets.size());
}

int CustomFilterSetsModel::
columnCount(const QModelIndex &parent/* = QModelIndex {}*/) const
{
    assert(!parent.isValid());
    return 2;
}

QModelIndex CustomFilterSetsModel::
index(const int row, const int column,
            const QModelIndex &parent/* = QModelIndex {}*/) const
{
    if (parent.isValid())        return {};
    if (column >= columnCount()) return {};
    if (row >= rowCount())       return {};

    return this->createIndex(row, column, nullptr);
}

QModelIndex CustomFilterSetsModel::
parent(const QModelIndex&) const
{
    return {};
}

//
// modifier
//
bool CustomFilterSetsModel::
setData(const QModelIndex &index, const QVariant &value,
                            const int role/* = Qt::EditRole*/)
{
    assert(index.isValid());
    assert(value.isValid());

    const auto row = index.row();
    assert(0 <= row && row < rowCount());

    auto &data = m_filterSets[::toSizeT(row)];

    if (role == Qt::CheckStateRole) {
        data.setEnabled(value.toInt() == Qt::Checked);
    }
    else if (role == Qt::EditRole) {
        const auto column = index.column();
        switch (column) {
        case static_cast<int>(Column::Name):
            data.setName(value.toString());
            break;
        case static_cast<int>(Column::Path):
            data.setPath(value.toString().toLocal8Bit().constData());
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

void CustomFilterSetsModel::
onAppended(const FilterSet &filterSet)
{
    const auto &begin = m_filterSets.begin();
    const auto &end = m_filterSets.end();
    const auto &it = std::find_if(begin, end,
        [&](const FilterSet &item) {
            return &item == &filterSet;
        }
    );
    assert(it != end);

    auto const row = boost::numeric_cast<int>(std::distance(begin, it));
    assert(0 <= row && row < boost::numeric_cast<int>(m_filterSets.size()));

    this->beginInsertRows({}, row, row);
    this->endInsertRows();
}

void CustomFilterSetsModel::
onRemoved(const FilterSet&, const size_t index)
{
    const auto row = ::toSignedInt(index);
    this->beginRemoveRows({}, row, row);
    this->endRemoveRows();
}

void CustomFilterSetsModel::
onMoved(const FilterSet&, const size_t from, const size_t to)
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

}} // namespace adblock_dash::setting_dialog
