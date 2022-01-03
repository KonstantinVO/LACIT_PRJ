
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"

#include "crcControl.h"
#include "mavlink.h"
#include "sciConfig.h"



extern struct uart_receive uart_rx;


#if EXTRA_BYTE

#define NUM_MESSAGE 1
struct messageID msgEXID[NUM_MESSAGE]= {EB_MSG_VERIF, ID_MSG_VERIF};

uint16_t getExtraByte(uint16_t messageID)
{

    for(uint16_t i = 0; i < NUM_MESSAGE; i++)
    {
        if(messageID == msgEXID[i].id)
        {
            return msgEXID[i].extraByte;
        }
    }
    return 0;
}
#endif


uint16_t calcCrc16(const uint16_t *pointer, uint16_t length, uint16_t auxByte)
{
  uint16_t i;
  uint16_t crc;
/*********************************
 * The TMS320C28x byte is 16 bits
 * description :
   https://software-dl.ti.com/ccs/esd/documents/c2000_byte-accesses-with-the-c28x-cpu.html
*********************************/
  int16_t value[0];
  __byte(value,0) = 0x00;// value;

  crc = START_CRC_VAL; // start value of crc = 0xFFFF

  for (i = 0; i < length; i++)
  {
      __byte(value,0) = pointer[i] ^ (crc & 0xff);
      __byte(value,0) ^= __byte(value,0)<<4;
    crc = (crc>>8) ^ (__byte(value,0)<<8) ^ (__byte(value,0)<<3) ^ (__byte(value,0)>>4);
  }
  //add crc_extra_byte and calculate crc.
//-----------------------------------------------
#if EXTRA_BYTE
  __byte(value,0) = auxByte ^ (crc & 0xff);
  __byte(value,0) ^= __byte(value,0)<<4;
    crc   = (crc>>8) ^ (__byte(value,0)<<8) ^ (__byte(value,0)<<3) ^ (__byte(value,0)>>4);
#endif
//-----------------------------------------------
 return crc;
}

bool checkCRC(uint16_t *data, uint16_t len)
{
#if EXTRA_BYTE
    uint16_t exb = getExtraByte(data[msg]);
#else
    uint16_t exb = 0;
#endif
    uint16_t crc_result;
    uint16_t cka, ckb, cka_2, ckb_2;
#if EXTRA_BYTE
    crc_result = calcCrc16((uint16_t*)data + 1, len - 3, exb);
#else
    crc_result = calcCrc16((uint16_t*)data, len - 2, exb);
#endif

#if EXTRA_BYTE
    cka = (crc_result >> 8) & 0xFF;
    ckb = crc_result & 0xFF;
    cka_2 = data[len - 2];
    ckb_2 = data[len - 1];
#else
    cka = crc_result & 0xFF;
    ckb = (crc_result >> 8) & 0xFF;
    cka_2 = data[len - 1];
    ckb_2 = data[len - 2];
#endif

  return (cka == cka_2 && ckb == ckb_2) ? true : false;
}

void crcControl(void)
{
    if(uart_rx.rx_msg_status)
    {
        uart_rx.crc_status = checkCRC(uart_rx.work_buffer, uart_rx.crc_counter);
        mavCrcAnsver(uart_rx.crc_status);
        uart_rx.crc_counter = 0;

        if(uart_rx.crc_status == false)
        {
            memset(uart_rx.work_buffer, 0x0, MAX_MAV_SIZE);
            uart_rx.rx_msg_status = false;
            return;
        }
    }
    uart_rx.rx_msg_status = false;
    return;
}
