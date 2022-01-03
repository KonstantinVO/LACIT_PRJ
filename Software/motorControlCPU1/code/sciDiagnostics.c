#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "mavlink.h"
#include "crcControl.h"
#include "sciConfig.h"
#include "sciDiagnostics.h"
#include "driverlib.h"
#include "device.h"
#include "crcControl.h"
#include "pi.h"
#include "utils.h"
#include "IQmathLib.h"
#include "configProject.h"


#if MOTOR_TYPE == ACI_MOTOR
    #include "ACImotorControl.h"
#elif MOTOR_TYPE == BLDC_MOTOR
    #include "BLDCmotorControl.h"
#endif


extern struct uart_receive uart_rx;
union conv4byte c4b;

extern _iq VdTesting;  // Vd reference (pu)
extern _iq VqTesting;  // Vq reference (pu)
extern _iq IdRef;  // Id reference (pu)
extern _iq IqRef; // Iq reference (pu)
extern _iq SpeedRef;  // Speed reference (pu)

extern PI_CONTROLLER pi_spd;
extern PI_CONTROLLER pi_id;
extern PI_CONTROLLER pi_iq;

extern uint16_t SpeedLoopPrescaler;

void testMsgRX(void)
{
    if(uart_rx.crc_status && uart_rx.work_buffer[msg] == 0xAA)//ID_MSG_DEBUG_PARAM_V1)
    {
       SpeedRef = glueFloat(&c4b,& uart_rx, 0);
       VdTesting = glueFloat(&c4b,& uart_rx, 4);
       VqTesting =  glueFloat(&c4b,& uart_rx, 8);
       IdRef = glueFloat(&c4b,& uart_rx, 12);
       IqRef = glueFloat(&c4b,& uart_rx, 16);

       uart_rx.crc_status = false;
       memset(uart_rx.work_buffer, 0x0, MAX_MAV_SIZE);
    }
}

void sciPiSpdParam(void)
{
    if(uart_rx.crc_status && uart_rx.work_buffer[msg] == ID_MSG_SPD_PARAM)
    {
        pi_spd.Kp=_IQ(glueFloat(&c4b,& uart_rx, 0));
        pi_spd.Ki=_IQ(T*SpeedLoopPrescaler/glueFloat(&c4b,& uart_rx, 4));
        pi_spd.Umax =_IQ(glueFloat(&c4b,& uart_rx, 8));
        pi_spd.Umin =_IQ(glueFloat(&c4b,& uart_rx, 12));

        uart_rx.crc_status = false;
        memset(uart_rx.work_buffer, 0x0, MAX_MAV_SIZE);
    }
}

void sciPiIdParam(void)
{
    if(uart_rx.crc_status && uart_rx.work_buffer[msg] == ID_MSG_ID_PARAM)
    {
        pi_id.Kp=_IQ(glueFloat(&c4b,& uart_rx, 0));
        pi_id.Ki=_IQ(T*SpeedLoopPrescaler/glueFloat(&c4b,& uart_rx, 4));
        pi_id.Umax =_IQ(glueFloat(&c4b,& uart_rx, 8));
        pi_id.Umin =_IQ(glueFloat(&c4b,& uart_rx, 12));

        uart_rx.crc_status = false;
        memset(uart_rx.work_buffer, 0x0, MAX_MAV_SIZE);
    }
}

void sciPiIqParam(void)
{
    if(uart_rx.crc_status && uart_rx.work_buffer[msg] == ID_MSG_IQ_PARAM)
    {
        pi_iq.Kp=_IQ(glueFloat(&c4b,& uart_rx, 0));
        pi_iq.Ki=_IQ(T*SpeedLoopPrescaler/glueFloat(&c4b,& uart_rx, 4));
        pi_iq.Umax =_IQ(glueFloat(&c4b,& uart_rx, 8));
        pi_iq.Umin =_IQ(glueFloat(&c4b,& uart_rx, 12));

        uart_rx.crc_status = false;
        memset(uart_rx.work_buffer, 0x0, MAX_MAV_SIZE);
    }
}


