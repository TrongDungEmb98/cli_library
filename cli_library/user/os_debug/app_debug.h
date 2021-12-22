#ifndef __DEBUG_H__
#define __DEBUG_H__

/* Includes */
#include <stdarg.h>
#include <stdint.h>

#define DEBUG_RTT               1

int app_debug_rtt(const char *fmt,...);
void app_debug_hex(char* TAG, uint8_t* data, uint32_t len);

#define os_trace 			if (DEBUG_RTT) app_debug_rtt
#define os_trace_hex		if (DEBUG_RTT) app_debug_hex

#endif /* __DEBUG_H__ */
