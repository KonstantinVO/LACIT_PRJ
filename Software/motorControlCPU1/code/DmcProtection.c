
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/c28/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include "stdint.h"
#include "F28x_Project.h"
#include "adcConfig.h"
#include "driverlib.h"
#include "device.h"

//==========================================================/
//===============Protection Configuration===================/
//==========================================================/


void ConfigProtection(void)
{

// Configure Trip Mechanism for the Motor control software
// -Cycle by cycle trip on CPU halt
// -One shot IPM trip zone trip
// These trips need to be repeated for EPWM1 ,2 & 3

//===========================================================================
//Motor Control Trip Config, EPwm1,2,3
//===========================================================================
      EALLOW;
// CPU Halt Trip
      EPwm1Regs.TZSEL.bit.CBC6=0x1;
      EPwm2Regs.TZSEL.bit.CBC6=0x1;
      EPwm3Regs.TZSEL.bit.CBC6=0x1;


      EPwm1Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT
      EPwm2Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT
      EPwm3Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT

// What do we want the OST/CBC events to do?
// TZA events can force EPWMxA
// TZB events can force EPWMxB

      EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EDIS;

     // Clear any spurious OV trip
      EPwm1Regs.TZCLR.bit.OST = 1;
      EPwm2Regs.TZCLR.bit.OST = 1;
      EPwm3Regs.TZCLR.bit.OST = 1;

}//End of protection configuration


/*void DMC1_Protection_Config(void)
{
    EALLOW;

    // Use Mux to generate TRIP4
    // Clear everything first
    EPwmXbarRegs.TRIP4MUX0TO15CFG.all  = 0x0000;    // clear all MUXes - MUX  0 to 15
    EPwmXbarRegs.TRIP4MUX16TO31CFG.all = 0x0000;    // clear all MUXes - MUX 16 to 31
    EPwmXbarRegs.TRIP4MUXENABLE.all    = 0x0000;    // Disable all the muxes first

    EDIS;

#if (MOTOR1_DRV != GaN_BP)
    // Configure GPIO used for Trip Mechanism
    // Configure as Input - Motor 1 - FAULT - (active low), trip PWM based on this
    GPIO_SetupPinOptions(MOTOR1_FAULT_GPIO, GPIO_INPUT, GPIO_INVERT);
    GPIO_SetupPinMux(MOTOR1_FAULT_GPIO, 0, MOTOR1_FAULT_MUX);

    EALLOW;
    // Enable Mux for InputXbar1
    InputXbarRegs.INPUT1SELECT = MOTOR1_FAULT_GPIO; // Select FAULT as INPUTXBAR1
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX1 = 1;     // Enable Mux for ored input of inputxbar1
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX1   = 1;     // Enable MUX1 to generate TRIP4
    EDIS;

#if (MOTOR1_DRV == DRV8301)
    // Configure GPIO used for Trip Mechanism
    // Configure as Input - Motor 1 - OCTW - (active low), trip PWM based on this
    GPIO_SetupPinOptions(MOTOR1_OCTW_GPIO, GPIO_INPUT, GPIO_INVERT);
    GPIO_SetupPinMux(MOTOR1_OCTW_GPIO, 0, MOTOR1_OCTW_MUX);

    EALLOW;
    // Enable Mux for InputXbar2
    InputXbarRegs.INPUT2SELECT = MOTOR1_OCTW_GPIO;  // Select OCTW as INPUTXBAR2
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX3 = 1;     // Enable Mux for ored input of inputxbar2
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX3   = 1;     // Enable MUX3 to generate TRIP4
    EDIS;
#endif
#endif

    // =========================================================
    // CMPSS used for programmable OCP
    // =========================================================
    // LEM Current phase V(ADC A2, COMP1) and W(ADC B2, COMP3), High Low Compare event trips
    OCP_curHi = OCP_INA240_HI(curLimit);
    OCP_curLo = OCP_INA240_LO(curLimit);
    cmpssConfig(motor1.CmpssARegs, OCP_curHi, OCP_curLo);  //Enable CMPSS1 - LEM A
    cmpssConfig(motor1.CmpssBRegs, OCP_curHi, OCP_curLo);  //Enable CMPSS3 - LEM B
    cmpssConfig(motor1.CmpssCRegs, OCP_curHi, OCP_curLo);  //Enable CMPSS6 - LEM C

    EALLOW;
    // Connect Muxes for ored input of CMPSSxH and xL
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX10 = 1;  //cmpss6 - tripH or tripL - current Ia
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX4  = 1;  //cmpss3 - tripH or tripL - current Ib
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX0  = 1;  //cmpss1 - tripH or tripL - current Ic

    // Enable Mux to generate TRIP4
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX10 = 1;   // current Ia
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4  = 1;   // current Ib
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX0  = 1;   // current Ic
    EDIS;

    PwmTripConfig(motor1.PwmARegs, TRIP4);
    PwmTripConfig(motor1.PwmBRegs, TRIP4);
    PwmTripConfig(motor1.PwmCRegs, TRIP4);

    return;
}

// Setup OCP limits and digital filter parameters of CMPSS
void CMPSS_DIG_FILTER(volatile struct CMPSS_REGS * v, Uint16 curHi, Uint16 curLo)
{
    // comparator references
    v->DACHVALS.bit.DACVAL = curHi;   // positive max current limit
    v->DACLVALS.bit.DACVAL = curLo;   // negative max current limit

    // digital filter settings - HIGH side
    v->CTRIPHFILCLKCTL.bit.CLKPRESCALE = clkPrescale;    // set time between samples, max : 1023
    v->CTRIPHFILCTL.bit.SAMPWIN        = sampwin;        // # of samples in window, max : 31
    v->CTRIPHFILCTL.bit.THRESH         = thresh;         // recommended : thresh > sampwin/2

    // digital filter settings - LOW side
    v->CTRIPLFILCLKCTL.bit.CLKPRESCALE = clkPrescale;    // Max count of 1023
    v->CTRIPLFILCTL.bit.SAMPWIN        = sampwin;        // # of samples in window, max : 31
    v->CTRIPLFILCTL.bit.THRESH         = thresh;         // recommended : thresh > sampwin/2

    return;
}


void DMC_Protection (MOTOR_VARS * motor)
{

    // Current limit setting / tuning in Debug environment

    EALLOW;
      OCP_curHi = OCP_INA240_HI(curLimit);
      OCP_curLo = OCP_INA240_LO(curLimit);

      CMPSS_DIG_FILTER(motor->CmpssARegs, OCP_curHi, OCP_curLo);     // LEM - A
      CMPSS_DIG_FILTER(motor->CmpssBRegs, OCP_curHi, OCP_curLo);     // LEM - B
      CMPSS_DIG_FILTER(motor->CmpssCRegs, OCP_curHi, OCP_curLo);     // LEM - C
    EDIS;

    // Check for PWM trip due to over current
    if ( (motor->PwmARegs)->TZFLG.bit.OST |
         (motor->PwmBRegs)->TZFLG.bit.OST |
         (motor->PwmCRegs)->TZFLG.bit.OST
       )
    {
        EALLOW;
          (motor->PwmARegs)->TZFRC.bit.OST = 0;
          (motor->PwmBRegs)->TZFRC.bit.OST = 0;
          (motor->PwmCRegs)->TZFRC.bit.OST = 0;
        EDIS;

        motor->TripFlagDMC = 1;           // Trip on DMC (fault trip )
        DISABLE_INVERTER(motor);
        motor->RunMotor = STOP;
        motor->cntlr_id.Out = 0;
        motor->cntlr_iq.Out = 0;
    }

    // If clear cmd received, reset PWM trip
    if (motor->clearTripFlagDMC)
    {
        ENABLE_INVERTER(motor);
        DELAY_US(50000);                        // DRV830x settling time

        motor->TripFlagDMC = 0;
        motor->clearTripFlagDMC = 0;

        // clear EPWM trip flags
        EALLOW;
          (motor->PwmARegs)->TZCLR.bit.OST = 1;
          (motor->PwmBRegs)->TZCLR.bit.OST = 1;
          (motor->PwmCRegs)->TZCLR.bit.OST = 1;

          // clear HLATCH - (not in TRIP gen path)
          (motor->CmpssARegs)->COMPSTSCLR.bit.HLATCHCLR = 1;
          (motor->CmpssBRegs)->COMPSTSCLR.bit.HLATCHCLR = 1;
          (motor->CmpssCRegs)->COMPSTSCLR.bit.HLATCHCLR = 1;

          // clear LLATCH - (not in TRIP gen path)
          (motor->CmpssARegs)->COMPSTSCLR.bit.LLATCHCLR = 1;
          (motor->CmpssBRegs)->COMPSTSCLR.bit.LLATCHCLR = 1;
          (motor->CmpssCRegs)->COMPSTSCLR.bit.LLATCHCLR = 1;
        EDIS;
    }

    return;
}*/


