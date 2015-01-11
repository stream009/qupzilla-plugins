#include "tabwatcher.h"

#include "error.h"

#include "browserwindow.h"
#include "tabwidget.h"

#include <algorithm>
#include <cassert>

#include <boost/function_output_iterator.hpp>

namespace lesschrome {

template<void (TabWatcher::*Func)(WebTab&) const>
struct Notifier {

    Notifier(TabWatcher* const watcher)
        : m_watcher(watcher)
    {
        assert(m_watcher);
    }

    void operator()(WebTab* const tab) {
        assert(tab);
        (m_watcher->*Func)(*tab);
    }

    TabWatcher *m_watcher;
};

TabWatcher::
TabWatcher(BrowserWindow &window)
    : m_tabWidget(NULL)
{
    m_tabWidget = window.tabWidget();
    if (!m_tabWidget) {
        throw RuntimeError("Fail to obtain tab widget.");
    }

    this->connect(m_tabWidget, SIGNAL(changed()),
                  this,        SLOT(slotTabChanged()));

    assert(m_tabWidget);
}

void TabWatcher::
connectNotify(const char* const) // throw()
{
    try {
        slotTabChanged();
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void TabWatcher::
notifyAdded(WebTab &tab) const
{
    // These indirection are due to inability of Qt's signal system
    // to work with template. It won't be necessary if we can use
    // boost::signal2 but we can't. Because if we put no_keyword to
    // project file to prevent Qt from using signal as reserved word,
    // Qupzilla's header files included from this file will got errors.
    emit tabAdded(tab);
}

void TabWatcher::
notifyDeleted(WebTab &tab) const
{
    emit tabDeleted(tab);
}

static void
validateWebTabs(const QList<WebTab*> &tabs) {
    foreach (WebTab* const tab, tabs) {
        if (!tab) {
            throw RuntimeError("Receive invalid WebTab pointer.");
        }
    }
}

void TabWatcher::
slotTabChanged() // throw()
{
    assert(m_tabWidget);

    try {
        QList<WebTab*> tabs = m_tabWidget->allTabs();
        validateWebTabs(tabs);

        std::sort(tabs.begin(), tabs.end());

        std::set_difference(
            tabs.begin(), tabs.end(),
            m_tabs.begin(), m_tabs.end(),
            boost::make_function_output_iterator(
                Notifier<&TabWatcher::notifyAdded>(this))
        );

        std::set_difference(
            m_tabs.begin(), m_tabs.end(),
            tabs.begin(), tabs.end(),
            boost::make_function_output_iterator(
                Notifier<&TabWatcher::notifyDeleted>(this))
        );

        m_tabs = tabs;
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

} // namespace lesschrome
