#include "stylesitemmodel.h"

#include "../styles.h"

#include <cassert>
#include <limits>

#include <QtCore/QModelIndex>

namespace stylist {

StylesItemModel::
StylesItemModel(Styles &styles)
    : m_styles { styles }
{}

QVariant StylesItemModel::
data(const QModelIndex &index, int role) const
{
    assert(index.isValid());

    if (role == Qt::DisplayRole) {
        assert(index.row() < rowCount(index));
        return m_styles.at(index.row()).name().c_str();
    }
    else if (role == Qt::CheckStateRole) {
        return m_styles.at(index.row()).enabled()
                                        ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant {};
}

bool StylesItemModel::
setData(const QModelIndex &index,
        const QVariant &value, int role /*= Qt::EditRole*/)
{
    assert(index.isValid());
    assert(value.isValid());

    //qDebug() << __FUNCTION__ << index << value << role;
    if (role == Qt::CheckStateRole) {
        m_styles.at(index.row()).setEnabled(value.toInt() == Qt::Checked);
        Q_EMIT dataChanged(index, index);
        return true;
    }
    return false;
}

int StylesItemModel::
rowCount(const QModelIndex &) const
{
    assert(m_styles.size() <= std::numeric_limits<int>::max()); //TODO better
    return static_cast<int>(m_styles.size());
}

Qt::ItemFlags StylesItemModel::
flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable
                                | Qt::ItemIsEnabled;
}

} // namespace stylist
