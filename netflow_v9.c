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
 */
ssize_t nf9PacketParse(const char *in, size_t len) {
  if (len < sizeof(NF9Header)) {
    log(T_PARSE, T_DEBUG, "nf9PacketParse not enough bytes to parse packet");
    return (NF9_ENEEDMORE);
  }
}

int nf9DataWrite(char *dst, size_t len, NF9Template *tp, NF9Data *flow) {
}

int nf9TemplateWrite(char *dst, size_t len, NF9Template *tp) {
}
