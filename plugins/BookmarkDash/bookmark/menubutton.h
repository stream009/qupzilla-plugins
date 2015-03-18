#ifndef MENU_DASH_MENU_BUTTON_H
#define MENU_DASH_MENU_BUTTON_H

#include <toolbutton.h>

class QDragEnterEvent;
class QWidget;

namespace bookmark_dash {

class MenuButton : public ToolButton
{
    using Base = ToolButton;
public:
    MenuButton(QWidget* const parent = nullptr);

private:
    // @override QWidget
    void dragEnterEvent(QDragEnterEvent*) override;

private:
    bool isSupported(const QMimeData&) const;
};

} // namespace bookmark_dash


#endif // MENU_DASH_MENU_BUTTON_H
