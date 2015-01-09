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
      m_tabWatcher(window),
      m_mousePos()
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

    m_navigationContainer->installEventFilter(this);

    assert(m_navigationContainer);
}

WindowHandler::
~WindowHandler() {}

void WindowHandler::
mouseMove(const QMouseEvent &event)
{
    // event is in BrowserWindow's coordinate system.

    // Ignore unchanged mouse move event. Sometimes it happens.
    if (event.pos() == m_mousePos) return;
    m_mousePos = event.pos();

    if (m_statusBar) {
        if (m_statusBar->geometry().contains(m_mousePos)) {
            m_statusBar->enter();
        }
        else {
            m_statusBar->leave();
        }
    }
}

//TODO consolidate event filter to here
bool WindowHandler::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(obj);
    assert(event);
    assert(m_navigationContainer);

    // If obj is invalid it just pass through, no need to check it out.
    if (!event) {
        throw RuntimeError("Receive invalid event.");
    }

    if (qobject_cast<WebView*>(obj)) {
        if (event->type() == QEvent::Enter && m_toolbar) {
            m_toolbar->hide();
        }

        if (m_menuBar) {
            m_menuBar->handleWebViewEvent(*event);
        }
    }
    else if (obj == m_navigationContainer && event->type() == QEvent::Enter) {
        //qDebug() << "navigationContainer enter";
        if (m_toolbar) {
            m_toolbar->show();
        }
    }
    else if (qobject_cast<LocationBar*>(obj) != NULL) {
        if (event->type() == QEvent::FocusIn) {
            //qDebug() << "locationbar focus";
            if (m_toolbar) {
                m_toolbar->show();
            }
        }
    }
    return QObject::eventFilter(obj, event);
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

    if (settings.menuBar) {
        m_menuBar.reset(new MenuBar(m_window));
    }
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
    else if (key == Settings::keyMenuBar) {
        if (settings.menuBar) {
            m_menuBar.reset(new MenuBar(m_window));
        }
        else {
            m_menuBar.reset();
        }
    }
}

void WindowHandler::
slotTabAdded(WebTab &tab)
{
    qDebug() << __FUNCTION__ << &tab;
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
    if (result.second) {
        locationBar->installEventFilter(this);
        webView->installEventFilter(this);
    }
}

void WindowHandler::
slotTabDeleted(WebTab &tab)
{
    qDebug() << __FUNCTION__ << &tab;
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
