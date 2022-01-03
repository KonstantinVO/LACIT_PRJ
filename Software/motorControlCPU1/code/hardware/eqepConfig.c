
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/c28/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include "stdint.h"
#include "F28x_Project.h"
#include "device.h"
#include "F2837xD_GlobalPrototypes.h" //init_gpio
#include "F2837xD_eqep.h"
#include "f2837xqep.h"
//#include "ACImotorControl.h"
#include "BLDCmotorControl.h"
#include "configProject.h"
#include "IQmathLib.h"
#include "eqepConfig.h"
#include "F2837xqep.h"
#include "F2837xD_eqep.h"


QEP qep1 = QEP_DEFAULTS;

void initQEPpins(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;    // Disable pull-up on GPIO24 (EQEP2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1;    // Disable pull-up on GPIO25 (EQEP2B)
    //GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1;    // Disable pull-up on GPIO26 (EQEP2S)
    //GpioCtrlRegs.GPAPUD.bit.GPIO27 = 1;    // Disable pull-up on GPIO27 (EQEP2I)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;   // Sync GPIO24 to SYSCLK  (EQEP2A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;   // Sync GPIO25 to SYSCLK  (EQEP2B)
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 0;   // Sync GPIO26 to SYSCLK  (EQEP2S)
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 0;   // Sync GPIO27 to SYSCLK  (EQEP2I)

    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;    // Configure GPIO24 as EQEP2A
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;    // Configure GPIO25 as EQEP2B
    //GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;    // Configure GPIO26 as EQEP2S
    //GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 2;    // Configure GPIO27 as EQEP2I

    EDIS;
}

void initQEPmodule(void)
{
    //InitEQep2Gpio();
    initQEPpins();

    // Initialize QEP module
    qep1.LineEncoder = 256;
    qep1.MechScaler = _IQ30(0.25/qep1.LineEncoder);
    qep1.PolePairs = POLES/2;
    qep1.CalibratedAngle = 0;

    QEP_INIT_MACRO(&EQep2Regs,qep1);

}





