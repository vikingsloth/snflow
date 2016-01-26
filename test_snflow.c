#include "snflow.h"
#include "netflow_v9.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

int main(int argc, char **argv) {
  int error;
  struct sockaddr_in sin;
  NF9Source *source;
  NF9Source *last_source;
  SNFConfig *conf;

  conf = snf_config_new();
  assert(conf != NULL);
  snf_config_init(conf);
  error = snf_config_add_listener(conf, "10.2.2.2", 4444);
  assert(!error);
  assert(conf->listener_count == 1);
  error = snf_config_add_listener(conf, "::1", 4444);
  assert(!error);
  assert(conf->listener_count == 2);

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("10.1.1.1");

  source = nf9AddSource(conf, (struct sockaddr *)&sin);
  assert(source == &g_ipv4SourceTable->source);

  sin.sin_addr.s_addr = inet_addr("10.2.2.2");
  last_source = source;
  source = nf9AddSource(conf, (struct sockaddr *)&sin);
  assert(source == &g_ipv4SourceTable->source);

  assert(last_source == &g_ipv4SourceTable->next->source);

  printf("Success!\n");
  return (0);
}
