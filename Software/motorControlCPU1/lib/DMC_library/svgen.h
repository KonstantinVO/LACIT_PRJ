/* =================================================================================
File name:       SVGEN_DQ.H  (IQ version)

Originator: Digital Control Systems Group
            Texas Instruments

Description:
Header file containing constants, data type, and macro  definitions for the SVGEN_DQ module .
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 10-15-2009 Version 1.0
------------------------------------------------------------------------------*/
#ifndef __SVGEN_DQ_H__
#define __SVGEN_DQ_H__

#include "IQmathLib.h"
#include "F2837xD_device.h"

typedef struct  { _iq  Ualpha;          // Input: reference alpha-axis phase voltage
                  _iq  Ubeta;           // Input: reference beta-axis phase voltage
                  _iq  Ta;              // Output: reference phase-a switching function
                  _iq  Tb;              // Output: reference phase-b switching function
                  _iq  Tc;              // Output: reference phase-c switching function
                  _iq  tmp1;            // Variable: temp variable
                  _iq  tmp2;            // Variable: temp variable
                  _iq  tmp3;            // Variable: temp variable
                  Uint16 VecSector;     // Space vector sector
                } SVGEN;


/*-----------------------------------------------------------------------------
Default initalizer for the SVGEN object.
-----------------------------------------------------------------------------*/                     
#define SVGEN_DEFAULTS { 0,0,0,0,0 }

/*------------------------------------------------------------------------------
    Space Vector  Generator (SVGEN) Macro Definition
------------------------------------------------------------------------------*/


#define SVGENDQ_MACRO(v)                                                        \
    v.tmp1= v.Ubeta;                                                            \
    v.tmp2= _IQdiv2(v.Ubeta) + (_IQmpy(_IQ(0.866),v.Ualpha));                   \
    v.tmp3= v.tmp2 - v.tmp1;                                                    \
                                                                                \
    v.VecSector=3;                                                              \
    v.VecSector=(v.tmp2> 0)?( v.VecSector-1):v.VecSector;                       \
    v.VecSector=(v.tmp3> 0)?( v.VecSector-1):v.VecSector;                       \
    v.VecSector=(v.tmp1< 0)?(7-v.VecSector) :v.VecSector;                       \
                                                                                \
    if     (v.VecSector==1 || v.VecSector==4)                                   \
      {     v.Ta= v.tmp2;                                                       \
            v.Tb= v.tmp1-v.tmp3;                                                \
            v.Tc=-v.tmp2;                                                       \
      }                                                                         \
                                                                                \
    else if(v.VecSector==2 || v.VecSector==5)                                   \
      {     v.Ta= v.tmp3+v.tmp2;                                                \
            v.Tb= v.tmp1;                                                       \
            v.Tc=-v.tmp1;                                                       \
      }                                                                         \
                                                                                \
    else                                                                        \
      {     v.Ta= v.tmp3;                                                       \
            v.Tb=-v.tmp3;                                                       \
            v.Tc=-(v.tmp1+v.tmp2);                                              \
      }                                                                         \

#endif // __SVGEN_DQ_H__
