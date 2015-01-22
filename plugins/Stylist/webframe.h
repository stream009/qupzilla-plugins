#ifndef STYLIST_WEBFRAME_H
#define STYLIST_WEBFRAME_H

#include <QtCore/QObject>

class QWebFrame;

namespace stylist {

class WebFrame : QObject
{
    Q_OBJECT
public:
    WebFrame(QWebFrame* const);

private slots:
    void slotInitialLayoutCompleted();

private:
    QWebFrame *m_frame;
};

} // namespace stylist

#endif // STYLIST_WEBFRAME_H
