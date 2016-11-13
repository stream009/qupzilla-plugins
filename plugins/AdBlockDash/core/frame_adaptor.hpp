#ifndef ADBLOCK_DASH_FRAME_ADAPTOR_HPP
#define ADBLOCK_DASH_FRAME_ADAPTOR_HPP

#include <boost/container/flat_map.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>

class QWebElement;
class QWebFrame;

namespace adblock_dash {

class AdBlockDash;

class FrameAdaptor : public QObject
{
    Q_OBJECT
    using Base = QObject;
    using HiddenElementToOriginalStyle
            = boost::container::flat_map<QWebElement*, QString>;

public:
    FrameAdaptor(QWebFrame&, AdBlockDash&);
    ~FrameAdaptor() override;

    void install();
    void uninstall();

    Q_INVOKABLE bool shouldLoad(
            QString const& url, QWebElement const& element);
    Q_INVOKABLE bool shouldLoadWebSocket(const QString &url) const;

private:
    Q_SLOT void installCustomObject();
           void uninstallCustomObject();
           void installWebSocketWrapper();
           void uninstallWebSocketWrapper();
    Q_SLOT void injectElementHidingCss();
           void removeElementHidingCss();
    Q_SLOT void onStatusChanged(bool const enabled);
           void hideElement(QWebElement&);

private:
    QWebFrame &m_frame;
    AdBlockDash &m_adBlockDash;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_FRAME_ADAPTOR_HPP
