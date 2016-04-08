#include "filter_list_viewer.hpp"

#include "core/text_file.hpp"
#include "filter_list_model.hpp"

#include <cassert>

#include <boost/filesystem.hpp>

#include <QtWidgets/QMessageBox>

namespace adblock_dash { namespace setting_dialog {

FilterListViewer::
FilterListViewer(const Settings &settings)
    : m_settings { settings }
    , m_view    { new FilterListView { nullptr } }
    , m_actions { *m_view }
{
    assert(m_view && m_view->parent() == nullptr);
}

FilterListViewer::
~FilterListViewer()
{
    if (m_view != nullptr && m_view->parent() == nullptr) {
        m_view->deleteLater();
    }
}

void FilterListViewer::
setPath(const Path &path, const bool editable/*= false*/)
{
    namespace bfs = boost::filesystem;
    assert(!path.empty());
    assert(bfs::exists(path));

    saveFileIfNecessary();

    m_file.reset(new TextFile { path });
    assert(m_file);
    m_model.reset(new FilterListModel { m_settings, *m_file, editable });
    assert(m_model);

    m_view->setModel(m_model.get());

    changeState(editable ? State::Edit : State::View);
}

void FilterListViewer::
clear()
{
    saveFileIfNecessary();

    m_file.reset();
    m_model.reset();
    m_view->setModel(nullptr);

    changeState(State::Idle);
}

void FilterListViewer::
saveFileIfNecessary()
{
    if (!m_file || !m_file->edited()) return;

    const auto &result = QMessageBox::question(
        m_view->window(),
        tr("Save"),
        tr("Filter has edited. Do you want to save it?"),
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (result == QMessageBox::Ok) {
        m_file->save();
        Q_EMIT requestReload();
    }
}

void FilterListViewer::
changeState(const State newState)
{
    if (m_state == newState) return;

    switch (m_state) {
    case State::Idle:
        assert(newState == State::View || newState == State::Edit);
        break;
    case State::View:
    case State::Edit:
        assert(newState == State::Idle);
        break;
    default:
        assert(false);
    }

    m_state = newState;
    Q_EMIT stateChanged(newState);
}

}} // namespace adblock_dash::setting_dialog
