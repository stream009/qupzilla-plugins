#ifndef ADBLOCK_DASH_SETTING_DIALOG_FIND_BAR_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_FIND_BAR_HPP

#include "line_edit.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

class QFocusEvent;

namespace adblock_dash { namespace setting_dialog {

class FilterListActions;

class FindBar : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    FindBar(FilterListActions&, QWidget &parent);

protected:
    // @override QWidget
    void focusInEvent(QFocusEvent* const) override;

private Q_SLOTS:
    void search(const QString &keyword,
                const int step = +1, const int startOffset = 0) const;
    void searchNext() const;
    void searchPrevious() const;

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();

    void validate() const
    {
        assert(m_text && m_text->parent() == this);
        assert(m_next && m_next->parent() == this);
        assert(m_prev && m_prev->parent() == this);
        assert(m_matchCase && m_matchCase->parent() == this);
        assert(m_message && m_message->parent() == this);
        assert(m_close && m_close->parent() == this);
    }

private:
    LineEdit    *m_text;
    QToolButton *m_next, *m_prev;
    QPushButton *m_matchCase;
    QLabel      *m_message;
    QToolButton *m_close;

    FilterListActions &m_actions;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_FIND_BAR_HPP
