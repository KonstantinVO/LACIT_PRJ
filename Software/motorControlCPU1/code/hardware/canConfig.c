
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/c28/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include "string.h"
#include "F28x_Project.h"
#include "can.h"
#include "driverlib.h"
#include "device.h"
#include "canConfig.h"


#define CPU_FREQ        200000000
#define BIT_RATE        250000

#define CAN_A_RX_PIN    36
#define CAN_A_TX_PIN    37
#define CAN_B_RX_PIN    17
#define CAN_B_TX_PIN    12

#define TXCOUNT  100
#define MSG_DATA_LENGTH    4
#define TX_MSG_OBJ_ID      1
#define RX_MSG_OBJ_ID      1

//******testing********//
uint16_t txMsgData[4];
uint16_t rxMsgData[4];
volatile uint32_t txMsgCount = 0;
volatile uint32_t rxMsgCount = 0;
volatile uint32_t errorFlag = 0;


void initCanPins(void)
{
    GPIO_SetupPinMux(CAN_A_RX_PIN, GPIO_MUX_CPU1, 1); //GPIO30 -  CANRXA
    GPIO_SetupPinOptions(CAN_A_RX_PIN, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(CAN_A_TX_PIN, GPIO_MUX_CPU1, 1); //GPIO31 - CANTXA
    GPIO_SetupPinOptions(CAN_A_TX_PIN, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(CAN_B_RX_PIN, GPIO_MUX_CPU1, 2); //GPIO10 -  CANRXB
    GPIO_SetupPinOptions(CAN_B_RX_PIN, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(CAN_B_TX_PIN, GPIO_MUX_CPU1, 2);  //GPIO8 - CANTXB
    GPIO_SetupPinOptions(CAN_B_TX_PIN, GPIO_OUTPUT, GPIO_PUSHPULL);
}

void initCan(void)
{
    initCanPins();

    CAN_initModule(CANA_BASE);
    CAN_initModule(CANB_BASE);

    CAN_setBitRate(CANA_BASE, CPU_FREQ, BIT_RATE, 10);
    CAN_setBitRate(CANB_BASE, CPU_FREQ, BIT_RATE, 10);

    CAN_enableInterrupt(CANA_BASE, CAN_INT_IE0 | CAN_INT_ERROR |
                        CAN_INT_STATUS);
    CAN_enableInterrupt(CANB_BASE, CAN_INT_IE0 | CAN_INT_ERROR |
                        CAN_INT_STATUS);

    CAN_enableGlobalInterrupt(CANA_BASE, CAN_GLOBAL_INT_CANINT0);
    CAN_enableGlobalInterrupt(CANB_BASE, CAN_GLOBAL_INT_CANINT0);
//------------------------------------------------------------------------
/*    CAN_setupMessageObject(CANB_BASE, TX_MSG_OBJ_ID, 0x95555555,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_TX, 0,
                           CAN_MSG_OBJ_NO_FLAGS, MSG_DATA_LENGTH);*/
    CAN_setupMessageObject(CANB_BASE, TX_MSG_OBJ_ID, 0x95555555,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_TX, 0,
                           CAN_MSG_OBJ_TX_INT_ENABLE, MSG_DATA_LENGTH);




//--------------------------------------

    CAN_setupMessageObject(CANB_BASE, 2, 0x1111111,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_RX, 0,
                           CAN_MSG_OBJ_RX_INT_ENABLE, MSG_DATA_LENGTH);
//------------------------------------------------------------------------
    CAN_startModule(CANA_BASE);
    CAN_startModule(CANB_BASE);

}

void canTest(void)
{
    // Initialize the transmit message object data buffer to be sent
    txMsgData[0] = 0x55;
    txMsgData[1] = 0x55;
    txMsgData[2] = 0x55;
    txMsgData[3] = 0x55;

    CAN_sendMessage(CANB_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH,
                    txMsgData);

}

Void can1Isr(UArg arg)
{

    return;
}

Void can2Isr(UArg arg)
{
    uint32_t status;
    // Read the CAN-B interrupt status (in the CAN_INT register) to find the
    // cause of the interrupt
    status = CAN_getInterruptCause(CANB_BASE);

    if(status == CAN_INT_INT0ID_STATUS)
    {

        status = CAN_getStatus(CANB_BASE);  // Return CAN_ES value.

        // Now status = 0x00000010, indicating RxOK.
        // Check to see if an error occurred.

        if(((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_MSK) &&
           ((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_NONE))
        {
            // Set a flag to indicate some errors may have occurred.
            errorFlag = 1;
        }
    }
    // Check if the cause is the CAN-B receive message object 1. Will be skipped
    // in the first iteration of every ISR execution
    else if(status == 2)
    {
        //
        // Get the received message
        //
        CAN_readMessage(CANB_BASE, 2, rxMsgData);

        CAN_clearInterruptStatus(CANB_BASE, 2);

        errorFlag = 0;
    }

    // If something unexpected caused the interrupt, this would handle it.

    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }


    CAN_clearGlobalInterruptStatus(CANB_BASE, CAN_GLOBAL_INT_CANINT0);

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

    return;
}

