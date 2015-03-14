#include "toolbaradaptor.h"

#include "bookmark/bar.h"
#include "settings.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QBoxLayout>

#include <bookmarks.h>
#include <bookmarkstoolbar.h>
#include <browserwindow.h>
#include <mainapplication.h>

namespace bookmark_dash {

ToolBarAdaptor::
ToolBarAdaptor(BrowserWindow &window, Settings &settings)
    : m_window { window }
{
    m_original = m_window.bookmarksToolbar();
    assert(m_original);
    auto* const container = m_original->parentWidget();
    assert(container);
    auto* const layout = dynamic_cast<QBoxLayout*>(container->layout());
    assert(layout);

    m_bar.reset(new Bar { m_window, nullptr });
    assert(m_bar);

    layout->insertWidget(layout->indexOf(m_original), m_bar.get());

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);
    auto* const toolBarFolder = bookmarks->toolbarFolder();
    assert(toolBarFolder);

    auto* const model = bookmarks->model();
    assert(model);
    const auto &root = model->index(toolBarFolder);
    assert(root.isValid());

    m_bar->setModel(*model, root);

    m_originalVisibility = m_original->isVisible();

    const auto &setting = settings.bookmarksBar();
    m_bar->setVisible(setting.value());
    if (m_bar->isVisible()) {
        m_original->hide();
    }
    this->connect(&setting,    SIGNAL(changed(bool)),
                  m_bar.get(),   SLOT(setVisible(bool)));
}

ToolBarAdaptor::
~ToolBarAdaptor()
{
    auto* const container = m_bar->parentWidget();
    assert(container);
    auto* const layout = dynamic_cast<QBoxLayout*>(container->layout());
    assert(layout);

    layout->removeWidget(m_bar.get());
    m_original->setVisible(m_originalVisibility);
}

} // namespace bookmark_dash
