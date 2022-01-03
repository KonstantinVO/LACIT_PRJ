//#############################################################################
// FILE    : fast_current_loop.h
// TITLE   : Header file to be shared between example and library for data.
// VERSION : 1.0
//
//  Group		  : C2000
//  Target Family :	F2837x
//	Created on    : Nov 9, 2017
//  Author        : Ramesh Ramamoorthy 
//#############################################################################
// $TI Release: C2000 FCL SFRA $
// $Release Date: 11/2017 $
// $Copyright: Copyright (C) 2013-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//#############################################################################

#ifndef LIB_HEADERS_FAST_CURRENT_LOOP_H_
#define LIB_HEADERS_FAST_CURRENT_LOOP_H_

#include "motorVars.h"

#ifdef FCL_LIB
extern
#endif
uint32_t         FF_COMP
#ifndef FCL_LIB
= 0
#endif
;

#ifdef FCL_LIB
extern
#endif
uint32_t         COMPLEX_PIC
#ifndef FCL_LIB
= 1
#endif
;


Uint32 FCL_GetSwVersion(void);
void FCL_initPWM(MOTOR_VARS * m);

// *****************************
// PI CONTROL ROUTINES
// *****************************
extern void FCL_PI_CtrlWrap ( MOTOR_VARS * m );
extern void FCL_PI_Ctrl(MOTOR_VARS * m);

// *****************************
// COMPLEX CONTROL ROUTINES
// *****************************
extern void FCL_CC_CtrlWrap (MOTOR_VARS * m);
extern void FCL_CC_Ctrl(MOTOR_VARS * m);

// *****************************
// QEP INTERFACE ROUTINES
// *****************************
extern void QepPosEstModule ( MOTOR_VARS * m );
extern void QepWrapModule ( MOTOR_VARS * m );


#endif /* LIB_HEADERS_FAST_CURRENT_LOOP_H_ */
