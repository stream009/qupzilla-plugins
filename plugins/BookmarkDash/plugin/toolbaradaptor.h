#ifndef MENU_DASH_TOOL_BAR_ADAPTOR_H
#define MENU_DASH_TOOL_BAR_ADAPTOR_H

#include <memory>

#include <QtCore/QObject>

class BookmarksToolbar;
class BrowserWindow;

namespace bookmark_dash {

class Bar;
class Settings;

class ToolBarAdaptor : public QObject
{
public:
    ToolBarAdaptor(BrowserWindow&, Settings &);
    ~ToolBarAdaptor() override;

private:
    BrowserWindow &m_window;
    BookmarksToolbar *m_original = nullptr;
    bool m_originalVisibility = false;
    std::unique_ptr<Bar> m_bar;
};

} // namespace bookmark_dash

#endif // MENU_DASH_TOOL_BAR_ADAPTOR_H
