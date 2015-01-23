#ifndef STYLIST_STYLES_ITEM_MODEL_H
#define STYLIST_STYLES_ITEM_MODEL_H

#include <QtCore/QAbstractListModel>

class QModelIndex;

namespace stylist {

class Styles;

class StylesItemModel : public QAbstractListModel
{
public:
    StylesItemModel(Styles &);

private:
    // @override QAbstractItemModel
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    Styles &m_styles;
};

} // namespace stylist

#endif // STYLIST_STYLES_ITEM_MODEL_H
