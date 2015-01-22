#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/make_unique.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/any_range.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/size.hpp>
#include <boost/regex.hpp>
#include <boost/utility/string_ref.hpp>

#include <QtCore/QObject>
#include <QtGui/QApplication>
#include <QtTest/QtTest>
