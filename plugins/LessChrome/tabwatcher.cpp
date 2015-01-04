#include "tabwatcher.h"

#include "browserwindow.h"
#include "tabwidget.h"

#include <algorithm>
#include <cassert>

#include <boost/function_output_iterator.hpp>

namespace lesschrome {

struct Notifier {
    typedef void (TabWatcher::*Func)(WebTab* const) const;

    Notifier(TabWatcher* const watcher, Func func)
        : m_watcher(watcher), m_func(func)
    {
        assert(m_watcher);
        assert(m_func);
    }

    void operator()(WebTab* const tab) {
        assert(tab);
        (m_watcher->*m_func)(tab);
    }

    TabWatcher *m_watcher;
    Func m_func;
};

TabWatcher::
TabWatcher(BrowserWindow* const window)
    : m_tabWidget(NULL)
{
    assert(window);

    m_tabWidget = window->tabWidget();
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
    QList<WebTab*> tabs = m_tabWidget->allTabs();
    std::sort(tabs.begin(), tabs.end());

    std::set_difference(
        tabs.begin(), tabs.end(),
        m_tabs.begin(), m_tabs.end(),
        boost::make_function_output_iterator(
            Notifier(this, &TabWatcher::notifyAdded))
    );

    std::set_difference(
        m_tabs.begin(), m_tabs.end(),
        tabs.begin(), tabs.end(),
        boost::make_function_output_iterator(
            Notifier(this, &TabWatcher::notifyDeleted))
    );

    m_tabs = tabs;
}

} // namespace lesschrome
