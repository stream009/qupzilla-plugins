#include "windowhandler.h"

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
      m_webView(NULL),
      m_tabWatcher(window),
      m_mousePos()
{
    m_navigationContainer = navigationBar().parentWidget();
    assert(m_navigationContainer); //TODO better

    captureWidgets();

    this->connect(&m_tabWatcher, SIGNAL(tabAdded(WebTab*)),
                  this,          SLOT(slotTabAdded(WebTab*)));
    this->connect(&m_tabWatcher, SIGNAL(tabDeleted(WebTab*)),
                  this,          SLOT(slotTabDeleted(WebTab*)));

    m_navigationContainer->installEventFilter(this);

    m_webView = window.weView();
    assert(m_webView);
    m_webView->installEventFilter(this);
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

bool WindowHandler::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(obj);
    assert(event);
    assert(m_webView);
    assert(m_navigationContainer);

    if (obj == m_webView && event->type() == QEvent::Enter) {
        if (m_toolbar) {
            m_toolbar->hide();
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
    QWidget* const widget = m_window.navigationBar();
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
slotTabAdded(WebTab* const tab)
{
    assert(tab);
    //qDebug() << __FUNCTION__ << tab;

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
