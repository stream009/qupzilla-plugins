#include "error.h"

#include <QtCore/QDebug>

namespace stylist {

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
    // If we can unload this plugin itself it will be the preferable
    // course of action in case of unrecoverable error.
    // But I can't find the way to do that.
}

} // namespace stylist
