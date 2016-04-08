#ifndef ADBLOCK_DASH_FILTER_LIST_VIEWER_HPP
#define ADBLOCK_DASH_FILTER_LIST_VIEWER_HPP

#include "core/settings.hpp"
#include "filter_list_actions.hpp"
#include "filter_list_view.hpp"

#include <memory>

#include <boost/filesystem/path.hpp>

#include <QtCore/QObject>
#include <QtCore/QPointer>

namespace adblock_dash {

class Settings;
class TextFile;

namespace setting_dialog {

class FilterListModel;

class FilterListViewer : public QObject
{
    Q_OBJECT

    using Path = boost::filesystem::path;
public:
    enum class State { Idle, View, Edit };

public:
    FilterListViewer(const Settings&);
    ~FilterListViewer();

    // accessor
    FilterListView &view() { return *m_view; }
    const FilterListView &view() const { return *m_view; }

    FilterListActions &actions() { return m_actions; }

    // command
    void saveFileIfNecessary();

public Q_SLOTS:
    // modifier
    void setPath(const Path&, const bool editable = false);
    void clear();

Q_SIGNALS:
    void stateChanged(const FilterListViewer::State);
    void requestReload();

private:
    void changeState(const State);

    void validate() const
    {
        assert(m_view);
    }

private:
    const Settings &m_settings;
    QPointer<FilterListView> m_view;
    FilterListActions m_actions;
    std::unique_ptr<TextFile> m_file;
    std::unique_ptr<FilterListModel> m_model;
    State m_state = State::Idle;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_FILTER_LIST_VIEWER_HPP
