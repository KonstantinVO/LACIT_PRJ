//#############################################################################
// FILE    : FCL_PARS.H
// TITLE   : Header file having structure for FCL parameters
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


#ifndef FCL_PARS_H_
#define FCL_PARS_H_

// ==============================================================
typedef struct currentLoopPars {
    float32  CARRIER_MID,    // Mid point value of carrier count
             ADC_SCALE,      // ADC conversion scale to pu
             cmidsqrt3;      // internal variable

    float32 tSamp,           // sampling time
            Rd,              // Motor resistance in D axis
            Rq,              // Motor resistance in Q axis
            Ld,              // Motor inductance in D axis
            Lq,              // Motor inductance in Q axis
            Vbase,           // Base voltage for the controller
            Ibase,           // Base current for the controller
            FccD,            // D axis current controller bandwidth in Hz
            FccQ,            // Q axis current controller bandwidth in Hz
            wccD,            // D axis current controller bandwidth in rad/sec
            wccQ,            // Q axis current controller bandwidth in rad/sec
            Vdcbus,          // DC bus voltage
            BemfK,           // Motor Bemf constant
            Wbase;           // Controller base frequency (Motor) in rad/sec
} FastCurrentLoopPars_t;

#define  FCL_Pars_DEFAULTS  {            \
        0.0,     /*  CARRIER_MID    */   \
        0.0,     /*  ADC_SCALE,     */   \
        0.0,     /*  cmidsqrt3;     */   \
        0.0,     /*  tSamp,         */   \
        0.0,     /*  Rd,            */   \
        0.0,     /*  Rq,            */   \
        0.0,     /*  Ld,            */   \
        0.0,     /*  Lq,            */   \
        0.0,     /*  Vbase,         */   \
        0.0,     /*  Ibase,         */   \
        0.0,     /*  FccD,          */   \
        0.0,     /*  FccQ,          */   \
        0.0,     /*  wccD,          */   \
        0.0,     /*  wccQ,          */   \
        0.0,     /*  Vdcbus,        */   \
        0.0,     /*  BemfK,         */   \
        0.0,     /*  Wbase;         */   \
}
// ==============================================================


#endif /* FCL_PARS_H_ */
