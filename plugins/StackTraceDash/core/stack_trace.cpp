#include "stack_trace.hpp"

#include <utility>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include <boost/optional.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>

#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QDebug>

namespace stack_trace_dash {

static boost::optional<std::pair<QString, size_t>>
getLine(QString const& filename, void* const address)
{
    auto const& cmdLine = QString("/usr/bin/addr2line -e %1 0x%2")
        .arg(filename)
        .arg(reinterpret_cast<qulonglong>(address), 0, 16)
        ;

    QProcess process;
    process.start(cmdLine);

    if (process.waitForFinished()) {
        QString result = process.readAllStandardOutput();
        result.remove("\n");

        if (result.contains("?")) {
            return {};
        }
        else {
            auto const idx = result.lastIndexOf(':');
            if (idx == -1) return {};

            return std::make_pair(
                result.left(idx),
                static_cast<size_t>(result.mid(idx+1).toUInt())
            );
        }
    }
    else {
        return {};
    }
}

static void*
getOffset(void* const address, void* const base)
{
    return reinterpret_cast<void*>(
        static_cast<char*>(address) - static_cast<char*>(base)
    );
}

static std::string
demangle(const char *mangled)
{
    if (!mangled) return {};

    int status = 0;
    char *demangled =
        abi::__cxa_demangle(mangled, nullptr, nullptr, &status);

    assert(status != 0 || demangled != nullptr);

    std::string result {
        demangled ? demangled : mangled
    };
    free(demangled);

    return result;
}

static std::string
toStdString(QString const& str)
{
    if (str.isEmpty()) return {};

    auto const& utf8 = str.toUtf8();
    assert(utf8.constData() != nullptr && utf8.size() != 0);

    return { utf8.constData(), boost::numeric_cast<size_t>(utf8.size()) };
}

static QString
toQString(std::string const& str)
{
    return QString::fromUtf8(str.c_str(), str.size());
}


StackTrace::
StackTrace(size_type const size, size_type const ignore)
    : m_addrs { size }
    , m_ignore { ignore }
{
    assert(size > ignore);
    m_numTrace = backtrace(m_addrs.data(), size);
}

StackTrace::Items& StackTrace::
items() const
{
    if (m_items) return *m_items;

    m_items = std::vector<Item> {};
    for (size_type i = 0; i < m_numTrace; ++i) {
        auto* const address = m_addrs[i];

        Dl_info info;
        if (::dladdr(address, &info) == 0) {
            auto* const err = dlerror();
            if (err) {
                throw std::runtime_error { err };
            }
            else {
                std::cerr << "StackTrace: can't resolve address: "
                    << std::hex << address << std::endl;
                continue;
            }
        }
        assert(info.dli_fname);

        boost::optional<std::string> functionName;
        if (info.dli_sname) {
            functionName = demangle(info.dli_sname);
        };
        const Item::ObjectInfo objInfo {
            address,
            info.dli_fbase,
            info.dli_fname,
            functionName
        };

        boost::optional<Item::SourceInfo> sourceInfo;

        auto const& line =
            getLine(info.dli_fname, getOffset(address, info.dli_fbase));

        if (line) {
            auto &&filename = toStdString(line->first);
            const auto lineNo = line->second;

            sourceInfo = Item::SourceInfo {
                std::move(filename), lineNo
            };
        }

        m_items->push_back(Item { objInfo, sourceInfo });
    }

    return *m_items;
}

QTextStream&
operator<<(QTextStream& stream, StackTrace const& trace)
{
    auto const size = trace.size();
    auto const ignore = trace.ignore();

    for (auto i = ignore; i < size; ++i) {
        auto const& item = trace[i];

        stream << QString("#%1: %2, ")
            .arg(i)
            .arg(toQString(item.object.filename));

        if (item.object.functionName) {
            stream << QString("%1")
                .arg(toQString(item.object.functionName.get()));
        } else {
            stream << QString("0x%1")
                .arg(
                    reinterpret_cast<qulonglong>(
                        getOffset(item.object.address,
                                  item.object.baseAddress)),
                    0, 16
                );
        }

        if (item.source) {
            stream << QString("\n\tat line %1 of %2")
                .arg(item.source->lineNo)
                .arg(toQString(item.source->filename));
        }

        stream << "\n";
    }

    return stream;
}

} // namespace stack_trace_dash
