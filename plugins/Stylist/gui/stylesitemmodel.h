#ifndef STYLIST_STYLES_ITEM_MODEL_H
#define STYLIST_STYLES_ITEM_MODEL_H

#include <boost/filesystem.hpp>

#include <QtCore/QAbstractListModel>

class QModelIndex;

namespace stylist {

class SettingActions;
class Styles;

class StylesItemModel : public QAbstractListModel
{
    Q_OBJECT
    using Path = boost::filesystem::path;
public:
    StylesItemModel(Styles &);

    void setActions(const SettingActions &);

    const Path &path(const QModelIndex&) const;

private:
    // @override QAbstractItemModel
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private Q_SLOTS:
    void slotStylesChanged();

public:
    static const int PathRole = Qt::UserRole;

private:
    Styles &m_styles;
    const SettingActions *m_actions;
};

} // namespace stylist

#endif // STYLIST_STYLES_ITEM_MODEL_H
