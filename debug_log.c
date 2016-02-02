#include "debug_log.h"

#include <stdio.h>
#include <stdarg.h>

const char *g_logLevelToName[] = {
  "NONE",
  "CRITICAL",
  "ERROR",
  "WARNING",
  "INFO",
  "DEBUG"
};

const char *g_logTypeToName[] = {
  "default",
  "parsing"
};

/* setup debug log defaults */
struct DebugLogTable g_debugLogTable[] = {
    { TDEFAULT, LDEBUG },
      { TPARSE, LDEBUG }
};

void flog(LogType type, LogLevel level, const char *fmt, ...) {
  va_list args;

  if (g_debugLogTable[type].level <= level) {
    /* example: WARNING.parsing: some error */
    puts(g_logLevelToName[level]);
    putchar('.');
    puts(g_logTypeToName[type]);
    puts(": ");
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
  }
}
