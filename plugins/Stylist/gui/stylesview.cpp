#include "stylesview.h"

#include "action/addstyle.h"
#include "action/openstyle.h"
#include "action/removestyle.h"
#include "action/renamestyle.h"
#include "settingactions.h"
#include "stylesitemmodel.h"

#include "common/utility.h"
#include "core/styles.h"

#include <boost/make_unique.hpp>

#include <QtWidgets/QAction>
#include <QtCore/QItemSelection>
#include <QtCore/QItemSelectionModel>

namespace stylist {

StylesView::
StylesView(QWidget* const parent /*= 0*/)
    : QListView { parent }
{
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

StylesView::~StylesView() = default;

void StylesView::
setActions(const SettingActions &actions)
{
    m_actions = &actions;
    if (m_model) {
        m_model->setActions(actions);
    }

    auto &addAction = m_actions->addStyle();
    auto &openAction = m_actions->openStyle();
    auto &removeAction = m_actions->removeStyle();
    auto &renameAction = m_actions->renameStyle();

    this->addAction(&addAction);
    this->addAction(&openAction);
    this->addAction(&removeAction);
    this->addAction(&renameAction);

    this->connect(&openAction, SIGNAL(triggered()),
                  this,        SLOT(openCurrentStyle()));
    this->connect(&removeAction, SIGNAL(triggered()),
                  this,          SLOT(removeCurrentStyle()));
    this->connect(&renameAction, SIGNAL(triggered()),
                  this,          SLOT(renameCurrentStyle()));
    this->connect(this,      SIGNAL(styleSelected(bool)),
                  m_actions, SLOT(onStyleSelected(bool)));

    assert(m_actions);
}

void StylesView::
setStyles(Styles &styles)
{
    m_model = boost::make_unique<StylesItemModel>(styles);
    this->setModel(m_model.get());
    if (m_actions) {
        m_model->setActions(*m_actions);
    }

    connectWithSelectionModel();
    // To setup actions' initial state
    onSelectionChanged(QItemSelection {}, QItemSelection {});
}

void StylesView::
openCurrentStyle()
{
    //qDebug() << __func__;
    assert(m_actions);

    const auto current = this->currentIndex();
    assert(current.isValid());

    auto* const model = dynamic_cast<StylesItemModel*>(this->model());
    assert(model);
    const auto &path = model->path(current);

    m_actions->openStyle().run(path);
}

void StylesView::
removeCurrentStyle()
{
    //qDebug() << __func__;
    assert(m_actions);

    const auto current = this->currentIndex();
    assert(current.isValid());

    auto* const model = dynamic_cast<StylesItemModel*>(this->model());
    assert(model);
    const auto &path = model->path(current);

    m_actions->removeStyle().run(path);
}

void StylesView::
renameCurrentStyle()
{
    //qDebug() << __func__;
    const auto current = this->currentIndex();
    assert(current.isValid());

    this->edit(current);
}

void StylesView::
onSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    //qDebug() << __func__;
    auto* const selectionModel = this->selectionModel();
    assert(selectionModel);

    Q_EMIT styleSelected(selectionModel->hasSelection());
}

void StylesView::
connectWithSelectionModel()
{
    auto* const selection = this->selectionModel();
    assert(selection);
    this->connect(selection, SIGNAL(
            selectionChanged(const QItemSelection&, const QItemSelection&)),
                  this,      SLOT(
            onSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

} // namespace stylist
