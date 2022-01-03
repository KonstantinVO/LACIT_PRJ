/* =================================================================================
File name:        RMP_CNTL.H
===================================================================================*/


#ifndef __RMP_CNTL_H__
#define __RMP_CNTL_H__

#include "IQmathLib.h"
#include "F2837xD_device.h"

typedef struct { _iq    TargetValue;    // Input: Target input (pu)
                 Uint32 RampDelayMax;   // Parameter: Maximum delay rate (Q0) - independently with global Q
                 _iq    RampLowLimit;   // Parameter: Minimum limit (pu)
                 _iq    RampHighLimit;  // Parameter: Maximum limit (pu)
                 Uint32 RampDelayCount; // Variable: Incremental delay (Q0) - independently with global Q
                 _iq    SetpointValue;  // Output: Target output (pu)
                 Uint32 EqualFlag;      // Output: Flag output (Q0) - independently with global Q
                 _iq    Tmp;            // Variable: Temp variable
               } RMPCNTL;


/*-----------------------------------------------------------------------------
Default initalizer for the RMPCNTL object.
-----------------------------------------------------------------------------*/
#define DEFAULT_RDM      3
#define DEFAULT_TS       0.0000305

#define RMPCNTL_DEFAULTS {  0,           \
                            DEFAULT_RDM, \
                           _IQ(-1),      \
                           _IQ(1),       \
                            0,           \
                            0,           \
                            0,           \
                            0,           \
                          }

/*------------------------------------------------------------------------------
    RAMP Controller Macro Definition
------------------------------------------------------------------------------*/

#define RC_MACRO(v)                                                                 \
    v.Tmp = v.TargetValue - v.SetpointValue;                                        \
/*  DEFAULT_TS is resolution of Q15 */                                               \
if (_IQabs(v.Tmp) >= _IQ(DEFAULT_TS))                                               \
{                                                                                   \
    v.RampDelayCount++  ;                                                           \
        if (v.RampDelayCount >= v.RampDelayMax)                                     \
        {                                                                           \
            if (v.TargetValue >= v.SetpointValue)                                   \
                v.SetpointValue += _IQ(DEFAULT_TS);                                 \
            else                                                                    \
                v.SetpointValue -= _IQ(DEFAULT_TS);                                 \
                                                                                    \
            v.SetpointValue=_IQsat(v.SetpointValue,v.RampHighLimit,v.RampLowLimit); \
            v.RampDelayCount = 0;                                                   \
                                                                                    \
        }                                                                           \
}                                                                                   \
else v.EqualFlag = 0x7FFFFFFF;

#endif // __RMP_CNTL_H__
