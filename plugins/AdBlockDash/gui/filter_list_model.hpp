#ifndef ADBLOCK_DASH_FILTER_LIST_MODEL_HPP
#define ADBLOCK_DASH_FILTER_LIST_MODEL_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/container/flat_set.hpp>

#include <QtCore/QAbstractItemModel>

class QString;

namespace adblock_dash {

class Settings;
class TextFile;

namespace setting_dialog {

class FilterListModel : public QAbstractItemModel
{
    Q_OBJECT
    using Path = boost::filesystem::path;

public:
    enum class Column { Enabled = 0, Name = 1 };
    static constexpr const int COLUMN_COUNT = 2;

public:
    FilterListModel(const Settings&, TextFile&, const bool editable = false);

    bool isEditable() const { return m_editable; }

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
    bool insertRows(const int row, const int count,
                                   const QModelIndex &parent = {}) override;
    bool removeRows(const int row, const int count,
                                   const QModelIndex &parent = {}) override;
private:
    void detectHeader();
    size_t row2LineNo(const int) const;
    QVariant enabledData(const int role, const int row) const;
    QVariant filterData(const int role, const int row) const;
    bool isComment(const int row) const;
    bool isElementHidingRule(const int row) const;
    bool isExceptionRule(const int row) const;

private:
    const Settings &m_settings;
    TextFile &m_file;
    boost::container::flat_set<size_t> m_disabled;
    size_t m_offset = 1;
    bool m_editable = false;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_FILTER_LIST_MODEL_HPP
