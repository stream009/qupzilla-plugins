#ifndef STYLIST_GUI_ACTION_OPEN_H
#define STYLIST_GUI_ACTION_OPEN_H

#include <boost/filesystem.hpp>

#include <QtGui/QAction>

namespace stylist { namespace action {

class OpenStyle : public QAction
{
    using Path = boost::filesystem::path;
public:
    OpenStyle(QObject* const parent = nullptr);

    void run(const Path &) const;
};

}} // namespace stylist::action

#endif // STYLIST_GUI_ACTION_OPEN_H
