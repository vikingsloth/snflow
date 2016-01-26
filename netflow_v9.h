#ifndef __SNFLOW_NETFLOW_V9_H
#define __SNFLOW_NETFLOW_V9_H

// Netflow v9 protocol definitions. Field type definitions were pulled from
// the Cisco whitepaper on Netflow v9


#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_NETINET6_IN6_H
  #include <netinet6/in6.h> // FreeBSD
#endif
#include <netinet/in.h>

typedef struct NF9Header {
  uint16_t version;
  uint16_t count;
  uint32_t uptime;
  uint32_t unixtime;
  uint32_t sequence;
  uint32_t source_id;
} NF9Header;

typedef enum {
  IN_BYTES = 1,
  IN_PKTS = 2,
  FLOWS = 3,
  PROTOCOL = 4,
  SRC_TOS = 5,
  TCP_FLAGS = 6,
  L4_SRC_PORT = 7,
  IPV4_SRC_ADDR = 8,
  SRC_MASK = 9,
  INPUT_SNMP = 10,
  L4_DST_PORT = 11,
  IPV4_DST_ADDR = 12,
  DST_MASK = 13,
  OUTPUT_SNMP = 14,
  IPV4_NEXT_HOP = 15,
  SRC_AS = 16,
  DST_AS = 17,
  BGP_IPV4_NEXT_HOP = 18,
  MUL_DST_PKTS = 19,
  MUL_DST_BYTES = 20,
  LAST_SWITCHED = 21,
  FIRST_SWITCHED = 22,
  OUT_BYTES = 23,
  OUT_PKTS = 24,
  MIN_PKT_LNGTH = 25,
  MAX_PKT_LNGTH = 26,
  IPV6_SRC_ADDR = 27,
  IPV6_DST_ADDR = 28,
  IPV6_SRC_MASK = 29,
  IPV6_DST_MASK = 30,
  IPV6_FLOW_LABEL = 31,
  ICMP_TYPE = 32,
  MUL_IGMP_TYPE = 33,
  SAMPLING_INTERVAL = 34,
  SAMPLING_ALGORITHM = 35,
  FLOW_ACTIVE_TIMEOUT = 36,
  FLOW_INACTIVE_TIMEOUT = 37,
  ENGINE_TYPE = 38,
  ENGINE_ID = 39,
  TOTAL_BYTES_EXP = 40,
  TOTAL_PKTS_EXP = 41,
  TOTAL_FLOWS_EXP = 42,
  // vendor proprietary 43
  IPV4_SRC_PREFIX = 44,
  IPV4_DST_PREFIX = 45,
  MPLS_TOP_LABEL_TYPE = 46,
  MPLS_TOP_LABEL_IP_ADDR = 47,
  FLOW_SAMPLER_ID = 48,
  FLOW_SAMPLER_MODE = 49,
  FLOW_SAMPLER_RANDOM_INTERVAL = 50,
  // vendor proprietary 51
  MIN_TTL = 52,
  MAX_TTL = 53,
  IPV4_IDENT = 54,
  DST_TOS = 55,
  IN_SRC_MAC = 56,
  OUT_DST_MAC = 57,
  SRC_VLAN = 58,
  DST_VLAN = 59,
  IP_PROTOCOL_VERSION = 60,
  DIRECTION = 61,
  IPV6_NEXT_HOP = 62,
  BGP_IPV6_NEXT_HOP = 63,
  IPV6_OPTION_HEADERS = 64,
  // vendor proprietary 65
  // vendor proprietary 66
  // vendor proprietary 67
  // vendor proprietary 68
  // vendor proprietary 69
  MPLS_LABEL_1 = 70,
  MPLS_LABEL_2 = 71,
  MPLS_LABEL_3 = 72,
  MPLS_LABEL_4 = 73,
  MPLS_LABEL_5 = 74,
  MPLS_LABEL_6 = 75,
  MPLS_LABEL_7 = 76,
  MPLS_LABEL_8 = 77,
  MPLS_LABEL_9 = 78,
  MPLS_LABEL_10 = 79,
  IN_DST_MAC = 80,
  OUT_SRC_MAC = 81,
  IF_NAME = 82,
  IF_DESC = 83,
  SAMPLER_NAME = 84,
  IN_PERMANENT_BYTES = 85,
  IN_PERMANENT_PKTS = 86,
  // vendor proprietary 87
  FRAGMENT_OFFSET = 88,
  FORWARDING_STATUS = 89,
  MPLS_PAL_RD = 90,
  MPLS_PREFIX_LEN = 91,
  SRC_TRAFFIC_INDEX = 92,
  DST_TRAFFIC_INDEX = 93,
  APPLICATION_DESCRIPTION = 94,
  APPLICATION_TAG = 95,
  APPLICATION_NAME = 96,
  postipDiffServCodePoint = 98,
  REPLICATION_FACTOR = 99,
  DEPRECATED = 100,
  layer2packetSectionOffset = 102,
  layer2packetSectionSize = 103,
  layer2packetSectionData = 104
} NF9FieldType;

// storage structure for template records
typedef struct _NF9Template {
  uint16_t tid;       // template id
  NF9FieldType *tmap; // field type map using dynamic length array
  size_t tmap_len;    // number of elements in the type map
} NF9Template;

// storage structure for data records. Can be imported from or exported to
// the network using a flow template
typedef struct _NF9Data {
  // Incoming counter with length N x 8 bits for number of bytes associated with
  // an IP Flow.
  size_t in_bytes;                     // IN_BYTES
  // Incoming counter with length N x 8 bits for the number of packets
  // associated with an IP Flow
  size_t in_pkts;                      // IN_PKTS 
  // Number of flows that were aggregated
  size_t flows;                        // FLOWS
  // IP protocol
  u_char protocol;                     // PROTOCOL
  u_char src_tos;                      // SRC_TOS
  u_char tcp_flags;                    // TCP_FLAGS
  uint16_t l4_src_port;                // L4_SRC_PORT
  struct in_addr ipv4_src_addr;        // IPV4_SRC_ADDR
  // The number of contiguous bits in the source address subnet mask i.e.: the
  // submask in slash notation
  u_char src_mask;                     // SRC_MASK
  uint16_t input_snmp;                 // INPUT_SNMP
  uint16_t l4_dst_port;                // L4_DST_PORT
  struct in_addr ipv4_dst_addr;        // IPV4_DST_ADDR
  // The number of contiguous bits in the destination address subnet mask i.e.:
  // the submask in slash notation
  u_char dst_mask;                     // DST_MASK
  uint16_t output_snmp;                // OUTPUT_SNMP
  uint32_t ipv4_next_hop;              // IPV4_NEXT_HOP
  // Source BGP autonomous system number. could be 2 or 4 bytes
  uint32_t src_as;                     // SRC_AS
  // Destination BGP autonomous system number. could be 2 or 4 bytes
  uint32_t dst_as;                     // DST_AS
  uint32_t bgp_ipv4_next_hop;          // BGP_IPV4_NEXT_HOP
  // IP multicast outgoing packet counter with length N x 8 bits for packets
  // associated with the IP Flow
  uint32_t mul_dst_pkts;               // MUL_DST_PKTS
  // IP multicast outgoing byte counter with length N x 8 bits for bytes
  // associated with the IP Flow
  uint32_t mul_dst_bytes;              // MUL_DST_BYTES
  // System uptime at which the last packet of this flow was switched
  uint32_t last_switched;              // LAST_SWITCHED
  // System uptime at which the first packet of this flow was switched
  uint32_t first_switched;             // FIRST_SWITCHED
  // Outgoing counter with length N x 8 bits for the number of bytes associated
  // with an IP Flow
  uint32_t out_bytes;                  // OUT_BYTES
  // Outgoing counter with length N x 8 bits for the number of packets
  // associated with an IP Flow.
  uint32_t out_pkts;                   // OUT_PKTS
  // Minimum IP packet length on incoming packets of the flow
  uint16_t min_pkt_lngth;              // MIN_PKT_LNGTH
  // Maximum IP packet length on incoming packets of the flow
  uint16_t max_pkt_lngth;              // MAX_PKT_LNGTH
  struct in6_addr ipv6_src_addr;        // IPV6_SRC_ADDR
  struct in6_addr ipv6_dst_addr;        // IPV6_DST_ADDR
  // Length of the IPv6 source mask in contiguous bits
  u_char ipv6_src_mask;                 // IPV6_SRC_MASK
  // Length of the IPv6 destination mask in contiguous bits
  u_char ipv6_dst_mask;                 // IPV6_DST_MASK
  uint32_t ipv6_flow_label;             // IPV6_FLOW_LABEL
  // Internet Control Message Protocol (ICMP) packet type; reported as ((ICMP
  // Type*256) + ICMP code)
  uint16_t icmp_type;                   // ICMP_TYPE
  u_char mul_igmp_type;                 // MUL_IGMP_TYPE
  // When using sampled NetFlow, the rate at which packets are sampled i.e.: a
  // value of 100 indicates that one of every 100 packets is sampled
  uint32_t sampling_interval;           // SAMPLING_INTERVAL
  // The type of algorithm used for sampled NetFlow: 0x01 Deterministic Sampling
  // ,0x02 Random Sampling
  u_char sampling_algorithm;            // SAMPLING_ALGORITHM
  // Timeout value (in seconds) for active flow entries in the NetFlow cache
  uint16_t flow_active_timeout;         // FLOW_ACTIVE_TIMEOUT
  // Timeout value (in seconds) for inactive flow entries in the NetFlow cache
  uint16_t flow_inactive_timeout;        // FLOW_INACTIVE_TIMEOUT
  // Type of flow switching engine: RP = 0, VIP/Linecard = 1
  u_char engine_type;                    // ENGINE_TYPE
  // ID number of the flow switching engine
  u_char engine_id;                      // ENGINE_ID
  // Counter with length N x 8 bits for bytes for the number of bytes exported
  // by the Observation Domain
  uint32_t total_bytes_exp;              // TOTAL_BYTES_EXP
  // Counter with length N x 8 bits for bytes for the number of packets exported
  // by the Observation Domain
  uint32_t total_pkts_exp;               // TOTAL_PKTS_EXP
  // Counter with length N x 8 bits for bytes for the number of flows exported
  // by the Observation Domain
  uint32_t total_flows_exp;              // TOTAL_FLOWS_EXP
  // IPv4 source address prefix (specific for Catalyst architecture)
  struct in_addr ipv4_src_prefix;        // IPV4_SRC_PREFIX
  // IPv4 destination address prefix (specific for Catalyst architecture)
  struct in_addr ipv4_dst_prefix;        // IPV4_DST_PREFIX
  uint32_t mpls_top_label_type;          // MPLS_TOP_LABEL_TYPE
  struct in_addr mpls_top_label_ip_addr; // MPLS_TOP_LABEL_IP_ADDR
  u_char flow_sampler_id;                // FLOW_SAMPLER_ID
  u_char flow_sampler_mode;              // FLOW_SAMPLER_MODE
  uint32_t flow_sampler_random_interval; // FLOW_SAMPLER_RANDOM_INTERVAL
  u_char min_ttl;                        // MIN_TTL
  u_char max_ttl;                        // MAX_TTL
  uint16_t ipv4_ident;                   // IPV4_IDENT
  u_char dst_tos;                        // DST_TOS
  uint64_t in_src_mac;                   // IN_SRC_MAC
  uint64_t out_dst_mac;                  // OUT_DST_MAC
  uint16_t src_vlan;                     // SRC_VLAN
  uint16_t dst_vlan;                     // DST_VLAN
  // Internet Protocol Version Set to 4 for IPv4, set to 6 for IPv6. If not
  // present in the template, then version 4 is assumed
  u_char ip_protocol_version;            // IP_PROTOCOL_VERSION
  // Flow direction: 0 - ingress flow, 1 - egress flow
  u_char direction;                      // DIRECTION
  struct in6_addr ipv6_next_hop;         // IPV6_NEXT_HOP
  struct in6_addr bgp_ipv6_next_hop;     // BPG_IPV6_NEXT_HOP
  // Bit-encoded field identifying IPv6 option headers found in the flow
  uint32_t ipv6_option_headers;          // IPV6_OPTION_HEADERS
  uint32_t mpls_label_1;                 // MPLS_LABEL_1
  uint32_t mpls_label_2;                 // MPLS_LABEL_2
  uint32_t mpls_label_3;                 // MPLS_LABEL_3
  uint32_t mpls_label_4;                 // MPLS_LABEL_4
  uint32_t mpls_label_5;                 // MPLS_LABEL_5
  uint32_t mpls_label_6;                 // MPLS_LABEL_6
  uint32_t mpls_label_7;                 // MPLS_LABEL_7
  uint32_t mpls_label_8;                 // MPLS_LABEL_8
  uint32_t mpls_label_9;                 // MPLS_LABEL_9
  uint32_t mpls_label_10;                // MPLS_LABEL_10
  uint64_t in_dst_mac;                   // IN_DST_MAC
  uint64_t out_src_mac;                  // OUT_SRC_MAC
  // Shortened interface name i.e.: "FE1/0"
  char *if_name;                         // IF_NAME
  // Full interface name i.e.: "'FastEthernet 1/0"
  char *if_desc;                         // IF_DESC
  // Name of the flow sampler
  char *sampler_name;                    // SAMPLER_NAME
  // Running byte counter for a permanent flow
  uint32_t in_permanent_bytes;           // IN_PERMENENT_BYTES
  // Running packet counter for a permanent flow
  uint32_t in_permanent_pkts;            // IN_PERMENENT_PKTS
  // The fragment-offset value from fragmented IP packets
  uint16_t fragment_offset;              // FRAGMENT_OFFSET
  // Forwarding status is encoded on 1 byte with the 2 left bits giving the
  // status and the 6 remaining bits giving the reason code.
  u_char forwarding_status;              // FORWARDING_STATUS
} NF9Data;

typedef struct _NF9Source {
  struct sockaddr_storage ss;
  NF9Template *tp_table; // Template ID lookup table
} NF9Source;

typedef struct _NF9SourceTable {
  struct _NF9SourceTable *next;
  NF9Source source;
} NF9SourceTable;

#define MAX_IPV4_SOURCES 128
#define MAX_IPv6_SOURCE 128
extern NF9SourceTable *g_ipv4SourceTable;
extern NF9SourceTable *g_ipv6SourceTable;

// return values for nf9PacketParse
#define NF9_EUNKNOWN -1
#define NF9_ENEEDMORE -2
#define NF9_EWRONGVER -3
extern ssize_t nf9PacketParse(const char *in, size_t len, struct sockaddr *sa);
extern int nf9DataWrite(char *dst, size_t len, NF9Template *tp, NF9Data *flow);
extern int nf9TemplateWrite(char *dst, size_t len, NF9Template *tp);
extern NF9Source *nf9AddSource(struct sockaddr *sa);
extern NF9Source *nf9SourceLookup(struct sockaddr *sa);
#endif
