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



/*
 *  ======== taskFxn ========
 */
Void taskLed(UArg a0, UArg a1)
{
    while(1)
    {
        GPIO_togglePin(34);//LED_R
        GPIO_togglePin(123);//J2_18

        Task_sleep(100);
    }
}

/*
 *  ======== main ========
 */
Int main()//CPU2
{ 

    Device_init();
    System_printf("enter CPU2\n");
    System_flush();
    EINT;
    ERTM;

    BIOS_start();    /* does not return */
    return(0);
}

