#include "windowhandler.h"

#include "bookmarkstoolbar.h"
#include "browserwindow.h"
#include "locationbar.h"
#include "navigationbar.h"
#include "tabbar.h"
#include "webview.h"

#include <cassert>

#include <QtGui/QMouseEvent>

namespace lesschrome {

WindowHandler::
WindowHandler(BrowserWindow* const window)
    : m_window(window),
      m_tabBar(NULL),
      m_tabWatcher(window),
      m_toolbar(window),
      m_statusBar(window),
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

    m_toolbar.mouseMove(m_mousePos);
    m_statusBar.mouseMove(m_mousePos);
}

bool WindowHandler::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(obj);
    assert(event);

    if (obj == m_tabBar && event->type() == QEvent::Enter) {
        //qDebug() << "tabBar enter";
        m_toolbar.show();
    }
    else if (qobject_cast<LocationBar*>(obj) != NULL) {
        if (event->type() == QEvent::FocusIn) {
            //qDebug() << "locationbar focus";
            m_toolbar.show();
        }
    }
    return false;
}

void WindowHandler::
captureWidgets()
{
    assert(m_window);

    //TODO BrowserWindow::navigationBar()
    QWidget* const navigationBar = m_window->findChild<NavigationBar*>();
    if (navigationBar == NULL) {
        qDebug() << "widget";
        throw "widget"; //TODO
    }

    //TODO BrowserWindow::bookmarksToolbar()
    QWidget* const bookmarksToolbar =
                            m_window->findChild<BookmarksToolbar*>();
    if (bookmarksToolbar == NULL) {
        qDebug() << "widget";
        throw "widget"; //TODO
    }

    m_tabBar = m_window->findChild<TabBar*>();
    assert(m_tabBar); //TODO handle more properly

    m_toolbar.capture(navigationBar);
    m_toolbar.capture(bookmarksToolbar);

    assert(m_tabBar);
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
