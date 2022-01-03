
#ifndef CODE_MAVLINK_H_
#define CODE_MAVLINK_H_

#include "stdint.h"
#include "string.h"
#include "stdbool.h"

#define MIN_MAV_SIZE        8
#define MAX_MAV_SIZE        263

#define STX_PACK_VERSION   0xFE

#define SERVER_ID          0x01
#define MOTOR_BOARD_ID     0x02

#define ID_MSG_VERIF       0xAA
//#define EB_MSG_VERIF       0xFB



enum mavlink_service_byte
{
    stx,
    len,
    seq,
    sys,
    comp,
    msg,
    payload,
    ckb,
    cka
};

void mavCrcAnsver(bool result);
void mavMessageConfig(uint16_t* data, uint16_t data_len, uint16_t msg_mes);

#endif /* CODE_MAVLINK_H_ */
