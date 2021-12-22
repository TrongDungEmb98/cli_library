#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1

#include "app_debug.h"
//#include "board_hw.h"
//#include "app_flash.h"

#define PRINTF_OVER_RTT DEBUG_RTT
#define PRINTF_OVER_UART (!PRINTF_OVER_RTT)

#include "app_cli.h"
#include "app_shell.h"
#if PRINTF_OVER_RTT
#include "SEGGER_RTT.h"
#endif

#include "stm32f4xx.h"
//#include "DataDefine.h"
//#include "gsm.h"

#if PRINTF_OVER_RTT
int rtt_custom_printf(const char *format, ...)
{
    int r;
    va_list ParamList;

    va_start(ParamList, format);
    r = SEGGER_RTT_vprintf(0, format, &ParamList);
    va_end(ParamList);

    return r;
}
#else
#define rtt_custom_printf DEBUG_RAW
#endif

static shell_context_struct m_user_context;
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc,
                                char **argv);
static int32_t cli_uart_cmd(p_shell_context_t context, int32_t argc,
                            char **argv);

static const shell_command_context_t cli_command_table[] = {
    {"reset", "\treset: reset system\r\n", cli_reset_system, 0},
    {"uart", "\tuart: Enable/disable uart\r\n", cli_uart_cmd, 1}, /* need 1 parameter */
};

void app_cli_puts(uint8_t *buf, uint32_t len)
{
#if PRINTF_OVER_RTT
    SEGGER_RTT_Write(0, buf, len);
#else
    extern void app_debug_uart_print(uint8_t * data, uint32_t len);
    app_debug_uart_print(buf, len);
#endif
}

extern uint8_t get_debug_rx_data(void);
void app_cli_gets(uint8_t *buf, uint32_t len)
{
#if PRINTF_OVER_RTT
    for (uint32_t i = 0; i < len; i++) {
        buf[i] = 0xFF;
    }

    if (!SEGGER_RTT_HASDATA(0)) {
        return;
    }

    int read = SEGGER_RTT_Read(0, buf, len);
    if (read > 0 && read < len) {
        for (uint32_t i = read; i < len; i++) {
            buf[i] = 0xFF;
        }
    }
#else
    for (uint32_t i = 0; i < len; i++) {
        buf[i] = get_debug_rx_data();
    }
#endif
}

void app_cli_start()
{
    app_shell_set_context(&m_user_context);
    app_shell_init(&m_user_context, app_cli_puts, app_cli_gets,
                   rtt_custom_printf, ">", false);

    /* Register CLI commands */
    for (int i = 0;
         i < sizeof(cli_command_table) / sizeof(shell_command_context_t); i++) {
        app_shell_register_cmd(&cli_command_table[i]);
    }

    /* Run CLI task */
    app_shell_task();
}

/* Reset System */
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc,
                                char **argv)
{
    os_trace("System reset\r\n");
    NVIC_SystemReset();
    return 0;
}

static int32_t cli_uart_cmd(p_shell_context_t context, int32_t argc,
                            char **argv)
{
    os_trace("uart command\r\n");
    return 0;
}

/* run from main loop */
void app_cli_poll(void) { app_shell_task(); }

#endif /* APP_CLI_ENABLE */
