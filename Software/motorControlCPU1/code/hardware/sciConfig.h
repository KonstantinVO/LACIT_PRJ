

#ifndef CODE_SCI_CONFIG_H_
#define CODE_SCI_CONFIG_H_

#include "mavlink.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"


#define UART_RX_BUFF_SIZE   MAX_MAV_SIZE
#define UART_TX_BUFF_SIZE   MAX_MAV_SIZE


void initSci(void);


struct uart_receive
{
    uint16_t rx_buffer[UART_RX_BUFF_SIZE];
    uint16_t work_buffer[UART_RX_BUFF_SIZE];
    uint16_t byte_counter;
    uint16_t crc_counter;
    bool crc_status;
    bool rx_msg_status;
};


#endif /* CODE_SCI_H_ */
