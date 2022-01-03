
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/c28/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include "string.h"
#include "F28x_Project.h"
#include "sciConfig.h"
#include "driverlib.h"
#include "device.h"
#include "crcControl.h"


#define BAUD_RATE       115200
#define CPU_FREQ        200000000
#define LSPCLK_FREQ     CPU_FREQ/4


struct uart_receive uart_rx;


void initSciPins(void);
void error(void);



void error(void)
{
    asm("     ESTOP0"); // Test failed!! Stop!
    for (;;);
}

void initSciPins(void)
{
    // GPIO28 is the SCI Rx pin.
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    // GPIO29 is the SCI Tx pin.
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);
}

void initSci(void)
{
    initSciPins();
    // Initialize SCIA and its FIFO.
    SCI_performSoftwareReset(SCIA_BASE);

    // Configure SCIA for echoback.
    SCI_setConfig(SCIA_BASE, LSPCLK_FREQ, BAUD_RATE, (SCI_CONFIG_WLEN_8 |
                                                      SCI_CONFIG_STOP_ONE |
                                                      SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXRDY | SCI_INT_RXRDY_BRKDT | SCI_INT_RXERR);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

    // Enable the TXRDY and RXRDY interrupts.
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_TXRDY | SCI_INT_RXRDY_BRKDT | SCI_INT_RXERR);

}
//****************************************************************************************************//


static inline void uart_receive(struct uart_receive *rx_str)
{
    rx_str->crc_status = false;
    if(rx_str->byte_counter >= MAX_MAV_SIZE) rx_str->byte_counter = 0;

    rx_str->rx_buffer[rx_str->byte_counter++] = SCI_readCharNonBlocking(SCIA_BASE);//read byte

    if(rx_str->byte_counter == 1
       && rx_str->rx_buffer[stx] != STX_PACK_VERSION)
    {
       rx_str->byte_counter = 0;
    }
    else if(rx_str->byte_counter >= MIN_MAV_SIZE
            && rx_str->byte_counter >= rx_str->rx_buffer[len] + MIN_MAV_SIZE)
    {
        memcpy(&rx_str->work_buffer, rx_str->rx_buffer, rx_str->byte_counter);
        rx_str->rx_msg_status = true;
        rx_str->crc_counter = rx_str->byte_counter;
        rx_str->byte_counter = 0;
    }

    return;
}


Void sciaRxIsr(UArg arg)
{
    uart_receive(&uart_rx);

    return;
}


Void sciaTxIsr(UArg arg)
{
    //todo:
    //ticks++;
    return;
}


