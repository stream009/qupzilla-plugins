#ifndef LC_MENUBAR_H
#define LC_MENUBAR_H

#include <QtCore/QObject>

class BrowserWindow;
class QEvent;
class QWidget;

namespace lesschrome {

class MenuBar : public QObject
{
    Q_OBJECT
public:
    MenuBar(BrowserWindow &);

    void handleWebViewEvent(QEvent &);

private:
    // @override QObject
    virtual bool eventFilter(QObject* const, QEvent* const);

    QWidget& menuBar() const;

private slots:
    void slotSettingChanged(const QString &key);

private:
    BrowserWindow &m_window;
    bool m_altPressed;
    int m_keyCode;
};

} // namespace lesschrome

#endif // LC_MENUBAR_H
