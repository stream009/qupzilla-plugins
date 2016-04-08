#ifndef ADBLOCK_DASH_PATH_HPP
#define ADBLOCK_DASH_PATH_HPP

#include <boost/filesystem/path.hpp>

#include <QMetaType>

using Path = boost::filesystem::path;

Q_DECLARE_METATYPE(Path)

#endif // ADBLOCK_DASH_PATH_HPP
