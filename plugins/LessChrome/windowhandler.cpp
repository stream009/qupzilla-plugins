#include "windowhandler.h"

#include "bookmarkstoolbar.h"
#include "browserwindow.h"
#include "locationbar.h"
#include "navigationbar.h"
#include "navigationcontainer.h"
#include "tabbar.h"
#include "webview.h"

#include <QtGui/QLayout>
#include <QtGui/QMouseEvent>

namespace lesschrome {

WindowHandler::
WindowHandler(BrowserWindow* const window)
    : m_window(window),
      m_navigationContainer(NULL),
      m_webView(NULL),
      m_tabBar(NULL),
      m_tabWatcher(window),
      m_container(window)
{
    assert(m_window);

    captureWidgets();
    assert(m_navigationContainer);
    assert(m_tabBar);

    this->connect(&m_tabWatcher, SIGNAL(tabAdded(WebTab*)),
                  this,          SLOT(slotTabAdded(WebTab*)));
    this->connect(&m_tabWatcher, SIGNAL(tabDeleted(WebTab*)),
                  this,          SLOT(slotTabDeleted(WebTab*)));
    m_navigationContainer->installEventFilter(this);
    m_tabBar->installEventFilter(this);
}

WindowHandler::
~WindowHandler()
{
    //qDebug() << __FUNCTION__;
    m_navigationContainer->removeEventFilter(this);
    m_tabBar->removeEventFilter(this);

    foreach (QWidget* const locationBar, m_locationBars) {
        locationBar->removeEventFilter(this);
    }
}

void WindowHandler::
mouseMove(QMouseEvent * const event)
{
    assert(event);

    // Ignore unchanged mouse move event. Sometimes it happens.
    if (event->pos() == m_mousePos) return;
    m_mousePos = event->pos();

    //TODO If only mouse events from webview come here,
    //there is no need to translate them.
    QPoint pos = m_container.mapFromGlobal(event->globalPos());

    if (m_container.rect().contains(pos)) {
        if (!m_container.isEntered()) {
            m_container.enter();
        }
    }
    else {
        if (m_container.isEntered()) {
            m_container.leave();
        }
        else if (m_container.isVisible()){
            m_container.hide();
        }
    }

}

bool WindowHandler::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(event);
    if (obj == m_navigationContainer &&
                               event->type() == QEvent::Resize)
    {
        resizeToolBars();
    }
    else if (obj == m_tabBar && event->type() == QEvent::Enter) {
        //qDebug() << "tabBar enter";
        m_container.show();
    }
    else if (qobject_cast<LocationBar*>(obj) != NULL) {
        if (event->type() == QEvent::FocusIn) {
            //qDebug() << "locationbar focus";
            m_container.show();
        }
    }
    return false;
}

void WindowHandler::
captureWidgets()
{
    assert(m_window);

    QWidget* const navigationBar = m_window->findChild<NavigationBar*>();
    if (navigationBar == NULL) {
        qDebug() << "widget";
        throw "widget"; //TODO
    }

    QWidget* const bookmarksToolbar =
                            m_window->findChild<BookmarksToolbar*>();
    if (bookmarksToolbar == NULL) {
        qDebug() << "widget";
        throw "widget"; //TODO
    }

    m_navigationContainer = navigationBar->parentWidget();

    m_webView = m_window->findChild<WebView*>();
    assert(m_webView); //TODO handle more properly

    m_tabBar = m_window->findChild<TabBar*>();
    assert(m_tabBar); //TODO handle more properly

    m_container.capture(navigationBar);
    m_container.capture(bookmarksToolbar);

    assert(m_navigationContainer);
    assert(m_webView);
    assert(m_tabBar);
}

void WindowHandler::
resizeToolBars()
{
    //TODO if Toolbar is WebView's child, this mapping won't necessary.
    QRect webViewRect = m_webView->rect();
    webViewRect.moveTopLeft(
        m_webView->mapTo(m_window, webViewRect.topLeft())
    );

    m_container.setGeometry(
        webViewRect.x(), webViewRect.y(),
        m_navigationContainer->width(), m_container.height()
    );
    //qDebug() << __FUNCTION__ << m_navigationContainer->geometry();
}

void WindowHandler::
slotTabAdded(WebTab* const tab)
{
    assert(tab);
    //qDebug() << __FUNCTION__ << tab;

    LocationBar* const locationBar = tab->locationBar();
    assert(locationBar);

    if (m_locationBars.count(locationBar) == 0) {
        locationBar->installEventFilter(this);
        m_locationBars.insert(locationBar);
    }
}

void WindowHandler::
slotTabDeleted(WebTab* const tab)
{
    assert(tab);
    //qDebug() << __FUNCTION__ << tab;

    LocationBar* const locationBar = tab->locationBar();
    assert(locationBar);

    if (m_locationBars.count(locationBar) == 1) {
        locationBar->removeEventFilter(this);
        m_locationBars.erase(locationBar);
    }
}

} // namespace lesschrome
