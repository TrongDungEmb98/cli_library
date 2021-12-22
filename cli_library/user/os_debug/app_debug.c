#include "app_debug.h"
#include "SEGGER_RTT.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#define RTT_PRINTF_BUFFER_SIZE 512
#define BLE_PRINTF_MTU_SIZE 23

int app_debug_rtt(const char *fmt,...)
{
    // Get debug data
    if (!(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk))
    {
        return 0;
    }

    int     n;
    char    aBuffer[RTT_PRINTF_BUFFER_SIZE];
    char *p = &aBuffer[0];
    int size = RTT_PRINTF_BUFFER_SIZE;

    va_list args;

    va_start (args, fmt);
    n = vsnprintf(p, size, fmt, args);
    if (n > (int)size) 
    {
        SEGGER_RTT_Write(0, aBuffer, size);
    } 
    else if (n > 0) 
    {
        SEGGER_RTT_Write(0, aBuffer, n);
    }
    va_end(args);
    return n;
}

void app_debug_hex(char* TAG, uint8_t* data, uint32_t len)
{
	char buff[RTT_PRINTF_BUFFER_SIZE];

	for(uint16_t i = 0; i < len; i++)
	{
		sprintf(&buff[i], "%02X ", data[i]);
	}

	app_debug_rtt(buff);
    app_debug_rtt("\n");
}
