#ifndef __SNFLOW_DEBUG_LOG_H
#define __SNFLOW_DEBUG_LOG_H

#include "snflow.h"

#define would_log(cnf, typ, lvl) (cnf->log_levels[typ] <= lvl)

extern const char *g_logLevelToName[];
extern const char *g_logTypeToName[];

extern void snflog(SNFConfig *conf, SNFLogType type, SNFLogLevel level,
    const char *fmt, ...);
#endif
