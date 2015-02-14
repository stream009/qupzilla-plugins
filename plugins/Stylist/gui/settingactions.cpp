#include "settingactions.h"

#include "action/addstyle.h"
#include "action/enablestyle.h"
#include "action/openstyle.h"
#include "action/removestyle.h"
#include "action/renamestyle.h"

#include "core/styles.h"

#include <cassert>

namespace stylist {

SettingActions::
SettingActions(Styles &styles)
    : m_openStyle { new action::OpenStyle { this } },
      m_addStyle { new action::AddStyle { styles, this } },
      m_removeStyle { new action::RemoveStyle { styles, this } },
      m_renameStyle { new action::RenameStyle { styles, this } },
      m_enableStyle { new action::EnableStyle { styles, this } }
{
    assert(m_openStyle);
    assert(m_addStyle);
    assert(m_removeStyle);
    assert(m_renameStyle);
    assert(m_enableStyle);
}

action::OpenStyle &SettingActions::
openStyle() const
{
    assert(m_openStyle);
    return *m_openStyle;
}

action::AddStyle &SettingActions::
addStyle() const
{
    assert(m_addStyle);
    return *m_addStyle;
}

action::RemoveStyle &SettingActions::
removeStyle() const
{
    assert(m_removeStyle);
    return *m_removeStyle;
}

action::RenameStyle &SettingActions::
renameStyle() const
{
    assert(m_renameStyle);
    return *m_renameStyle;
}

action::EnableStyle &SettingActions::
enableStyle() const
{
    assert(m_enableStyle);
    return *m_enableStyle;
}

void SettingActions::
onStyleSelected(const bool selected)
{
    m_openStyle->setEnabled(selected);
    m_removeStyle->setEnabled(selected);
    m_renameStyle->setEnabled(selected);
}

} // namespace stylist
