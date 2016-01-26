#include "netflow_v9.h"
#include "debug_log.h"

#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

// XXX use jenkins hash here and make thread safe
NF9SourceTable *g_ipv4SourceTable = NULL;
NF9SourceTable *g_ipv6SourceTable = NULL;

/* parse an export packet off the wire which could contain multiple flowsets of
 * templates and data. This function is designed to take data read accumulated
 * data from a socket. The function will return the amount of data used from
 * the input buffer. On error the function will return less than 0.
 *
 * Error return values:
 *   NF9_EUNKNOWN unknown error. Terminate connection
 *   NF9_ENEEDMORE not enough data to parse a packet yet
 *   NF9_EWRONGVER wrong version. Terminate connection
 */
ssize_t nf9PacketParse(SNFConfig *conf, const char *in, size_t len,
    struct sockaddr *sa) {
  NF9Header *header;
  NF9Source *source;
  char ip_str[64];

  if (len < sizeof(NF9Header)) {
    snflog(conf, TPARSE, LDEBUG, "v9 not enough bytes to parse packet");
    return (NF9_ENEEDMORE);
  }

  header = (NF9Header *)in;
  if (header->version != 9) {
    snflog(conf, TPARSE, LCRIT, "version mismatch");
    return (NF9_EWRONGVER);
  }

  source = nf9SourceLookup(conf, sa);
  if (!source) {
    source = nf9AddSource(conf, sa);
    if (!source) {
      return (NF9_EUNKNOWN);
    }
  }

  if (would_log(conf, TPARSE, LDEBUG)) {
    inet_ntop(sa->sa_family, sa, ip_str, sizeof(ip_str));
  }
  snflog(conf, TPARSE, LDEBUG, "%s: bytes:%u version:%u count:%u uptime:%u "
      "unixtime:%u sequence:%u source_id:%u",
      ip_str, len, header->version, header->count, header->uptime,
      header->unixtime, header->sequence, header->source_id);

  return (len);
}

int nf9DataWrite(char *dst, size_t len, NF9Template *tp, NF9Data *flow) {
  return (-1);
}

int nf9TemplateWrite(char *dst, size_t len, NF9Template *tp) {
  return (-1);
}

inline NF9Source *ipv4FindSource(struct sockaddr_in *ip,
                                 NF9SourceTable *elem) {
  struct sockaddr_in *sin;

  while (elem) {
    sin = (struct sockaddr_in *)&elem->source.ss;
    if (sin->sin_addr.s_addr == ip->sin_addr.s_addr) {
      return (&elem->source);
    }
    elem = elem->next;
  }
  return (NULL);
}

inline NF9Source *ipv6FindSource(struct sockaddr_in6 *ip,
                                 NF9SourceTable *elem) {
  struct sockaddr_in6 *sin6;

  while (elem) {
    sin6 = (struct sockaddr_in6 *)&elem->source.ss;
    if (memcmp(&sin6->sin6_addr, &ip->sin6_addr, 16) == 0) {
      return (&elem->source);
    }
    elem = elem->next;
  }
  return (NULL);
}

// adds source to table, does not check for dupes
NF9Source *nf9AddSource(SNFConfig *conf, struct sockaddr *sa) {
  NF9SourceTable *st;

  if (sa->sa_family == AF_INET) {
    st = malloc(sizeof(*st));
    memcpy(&st->source.ss, sa, sizeof(struct sockaddr_in));
    st->next = g_ipv4SourceTable;
    st->source.tp_table = NULL;
    g_ipv4SourceTable = st;
    return (&st->source);
  } else if (sa->sa_family == AF_INET6) {
    st = malloc(sizeof(*st));
    memcpy(&st->source.ss, sa, sizeof(struct sockaddr_in6));
    st->next = g_ipv6SourceTable;
    st->source.tp_table = NULL;
    g_ipv6SourceTable = st;
    return (&st->source);
  } else {
    snflog(conf, TPARSE, LCRIT, "can't add source, invalid addr in %s",
       	__func__);
    return (NULL);
  }
}

// traverse the source tables and find a a match or return NULL
NF9Source *nf9SourceLookup(SNFConfig *conf, struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return (ipv4FindSource((struct sockaddr_in *)sa, g_ipv4SourceTable));
  } else if (sa->sa_family == AF_INET6) {
    return (ipv6FindSource((struct sockaddr_in6 *)sa, g_ipv6SourceTable));
  } else {
    snflog(conf, TPARSE, LCRIT, "can't lookup, invalid addr in %s", __func__);
    return (NULL);
  }
}
