#ifndef STYLIST_ACTION_ADDSTYLE_H
#define STYLIST_ACTION_ADDSTYLE_H

#include <QtGui/QAction>

class QObject;

namespace stylist { namespace action {

class AddStyle : public QAction
{
    Q_OBJECT
public:
    AddStyle(QObject* const parent = nullptr);

private Q_SLOTS:
    void run() const;
};

}} // namespace stylist::action

#endif // STYLIST_ACTION_ADDSTYLE_H
