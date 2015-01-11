#include "error.h"

#include <QtCore/QDebug>

namespace lesschrome {

void
defaultExceptionHandler(const char* const fileName,
                        const size_t lineNo,
                        const char* const functionName,
                        const std::exception &e)
{
    qCritical() << QString::fromUtf8(
        "Caught exception at function '%1', in file '%2', line %3\n"
        "Message: %4")
        .arg(functionName).arg(fileName).arg(lineNo).arg(e.what());
}

} // namespace lesschrome
