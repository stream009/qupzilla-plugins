#include "tabwatcher.h"

#include "browserwindow.h"
#include "tabwidget.h"

#include <algorithm>
#include <cassert>

#include <boost/function_output_iterator.hpp>

namespace lesschrome {

template<void (TabWatcher::*Func)(WebTab* const) const>
struct Notifier {

    Notifier(TabWatcher* const watcher)
        : m_watcher(watcher)
    {
        assert(m_watcher);
    }

    void operator()(WebTab* const tab) {
        assert(tab);
        (m_watcher->*Func)(tab);
    }

    TabWatcher *m_watcher;
};

TabWatcher::
TabWatcher(BrowserWindow &window)
    : m_tabWidget(NULL)
{
    m_tabWidget = window.tabWidget();
    assert(m_tabWidget); //TODO do better

    this->connect(m_tabWidget, SIGNAL(changed()),
                  this,        SLOT(slotTabChanged()));
}

void TabWatcher::
connectNotify(const char* const)
{
    slotTabChanged();
}

void TabWatcher::
notifyAdded(WebTab* const tab) const
{
    emit tabAdded(tab);
}

void TabWatcher::
notifyDeleted(WebTab* const tab) const
{
    emit tabDeleted(tab);
}

void TabWatcher::
slotTabChanged()
{
    assert(m_tabWidget);

    QList<WebTab*> tabs = m_tabWidget->allTabs();
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

} // namespace lesschrome
