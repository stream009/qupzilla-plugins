#include "filter_set.hpp"

#include <boost/filesystem.hpp>

namespace adblock_dash {

FilterSet::FilterSet() = default;

FilterSet::
FilterSet(const QString &name, const Path &path,
                               const bool enabled/*= true*/)
    : m_name { name }
    , m_path { path }
    , m_enabled { enabled }
{
    assert(!m_name.isNull());
    assert(boost::filesystem::exists(m_path));
}

void FilterSetsNotifier::
emitAppended(const FilterSet &value)
{
    Q_EMIT appended(value);
}

void FilterSetsNotifier::
emitRemoved(const FilterSet &value, const size_t index)
{
    Q_EMIT removed(value, index);
}

void FilterSetsNotifier::
emitMoved(const FilterSet &value, const size_t from, const size_t to)
{
    Q_EMIT moved(value, from, to);
}

} // namespace vector_setting

namespace vector_setting {

template<>
adblock_dash::FilterSet
load(QSettings &settings)
{
    namespace bfs = boost::filesystem;

    const auto &name = settings.value("name").toString();
    assert(!name.isEmpty());

    const auto &pathStr = settings.value("path").toString();
    const bfs::path &path { pathStr.toLocal8Bit().constData() };
    assert(bfs::exists(path)); //TODO

    const auto enabled = settings.value("enabled", true).toBool();

    return { name, path, enabled };
}

template<>
void
save(QSettings &settings, const adblock_dash::FilterSet &value)
{
    settings.setValue("name", value.name());
    settings.setValue("path", QString { value.path().c_str() });
    settings.setValue("enabled", value.enabled());
}

} // namespace vector_setting
