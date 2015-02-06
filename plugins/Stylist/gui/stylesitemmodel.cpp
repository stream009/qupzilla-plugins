#include "stylesitemmodel.h"

#include "core/styles.h"

#include <cassert>
#include <limits>

#include <QtCore/QModelIndex>

namespace stylist {

StylesItemModel::
StylesItemModel(Styles &styles)
    : m_styles { styles }
{
    this->connect(&m_styles, SIGNAL(changed()),
                  this,      SLOT(slotStylesChanged()));
}

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

void StylesItemModel::
slotStylesChanged()
{
    const auto &topLeft = this->index(0, 0);
    const auto &bottomRight =
        this->index(this->rowCount() - 1);

    //qDebug() << __func__ << topLeft << bottomRight;
    Q_EMIT dataChanged(topLeft, bottomRight);
}

void StylesItemModel::
slotOpenStyle(const QModelIndex &index)
{
    const auto data = this->data(index, StylesItemModel::PathRole);
    assert(data.isValid());
    assert(strcmp(data.typeName(), "boost::filesystem::path") == 0);

    const auto path = data.value<boost::filesystem::path>();
    Q_EMIT openStyle(path);
}

} // namespace stylist
