#ifndef ACTION_MIME_DATA_H
#define ACTION_MIME_DATA_H

namespace bookmark_dash {

class ActionMimeData : public QMimeData
{
public:
    ActionMimeData(QAction &action)
        : m_action { &action } {}

    QAction &action() const { return *m_action; }

private:
    QAction *m_action;
};

} // namespace bookmark_dash

#endif // ACTION_MIME_DATA_H
