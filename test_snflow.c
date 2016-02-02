#include "netflow_v9.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

int main(int argc, char **argv) {
  struct sockaddr_in sin;
  NF9Source *source;
  NF9Source *last_source;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("10.1.1.1");

  source = nf9AddSource((struct sockaddr *)&sin);
  assert(source == &g_ipv4SourceTable->source);  

  sin.sin_addr.s_addr = inet_addr("10.2.2.2");
  last_source = source;
  source = nf9AddSource((struct sockaddr *)&sin);
  assert(source == &g_ipv4SourceTable->source);

  assert(last_source == &g_ipv4SourceTable->next->source);

  printf("Success!\n");
  return (0);
}
