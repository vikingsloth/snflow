#include "netflow_v9.h"
#include "debug_log.h"

#include <unistd.h>

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
ssize_t nf9PacketParse(const char *in, size_t len) {
  NF9Header *header;

  if (len < sizeof(NF9Header)) {
    flog(TPARSE, LDEBUG, "v9 not enough bytes to parse packet");
    return (NF9_ENEEDMORE);
  }

  header = (NF9Header *)in;
  if (header->version != 9) {
    flog(TPARSE, LCRIT, "version mismatch");
    return (NF9_EWRONGVER);
  }

  return (len);
}

int nf9DataWrite(char *dst, size_t len, NF9Template *tp, NF9Data *flow) {
  return (-1);
}

int nf9TemplateWrite(char *dst, size_t len, NF9Template *tp) {
  return (-1);
}
