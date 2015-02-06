#ifndef STYLIST_STYLEVIEW_H
#define STYLIST_STYLEVIEW_H

#include "stylesitemmodel.h"

#include <boost/filesystem.hpp>

#include <QtGui/QActionGroup>
#include <QtGui/QListView>

class QItemSelection;

namespace stylist {

class StylesView : public QListView
{
    Q_OBJECT
    using Path = boost::filesystem::path;
public:
    explicit StylesView(QWidget* const parent = 0);

Q_SIGNALS:
    void openStyle(const QModelIndex&);
    void openStyle(const Path&);

private Q_SLOTS:
    void slotOpenStyle();
    void renameStyle();
    void slotSelectionChanged(const QItemSelection&, const QItemSelection &);

private:
    void createActions();
    void connectWithModel();
    void connectWithSelectionModel();

private:
    StylesItemModel m_model;
    QActionGroup m_styleActions;
};

} // namespace stylist

#endif // STYLIST_STYLEVIEW_H
