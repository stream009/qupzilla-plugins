#ifndef STYLIST_STYLEVIEW_H
#define STYLIST_STYLEVIEW_H

#include <memory>

#include <QtGui/QListView>

class QItemSelection;

namespace stylist {

class SettingActions;
class Styles;
class StylesItemModel;

class StylesView : public QListView
{
    Q_OBJECT
public:
    StylesView(QWidget* const parent = 0);
    ~StylesView();

    void setActions(const SettingActions &);
    void setStyles(Styles &);

Q_SIGNALS:
    void styleSelected(bool);

private Q_SLOTS:
    void openCurrentStyle();
    void removeCurrentStyle();
    void renameCurrentStyle();
    void onSelectionChanged(const QItemSelection&, const QItemSelection &);

private:
    void connectWithSelectionModel();

private:
    std::unique_ptr<StylesItemModel> m_model;
    const SettingActions *m_actions = nullptr;
};

} // namespace stylist

#endif // STYLIST_STYLEVIEW_H
