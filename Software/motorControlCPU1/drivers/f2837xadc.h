

#ifndef LIB_DRIVERS_F2837XADC_H_
#define LIB_DRIVERS_F2837XADC_H_


#include "f2837xbmsk.h"

/*------------------------------------------------------------------------------
 ADC Initialization Macro Definition
------------------------------------------------------------------------------*/


#define CPU_CLOCK_SPEED      16.666L   // 10.000L for a 100MHz CPU clock speed
#define ADC_usDELAY 10000L


#define ADC_MACRO_INIT(AdcRegs,ChSel,Trigsel,ACQPS)                                             \
                                                                                                \
    DEVICE_DELAY_US(1000);                                                                      \
    AdcRegs.ADCCTL1.all=ADC_RESET_FLAG;                                                         \
    asm(" NOP ");                                                                               \
    asm(" NOP ");                                                                               \
                                                                                                \
    EALLOW;                                                                                     \
                                                                                                \
    DEVICE_DELAY_US(1000);                  /* Delay before powering up rest of ADC */          \
                                                                                        \
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;                                                          \
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;                                                          \
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;                                                          \
                                                                                                \
    asm(" RPT#100 || NOP");                                                                     \
                                                                                                \
    AdcRegs.ADCCTL1.bit.INTPULSEPOS=1;                                                          \
                                                                                                \
    DELAY_US(ADC_usDELAY);                                                                      \
                                                                                                \
    /******* CHANNEL SELECT *******/                                                            \
                                                                                                \
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ChSel[0];                                                   \
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ChSel[1];                                                   \
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ChSel[2];                                                   \
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ChSel[3];                                                   \
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ChSel[4];                                                   \
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ChSel[5];                                                   \
    AdcRegs.ADCSOC6CTL.bit.CHSEL  = ChSel[6];                                                   \
    AdcRegs.ADCSOC7CTL.bit.CHSEL  = ChSel[7];                                                   \
    AdcRegs.ADCSOC8CTL.bit.CHSEL  = ChSel[8];                                                   \
    AdcRegs.ADCSOC9CTL.bit.CHSEL  = ChSel[9];                                                   \
    AdcRegs.ADCSOC10CTL.bit.CHSEL = ChSel[10];                                                  \
    AdcRegs.ADCSOC11CTL.bit.CHSEL = ChSel[11];                                                  \
    AdcRegs.ADCSOC12CTL.bit.CHSEL = ChSel[12];                                                  \
    AdcRegs.ADCSOC13CTL.bit.CHSEL = ChSel[13];                                                  \
    AdcRegs.ADCSOC14CTL.bit.CHSEL = ChSel[14];                                                  \
    AdcRegs.ADCSOC15CTL.bit.CHSEL = ChSel[15];                                                  \
                                                                                                \
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = Trigsel[0]; /* Set SOC0 start trigger on EPWM1A */        \
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = Trigsel[1];                                               \
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = Trigsel[2];                                               \
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = Trigsel[3];                                               \
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL  = Trigsel[4];                                               \
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL  = Trigsel[5];                                               \
    AdcRegs.ADCSOC6CTL.bit.TRIGSEL  = Trigsel[6];                                               \
    AdcRegs.ADCSOC7CTL.bit.TRIGSEL  = Trigsel[7];                                               \
    AdcRegs.ADCSOC8CTL.bit.TRIGSEL  = Trigsel[8];                                               \
    AdcRegs.ADCSOC9CTL.bit.TRIGSEL  = Trigsel[9];                                               \
    AdcRegs.ADCSOC10CTL.bit.TRIGSEL = Trigsel[10];                                              \
    AdcRegs.ADCSOC11CTL.bit.TRIGSEL = Trigsel[11];                                              \
    AdcRegs.ADCSOC12CTL.bit.TRIGSEL = Trigsel[12];                                              \
    AdcRegs.ADCSOC13CTL.bit.TRIGSEL = Trigsel[13];                                              \
    AdcRegs.ADCSOC14CTL.bit.TRIGSEL = Trigsel[14];                                              \
    AdcRegs.ADCSOC15CTL.bit.TRIGSEL = Trigsel[15];                                              \
                                                                                                \
                                                                                                \
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS[0];/* Set SOC0 S/H Window to 6+1 ADC Clock Cycles */  \
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS[1];                                                   \
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS[2];                                                   \
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS[3];                                                   \
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS[4];                                                   \
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS[5];                                                   \
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS[6];                                                   \
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS[7];                                                   \
    AdcRegs.ADCSOC8CTL.bit.ACQPS  = ACQPS[8];                                                   \
    AdcRegs.ADCSOC9CTL.bit.ACQPS  = ACQPS[9];                                                   \
    AdcRegs.ADCSOC10CTL.bit.ACQPS = ACQPS[10];                                                  \
    AdcRegs.ADCSOC11CTL.bit.ACQPS = ACQPS[11];                                                  \
    AdcRegs.ADCSOC12CTL.bit.ACQPS = ACQPS[12];                                                  \
    AdcRegs.ADCSOC13CTL.bit.ACQPS = ACQPS[13];                                                  \
    AdcRegs.ADCSOC14CTL.bit.ACQPS = ACQPS[14];                                                  \
    AdcRegs.ADCSOC15CTL.bit.ACQPS = ACQPS[15];                                                  \
                                                                                                \
    EDIS;                                                                                       \
                                                                                                \
                                                                                                \
    /* Set up Event Trigger with period enable for Time-base of EPWM1 */                        \
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;     /* Enable SOCA */                                       \
    EPwm1Regs.ETSEL.bit.SOCASEL = 2;    /* Enable period event for SOCA */                      \
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;     /* Generate SOCA on the 1st event */                    \
    EPwm1Regs.ETCLR.bit.SOCA = 1;       /* Clear SOCA flag */


#endif /* LIB_DRIVERS_F2837XADC_H_ */
