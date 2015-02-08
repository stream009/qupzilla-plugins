#include "removestyle.h"

#include "core/styles.h"

namespace stylist { namespace action {

RemoveStyle::
RemoveStyle(Styles &styles, QObject* const parent/* = nullptr*/)
    : QAction { tr("&Remove"), parent },
      m_styles { styles }
{}

void RemoveStyle::
run(const Path &path) const
{
    m_styles.remove(path);
}

}} // namespace stylist::action
