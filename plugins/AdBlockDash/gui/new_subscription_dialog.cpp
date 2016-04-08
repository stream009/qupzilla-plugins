#include "new_subscription_dialog.hpp"

#include "core/settings.hpp"
#include "ui_new_subscription_dialog.h"

#include <cassert>

#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

namespace adblock_dash {

NewSubscriptionDialog::
NewSubscriptionDialog(Settings &settings, QWidget &parent)
    : Base { &parent },
      m_ui { new Ui::NewSubscriptionDialog },
      m_settings { settings }
{
    assert(m_ui);
    m_ui->setupUi(this);

    assert(m_ui->name);
    assert(m_ui->path);
    assert(m_ui->buttonBox);

    auto* const okButton = m_ui->buttonBox->button(QDialogButtonBox::Ok);
    assert(okButton);
    okButton->setEnabled(false);

    this->connect(m_ui->name, SIGNAL(textChanged(const QString&)),
                  this,         SLOT(onChanged()));
    this->connect(m_ui->path, SIGNAL(textChanged(const QString&)),
                  this,         SLOT(onChanged()));
    this->connect(m_ui->pathButton, SIGNAL(clicked()),
                  this,               SLOT(openFileDialog()));
    this->connect(okButton, SIGNAL(clicked()),
                  this,       SLOT(validate()));
}

NewSubscriptionDialog::~NewSubscriptionDialog() = default;

QString NewSubscriptionDialog::
name() const
{
    return m_ui->name->text();
}

QString NewSubscriptionDialog::
path() const
{
    return m_ui->path->text();
}

void NewSubscriptionDialog::
onChanged()
{
    auto* const okButton = m_ui->buttonBox->button(QDialogButtonBox::Ok);
    assert(okButton);
    okButton->setEnabled(
         !m_ui->name->text().isEmpty() && !m_ui->path->text().isEmpty());
}

void NewSubscriptionDialog::
openFileDialog()
{
    QFileDialog dialog {
        this, tr("Select Subscription"),
        "", tr("Subscriptoin (*.txt)")
    };

    dialog.setFileMode(QFileDialog::ExistingFile);

    const auto &result = dialog.exec();
    if (result == QDialog::Accepted) {
        const auto &filenames = dialog.selectedFiles();
        assert(!filenames.empty());

        m_ui->path->setText(filenames.front());
    }
}

static bool
isFileExists(const QString &path)
{
    namespace bfs = boost::filesystem;
    return bfs::exists({ path.toUtf8() });
}

static bool
isDuplicatePath(const Settings::Subscriptions &subscriptions,
                                                    const QString &path)
{
    const auto &path_ = path.toLocal8Bit();
    const auto &it = boost::find_if(subscriptions,
        [&path_](const Subscription &subscription) {
            return subscription.path() == path_.constData();
        }
    );
    return it != subscriptions.end();
}

static bool
isDuplicateName(const Settings::Subscriptions &subscriptions,
                                                    const QString &name)
{
    const auto &it = boost::find_if(subscriptions,
        [&name](const Subscription &subscription) {
            return subscription.name() == name;
        }
    );
    return it != subscriptions.end();
}


void NewSubscriptionDialog::
validate()
{
    const auto &name = m_ui->name->text();
    assert(!name.isEmpty());
    const auto &path = m_ui->path->text();
    assert(!path.isEmpty());

    if (!isFileExists(path)) {
        QMessageBox::critical(this, "Error", "File doesn't exists.");
        m_ui->path->setFocus();
        m_ui->path->selectAll();
        return;
    }
    if (isDuplicatePath(m_settings.subscriptions(), path)) {
        QMessageBox::critical(this, "Error", "The path is already registered.");
        m_ui->path->setFocus();
        m_ui->path->selectAll();
        return;
    }
    if (isDuplicateName(m_settings.subscriptions(), name)) {
        QMessageBox::critical(this, "Error", "The name is already used.");
        m_ui->name->setFocus();
        m_ui->name->selectAll();
        return;
    }

    this->accept();
}

} // namespace adblock_dash
