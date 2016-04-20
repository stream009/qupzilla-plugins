#ifndef VIEW_SOURCE_DASH_VIEW_SOURCE_HPP
#define VIEW_SOURCE_DASH_VIEW_SOURCE_HPP

#include "process_manager.hpp"

#include <boost/filesystem/path.hpp>

#include <QtWidgets/QAction>

class QProcess;
class QWebFrame;

namespace view_source_dash {

class ProcessManager;

class ViewSource : public QAction
{
    Q_OBJECT

public:
    using Path = boost::filesystem::path;
public:
    ViewSource(const QWebFrame &frame, ProcessManager&);

private:
    Q_SLOT void run();

    Path saveSource() const;

private:
    const QWebFrame &m_frame;
    ProcessManager &m_processManager;
};

} // namespace view_source_dash

#endif // VIEW_SOURCE_DASH_VIEW_SOURCE_HPP
