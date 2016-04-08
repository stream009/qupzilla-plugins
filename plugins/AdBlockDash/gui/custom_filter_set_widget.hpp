#ifndef ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTER_SET_WIDGET_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTER_SET_WIDGET_HPP

#include <cassert>

#include <QtCore/QPersistentModelIndex>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class QAbstractItemModel;
class QModelIndex;

namespace adblock_dash { namespace setting_dialog {

class CustomFilterSetWidget : public QWidget
{
    Q_OBJECT

    using Base = QWidget;
public:
    CustomFilterSetWidget(QAbstractItemModel&, const QModelIndex&,
                          QWidget &parent);
public Q_SLOTS:
    void editTitle();

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();
    void setValues();
    void setTitle(const QString &title);

private Q_SLOTS:
    void onDataChanged(const QModelIndex&, const QModelIndex&);
    void onEditingTitleFinished();

private:
    void validate() const
    {
        assert(m_title && m_title->parent() == this);
        assert(m_titleEdit && m_titleEdit->parent() == this);
        assert(m_index.isValid());
    }

private:
    QLabel    *m_title;
    QLineEdit *m_titleEdit;

    QAbstractItemModel &m_model;
    QPersistentModelIndex m_index;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTER_SET_WIDGET_HPP
