#ifndef STYLIST_ACTION_ENABLE_STYLE_H
#define STYLIST_ACTION_ENABLE_STYLE_H

#include <boost/filesystem.hpp>

#include <QtWidgets/QAction>

class QObject;

namespace stylist {

class Styles;

namespace action {

class EnableStyle : public QAction
{
    using Path = boost::filesystem::path;
public:
    explicit EnableStyle(Styles &, QObject* const parent = nullptr);

    void run(const Path &, const bool enabled);

private:
    Styles &m_styles;
};

}} // namespace stylist::action

#endif // STYLIST_ACTION_ENABLE_STYLE_H
