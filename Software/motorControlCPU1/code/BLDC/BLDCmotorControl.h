

#ifndef BLDC_MOTOR_CONTROL_H_
#define BLDC_MOTOR_CONTROL_H_


#include "pi.h"         // Include header for the PIDREG3 object
#include "rmp2cntl.h"           // Include header for the RMP2 object
#include "rmp3cntl.h"           // Include header for the RMP3 object
#include "impulse.h"            // Include header for the IMPULSE object
#include "mod6_cnt.h"           // Include header for the MOD6CNT object
#include "speed_pr.h"           // Include header for the SPEED_MEAS_REV object
#include "rmp_cntl.h"           // Include header for the RMPCNTL object

// Parameter Initializations
#define ALIGN_DUTY   0x0500
#define LOOP_CNT_MAX 0

// This machine parameters are based on BLDC motor
// Define the BLDC motor parameters

#define RS      2.35                    // Stator resistance (ohm)
#define RR                              // Rotor resistance (ohm)
#define LS      0.007                   // Stator inductance (H)
#define LR                              // Rotor inductance (H)
#define LM                              // Magnatizing inductance (H)
#define POLES   8                       // Number of poles

// Define the base quantites
#define BASE_VOLTAGE    236             // Base peak phase voltage (volt)
#define BASE_CURRENT    10              // Base peak phase current (amp)
#define BASE_TORQUE                     // Base torque (N.m)
#define BASE_FLUX                       // Base flux linkage (volt.sec/rad)
#define BASE_FREQ       200             // Base electrical frequency (Hz)

void HWconfigBLDCmotorControl(void);
void SWconfigBLDCmotorControl(void);
void BLDCmotorControl(void);

#endif /* BLDC_MOTOR_CONTROL_H_ */
