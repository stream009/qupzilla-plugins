#include "settings.h"

namespace bookmark_dash {

const QString Settings::keyNavigationBar = QLatin1String("navigationBar");
const QString Settings::keyBookmarksBar = QLatin1String("bookmarksBar");
const QString Settings::keyStatusBar = QLatin1String("statusBar");
const QString Settings::keyMenuBar = QLatin1String("menuBar");
const QString Settings::keyMenuBarKey = QLatin1String("menuBarKey");
const QString Settings::keyWaitTimer = QLatin1String("waitTimer");

Settings::
Settings(const QString &filename)
    : navigationBar { *this },
      bookmarksBar { *this },
      statusBar { *this },
      menuBar { *this },
      menuBarKey { *this },
      waitTimer { *this },
      m_settings { filename, QSettings::IniFormat }
{
    m_settings.beginGroup("BookmarkDash");
}

} // namespace bookmark_dash
