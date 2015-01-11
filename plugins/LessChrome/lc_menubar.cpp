#include "lc_menubar.h"

#include "error.h"
#include "plugin.h"

#include "browserwindow.h"
#include "tabbedwebview.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtGui/QMenuBar>

namespace lesschrome {

MenuBar::
MenuBar(BrowserWindow &window)
    : m_window(window),
      m_altPressed(false),
      m_keyCode(Plugin::settings().menuBarKey)
{
    this->connect(
        &Plugin::settings(), SIGNAL(change(QString)),
        this,                SLOT(slotSettingChanged(const QString&)));
}

void MenuBar::
handleWebViewEvent(const QEvent& event)
{
    if (event.type() == QEvent::FocusIn) {
        menuBar().hide();
    }
}

void MenuBar::
handleWindowEvent(const QEvent &event)
{
    if (event.type() == QEvent::KeyPress) {
        if (static_cast<const QKeyEvent&>(event).key() == m_keyCode) {
            m_altPressed = true;
        }
        else {
            m_altPressed = false;
        }
        //qDebug() << "KeyPressed" << m_altPressed;
    }
    else if (event.type() == QEvent::KeyRelease) {
        //qDebug() << "KeyRelease" << m_altPressed;
        if (m_altPressed) {
            if (!menuBar().isVisible()) {
                menuBar().show();
                menuBar().setFocus(Qt::MenuBarFocusReason);
            }
            else {
                menuBar().hide();
            }
        }
    }
}

QWidget& MenuBar::
menuBar() const
{
    QWidget* const widget = m_window.menuBar();
    if (!widget) {
        throw RuntimeError("Fail to obtain menu bar");
    }
    return *widget;
}

void MenuBar::
slotSettingChanged(const QString &key)
{
    if (key == Settings::keyMenuBarKey) {
        m_keyCode = Plugin::settings().menuBarKey;
    }
}

} // namespace lesschrome
