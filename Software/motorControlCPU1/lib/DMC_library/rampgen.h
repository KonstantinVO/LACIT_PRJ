/* =================================================================================
File name:        RAMPGEN.H
===================================================================================*/

#ifndef __RAMPGEN_H__
#define __RAMPGEN_H__

#include "IQmathLib.h"
#include "F2837xD_device.h"
#include "ACImotorControl.h"

typedef struct { _iq  Freq;         // Input: Ramp frequency (pu)
                 _iq  StepAngleMax; // Parameter: Maximum step angle (pu)
                 _iq  Angle;        // Variable: Step angle (pu)
                 _iq  Gain;         // Input: Ramp gain (pu)
                 _iq  Out;          // Output: Ramp signal (pu)
                 _iq  Offset;       // Input: Ramp offset (pu)
               } RAMPGEN;

/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/
#define DEFAULT_SAM  _IQ(BASE_FREQ*T)//_IQ(0.00012)//BASE_FREQ

#define RAMPGEN_DEFAULTS {0,           \
                          DEFAULT_SAM, \
                          0,           \
                          _IQ(1),      \
                          0,           \
                          _IQ(1),      \
                         }

/*------------------------------------------------------------------------------
    RAMP(Sawtooh) Generator Macro Definition
------------------------------------------------------------------------------*/

#define RG_MACRO(v)                                 \
                                                    \
/* Compute the angle rate */                        \
    v.Angle += _IQmpy(v.StepAngleMax,v.Freq);       \
                                                    \
/* Saturate the angle rate within (-1,1) */         \
    if (v.Angle>_IQ(1.0))                           \
        v.Angle -= _IQ(1.0);                        \
    else if (v.Angle<_IQ(-1.0))                     \
        v.Angle += _IQ(1.0);                        \
        v.Out=v.Angle;
        //\
/* Compute the ramp output */                       \
    v.Out = _IQmpy(v.Angle,v.Gain) + v.Offset;      \
/* Saturate the ramp output within (-1,1) */        \
    if (v.Out>_IQ(1.0))                             \
        v.Out -= _IQ(1.0);                          \
    else if (v.Out<_IQ(-1.0))                       \
        v.Out += _IQ(1.0);

#endif // __RAMPGEN_H__
