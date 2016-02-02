#ifndef __SNFLOW_DEBUG_LOG_H
#define __SNFLOW_DEBUG_LOG_H

typedef enum {
  L_NONE = 0,
  L_CRITICAL = 1,
  L_ERROR = 2,
  L_WARNING = 3,
  L_INFO = 4,
  L_DEBUG = 5
} LogLevel;

const char *g_logLevelToName[] = {
  "NONE",
  "CRITICAL",
  "ERROR",
  "WARNING",
  "INFO",
  "DEBUG"
}

typedef enum {
  T_DEFAULT = 0,
  T_PARSE = 1
} LogType;

const char *g_logTypeToName[] = {
  "default",
  "parsing"
}

struct DebugLogTable {
  LogType type;
  LogLevel level;
};

/* setup debug log defaults */
struct DebugLogTable g_debugLogTable[] = {
  { T_DEFAULT, L_WARNING },
  { T_PARSE, L_WARNING }
};

extern void log(LogType type, LogLevel level, const char *fmt, ...);
#endif
