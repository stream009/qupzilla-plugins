#ifndef ADBLOCK_DASH_FRAME_ADAPTOR_HPP
#define ADBLOCK_DASH_FRAME_ADAPTOR_HPP

#include <QtCore/QObject>

class QString;
class QWebElement;
class QWebFrame;

namespace adblock_dash {

class AdBlockDash;

class FrameAdaptor : public QObject
{
    Q_OBJECT
    using Base = QObject;
public:
    FrameAdaptor(QWebFrame&, AdBlockDash&);

    Q_INVOKABLE bool shouldLoad(
            const QString &url, const QWebElement &element) const;
    Q_INVOKABLE bool shouldLoadWebSocket(const QString &url) const;

private:
    Q_SLOT void installCustomObject();
    Q_SLOT void injectElementHidingCss();
           void installWebSocketWrapper();

private:
    QWebFrame &m_frame;
    AdBlockDash &m_adBlockDash;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_FRAME_ADAPTOR_HPP
