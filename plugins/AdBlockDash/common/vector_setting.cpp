#include "vector_setting.hpp"

namespace vector_setting {

DefaultNotifier::~DefaultNotifier() = default;

void DefaultNotifier::
emitAppended(const QVariant &v)
{
    Q_EMIT appended(v);
}

void DefaultNotifier::
emitRemoved(const QVariant v, const size_t index)
{
    Q_EMIT removed(v, index);
}

void DefaultNotifier::
emitMoved(const QVariant v, const size_t from, const size_t to)
{
    Q_EMIT moved(v, from, to);
}

} // namespace vector_setting
