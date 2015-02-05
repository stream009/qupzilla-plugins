#ifndef STYLIST_STYLEVIEW_H
#define STYLIST_STYLEVIEW_H

#include "stylesitemmodel.h"

#include <QtGui/QActionGroup>
#include <QtGui/QListView>

class QItemSelection;

namespace stylist {

class StylesView : public QListView
{
    Q_OBJECT
public:
    explicit StylesView(QWidget* const parent = 0);

private Q_SLOTS:
    void openEditor();
    void renameStyle();
    void slotSelectionChanged(const QItemSelection&, const QItemSelection &);

private:
    StylesItemModel m_model;
    QActionGroup m_styleActions;
};

} // namespace stylist

#endif // STYLIST_STYLEVIEW_H
