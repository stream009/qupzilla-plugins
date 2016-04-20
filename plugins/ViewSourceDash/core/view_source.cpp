#include "view_source.hpp"

#include <boost/filesystem.hpp>

#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtWebKitWidgets/QWebFrame>

namespace view_source_dash {

using Path = ViewSource::Path;

ViewSource::
ViewSource(const QWebFrame &frame, ProcessManager &processManager)
    : QAction {
        QIcon::fromTheme("text-html"),
        tr("View Source"),
        nullptr
      }
      , m_frame { frame }
      , m_processManager { processManager }
{
    this->connect(this, &QAction::triggered,
                  this, &ViewSource::run);
}

void ViewSource::
run()
{
    const auto &path = saveSource();

    m_processManager.launchViewer(path);
}

Path ViewSource::
saveSource() const
{
    namespace bfs = boost::filesystem;

    const auto &path = bfs::temp_directory_path() /
                       bfs::unique_path("%%%%-%%%%-%%%%-%%%%.html");
    assert(!bfs::exists(path));

    std::ofstream ofs { path.c_str() };
    assert(ofs.good());

    const auto &html = m_frame.toHtml().toUtf8();

    ofs << html.constData();
    ofs.close();

    return path;
}

} // namespace view_source_dash
