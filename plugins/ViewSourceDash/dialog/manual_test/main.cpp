#include "core/settings.hpp"
#include "dialog/setting_dialog.hpp"
#include "dialog/setting_dialog_interface.hpp"

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QPluginLoader>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include <settings.h>

int main(int argc, char *argv[])
{
    QApplication app { argc, argv };
    QPluginLoader loader { "/home/stream/src/qupzilla/bin/plugins/libViewSourceDashSetting.so" };

    using namespace view_source_dash;

    auto* const plugin =
            dynamic_cast<SettingDialogInterface*>(loader.instance());
    if (!plugin) {
        qDebug() << loader.errorString();
        assert(false);
    }

    ::Settings::createSettings("./settings.ini");

    ::view_source_dash::Settings settings { "." };
    qDebug() << settings.dialogPluginPath();

    QWidget parent;
    auto &&dialog = plugin->create(settings, parent);

    return dialog->exec();
}
