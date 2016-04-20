#include "setting_dialog_plugin.hpp"

#include "setting_dialog.hpp"
#include "core/settings.hpp"

#include <boost/make_unique.hpp>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

namespace view_source_dash {

std::unique_ptr<SettingDialog> SettingDialogPlugin::
            create(Settings &settings, QWidget &parent) const
{
    return boost::make_unique<SettingDialog>(settings, parent);
}

} // namespace view_source_dash
