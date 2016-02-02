#ifndef __SNFLOW_DEBUG_LOG_H
#define __SNFLOW_DEBUG_LOG_H

typedef enum _LogLevel {
  LNONE = 0,
  LCRITICAL = 1,
  LERROR = 2,
  LWARNING = 3,
  LINFO = 4,
  LDEBUG = 5
} LogLevel;

extern const char *g_logLevelToName[];

typedef enum _LogType {
  TDEFAULT = 0,
  TPARSE = 1
} LogType;

extern const char *g_logTypeToName[];

struct DebugLogTable {
  LogType type;
  LogLevel level;
};

extern struct DebugLogTable g_debugLogTable[];

extern void flog(LogType type, LogLevel level, const char *fmt, ...);
#endif
