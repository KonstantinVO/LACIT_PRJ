#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/c28/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include "stdint.h"
#include "stdbool.h"
#include "F28x_Project.h"
#include "adcConfig.h"
#include "driverlib.h"
#include "device.h"
#include "utils.h"
#include "spiConfig.h"







uint16_t rData[DATA_SIZE];        // Receive data buffer

Void spiaRxIsr (UArg arg);

void initSpiPins(void)
{
    //InitSpiaGpio();
    //GPIO_writePin(SPI_CS_PIN, true);

    // GPIO17 is the SPISOMIA.
    //
    GPIO_setMasterCore(SPISOMIA_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_17_SPISOMIA);
    GPIO_setPadConfig(SPISOMIA_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(SPISOMIA_PIN, GPIO_QUAL_ASYNC);

    // GPIO16 is the SPISIMOA.
    //
    GPIO_setMasterCore(SPISIMOA_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_16_SPISIMOA);
    GPIO_setPadConfig(SPISIMOA_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(SPISIMOA_PIN, GPIO_QUAL_ASYNC);

    // GPIO19 is the SPISTEA.
    //
    GPIO_setMasterCore(SPISTEA_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_19_SPISTEA);
    GPIO_setPadConfig(SPISTEA_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(SPISTEA_PIN, GPIO_QUAL_ASYNC);

    // GPIO18 is the SPICLKA.
    //
    GPIO_setMasterCore(SPICLKA_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_18_SPICLKA);
    GPIO_setPadConfig(SPICLKA_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(SPICLKA_PIN, GPIO_QUAL_ASYNC);

}

// scia_fifo_init - Configure SCIA FIFO

void initSpi(void)
{
    initSpiPins();
    // Must put SPI into reset before configuring it
    //
    SPI_disableModule(SPI_MODULE);

    //
    // SPI configuration. Use a 1MHz SPICLK and X-bit word size.
    //
    SPI_setConfig(SPI_MODULE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                  SPI_MODE_MASTER, BAUD_RATE, DATA_WIDTH);
    SPI_disableLoopback(SPI_MODULE);
    SPI_enableFIFO(SPI_MODULE);
    SPI_setEmulationMode(SPI_MODULE, SPI_EMULATION_STOP_AFTER_TRANSMIT);
    SPI_setSTESignalPolarity(SPI_MODULE,SPI_STE_ACTIVE_HIGH);

    //
    // FIFO and interrupt configuration
    //
    SPI_enableFIFO(SPI_MODULE);
    SPI_clearInterruptStatus(SPI_MODULE, SPI_INT_RXFF | SPI_INT_TXFF);
    SPI_setFIFOInterruptLevel(SPI_MODULE, SPI_FIFO_TX2, SPI_FIFO_RX2);
    SPI_enableInterrupt(SPI_MODULE, SPI_INT_RXFF | SPI_INT_TXFF);

    Interrupt_enable(INT_SPIA_RX);

    //
    // Configuration complete. Enable the module.
    //
    SPI_enableModule(SPI_MODULE);

}



Void spiaRxIsr (UArg arg)
{
    for(uint16_t i = 0; i < DATA_SIZE; i++)
    {
        rData[i] = SPI_readDataNonBlocking(SPI_MODULE);
    }
    //GPIO_writePin(SPISTEA_PIN, true);

    // Clear interrupt flag and issue ACK
    //
    SPI_clearInterruptStatus(SPI_MODULE, SPI_INT_RXFF);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP6);
    return;
}

