#include "renamestyle.h"

#include "core/styles.h"

namespace stylist { namespace action {

RenameStyle::
RenameStyle(Styles &styles, QObject* const parent/* = nullptr*/)
    : QAction { tr("Re&name"), parent },
      m_styles { styles }
{}

void RenameStyle::
run(const Path &path, const std::string &name)
{
    auto &style = m_styles.at(path);
    style.setName(name);
}

}} // namespace stylist::action
