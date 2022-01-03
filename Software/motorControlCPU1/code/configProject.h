
#ifndef CONFIG_PROJECTS_H_
#define CONFIG_PROJECTS_H_



#define PI 3.14159265358979

//This line sets the SAMPLING FREQUENCY to one of the available choices
#define  SINGLE_SAMPLING    1
#define  DOUBLE_SAMPLING    2

/*********MICROCONTROLLER****/
#define SYSTEM_FREQUENCY 200

/******MOTOR_TYPE********/

#define ACI_MOTOR   0x01
#define BLDC_MOTOR  0x02
#define PMSM_MOTOR  0x03

#define MOTOR_TYPE  BLDC_MOTOR

/******NUM_MOTOR********/
#define MOTOR_1   0x01
#define MOTOR_2   0x02
#define MOTOR_ALL 0x03
//#define MOTOR_NUM  macro is defined in MOTOR_TYPE

enum
{
   motor1,
   motor2
};

enum
{
    phase_a,
    phase_b,
    phase_c,
    dc_bus

};


#if MOTOR_TYPE == ACI_MOTOR

    #define LEVEL1  1           // Module check out, duty cycle waveforms and PWM update
    #define LEVEL2  2           // Verify ADC, park/clarke, calibrate the offset
    #define LEVEL3  3           // Two current PI regulator and speed measurement test
    #define LEVEL4  4           // Verify current model
    #define LEVEL5  5           // Sensored closed-loop FOC

    #define BUILDLEVEL LEVEL4

    #define   SAMPLING_METHOD     SINGLE_SAMPLING    // SINGLE_SAMPLING // DOUBLE_SAMPLING

    // Define the PWM frequency and ISR frequency (kHz)
    #define PWM_FREQUENCY    (8 * 2)

    #if (SAMPLING_METHOD == SINGLE_SAMPLING)
      #define ISR_FREQUENCY           (PWM_FREQUENCY)
    #elif (SAMPLING_METHOD == DOUBLE_SAMPLING)
      #define ISR_FREQUENCY           (2*PWM_FREQUENCY)
    #endif

    #define T  0.001/ISR_FREQUENCY // Samping period (sec)

    #define MOTOR_NUM MOTOR_1 //motor selection

#elif MOTOR_TYPE == BLDC_MOTOR

    #define BLDC_LEVEL1  1           // Module check out (do not connect the motors)
    #define BLDC_LEVEL2  2           // Verify ADC, calibrate the offset, run the motor open loop
    #define BLDC_LEVEL3  3           // Verify closed-loop operation of BLDC, not regulated.
    #define BLDC_LEVEL4  4           // Verify the closed current loop and current PI controller.
    #define BLDC_LEVEL5  5           // Verify the closed speed loop and speed PI controller.

    #define   BUILDLEVEL          BLDC_LEVEL2         // FCL_LEVEL1-6       //

    #define   SAMPLING_METHOD     DOUBLE_SAMPLING    // SINGLE_SAMPLING // DOUBLE_SAMPLING

    // Define the PWM frequency and ISR frequency (kHz)
    #define  PWM_FREQUENCY           (8 * 2)


    #if (SAMPLING_METHOD == SINGLE_SAMPLING)
      #define ISR_FREQUENCY           (PWM_FREQUENCY)
    #elif (SAMPLING_METHOD == DOUBLE_SAMPLING)
      #define ISR_FREQUENCY           (2*PWM_FREQUENCY)
    #endif

    #define  T 0.001/ISR_FREQUENCY   // Samping period (sec)

    #define MOTOR_NUM MOTOR_1 //motor selection

#elif MOTOR_TYPE == PMSM_MOTOR


    #define MOTOR_NUM MOTOR_1

#endif


#endif /* CONFIG_PROJECTS_H_ */
