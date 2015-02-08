#ifndef STYLIST_GUI_ACTION_RENAME_H
#define STYLIST_GUI_ACTION_RENAME_H

#include <boost/filesystem.hpp>

#include <QtGui/QAction>

namespace stylist {

class Styles;

namespace action {

class RenameStyle : public QAction
{
    using Path = boost::filesystem::path;
public:
    RenameStyle(Styles &, QObject* const parent = nullptr);

    void run(const Path &, const std::string &name);

private:
    Styles &m_styles;
};

}} // namespace stylist::action

#endif // STYLIST_GUI_ACTION_RENAME_H
