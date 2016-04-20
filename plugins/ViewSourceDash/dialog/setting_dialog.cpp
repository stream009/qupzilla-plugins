#include "setting_dialog.hpp"

#include <boost/make_unique.hpp>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>

namespace view_source_dash {

SettingDialog::
SettingDialog(Settings &settings, QWidget &parent)
    : QDialog { &parent }
    , m_settings { settings }
{
    layoutWidgets();
    initWidgets();
    connectWidgets();

    validate();
}

void SettingDialog::
layoutWidgets()
{
    auto* const mainLayout = new QVBoxLayout { this };
    assert(mainLayout);

    {
        auto* const layout = new QHBoxLayout;
        assert(layout);

        auto* const label = new QLabel { tr("External Viewer Excutable:") };
        assert(label);
        m_viewer = new QLineEdit;
        assert(m_viewer);

        layout->addWidget(label);
        layout->addWidget(m_viewer);
        mainLayout->addLayout(layout);

        assert(label->parent() == this);
        assert(layout->parent() == mainLayout);
    }

    m_buttonBox = new QDialogButtonBox {
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    };
    assert(m_buttonBox);

    mainLayout->addWidget(m_buttonBox);
}

void SettingDialog::
connectWidgets()
{
    this->connect(m_viewer, &QLineEdit::textEdited,
                  [this] { m_edited = true; });

    this->connect(m_buttonBox, &QDialogButtonBox::accepted,
                  this,        &QDialog::accept);
    this->connect(m_buttonBox, &QDialogButtonBox::rejected,
                  this,        &QDialog::reject);

    this->connect(this, &QDialog::accepted,
                  [this] {
                      if (!m_edited) return;
                      m_settings.setViewer(m_viewer->text());
                  });
}

void SettingDialog::
initWidgets()
{
    m_viewer->setText(m_settings.viewer());
    m_viewer->selectAll();
}

} // namespace view_source_dash
