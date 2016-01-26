#include "snflow.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

SNFConfig *snf_config_new(void) {
  SNFConfig *conf;

  conf = malloc(sizeof(*conf));
  if (!conf) {
    return (NULL);
  }

  snf_config_init(conf);
  return(conf);
}

void snf_config_init(SNFConfig *conf) {
  conf->listener_count = 0;
  conf->log_levels[TDEFAULT] = LDEBUG;
  conf->log_levels[TPARSE] = LDEBUG;
  conf->sources = NULL;
  conf->allow_dynamic_sources = 0;
}

int snf_config_add_listener_sa(SNFConfig *conf, struct sockaddr *sa,
    socklen_t len) {
  if (sa->sa_family != AF_INET || sa->sa_family != AF_INET6) {
    // Invalid listener
    return (-1);
  }
  if (len > sizeof(struct sockaddr_storage)) {
    return (-1);
  }
  if (conf->listener_count >= SNF_MAX_LISTENERS) {
    // Listener count maxed out. Change SNF_MAX_LISTENERS and recompile
    return (-1);
  }
  memcpy(&conf->listeners[conf->listener_count], sa, len);
  conf->listener_count++;
  return (0);
}

int snf_config_add_listener(SNFConfig *conf, const char *ip,
    uint16_t port) {
  int ret;
  union {
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
  } addr;

  ret = inet_pton(AF_INET, ip, &addr.sin.sin_addr);
  if (ret > 0) {
    // IPv4 address
    snf_config_add_listener_sa(conf, (struct sockaddr *)&addr,
	sizeof(addr.sin));
    return (0);
  }
  ret = inet_pton(AF_INET6, ip, &addr.sin6.sin6_addr);
  if (ret > 0) {
    // IPv6 address
    snf_config_add_listener_sa(conf, (struct sockaddr *)&addr,
	sizeof(addr.sin6));
    return (0);
  }
  // Invalid IP address
  return (-1);
}

void snf_register(SNFConfig *conf) {
}

void snf_start(SNFConfig *conf) {
}
