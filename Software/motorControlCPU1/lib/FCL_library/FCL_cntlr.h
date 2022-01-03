//#############################################################################
// FILE    : FCL_cntlr.h
// TITLE   : Header file to be shared between example and library for CPU data.
// Version : 1.0
//
//  Group           : C2000
//  Target Family   : F2837x
//  Created on      : Nov 9, 2017
//  Author          : Ramesh Ramamoorthy
//#############################################################################
// $TI Release: C2000 FCL SFRA $
// $Release Date: 11/2017 $
// $Copyright: Copyright (C) 2013-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//#############################################################################


#ifndef INCLUDES_FCL_CNTLR_H_
#define INCLUDES_FCL_CNTLR_H_

typedef struct {  float32  Ref;             // Input: reference set-point
                  float32  Fbk;             // Input: feedback
                  float32  Err;             // Output : current error
                  float32  xErr;            // Output : X current error (other co-ordinate)
                  float32  Out;             // Output: controller output
                  float32  CarryOver;       // Output : carry over for next iteration
                  float32  Kp;              // Parameter: proportional loop gain
                  float32  Ki;              // Parameter: integral gain
                  float32  Kerr;            // Parameter: gain for latest error
                  float32  Kxerr;           // Parameter: gain for latest x error
                  float32  KerrOld;         // Parameter: gain for prev error

                  float32 cosWTs,
                          sinWTs,
                          expVal,
                          kDirect;

                  float32  Umax;            // Parameter: upper saturation limit
                  float32  Umin;            // Parameter: lower saturation limit
                } CURRENT_CONTROLLER;

/*-----------------------------------------------------------------------------
Default initialization values for the CURRENT_CONTROLLER objects
-----------------------------------------------------------------------------*/
#define CURRENT_CONTROLLER_DEFAULTS {      \
        0,       /*  Ref            */    \
        0,       /*  Fbk            */    \
        0,       /*  Err            */    \
        0,       /*  xErr           */    \
        0,       /*  Out            */    \
        0,       /*  CarryOver      */    \
        0,       /*  Kp             */    \
        0,       /*  Ki             */    \
        0,       /*  Kerr           */    \
        0,       /*  Kxerr          */    \
        0,       /*  KerrOld        */    \
        0,       /*  cosWTs         */    \
        0,       /*  sinWTs         */    \
        0,       /*  expVal         */    \
        0,       /*  kDirect        */    \
        1.0,     /*  Umax           */    \
       -1.0,     /*  Umin           */    \
}


#endif /* INCLUDES_FCL_CNTLR_H_ */
