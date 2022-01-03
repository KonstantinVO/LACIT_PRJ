

#ifndef CODE_ADC_CONFIG_H_
#define CODE_ADC_CONFIG_H_

#include "configProject.h"
#include "adc.h"



#define NUM_ADC_CH 4


#if MOTOR_TYPE == ACI_MOTOR

    #define RESAULT_ADC_PHASE_A ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER0)
    #define RESAULT_ADC_PHASE_B ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER1)
    #define RESAULT_ADC_PHASE_C ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER2)
    #define RESAULT_ADC_DC_BUS  ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER3)

#elif MOTOR_TYPE == BLDC_MOTOR

    #define RESAULT_ADC_PHASE_A      AdccResultRegs.ADCRESULT1
    #define RESAULT_ADC_PHASE_B      AdcbResultRegs.ADCRESULT1
    #define RESAULT_ADC_PHASE_C      AdcaResultRegs.ADCRESULT1
    #define RESAULT_ADC_DC_BUS_CURR  AdcaResultRegs.ADCRESULT2

#elif MOTOR_TYPE == PMSM_MOTOR

#endif



//void ConfigureADC(void);
void initADC(void);
void adcData(void);
void ConfigProtection(void);

#endif /* CODE_ADC_CONFIG_H_ */
