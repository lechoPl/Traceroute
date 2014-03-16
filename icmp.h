#pragma once

#define ICMP_HEADER_LEN 8

u_short in_cksum(const u_short *addr, register int len, u_short csum);
