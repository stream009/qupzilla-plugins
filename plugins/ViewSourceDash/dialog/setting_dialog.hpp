#ifndef VIEW_SOURCE_DASH_SETTING_DIALOG_HPP
#define VIEW_SOURCE_DASH_SETTING_DIALOG_HPP

#include "core/settings.hpp"

#include <cassert>
#include <memory>

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>

class QWidget;

namespace view_source_dash {

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    SettingDialog() = default;
    SettingDialog(Settings&, QWidget &parent);

private:
    void layoutWidgets();
    void connectWidgets();
    void initWidgets();

private:
    Settings &m_settings;
    QLineEdit *m_viewer = nullptr;
    QDialogButtonBox *m_buttonBox = nullptr;
    bool m_edited = false;

    void validate()
    {
        assert(m_viewer);
        assert(m_viewer->parent() == this);

        assert(m_buttonBox);
        assert(m_buttonBox->parent() == this);
    }
};

} // namespace view_source_dash

#endif // VIEW_SOURCE_DASH_SETTING_DIALOG_HPP
