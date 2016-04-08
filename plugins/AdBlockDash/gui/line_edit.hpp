#ifndef ADBLOCK_DASH_GUI_LINE_EDIT_HPP
#define ADBLOCK_DASH_GUI_LINE_EDIT_HPP

#include <QtWidgets/QLineEdit>

class QKeyEvent;

namespace adblock_dash { namespace setting_dialog {

class LineEdit : public QLineEdit
{
    using Base = QLineEdit;
public:
    using Base::Base;

private:
    void keyPressEvent(QKeyEvent* const) override;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_GUI_LINE_EDIT_HPP
