#include "lc_menubar.h"

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
    m_window.installEventFilter(this);
    webView().installEventFilter(this);

    this->connect(
        &Plugin::settings(), SIGNAL(change(QString)),
        this,                SLOT(slotSettingChanged(const QString&)));
}

bool MenuBar::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(event);

    if (obj == &m_window) {
        if (event->type() == QEvent::KeyPress) {
            if (static_cast<QKeyEvent*>(event)->key() == m_keyCode) {
                m_altPressed = true;
            }
            else {
                m_altPressed = false;
            }
            //qDebug() << "KeyPressed" << m_altPressed;
        }
        else if (event->type() == QEvent::KeyRelease) {
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
    else if (obj == &webView()) {
        if (event->type() == QEvent::FocusIn) {
            menuBar().hide();
        }
    }

    return QObject::eventFilter(obj, event);
}

QWidget& MenuBar::
menuBar()
{
    QWidget* const widget = m_window.menuBar();
    assert(widget); //TODO better
    return *widget;
}

QWidget& MenuBar::
webView()
{
    QWidget* const widget = m_window.weView();
    assert(widget); //TODO better
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
