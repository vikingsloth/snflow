#include "snflow.h"

#include "debug_log.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <libev/ev.h>
#include <errno.h>
#include <unistd.h>

SNFConfig *snf_config_new(void) {
  SNFConfig *conf;

  conf = malloc(sizeof(*conf));
  if (!conf) {
    return (NULL);
  }

  snf_config_init(conf);
  return(conf);
}

// libev expects "struct ev_io" but we can overload the struct with custom
// data at the end.
struct libevio_ctx {
  ev_io w;
  SNFListener *listener;
  SNFConfig *conf;
};

static void libev_accept_cb(struct ev_loop *loop, struct ev_io *w,
    int revents) {
  struct libevio_ctx *actx = (struct libevio_ctx *)w;
  snflog(actx->conf, TIO, LDEBUG, "Connection received");
  // XXX accept(), ev_io_init client
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
  SNFListener *listener;
  char ip_str[64];
  uint16_t port;

  if (len > sizeof(struct sockaddr_storage)) {
    return (-1);
  }
  if (conf->listener_count >= SNF_MAX_LISTENERS) {
    // Listener count maxed out. Change SNF_MAX_LISTENERS and recompile
    return (-1);
  }

  listener = malloc(sizeof(*listener));
  if (!listener) {
    return (-1);
  }

  if (sa->sa_family == AF_INET) {
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;
    port = ntohs(sin->sin_port);
  } else if (sa->sa_family == AF_INET6) {
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
    port = ntohs(sin6->sin6_port);
  } else {
    // Invalid listener sockaddr
    free(listener);
    return(-1);
  }
  inet_ntop(sa->sa_family, sa, ip_str, sizeof(ip_str));
  listener->ip_str = strdup(ip_str);
  listener->port = port;
  listener->sock = 0;
  memcpy(&listener->ss, sa, len);
  // Insert at head of linked list
  listener->next = conf->listeners;
  conf->listener_count++;
  return (0);
}

int snf_config_add_listener(SNFConfig *conf, const char *ip,
    uint16_t port) {
  int ret;
  struct sockaddr_in sin;
  struct sockaddr_in6 sin6;

  ret = inet_pton(AF_INET, ip, &sin.sin_addr);
  if (ret > 0) {
    // IPv4 address
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_len = sizeof(sin);
    snf_config_add_listener_sa(conf, (struct sockaddr *)&sin, sizeof(sin));
    return (0);
  }
  ret = inet_pton(AF_INET6, ip, &sin6.sin6_addr);
  if (ret > 0) {
    // IPv6 address
    memset(&sin6, 0, sizeof(sin6)); // zero for portability
    sin6.sin6_family = AF_INET6;
    sin6.sin6_port = htons(port);
    sin6.sin6_len = sizeof(sin6);
    sin6.sin6_flowinfo = 0;
    sin6.sin6_scope_id = 0;
    snf_config_add_listener_sa(conf, (struct sockaddr *)&sin6, sizeof(sin6));
    return (0);
  }
  // Invalid IP address
  return (-1);
}

void snf_register(SNFConfig *conf) {
}

void snf_start(SNFConfig *conf) {
  struct libevio_ctx *actx;
  struct ev_loop *loop = ev_default_loop(0);
  int sock;
  int error;
  SNFListener *listener = conf->listeners;
  int opt = 1;

  // Bind all the listeners and register them in the libev loop
  while (listener) {
    listener->sock = -1;
    snflog(conf, TIO, LINFO, "Binding %s:%s", listener->ip_str, listener->port);
    sock = socket(listener->ss.ss_family, SOCK_STREAM, 0);
    if (sock < 0) {
      snflog(conf, TIO, LERROR, "Couldn't create socket: %s",
          strerror(errno));
      goto continue_loop;
    }
    // Let the listener rebind after a restart
    error = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (error) {
      snflog(conf, TIO, LERROR, "Couldn't set socket options");
      close(sock);
      goto continue_loop;
    }
    error = bind(sock, (struct sockaddr *)&listener->ss, listener->ss.ss_len);
    if (error) {
      snflog(conf, TIO, LERROR, "Couldn't bind to listener %s:%u: %s",
          listener->ip_str, listener->port, strerror(errno));
      close(sock);
      goto continue_loop;
    }
    error = listen(sock, 5);
    if (error) {
      snflog(conf, TIO, LERROR, "Couldn't listen: %s", strerror(errno));
      close(sock);
      goto continue_loop;
    }
    // Init the custom context for the listener callback
    actx = malloc(sizeof(*actx));
    actx->listener = listener;
    actx->conf = conf;
    // Initialize the watcher struct with our fd to watch
    ev_io_init((ev_io *)actx, libev_accept_cb, sock, EV_READ);
    // Register the watcher with our event loop
    ev_io_start(loop, (ev_io *)actx);
    listener->sock = sock;

continue_loop:
    listener = listener->next;
  }

  // Loop until unroll is called
  ev_loop(loop, 0);
}
