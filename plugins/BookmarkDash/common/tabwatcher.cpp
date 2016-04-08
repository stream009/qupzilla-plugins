#include "tabwatcher.h"

#include "error.h"

#include <algorithm>
#include <cassert>

#include <boost/function_output_iterator.hpp>

#include <browserwindow.h>
#include <tabwidget.h>

namespace bookmark_dash {

TabWatcher::
TabWatcher(BrowserWindow &window)
{
    m_tabWidget = window.tabWidget();
    if (!m_tabWidget) {
        throw RuntimeError("Fail to obtain tab widget.");
    }

    this->connect(m_tabWidget, SIGNAL(changed()),
                  this,          SLOT(slotTabChanged()));

    assert(m_tabWidget);
}

// Send signal at the moment of the initial connection
void TabWatcher::
connectNotify(const QMetaMethod&) // throw()
{
    try {
        slotTabChanged();
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

static void
validateWebTabs(const QList<WebTab*> &tabs)
{
    for (auto* const tab: tabs) {
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
        auto &&tabs = m_tabWidget->allTabs();
        validateWebTabs(tabs);

        std::sort(tabs.begin(), tabs.end());

        std::set_difference(
            tabs.begin(), tabs.end(),
            m_tabs.begin(), m_tabs.end(),
            boost::make_function_output_iterator(
                [this] (WebTab *tab) {
                    assert(tab);
                    /* Q_EMIT */ tabAdded(*tab);
                }
            )
        );

        std::set_difference(
            m_tabs.begin(), m_tabs.end(),
            tabs.begin(), tabs.end(),
            boost::make_function_output_iterator(
                [this] (WebTab *tab) {
                    assert(tab);
                    /* Q_EMIT */ tabDeleted(*tab);
                }
            )
        );

        m_tabs = tabs;
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

} // namespace bookmark_dash
