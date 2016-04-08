#ifndef ADBLOCK_DASH_NEW_SUBSCRIPTION_DIALOG_HPP
#define ADBLOCK_DASH_NEW_SUBSCRIPTION_DIALOG_HPP

#include <memory>

#include <QtWidgets/QDialog>

namespace Ui {
class NewSubscriptionDialog;
} // namespace Ui

namespace adblock_dash {

class Settings;

class NewSubscriptionDialog : public QDialog
{
    Q_OBJECT
    using Base = QDialog;
public:
    // constructor
    NewSubscriptionDialog(Settings&, QWidget &parent);
    ~NewSubscriptionDialog() override;

    // query
    QString name() const;
    QString path() const;

private Q_SLOTS:
    void onChanged();
    void openFileDialog();
    void validate();

private:
    std::unique_ptr<Ui::NewSubscriptionDialog> m_ui;
    Settings &m_settings;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_NEW_SUBSCRIPTION_DIALOG_HPP
