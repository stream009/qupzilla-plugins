#include "line_edit.hpp"

#include <cassert>

#include <QtGui/QKeyEvent>

namespace adblock_dash { namespace setting_dialog {

void LineEdit::
keyPressEvent(QKeyEvent* const event)
{
    assert(event);
    Base::keyPressEvent(event);

    // prevent key event (ex. Enter) to propagate
    event->accept();
}

}} // namespace adblock_dash::setting_dialog
