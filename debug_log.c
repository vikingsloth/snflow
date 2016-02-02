#include "debnug_log.h"

#include <stdio.h>
#include <stdarg.h>

void log(LogType type, LogLevel level, const char *fmt, ...) {
  va_list args;

  if (g_debugLogTable[type].level <= level) {
    /* example: WARNING.parsing: some error */
    puts(g_logLevelToName[level]);
    putc('.');
    puts(g_logTypeToName[type]);
    puts(": ");
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
  }
}
