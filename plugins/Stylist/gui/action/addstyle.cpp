#include "addstyle.h"

#include "core/styles.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QFileDialog>

namespace stylist { namespace action {

AddStyle::
AddStyle(Styles &styles, QObject* const parent/* = nullptr */)
    : QAction { tr("&Add"), parent },
      m_styles { styles }
{
    this->connect(this, SIGNAL(triggered()),
                  this, SLOT(run()));
}

void AddStyle::
run() const
{
    const auto &widgets = this->associatedWidgets();
    assert(!widgets.empty());

    const auto &filename = QFileDialog::getOpenFileName(
        widgets.front(),
        tr("Select style sheet to add"),
        "",
        tr("Style Sheet (*.css)")
    );
    if (filename.isNull()) return;

    //qDebug() << __func__ << filename;
    m_styles.import(filename.toUtf8().constData());
}

}} // namespace stylist::action
