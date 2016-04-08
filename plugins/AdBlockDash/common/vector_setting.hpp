#ifndef VECTOR_SETTING_HPP
#define VECTOR_SETTING_HPP

#include <utility>
#include <vector>

#include <QtCore/QtGlobal>
#include <QtCore/QVariant>
#include <QtCore/QSettings>

namespace vector_setting {

class DefaultNotifier : public QObject
{
    Q_OBJECT
public:
    ~DefaultNotifier() override;

    void emitAppended(const QVariant &v);
    void emitRemoved(const QVariant v, const size_t index);
    void emitMoved(const QVariant v, const size_t from, const size_t to);

Q_SIGNALS:
    void appended(const QVariant &v);
    void removed(const QVariant v, const size_t index);
    void moved(const QVariant v, const size_t from, const size_t to);
};

template<typename T, typename Notifier = DefaultNotifier>
class VectorSetting
{
    using Entries = std::vector<T>;
public:
    using iterator = typename Entries::iterator;
    using const_iterator = typename Entries::const_iterator;
    using const_reference = typename Entries::const_reference;
    using reference = typename Entries::reference;
    using size_type = typename Entries::size_type;

public:
    // constructors
    VectorSetting(QSettings&, const QString &key);
    ~VectorSetting();

    // accessors
    const_iterator begin() const { return m_entries.begin(); }
    const_iterator end() const { return m_entries.end(); }

    iterator begin() { return m_entries.begin(); }
    iterator end() { return m_entries.end(); }

    const_reference at(const size_type pos) const;
    reference at(const size_type pos);

    const_reference operator[](const size_type pos) const;
    reference operator[](const size_type pos);

    const_reference front() const;
    reference front();

    const_reference back() const;
    reference back();

    Notifier &notifier() { return m_notifier; }

    // query
    size_type size() const { return m_entries.size(); }
    bool empty() const { return m_entries.empty(); }

    // modifiers
    iterator insert(const const_iterator pos, const T&);
    void push_back(const T &value) { insert(end(), value); }

    template<typename... Args>
    iterator emplace(const const_iterator pos, Args&&... args);

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        emplace(end(), std::forward<Args>(args)...);
    }

    void erase(const const_iterator);
    void erase(const const_iterator begin, const const_iterator end);

    void move(const size_t from, const size_t to);

private:
    void load();
    void save();

private:
    QSettings &m_settings;
    QString m_key;
    Entries m_entries;
    Notifier m_notifier;
};

template<typename T>
T
load(QSettings &s)
{
    return s.value("value").value<T>();
}

template<typename T>
void
save(QSettings &s, const T &v)
{
    s.setValue("value", v);
}

} // namespace vector_setting

#include "vector_setting.tcc"

template<typename T, typename Notifier = vector_setting::DefaultNotifier>
using VectorSetting = vector_setting::VectorSetting<T, Notifier>;

#endif // VECTOR_SETTING_HPP
