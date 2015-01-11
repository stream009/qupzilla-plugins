#include "windowhandler.h"

#include "error.h"
#include "lc_menubar.h"
#include "lc_settings.h"
#include "plugin.h"
#include "toolbar.h"

#include "bookmarkstoolbar.h"
#include "browserwindow.h"
#include "locationbar.h"
#include "navigationbar.h"
#include "tabbedwebview.h"

#include <cassert>

#include <QtGui/QMouseEvent>

namespace lesschrome {

WindowHandler::
WindowHandler(BrowserWindow &window)
    : m_window(window),
      m_navigationContainer(NULL),
      m_tabWatcher(window)
{
    const Settings &settings = Plugin::settings();
    this->connect(&settings, SIGNAL(change(QString)),
                  this,      SLOT(slotSettingChanged(const QString&)));

    m_navigationContainer = navigationBar().parentWidget();
    if (!m_navigationContainer) {
        throw RuntimeError("Fail to access navigation container.");
    }

    captureWidgets();

    this->connect(&m_tabWatcher, SIGNAL(tabAdded(WebTab&)),
                  this,          SLOT(slotTabAdded(WebTab&)));
    this->connect(&m_tabWatcher, SIGNAL(tabDeleted(WebTab&)),
                  this,          SLOT(slotTabDeleted(WebTab&)));

    m_window.installEventFilter(this);
    m_navigationContainer->installEventFilter(this);

    assert(m_navigationContainer);
}

WindowHandler::
~WindowHandler() {}

bool WindowHandler::
eventFilter(QObject* const obj, QEvent* const event) // throw()
{
    assert(obj);
    assert(event);
    assert(m_navigationContainer);

    try {
        // If obj is invalid it just pass through, no need to check it out.
        if (!event) {
            throw RuntimeError("Receive invalid event.");
        }

        if (obj == &m_window) {
            if (m_menuBar) {
                m_menuBar->handleWindowEvent(*event);
            }

            if (m_statusBar) {
                m_statusBar->handleWindowEvent(*event);
            }
        }
        else if (qobject_cast<WebView*>(obj)) {
            if (m_menuBar) {
                m_menuBar->handleWebViewEvent(*event);
            }

            if (m_toolbar) {
                m_toolbar->handleWebViewEvent(*event);
            }
        }
        else if (obj == m_navigationContainer) {
            if (m_toolbar) {
                m_toolbar->handleNavigationContainerEvent(*event);
            }
        }
        else if (qobject_cast<LocationBar*>(obj)) {
            if (m_toolbar) {
                m_toolbar->handleLocationBarEvent(*event);
            }
        }

        return QObject::eventFilter(obj, event);
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
        return false;
    }
}

void WindowHandler::
captureWidgets()
{
    const Settings &settings = Plugin::settings();

    if (settings.navigationBar || settings.bookmarksBar) {
        m_toolbar.reset(new Toolbar(m_window));

        if (settings.navigationBar) {
            m_toolbar->capture(navigationBar());
        }

        if (settings.bookmarksBar) {
            m_toolbar->capture(bookmarksBar());
        }
    }

    if (settings.statusBar) {
        m_statusBar.reset(new StatusBar(m_window));
    }
#ifndef Q_OS_MAC
    if (settings.menuBar) {
        m_menuBar.reset(new MenuBar(m_window));
    }
#endif
}

QWidget &WindowHandler::
bookmarksBar() const
{
    QWidget* const widget = m_window.bookmarksToolbar();
    if (!widget) {
        throw RuntimeError("Fail to access bookmarks toolbar.");
    }

    assert(widget);
    return *widget;
}

QWidget &WindowHandler::
navigationBar() const
{
    QWidget* const widget = m_window.navigationBar();
    if (!widget) {
        throw RuntimeError("Fail to access navigation bar.");
    }

    assert(widget);
    return *widget;
}

void WindowHandler::
toolbarSettingChanged(QWidget& widget, bool enabled)
{
    if (!m_toolbar) {
        m_toolbar.reset(new Toolbar(m_window));
    }

    if (enabled) {
        m_toolbar->capture(widget);
    }
    else {
        m_toolbar->restore(widget);
        if (m_toolbar->empty()) {
            m_toolbar.reset();
        }
    }
}

void WindowHandler::
slotSettingChanged(const QString &key)
{
    const Settings &settings = Plugin::settings();

    if (key == Settings::keyNavigationBar) {
        toolbarSettingChanged(navigationBar(), settings.navigationBar);
    }
    else if (key == Settings::keyBookmarksBar) {
        toolbarSettingChanged(bookmarksBar(), settings.bookmarksBar);
    }
    else if (key == Settings::keyStatusBar) {
        if (settings.statusBar) {
            m_statusBar.reset(new StatusBar(m_window));
        }
        else {
            m_statusBar.reset();
        }
    }
#ifndef Q_OS_MAC
    else if (key == Settings::keyMenuBar) {
        if (settings.menuBar) {
            m_menuBar.reset(new MenuBar(m_window));
        }
        else {
            m_menuBar.reset();
        }
    }
#endif
}

void WindowHandler::
slotTabAdded(WebTab &tab)
{
    //qDebug() << __FUNCTION__ << &tab;
    QWidget* const locationBar = tab.locationBar();
    if (!locationBar) {
        throw RuntimeError("Fail to obtain location bar.");
    }
    QWidget* const webView = m_window.weView();
    if (!webView) {
        throw RuntimeError("Fail to obtain Web view.");
    }

    typedef boost::unordered_map<QWidget*, QWidget*>::iterator It;
    const std::pair<It, bool> &result
                        = m_tabInfos.emplace(locationBar, webView);
    if (!result.second) {
        throw InternalError("Fail to register tab.");
    }
    locationBar->installEventFilter(this);
    webView->installEventFilter(this);
}

void WindowHandler::
slotTabDeleted(WebTab &tab)
{
    //qDebug() << __FUNCTION__ << &tab;
    QWidget* const locationBar = tab.locationBar();
    if (!locationBar) {
        throw RuntimeError("Fail to obtain location bar.");
    }

    try {
        QWidget* const webView = m_tabInfos.at(locationBar);
        assert(webView);

        const size_t deleted = m_tabInfos.erase(locationBar);
        assert(deleted);

        locationBar->removeEventFilter(this);
        webView->removeEventFilter(this);
    }
    catch (const std::out_of_range &) {
        throw InternalError("Unregistered tab got deleted.");
    }
}

} // namespace lesschrome
