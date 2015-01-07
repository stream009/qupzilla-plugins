#include "windowhandler.h"

#include "lc_settings.h"
#include "plugin.h"

#include "bookmarkstoolbar.h"
#include "browserwindow.h"
#include "locationbar.h"
#include "navigationbar.h"
#include "tabbar.h"
#include "tabwidget.h"
#include "webview.h"

#include <cassert>

#include <QtGui/QMouseEvent>

namespace lesschrome {

WindowHandler::
WindowHandler(BrowserWindow* const window)
    : m_window(window),
      m_tabBar(NULL),
      m_tabWatcher(window),
      m_mousePos()
{
    assert(m_window);

    captureWidgets();
    assert(m_tabBar);

    this->connect(&m_tabWatcher, SIGNAL(tabAdded(WebTab*)),
                  this,          SLOT(slotTabAdded(WebTab*)));
    this->connect(&m_tabWatcher, SIGNAL(tabDeleted(WebTab*)),
                  this,          SLOT(slotTabDeleted(WebTab*)));
    m_tabBar->installEventFilter(this);
}

void WindowHandler::
mouseMove(QMouseEvent * const event)
{
    // event is in BrowserWindow's coordinate system.
    assert(event);

    // Ignore unchanged mouse move event. Sometimes it happens.
    if (event->pos() == m_mousePos) return;
    m_mousePos = event->pos();

    if (m_toolbar) {
        m_toolbar->mouseMove(m_mousePos);
    }
    if (m_statusBar) {
        m_statusBar->mouseMove(m_mousePos);
    }
}

bool WindowHandler::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(obj);
    assert(event);

    if (obj == m_tabBar && event->type() == QEvent::Enter) {
        //qDebug() << "tabBar enter";
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
    return false;
}

void WindowHandler::
captureWidgets()
{
    assert(m_window);

    TabWidget* const tabWidget = m_window->tabWidget();
    assert(tabWidget);
    m_tabBar = tabWidget->tabBar();
    assert(m_tabBar); //TODO handle more properly

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

    assert(m_tabBar);
}

QWidget &WindowHandler::
bookmarksBar() const
{
    QWidget* const widget = m_window->bookmarksToolbar();
    if (widget == NULL) {
        qDebug() << "widget";
        throw "widget"; //TODO
    }

    assert(widget);
    return *widget;
}

QWidget &WindowHandler::
navigationBar() const
{
    assert(m_window);
    QWidget* const widget = m_window->navigationBar();
    if (widget == NULL) {
        qDebug() << "widget";
        throw "widget"; //TODO
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
}

void WindowHandler::
slotTabAdded(WebTab* const tab)
{
    assert(tab);
    qDebug() << __FUNCTION__ << tab;

    LocationBar* const locationBar = tab->locationBar();
    assert(locationBar);

    typedef boost::unordered_set<QWidget*>::iterator It;
    const std::pair<It, bool> &result = m_locationBars.insert(locationBar);
    if (result.second) {
        locationBar->installEventFilter(this);
    }
}

void WindowHandler::
slotTabDeleted(WebTab* const tab)
{
    assert(tab);
    //qDebug() << __FUNCTION__ << tab;

    LocationBar* const locationBar = tab->locationBar();
    assert(locationBar);

    if (m_locationBars.erase(locationBar)) {
        locationBar->removeEventFilter(this);
    }
}

} // namespace lesschrome
