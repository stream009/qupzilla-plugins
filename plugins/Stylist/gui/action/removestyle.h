#ifndef STYLIST_GUI_ACTION_REMOVE_H
#define STYLIST_GUI_ACTION_REMOVE_H

#include <boost/filesystem.hpp>

#include <QtWidgets/QAction>

namespace stylist {

class Styles;

namespace action {

class RemoveStyle : public QAction
{
    using Path = boost::filesystem::path;
public:
    RemoveStyle(Styles &, QObject* const parent = nullptr);

    void run(const Path &) const;

private:
    Styles &m_styles;
};

}} // namespace stylist::action

#endif // STYLIST_GUI_ACTION_REMOVE_H
