#ifndef __SNFLOW_SOURCE_TABLE_H
#define __SNFLOW_SOURCE_TABLE_H

#ifdef HAVE_NETINET6_IN6_H
  #include <netinet6/in6.h> // FreeBSD
#endif
#include <netinet/in.h>

typedef struct SNFSource {
  union src {
    struct in_addr ip4;
    struct in6_addr ip6;
  };

};
#endif
