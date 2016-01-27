#ifndef __SNFLOW_SNFLOW_H
#define __SNFLOW_SNFLOW_H

#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_NETINET6_IN6_H
  #include <netinet6/in6.h> // FreeBSD IPv6 includes
#endif
#include <netinet/in.h>

typedef enum _SNFLogLevel {
  LNONE = 0,
  LCRIT = 1,
  LERROR = 2,
  LWARN = 3,
  LINFO = 4,
  LDEBUG = 5
} SNFLogLevel;

typedef enum _SNFLogType {
  TDEFAULT = 0,
  TPARSE = 1,
  TIO = 2,
  TCONF = 3,
  TMAX
} SNFLogType;
#define SNF_MAX_LOG_TYPE TMAX

typedef struct _SNFSource {
  struct _SNFSource *next;
  struct sockaddr_storage ss;
} SNFSource;

typedef struct _SNFListener {
  struct _SNFListener *next;
  struct sockaddr_storage ss;
  char *ip_str;
  uint16_t port; // host byte order
  int sock;
} SNFListener;

#define SNF_MAX_LISTENERS 16
typedef struct _SNFConfig {
  // Linked list of netflow listener data: sockaddr, port, socket
  SNFListener *listeners;
  size_t listener_count;
  uint8_t log_levels[SNF_MAX_LOG_TYPE];
  // Expected netflow sources. If allow_dynamic_sources is false, only these
  // pre-defined source IPs are allowed to send netflow data
  SNFSource *sources;
  u_char allow_dynamic_sources;
} SNFConfig;

extern SNFConfig *snf_config_new(void);
extern void snf_config_init(SNFConfig *conf);
extern int snf_config_add_listener_sa(SNFConfig *conf, struct sockaddr *sa,
    socklen_t len);
extern int snf_config_add_listener(SNFConfig *conf, const char *ip,
    uint16_t port);
extern void snf_register(SNFConfig *conf);
extern void snf_start(SNFConfig *conf);
#endif
