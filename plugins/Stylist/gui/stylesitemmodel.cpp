#include "stylesitemmodel.h"

#include "core/styles.h"

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

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        assert(index.row() < rowCount(index));
        return m_styles.at(index.row()).name().c_str();
    case Qt::CheckStateRole:
        return m_styles.at(index.row()).enabled()
                                        ? Qt::Checked : Qt::Unchecked;
    case PathRole:
        return QVariant::fromValue(m_styles.at(index.row()).path());
    default:
        //qDebug() << __func__ << "unimplemented role:" << role;
        break;
    }
    return QVariant {};
}

bool StylesItemModel::
setData(const QModelIndex &index,
        const QVariant &value, int role /*= Qt::EditRole*/)
{
    assert(index.isValid());
    assert(value.isValid());

    //qDebug() << __func__ << index << value << role;
    auto &style = m_styles.at(index.row()); //TODO throw
    switch (role) {
    case Qt::CheckStateRole:
        assert(value.type() == QVariant::Int);
        style.setEnabled(value.toInt() == Qt::Checked);
        Q_EMIT dataChanged(index, index);
        return true;
    case Qt::EditRole:
        assert(value.type() == QVariant::String);
        style.setName(value.toString().toUtf8().constData());
        Q_EMIT dataChanged(index, index);
        return true;
    default:
        //qDebug() << __func__ << "unimplemented role:" << role;
        break;
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
    return Qt::ItemIsSelectable
         | Qt::ItemIsEditable
         | Qt::ItemIsUserCheckable
         | Qt::ItemIsEnabled;
}

} // namespace stylist
