#include "vector_setting.hpp"

#include "utility.hpp"

#include <QtCore/QSettings>

namespace vector_setting {

template<typename T, typename Notifier>
VectorSetting<T, Notifier>::
VectorSetting(QSettings &settings, const QString &key)
    : m_settings { settings }
    , m_key { key }
{
    load();
}

template<typename T, typename Notifier>
VectorSetting<T, Notifier>::
~VectorSetting()
{
    save();
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::const_reference VectorSetting<T, Notifier>::
at(const size_type pos) const
{
    return m_entries.at(pos);
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::reference VectorSetting<T, Notifier>::
at(const size_type pos)
{
    return m_entries.at(pos);
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::const_reference VectorSetting<T, Notifier>::
operator[](const size_type pos) const
{
    assert(pos < m_entries.size());
    return m_entries[pos];
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::reference VectorSetting<T, Notifier>::
operator[](const size_type pos)
{
    assert(pos < m_entries.size());
    return m_entries[pos];
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::const_reference VectorSetting<T, Notifier>::
front() const
{
    return m_entries.front();
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::reference VectorSetting<T, Notifier>::
front()
{
    return m_entries.front();
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::const_reference VectorSetting<T, Notifier>::
back() const
{
    return m_entries.back();
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::reference VectorSetting<T, Notifier>::
back()
{
    return m_entries.back();
}

template<typename T, typename Notifier>
typename VectorSetting<T, Notifier>::iterator VectorSetting<T, Notifier>::
insert(const const_iterator pos, const T &data)
{
    const auto it = m_entries.insert(pos, data);

    m_notifier.emitAppended(*it);
    return it;
}

template<typename T, typename Notifier>
template<typename... Args>
typename VectorSetting<T, Notifier>::iterator VectorSetting<T, Notifier>::
emplace(const const_iterator pos, Args&&... args)
{
    const auto it = m_entries.emplace(pos, std::forward<Args>(args)...);

    m_notifier.emitAppended(*it);
    return it;
}

template<typename T, typename Notifier>
void VectorSetting<T, Notifier>::
erase(const const_iterator it)
{
    const auto &index = ::toSizeT(std::distance(m_entries.cbegin(), it));
    const auto copy = *it;
    m_entries.erase(it);

    m_notifier.emitRemoved(copy, index);
}

template<typename T, typename Notifier>
void VectorSetting<T, Notifier>::
erase(const const_iterator begin, const const_iterator end)
{
    for (auto it = begin; it != end; ++it) {
        const auto &index = ::toSizeT(std::distance(m_entries.cbegin(), it));
        const auto copy = *it;
        m_entries.erase(it);

        m_notifier.emitRemoved(copy, index);
    }
}

template<typename T, typename Notifier>
void VectorSetting<T, Notifier>::
move(const size_t from, const size_t to)
{
    assert(from < m_entries.size());
    assert(to < m_entries.size());
    assert(from != to);

    const auto &srcIt = m_entries.begin() + ::toSignedInt(from);
    const auto copy = *srcIt;
    m_entries.erase(srcIt);

    const auto &dstIt = m_entries.begin() + ::toSignedInt(to);
    m_entries.insert(dstIt, copy);

    m_notifier.emitMoved(copy, from, to);
}

template<typename T, typename Notifier>
void VectorSetting<T, Notifier>::
load()
{
    const auto len = m_settings.beginReadArray(m_key);

    for (auto i = 0; i < len; ++i) {
        m_settings.setArrayIndex(i);
        m_entries.push_back(vector_setting::load<T>(m_settings));
    }

    m_settings.endArray();
}

template<typename T, typename Notifier>
void VectorSetting<T, Notifier>::
save()
{
    m_settings.beginWriteArray(m_key);

    auto const len = m_entries.size();
    for (auto i = 0u; i < len; ++i) {
        m_settings.setArrayIndex(::toSignedInt(i));
        vector_setting::save(m_settings, m_entries[i]);
    }

    m_settings.endArray();
}

} // namespace vector_setting
