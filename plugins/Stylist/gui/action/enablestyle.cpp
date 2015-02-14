#include "enablestyle.h"

#include "core/styles.h"

namespace stylist { namespace action {

EnableStyle::
EnableStyle(Styles &style, QObject* const parent/* = nullptr*/)
    : QAction { parent },
      m_styles { style }
{}

void EnableStyle::
run(const Path &path, const bool enabled)
{
    auto &style = m_styles.at(path);
    style.setEnabled(enabled);
}

}} // namespace stylist::action
