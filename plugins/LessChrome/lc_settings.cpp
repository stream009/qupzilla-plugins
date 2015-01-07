#include "lc_settings.h"

namespace lesschrome {

const QString Settings::keyNavigationBar = QLatin1String("navigationBar");
const QString Settings::keyBookmarksBar = QLatin1String("bookmarksBar");
const QString Settings::keyStatusBar = QLatin1String("statusBar");
const QString Settings::keyWaitTimer = QLatin1String("waitTimer");

Settings::
Settings(const QString &filename)
    : navigationBar(this),
      bookmarksBar(this),
      statusBar(this),
      waitTimer(this),
      m_settings(filename, QSettings::IniFormat)
{
    m_settings.beginGroup("LessChrome");
}

} // namespace lesschrome
