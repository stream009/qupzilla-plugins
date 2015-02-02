#include "buffereddirectorywatcher.h"

namespace stylist {

void BufferedDirectoryWatcher::
init()
{
    establishPipeline(m_watcher, SIGNAL(fileAdded(const Path&)),
                      m_addConverter, m_addBuffer,
                      *this, SIGNAL(fileAdded(const Path&)));
    establishPipeline(m_watcher, SIGNAL(fileDeleted(const Path&)),
                      m_deleteConverter, m_deleteBuffer,
                      *this, SIGNAL(fileDeleted(const Path&)));
    establishPipeline(m_watcher, SIGNAL(fileModified(const Path&)),
                      m_modifyConverter, m_modifyBuffer,
                      *this, SIGNAL(fileModified(const Path&)));
}

void BufferedDirectoryWatcher::
establishPipeline(QObject &origin, const char* const signal,
                  const SignalConverter &converter,
                  const SignalBuffer &buffer,
                  QObject &destination, const char* const method)
{
    this->connect(&origin,      signal,
                  &converter,   SLOT(from(const Path&)));
    this->connect(&converter,   SIGNAL(into(const QVariant&)),
                  &buffer,      SLOT(slotTriggered(const QVariant&)));
    this->connect(&buffer,      SIGNAL(triggered(const QVariant&)),
                  &converter,   SLOT(from(const QVariant&)));
    this->connect(&converter,   SIGNAL(into(const Path&)),
                  &destination, method);
}

} // namespace stylist
