/*
 *  ======== main.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "driverlib.h"
#include "device.h"

#include "utils.h"
#include "configProject.h"

#if MOTOR_TYPE == ACI_MOTOR
    #include "ACImotorControl.h"
#elif MOTOR_TYPE == BLDC_MOTOR
    #include "BLDCmotorControl.h"
#elif MOTOR_TYPE == PMSM_MOTOR
    #include "PMSMCmotorControl.h"
#endif

#define LED_PIN_CPU1 61


void initTestPin(void)
{
    GPIO_setPadConfig(LED_PIN_CPU1, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO8
    GPIO_writePin(LED_PIN_CPU1, 1);                            // Load output latch
    GPIO_setPinConfig(GPIO_61_GPIO61);                // GPIO31 = GPIO31
    GPIO_setDirectionMode(LED_PIN_CPU1, GPIO_DIR_MODE_OUT);
    GPIO_setMasterCore(LED_PIN_CPU1, GPIO_CORE_CPU1);

 //-------------
    GPIO_setPadConfig(34, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO8
    GPIO_writePin(34, 1);                            // Load output latch
    GPIO_setPinConfig(GPIO_34_GPIO34);                // GPIO34 = GPIO34
    GPIO_setDirectionMode(34, GPIO_DIR_MODE_OUT);
    GPIO_setMasterCore(34, GPIO_CORE_CPU2);
}



/*
 *  ======== taskFxn ========
 */

Void taskLed(UArg a0, UArg a1)
{
    while(1)
    {
        GPIO_togglePin(LED_PIN_CPU1);

        //adcData();

        Task_sleep(50);
    }
}


/* 125us - 8KHz*/
Void timerMotorControl(UArg arg0)
{
    #if MOTOR_TYPE == ACI_MOTOR
        ACImotorControl();
    #elif MOTOR_TYPE == BLDC_MOTOR
        BLDCmotorControl();
    #elif MOTOR_TYPE == PMSM_MOTOR
        PMSMmotorControl();
    #endif

}



/*
 *  ======== main ========
 */


/*********************************
 * The TMS320C28x byte is 16 bits
 * description :
   https://software-dl.ti.com/ccs/esd/documents/c2000_byte-accesses-with-the-c28x-cpu.html
*********************************/

Int main()//CPU1
{ 
    //Device_init();
    Device_configureTMXAnalogTrim();
    Device_enableAllPeripherals();
    Device_initGPIO();

    System_printf("enter CPU1\n");
    System_flush();

    #ifdef _FLASH
    Device_bootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
    #else
    Device_bootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
    #endif

    initTestPin();

    #if MOTOR_TYPE == ACI_MOTOR

        HWconfigACImotorControl();
        SWconfigACImotorControl();

    #elif MOTOR_TYPE == BLDC_MOTOR

        HWconfigBLDCmotorControl();
        SWconfigBLDCmotorControl();

    #elif MOTOR_TYPE == PMSM_MOTOR

    #endif

    EINT;
    ERTM;

    BIOS_start();    /* does not return */
    return(0);
}
