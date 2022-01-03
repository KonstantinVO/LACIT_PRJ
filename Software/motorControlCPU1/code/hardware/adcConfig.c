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
#include "f2837xadc.h"
#include "utils.h"



#define RESOLUTION_12_BIT   0x01
#define RESOLUTION_16_BIT   0x02
#define EX_ADC_RESOLUTION   RESOLUTION_12_BIT

#define EPWM_BASE          EPWM4_BASE


#if(EX_ADC_RESOLUTION == RESOLUTION_12_BIT)
    #define  ADC_ACQPS    8
#elif(EX_ADC_RESOLUTION == RESOLUTION_16_BIT)
    #define  ADC_ACQPS    63// 320ns
#endif


void configureADC(uint32_t adcBase)
{
    // Set ADCDLK divider to /4
    ADC_setPrescaler(adcBase, ADC_CLK_DIV_4_0);
    // Set resolution and signal mode (see #defines above) and load
    // corresponding trims.
#if(EX_ADC_RESOLUTION == RESOLUTION_12_BIT)
    ADC_setMode(adcBase, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
#elif(EX_ADC_RESOLUTION == RESOLUTION_16_BIT)
    ADC_setMode(adcBase, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
#endif
    // Set pulse positions to late
    ADC_setInterruptPulseMode(adcBase, ADC_PULSE_END_OF_CONV);
    // Power up the ADCs and then delay for 1 ms
    ADC_enableConverter(adcBase);
    // Delay for 1ms to allow ADC time to power up
    DEVICE_DELAY_US(1000);
}


void configureEPWM(uint32_t epwmBase)
{
    // Make the timer count up with a period of 40us
    HWREGH(epwmBase + EPWM_O_TBCTL) = 0x0000U;
    //EPWM_setTimeBasePeriod(epwmBase, 4000U);
    EPWM_setTimeBasePeriod(epwmBase, 100U);

    // Set the A output on zero and reset on CMPA
    EPWM_setActionQualifierAction(epwmBase, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(epwmBase, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    // Set CMPA to 20us to get a 50% duty
    //EPWM_setCounterCompareValue(epwmBase, EPWM_COUNTER_COMPARE_A, 2000U);
    EPWM_setCounterCompareValue(epwmBase, EPWM_COUNTER_COMPARE_A, 50U);

    // Start ADC when timer equals zero (note: don't enable yet)
    EPWM_setADCTriggerSource(epwmBase, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);
    EPWM_setADCTriggerEventPrescale(epwmBase, EPWM_SOC_A, 1U);

    // Enable initialization of the SOCA event counter. Since we are
    // disabling the ETSEL.SOCAEN bit, we need a way to reset the SOCACNT.
    // Hence, enable the counter initialize control.
    EPWM_enableADCTriggerEventCountInit(epwmBase, EPWM_SOC_A);
}


void initADC(void)
{
    //Interrupt_enable(INT_ADCA1);

    #if MOTOR_TYPE == ACI_MOTOR

        configureADC(ADCA_BASE);

        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN0, ADC_ACQPS);
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN1, ADC_ACQPS);
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN2, ADC_ACQPS);
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN3, ADC_ACQPS);

        ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);
        ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER1,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);
        ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER2,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);
        ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER3,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);

        ADC_enableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);

        ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);



    #elif MOTOR_TYPE == BLDC_MOTOR

        configureADC(ADCA_BASE);
        configureADC(ADCB_BASE);
        configureADC(ADCC_BASE);


        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN2, ADC_ACQPS);
        ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN2, ADC_ACQPS);
        ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN2, ADC_ACQPS);
        ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA,
                                 (ADC_Channel)ADC_CH_ADCIN3, ADC_ACQPS);

        ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER1,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);
        ADC_setInterruptSOCTrigger(ADCB_BASE, ADC_SOC_NUMBER1,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);
        ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER2,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);
        ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER3,
                                   ADC_INT_SOC_TRIGGER_ADCINT1);

        ADC_enableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_enableContinuousMode(ADCB_BASE, ADC_INT_NUMBER1);
        ADC_enableContinuousMode(ADCC_BASE, ADC_INT_NUMBER1);

        ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_enableInterrupt(ADCB_BASE, ADC_INT_NUMBER1);
        ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER1);

        ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
        ADC_setInterruptSource(ADCB_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
        ADC_setInterruptSource(ADCC_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);

    #endif

    //configireEPWM();
    configureEPWM(EPWM_BASE);
    EPWM_forceADCTriggerEventCountInit(EPWM_BASE, EPWM_SOC_A); // EPWM4 SOCA
    EPWM_clearADCTriggerFlag(EPWM_BASE, EPWM_SOC_A);    // EPWM4 SOCA
    EPWM_enableADCTrigger(EPWM_BASE, EPWM_SOC_A);
}





// *****testing ****

float test_adc[6];
//struct filter_run_average fra[NUM_ADC_CH];


void adcData(void)
{

    test_adc[0] = RESAULT_ADC_PHASE_A / 4095.0 * 3.3;
    test_adc[1] = RESAULT_ADC_PHASE_B / 4095.0 * 3.3;
    test_adc[2] = RESAULT_ADC_PHASE_C / 4095.0 * 3.3;
    //test_adc[3] = RESAULT_ADC_DC_BUS  / 4095.0 * 3.3;
    //test_adc[4] = filterAverage(&fra[0],RESAULT_ADC_DC_BUS) / 4095.0 * 3.3;
    //test_adc[5] = kalmanFilter(RESAULT_ADC_DC_BUS)/ 4095.0 * 3.3;

}


