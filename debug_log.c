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
  "NETFLOW",
  "PARSE"
};

void snflog(SNFConfig *conf, SNFLogType type, SNFLogLevel level,
          const char *fmt, ...) {
  va_list args;

  if (would_log(conf, type, level)) {
    /* example: WARNING.parsing: some error */
    puts(g_logLevelToName[level]);
    putchar('.');
    puts(g_logTypeToName[type]);
    puts(": ");
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    putchar('\n');
  }
}
