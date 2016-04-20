#ifndef VIEW_SORUCE_DASH_DIALOG_PLUGIN_INTERFACE_HPP
#define VIEW_SORUCE_DASH_DIALOG_PLUGIN_INTERFACE_HPP

#include <memory>

#include <QtPlugin>

class QWidget;

namespace view_source_dash {

class Settings;
class SettingDialog;

class SettingDialogInterface
{
public:
    virtual ~SettingDialogInterface() = default;

    virtual std::unique_ptr<SettingDialog>
                            create(Settings&, QWidget &parent) const = 0;
};

} // namespace view_source_dash

Q_DECLARE_INTERFACE(view_source_dash::SettingDialogInterface, "ViewSourceDash.SettingDialogInterface/1.2")

#endif // VIEW_SORUCE_DASH_DIALOG_PLUGIN_INTERFACE_HPP
