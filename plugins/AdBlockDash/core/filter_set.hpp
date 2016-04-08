#ifndef ADBLOCK_DASH_FILTER_SET_HPP
#define ADBLOCK_DASH_FILTER_SET_HPP

#include "common/path.hpp"

#include <boost/filesystem.hpp>

#include <QtCore/QString>

namespace adblock_dash {

class FilterSet
{
public:
    FilterSet();
    FilterSet(const QString &name, const Path &path,
                                   const bool enabled = true);

    // accessor
    const QString &name() const { return m_name; }
    const Path    &path() const { return m_path; }
    bool          enabled() const { return m_enabled; }

    // modifier
    void setName(const QString &name) { m_name = name; }
    void setPath(const Path    &path) { m_path = path; }
    void setEnabled(const bool enabled) { m_enabled = enabled; }

private:
    QString m_name;
    Path    m_path;
    bool    m_enabled;
};

} // namespace adblock_dash

#include "common/vector_setting.hpp"

#include <QtCore/QObject>

namespace adblock_dash {

class FilterSetsNotifier : public QObject
{
    Q_OBJECT
public:
    void emitAppended(const FilterSet&);
    void emitRemoved(const FilterSet&, const size_t index);
    void emitMoved(const FilterSet&, const size_t from, const size_t to);

Q_SIGNALS:
    void appended(const FilterSet&);
    void removed(const FilterSet&, const size_t index);
    void moved(const FilterSet&, const size_t from, const size_t to);
};

} // namespace adblock_dash

namespace vector_setting {

template<>
adblock_dash::FilterSet load(QSettings &settings);

template<>
void save(QSettings&, const adblock_dash::FilterSet&);

} // namespace vector_setting

#endif // ADBLOCK_DASH_FILTER_SET_HPP
