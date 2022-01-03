
#include "pwmConfig.h"
#include "f2837xpwm.h"
#include "f2837xbldcpwm.h"
#include "driverlib.h"
#include "device.h"
 #include "svgen.h"
#include "configProject.h"
 #include "mod6_cnt.h"
#include "F2837xD_EPwm.h"
#include "F2837xD_GlobalPrototypes.h" //init_gpio


#if MOTOR_TYPE == ACI_MOTOR
    #include "ACImotorControl.h"
#elif MOTOR_TYPE == BLDC_MOTOR
    #include "BLDCmotorControl.h"
#endif




#if MOTOR_TYPE == ACI_MOTOR

    extern SVGEN svgen1;
    PWMGEN pwm1 = PWMGEN_DEFAULTS;

#elif MOTOR_TYPE == BLDC_MOTOR

    extern MOD6CNT mod1_bldc;
    PWMGEN_BLDC pwm1_bldc = PWMGEN_DEFAULTS_BLDC;

    int16  DFuncDesired = 0x1300;      // Desired duty cycle (Q15)
    int16  DfuncTesting = 0x1300;
#endif



void initPWMgpio(void)
{
    #if MOTOR_NUM == MOTOR_1

         //PWM_CH_1
         InitEPwm1Gpio();
         InitEPwm2Gpio();
         InitEPwm3Gpio();

    #elif MOTOR_NUM == MOTOR_2

         //PWM_CH_2
         InitEPwm6Gpio();
         InitEPwm7Gpio();
         InitEPwm8Gpio();


    #elif MOTOR_NUM == MOTOR_ALL
          EALLOW;
          //PWM_CH_1
          InitEPwm1Gpio();
          InitEPwm2Gpio();
          InitEPwm3Gpio();
          //PWM_CH_2
          InitEPwm6Gpio();
          InitEPwm7Gpio();
          InitEPwm8Gpio();

    #endif

}

void initPWMmodule(void)
{
    initPWMgpio();

    #if MOTOR_TYPE == ACI_MOTOR

        pwm1.PeriodMax = SYSTEM_FREQUENCY*1000000*T/2;  // Prescaler X1 (T1), ISR period = T x 1
        pwm1.HalfPerMax=pwm1.PeriodMax/2;
        pwm1.Deadband  = 2.0*SYSTEM_FREQUENCY;          // 120 counts -> 2.0 usec for TBCLK = SYSCLK/1
        PWM_INIT_MACRO(1,2,3,pwm1)

    #elif MOTOR_TYPE == BLDC_MOTOR

        pwm1_bldc.PeriodMax = (SYSTEM_FREQUENCY/PWM_FREQUENCY)*1000;  // Asymmetric PWM
        pwm1_bldc.DutyFunc  = ALIGN_DUTY;                             // DutyFunc = Q15

        BLDCPWM_INIT_MACRO(1,2,3,pwm1_bldc)
        //BLDCPWM_INIT_MACRO(4,5,6,pwm1_bldc) //only testing!

    #endif


}

void pwmMotor(void)
{

    #if MOTOR_TYPE == ACI_MOTOR
        pwm1.MfuncC1 = svgen1.Ta;
        pwm1.MfuncC2 = svgen1.Tb;
        pwm1.MfuncC3 = svgen1.Tc;
        PWM_MACRO(1,2,3,pwm1);
    #elif MOTOR_TYPE == BLDC_MOTOR

        pwm1_bldc.CmtnPointer = (int16)mod1_bldc.Counter;
        pwm1_bldc.DutyFunc = DfuncTesting;

        BLDCPWM_MACRO(1,2,3,pwm1_bldc);
       // BLDCPWM_MACRO(4,5,6,pwm1_bldc); //only testing!

    #endif


}
