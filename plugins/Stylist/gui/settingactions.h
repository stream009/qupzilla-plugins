#ifndef STYLIST_SETTING_ACTIONS_H
#define STYLIST_SETTING_ACTIONS_H

#include <QtCore/QObject>

class QAction;

namespace stylist { namespace action {

class OpenStyle;
class AddStyle;
class RemoveStyle;
class RenameStyle;
class EnableStyle;

}} // namespace stylist::action

namespace stylist {

class Styles;

class SettingActions : public QObject
{
    Q_OBJECT
public:
    SettingActions(Styles &styles);

    action::OpenStyle &openStyle() const;
    action::AddStyle &addStyle() const;
    action::RemoveStyle &removeStyle() const;
    action::RenameStyle &renameStyle() const;
    action::EnableStyle &enableStyle() const;

public Q_SLOTS:
    void onStyleSelected(bool);

private:
    // This object create actions and take initial ownership.
    // Ownership might eventually be transfered but this object
    // keep hold and provide reference.
    action::OpenStyle *m_openStyle;
    action::AddStyle *m_addStyle;
    action::RemoveStyle *m_removeStyle;
    action::RenameStyle *m_renameStyle;
    action::EnableStyle *m_enableStyle;
};

} // namespace stylist

#endif // STYLIST_SETTING_ACTIONS_H
