
#include "mavlink.h"
#include "crcControl.h"
#include "sciConfig.h"

#include "driverlib.h"
#include "device.h"

#include "crcControl.h"
#include "utils.h"


extern struct uart_receive uart_rx;


void mavCrcAnsver(bool result)
{
    uint16_t arr_msg[9]={0};
#if EXTRA_BYTE
    uint16_t ex_byte = getExtraByte(ID_MSG_VERIF);
#else
    uint16_t ex_byte = 0;
#endif
    arr_msg[stx] = STX_PACK_VERSION;
    arr_msg[len] = 0x1;
    arr_msg[seq] = uart_rx.work_buffer[seq];
    arr_msg[sys] = MOTOR_BOARD_ID;
    arr_msg[comp] = SERVER_ID;//mav_sys;
    arr_msg[msg] = ID_MSG_VERIF;
    arr_msg[payload] = result;//1 byte
#if EXTRA_BYTE
    uint16_t crc_result = calcCrc16((uint16_t*)arr_msg + 1, 6, ex_byte);
    arr_msg[cka] = (crc_result >> 8) & 0xFF;
    arr_msg[ckb] =  crc_result & 0xFF;
#else
    uint16_t crc_result = calcCrc16((uint16_t*)arr_msg, 7, ex_byte);
    arr_msg[cka] = crc_result & 0xFF;
    arr_msg[ckb] = (crc_result >> 8) & 0xFF;
#endif


    SCI_writeCharArray(SCIA_BASE, (uint16_t*)arr_msg, 9);

    return;
}


void mavMessageConfig(uint16_t* data, uint16_t data_len, uint16_t msg_mes)
{
    uint16_t arr_msg[MAX_MAV_SIZE]={0};
    uint16_t all_size_msg = data_len + MIN_MAV_SIZE;
    static uint16_t num_msg = 0;

#if EXTRA_BYTE
    uint16_t ex_byte = getExtraByte(data[msg]);
#else
    uint16_t ex_byte = 0;
#endif

    if(num_msg >= 255) num_msg = 0;
    if(all_size_msg > MAX_MAV_SIZE) return;

    arr_msg[stx] =  STX_PACK_VERSION;
    arr_msg[len] =  data_len;
    arr_msg[seq] =  num_msg++;
    arr_msg[sys] =  MOTOR_BOARD_ID;//TZ
    arr_msg[comp] = SERVER_ID;
    arr_msg[msg] =  msg_mes;

    for(uint16_t i = 0; i < data_len; i++)
    {
        arr_msg[payload + i] = data[i];
    }
#if EXTRA_BYTE
    uint16_t crc_result = calcCrc16((uint16_t*)arr_msg + 1, all_size_msg - 3, ex_byte);
    arr_msg[cka] = (crc_result >> 8) & 0xFF;
    arr_msg[ckb] =  crc_result & 0xFF;
#else
    uint16_t crc_result = calcCrc16((uint16_t*)arr_msg, all_size_msg - 2, ex_byte);
    arr_msg[cka] = crc_result & 0xFF;
    arr_msg[ckb] = (crc_result >> 8) & 0xFF;
#endif

    SCI_writeCharArray(SCIA_BASE, (uint16_t*)arr_msg, all_size_msg);

    return;
}



