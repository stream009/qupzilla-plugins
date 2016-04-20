#ifndef VIEW_SOURCE_DASH_SETTING_DIALOG_PLUGIN_HPP
#define VIEW_SOURCE_DASH_SETTING_DIALOG_PLUGIN_HPP

#include "setting_dialog_interface.hpp"

#include <memory>

#include <QtCore/QObject>

class QWidget;

namespace view_source_dash {

class Settings;
class SettingDialog;

class SettingDialogPlugin : public QObject, public SettingDialogInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.ViewSourceDashSetting")
    Q_INTERFACES(view_source_dash::SettingDialogInterface)

public:
    // @override SettingDialogInterface
    std::unique_ptr<SettingDialog>
                create(Settings &settings, QWidget &parent) const override;
};

} // namespace view_source_dash

#endif // VIEW_SOURCE_DASH_SETTING_DIALOG_PLUGIN_HPP
