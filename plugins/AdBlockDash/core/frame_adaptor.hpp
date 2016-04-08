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

private Q_SLOTS:
    void installCustomObject();
    void injectElementHidingCss();

private:
    QWebFrame &m_frame;
    AdBlockDash &m_adBlockDash;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_FRAME_ADAPTOR_HPP
