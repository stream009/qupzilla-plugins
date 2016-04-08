#ifndef STYLIST_ACTION_ADDSTYLE_H
#define STYLIST_ACTION_ADDSTYLE_H

#include <QtWidgets/QAction>

class QObject;

namespace stylist {

class Styles;

namespace action {

class AddStyle : public QAction
{
    Q_OBJECT
public:
    AddStyle(Styles &, QObject* const parent = nullptr);

public Q_SLOTS:
    void run() const;

private:
    Styles &m_styles;
};

}} // namespace stylist::action

#endif // STYLIST_ACTION_ADDSTYLE_H
