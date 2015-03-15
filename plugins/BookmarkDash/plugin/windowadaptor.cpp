#include "windowadaptor.h"

#include <QtCore/QObject>

#include <browserwindow.h>

namespace bookmark_dash {

WindowAdaptor::
WindowAdaptor(BrowserWindow &window, Settings &settings)
    : m_window { window },
      m_menu { window, settings },
      m_toolBar { window, settings },
      m_settingsMenu { settings },
      m_iconWatcher { window }
{
    toolsMenu().addMenu(&m_settingsMenu);
}

WindowAdaptor::
~WindowAdaptor()
{
    toolsMenu().removeAction(m_settingsMenu.menuAction());
}

QMenu &WindowAdaptor::
toolsMenu() const
{
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);

    const auto &actions = menuBar->actions();
    const auto it = std::find_if(actions.begin(), actions.end(),
        [] (QAction* const action) {
            return action->text() == QObject::tr("&Tools");
        }
    );
    assert(it != actions.end());

    auto* const result = (*it)->menu();
    assert(result);

    return *result;
}

} // namespace bookmark_dash
