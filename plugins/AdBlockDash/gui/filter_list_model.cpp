#include "filter_list_model.hpp"

#include "core/settings.hpp"
#include "core/text_file.hpp"
#include "common/utility.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

namespace adblock_dash { namespace setting_dialog {

FilterListModel::
FilterListModel(const Settings &settings,
                TextFile &file, const bool editable/*= false*/)
    : m_settings { settings }
    , m_file { file }
    , m_editable { editable }
{
    detectHeader();
}

QVariant FilterListModel::
data(const QModelIndex &index, const int role/* = Qt::DisplayRole*/) const
{
    if (!index.isValid()) return {};

    if (role != Qt::DisplayRole &&
        role != Qt::CheckStateRole &&
        role != Qt::EditRole &&
        role != Qt::ForegroundRole)
    {
        return {};
    }

    const auto row = index.row();
    const auto column = index.column();

    if (column < 0 || column >= columnCount()) return {};

    switch (column) {
    case static_cast<int>(Column::Enabled):
        return enabledData(role, row);
    case static_cast<int>(Column::Name):
        return filterData(role, row);
    default:
        assert(false);
    }
}

QVariant FilterListModel::
headerData(const int section, const Qt::Orientation orientation,
                         const int role/* = Qt::DisplayRole*/) const
{
    if (role != Qt::DisplayRole) return {};
    if (orientation != Qt::Horizontal) return {};

    switch (section) {
    case static_cast<int>(Column::Enabled):
        return "Enabled";
    case static_cast<int>(Column::Name):
        return "Filter";
    default:
        assert(false);
    }
}

Qt::ItemFlags FilterListModel::
flags(const QModelIndex &index) const
{
    assert(index.isValid());
    const auto &row = index.row();
    const auto &column = index.column();

    assert(0 <= row    && row    < rowCount());
    assert(0 <= column && column < columnCount());

    auto result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (column == static_cast<int>(Column::Enabled) && !isComment(row)) {
        result |= Qt::ItemIsUserCheckable;
    }
    else if (column == static_cast<int>(Column::Name) && m_editable) {
        result |= Qt::ItemIsEditable;
    }

    return result;
}

int FilterListModel::
rowCount(const QModelIndex &parent/* = {}*/) const
{
    if (parent.isValid()) return 0;

    assert(m_file.lineCount() >= m_offset);
    return ::toSignedInt(m_file.lineCount() - m_offset);
}

int FilterListModel::
columnCount(const QModelIndex &parent/* = {}*/) const
{
    assert(!parent.isValid());
    return COLUMN_COUNT;
}

QModelIndex FilterListModel::
index(const int row, const int column,
            const QModelIndex &parent/* = {}*/) const
{
    if (parent.isValid())        return {};
    if (column >= columnCount()) return {};
    if (row >= rowCount())       return {};

    return this->createIndex(row, column, nullptr);
}

QModelIndex FilterListModel::
parent(const QModelIndex&) const
{
    return {};
}

bool FilterListModel::
setData(const QModelIndex &index, const QVariant &value,
                            const int role/* = Qt::EditRole*/)
{
    assert(index.isValid());

    if (role == Qt::EditRole) {
        if (index.column() != static_cast<int>(Column::Name)) {
            return false;
        }

        const auto &lineNo = row2LineNo(index.row());
        assert(lineNo < m_file.lineCount());

        auto &&line = value.toString().toStdString();

        if (!m_file.setLine(lineNo, std::move(line))) {
            return false;
        }

        Q_EMIT dataChanged(index, index);
        return true;
    }
    else if (role == Qt::CheckStateRole) {
        if (index.column() != static_cast<int>(Column::Enabled)) {
            return false;
        }

        assert(value.isValid());
        assert(value.type() == QVariant::Int);
        const bool state = value.toInt() == Qt::Checked;

        const auto row = index.row();
        assert(0 <= row && row < rowCount());
        if (!state) {
            m_disabled.insert(::toSizeT(row));
        }
        else {
            m_disabled.erase(::toSizeT(row));
        }

        Q_EMIT dataChanged(index, index);
        return true;
    }
    else {
        return false;
    }

}

bool FilterListModel::
insertRows(const int row, const int count, const QModelIndex &parent/*= {}*/)
{
    assert(0 <= row && row <= rowCount()); // row == rowCount() mean append
    assert(0 < count);
    assert(!parent.isValid());

    beginInsertRows(parent, row, row + count - 1);

    for (auto i = 0; i < count; ++i) {
        m_file.newLine(row2LineNo(row));
    }

    endInsertRows();

    return true;
}

bool FilterListModel::
removeRows(const int row, const int count, const QModelIndex &parent/*= {}*/)
{
    assert(0 <= row && row < rowCount());
    assert(0 < count && row + count - 1 < rowCount());
    assert(!parent.isValid());

    beginRemoveRows(parent, row, row + count - 1);

    for (auto i = 0; i < count; ++i) {
        m_file.deleteLine(row2LineNo(row));
    }

    endRemoveRows();

    return true;
}

void FilterListModel::
detectHeader()
{
    namespace ba = boost::algorithm;
    const auto &firstLine = m_file.line(0);
    assert(ba::starts_with(firstLine, "[Adblock"));

    // find first filter
    boost::optional<size_t> firstFilter;
    for (auto i = 1u; i < m_file.lineCount(); ++i) {
        const auto &line = m_file.line(i);
        if (!ba::starts_with(line, "!")) {
            firstFilter = i;
            break;
        }
    }
    if (!firstFilter) {
        m_offset = 1;
        return;
    }

    // find first blank comment from first fileter backward
    boost::optional<size_t> lastBlankComment;
    for (auto i = firstFilter.get(); i >= 1; --i) {
        const auto &line = m_file.line(i);
        if (ba::equals(line, "!")) {
            lastBlankComment = i;
            break;
        }
    }

    m_offset = lastBlankComment ? lastBlankComment.get() + 1 : 1;
}

size_t FilterListModel::
row2LineNo(const int row) const
{
    return ::toSizeT(row) + m_offset;
}

QVariant FilterListModel::
enabledData(const int role, const int row) const
{
    if (role != Qt::CheckStateRole) return {};
    assert(0 <= row && row < rowCount());

    if (isComment(row)) return {};

    const auto disabled =
               m_disabled.find(::toSizeT(row)) != m_disabled.end();
    return disabled ? Qt::Unchecked : Qt::Checked;
}

QVariant FilterListModel::
filterData(const int role, const int row) const
{
    assert(0 <= row && row < rowCount());

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
            const auto &line = m_file.line(row2LineNo(row));
            return QString::fromUtf8(line.begin(), ::toSignedInt(line.size()));
        }
        break;
    case Qt::ForegroundRole:
        if (isComment(row)) {
            return m_settings.commentColor().value();
        }
        else if (isElementHidingRule(row)) {
            return m_settings.elementHidingRuleColor().value();
        }
        else if (isExceptionRule(row)) {
            return m_settings.exceptionRuleColor().value();
        }
        else {
            return {};
        }
        break;
    default:
        return {};
    }
}

bool FilterListModel::
isComment(const int row) const
{
    const auto &line = m_file.line(row2LineNo(row));
    return !line.empty() && line[0] == '!';
}

bool FilterListModel::
isElementHidingRule(const int row) const
{
    namespace ba = boost::algorithm;
    const auto &line = m_file.line(row2LineNo(row));
    return !line.empty() && ba::starts_with(line, "##");
}

bool FilterListModel::
isExceptionRule(const int row) const
{
    namespace ba = boost::algorithm;
    const auto &line = m_file.line(row2LineNo(row));
    return !line.empty() && ba::starts_with(line, "@@");
}

}} // namespace adblock_dash::setting_dialog
