
#include "utils.h"
#include "configProject.h"
#include "BLDCmotorControl.h"
#include "f2837xhall_gpio.h"
#include "f2837xdrvlib.h"
#include "pwmConfig.h"
#include "adcConfig.h"
#include "configProject.h"


// Instance PI regulator to regulate the DC-bus current and speed
PI_CONTROLLER pid1_idc_bldc = PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pid1_spd_bldc = PI_CONTROLLER_DEFAULTS;


// Instance a Hall effect driver
HALL3 hall1_bldc = HALL3_DEFAULTS;

// Instance a ramp controller to smoothly ramp the frequency
RMPCNTL rc1_bldc = RMPCNTL_DEFAULTS;

// Instance a RAMP2 Module
RMP2 rmp2_bldc = RMP2_DEFAULTS;

// Instance a RAMP3 Module
RMP3 rmp3_bldc = RMP3_DEFAULTS;

// Instance a MOD6 Module
MOD6CNT mod1_bldc = MOD6CNT_DEFAULTS;

// Instance a IMPULSE Module
IMPULSE impl1_bldc = IMPULSE_DEFAULTS;

// Instance a SPEED_PR Module
SPEED_MEAS_CAP speed1_bldc = SPEED_MEAS_CAP_DEFAULTS;



_iq CurrentSet = _IQ(0.01);
_iq DCbus_current=0;
_iq SpeedRef_bldc=_IQ(0.3);
_iq tempIdc=0;

_iq BemfA=0;
_iq BemfB=0;
_iq BemfC=0;

/*static int16 DlogCh1 = 0;
static int16 DlogCh2 = 0;
static int16 DlogCh3 = 0;
static int16 DlogCh4 = 0;*/

Uint16 ch1=0;
Uint16 ch2=0;
Uint16 ch3=0;

#if (BUILDLEVEL<= BLDC_LEVEL2)
    Uint32 CmtnPeriodTarget = 0x00000150;
    Uint32 CmtnPeriodSetpt = 0x00000200;
    Uint32 RampDelay = 10;
#else
    Uint32 CmtnPeriodTarget = 0x00000250;
    Uint32 CmtnPeriodSetpt = 0x00000400;
    Uint32 RampDelay = 10;
#endif

uint32_t VirtualTimer = 0;

void initHALL(void)
{
    // Initialize Hall module
    hall1_bldc.DebounceAmount = 0;
    hall1_bldc.Revolutions    = -3;
    HALL3_INIT_MACRO(hall1_bldc)
}

void HWconfigBLDCmotorControl(void)
{
    initPWMmodule();
    initHALL();
    initADC();
}

void SWconfigBLDCmotorControl(void)
{

    // Initialize the SPEED_PR module
    speed1_bldc.InputSelect = 0;
    speed1_bldc.BaseRpm = 120*(BASE_FREQ/POLES);
    speed1_bldc.SpeedScaler = (Uint32)(ISR_FREQUENCY/(1*BASE_FREQ*0.001));

    // Initialize RMPCNTL module
    rc1_bldc.RampDelayMax = 25;
    rc1_bldc.RampLowLimit = _IQ(0);
    rc1_bldc.RampHighLimit = _IQ(1);

    // Initialize RMP2 module
    rmp2_bldc.Out = (int32)ALIGN_DUTY;
    rmp2_bldc.Ramp2Delay = 0x00000050;
    rmp2_bldc.Ramp2Max = 0x00007FFF;
    rmp2_bldc.Ramp2Min = 0x0000000F;

    // Initialize RMP3 module
    rmp3_bldc.DesiredInput = CmtnPeriodTarget;
    rmp3_bldc.Ramp3Delay = RampDelay;
    rmp3_bldc.Out = CmtnPeriodSetpt;
    rmp3_bldc.Ramp3Min = 0x00000010;

    // Initialize the PI module for dc-bus current
    pid1_idc_bldc.Kp = _IQ(1.0);
    pid1_idc_bldc.Ki   = _IQ(T/0.003);
    pid1_idc_bldc.Umax = _IQ(1.0);
    pid1_idc_bldc.Umin = _IQ(0);

    // Initialize the PI module for speed
    pid1_spd_bldc.Kp   = _IQ(0.25);
    pid1_spd_bldc.Ki   = _IQ(T/0.025);
    pid1_spd_bldc.Umax = _IQ(1.0);
    pid1_spd_bldc.Umin = _IQ(0);

    // Initialize DATALOG module
/*    dlog_bldc.iptr1 = &DlogCh1;
    dlog_bldc.iptr2 = &DlogCh2;
    dlog_bldc.iptr3 = &DlogCh3;
    dlog_bldc.iptr4 = &DlogCh4;
    dlog_bldc.trig_value = 0x1;
    dlog_bldc.size = 0x0C8;
    dlog_bldc.prescalar = 25;
    dlog_bldc.init(&dlog_bldc);*/
}

void BLDCmotorControl(void)
{
    #if (BUILDLEVEL==BLDC_LEVEL1)

    // ------------------------------------------------------------------------------
    //    Connect inputs of the RMP3 module and call the RAMP Control 3 macro.
    // ------------------------------------------------------------------------------
         rmp3_bldc.DesiredInput = CmtnPeriodTarget;
         rmp3_bldc.Ramp3Delay = RampDelay;
         RC3_MACRO(rmp3_bldc)

    // ------------------------------------------------------------------------------
    //    Connect inputs of the IMPULSE module and call the Impulse macro.
    // ------------------------------------------------------------------------------
          impl1_bldc.Period = rmp3_bldc.Out;
          IMPULSE_MACRO(impl1_bldc)

    // ------------------------------------------------------------------------------
    //    Connect inputs of the MOD6 module and call the Mod 6 counter macro.
    // ------------------------------------------------------------------------------
          mod1_bldc.TrigInput = impl1_bldc.Out;
          MOD6CNT_MACRO(mod1_bldc)

    // ------------------------------------------------------------------------------
    //    Connect inputs of the PWM_DRV module and call the PWM signal generation
    //    update macro.
    // ------------------------------------------------------------------------------
          pwmMotor();

    #endif // (BUILDLEVEL==LEVEL1)

    #if (BUILDLEVEL==BLDC_LEVEL2)

    // ------------------------------------------------------------------------------
    //    ADC conversion and offset adjustment (observing back-emfs is optinal for this prj.)
    // ------------------------------------------------------------------------------
          BemfA =  _IQ12toIQ(RESAULT_ADC_PHASE_A);
          BemfB =  _IQ12toIQ(RESAULT_ADC_PHASE_B);
          BemfC =  _IQ12toIQ(RESAULT_ADC_PHASE_C);
          DCbus_current = _IQ12toIQ(RESAULT_ADC_DC_BUS_CURR)-_IQ(0.5); //1.65V offset added on HVDMC board.

    // ------------------------------------------------------------------------------
    //    Connect inputs of the RMP3 module and call the Ramp control 3 macro.
    // ------------------------------------------------------------------------------
          rmp3_bldc.DesiredInput = CmtnPeriodTarget;
          rmp3_bldc.Ramp3Delay = RampDelay;
          RC3_MACRO(rmp3_bldc)

    // ------------------------------------------------------------------------------
    //    Connect inputs of the IMPULSE module and call the Impulse macro.
    // ------------------------------------------------------------------------------
          impl1_bldc.Period = rmp3_bldc.Out;
          IMPULSE_MACRO(impl1_bldc)

    // ------------------------------------------------------------------------------
    //    Connect inputs of the MOD6 module and call the Modulo 6 counter macro.
    // ------------------------------------------------------------------------------
          mod1_bldc.TrigInput = impl1_bldc.Out;
          MOD6CNT_MACRO(mod1_bldc)

          pwmMotor();

    // ------------------------------------------------------------------------------
    //    Connect inputs of the DATALOG module
    // ------------------------------------------------------------------------------

/*          DlogCh1 = (int16)mod1_bldc.Counter;
          DlogCh2 = (int16)_IQtoIQ15(BemfA);
          DlogCh3 = (int16)_IQtoIQ15(BemfB);
          DlogCh4 = (int16)_IQtoIQ15(BemfC);
          dlog_bldc.update(&dlog_bldc);*/


    #endif // (BUILDLEVEL==LEVEL2)

#if (BUILDLEVEL==BLDC_LEVEL3)

// ------------------------------------------------------------------------------
//    ADC conversion and offset adjustment (observing back-emfs is optinal for this prj.)
// ------------------------------------------------------------------------------
      BemfA =  _IQ12toIQ(RESAULT_ADC_PHASE_A);
      BemfB =  _IQ12toIQ(RESAULT_ADC_PHASE_B);
      BemfC =  _IQ12toIQ(RESAULT_ADC_PHASE_C);
      DCbus_current = _IQ12toIQ(RESAULT_ADC_DC_BUS_CURR)-_IQ(0.5); //1.65V offset added on HVDMC board.

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP3 module and call the Ramp control 3 macro.
// ------------------------------------------------------------------------------
      rmp3_bldc.DesiredInput = CmtnPeriodTarget;
      rmp3_bldc.Ramp3Delay = RampDelay;
      RC3_MACRO(rmp3_bldc)

// ------------------------------------------------------------------------------
//    Connect inputs of the IMPULSE module and call the Impulse macro.
// ------------------------------------------------------------------------------
      impl1_bldc.Period = rmp3_bldc.Out;
      IMPULSE_MACRO(impl1_bldc)

// ------------------------------------------------------------------------------
//    Connect inputs of the HALL module and call the Hall sensor read macro.
// ------------------------------------------------------------------------------
      hall1_bldc.HallMapPointer = (int16)mod1_bldc.Counter;
      HALL3_READ_MACRO(hall1_bldc)
      if (hall1_bldc.Revolutions>=0)
        ClosedFlag=TRUE;

// ------------------------------------------------------------------------------
//    Connect inputs of the MOD6 module and call the Modulo 6 counter macro.
// ------------------------------------------------------------------------------
   if (ClosedFlag==FALSE)
   {
      mod1_bldc.TrigInput = impl1_bldc.Out;
      mod1_bldc.Counter = (int32)hall1_bldc.HallMapPointer;
   }
   else
      mod1_bldc.TrigInput = (int32)hall1_bldc.CmtnTrigHall;

     MOD6CNT_MACRO(mod1_bldc)

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP2 module and call the Ramp control 2 macro.
// ------------------------------------------------------------------------------
      rmp2_bldc.DesiredInput = (int32)DFuncDesired;
      RC2_MACRO(rmp2_bldc)

// ------------------------------------------------------------------------------
//    Connect inputs of the PWM_DRV module and call the PWM signal generation
//    update macro.
// ------------------------------------------------------------------------------
   if (ClosedFlag==TRUE)  {
     if (hall1_bldc.CmtnTrigHall==0x7FFF) {

      PreviousState = pwm1_bldc.CmtnPointer;



// Comment the following if-else-if statements in case of
// inverted Hall logics for commutation states.
      if (hall1_bldc.HallGpioAccepted==5)
        pwm1_bldc.CmtnPointer = 0;

      else if (hall1_bldc.HallGpioAccepted==1)
        pwm1_bldc.CmtnPointer = 1;

      else if (hall1_bldc.HallGpioAccepted==3)
        pwm1_bldc.CmtnPointer = 2;

      else if (hall1_bldc.HallGpioAccepted==2)
        pwm1_bldc.CmtnPointer = 3;

      else if (hall1_bldc.HallGpioAccepted==6)
        pwm1_bldc.CmtnPointer = 4;

      else if (hall1_bldc.HallGpioAccepted==4)
        pwm1_bldc.CmtnPointer = 5;


/*

// Comment the following if-else-if statements in case of
// non-inverted Hall logics for commutation states.
      if (hall1.HallGpioAccepted==2)
        pwm1.CmtnPointer = 0;

      else if (hall1.HallGpioAccepted==6)
        pwm1.CmtnPointer = 1;

      else if (hall1.HallGpioAccepted==4)
        pwm1.CmtnPointer = 2;

      else if (hall1.HallGpioAccepted==5)
        pwm1.CmtnPointer = 3;

      else if (hall1.HallGpioAccepted==1)
        pwm1.CmtnPointer = 4;

      else if (hall1.HallGpioAccepted==3)
        pwm1.CmtnPointer = 5;

*/
    }    //hall1.CmtnTrigHall == 0x7FFF
  } // ClosedFlag==TRUE
  else

      pwm1_bldc.CmtnPointer = (int16)mod1_bldc.Counter;
      pwm1_bldc.DutyFunc = DfuncTesting;
      BLDCPWM_MACRO(1,2,3,pwm1_bldc);

// ------------------------------------------------------------------------------
//    Connect inputs of the SPEED_PR module and call the speed calculation macro.
// ------------------------------------------------------------------------------
  if ((pwm1_bldc.CmtnPointer==5)&&(PreviousState==4)&&(hall1_bldc.CmtnTrigHall==0x7FFF))
  {
       speed1_bldc.TimeStamp = VirtualTimer;
       SPEED_PR_MACRO(speed1_bldc);
  }

// ------------------------------------------------------------------------------
//    Connect inputs of the PWMDAC module
// ------------------------------------------------------------------------------
/*      pwmdac1.MfuncC1 = (int32)hall1.HallGpioAccepted<<20;
      pwmdac1.MfuncC2 = BemfA;
      PWMDAC_MACRO(6,pwmdac1)                           // PWMDAC 6A, 6B

      pwmdac1.MfuncC1 = DCbus_current;
      pwmdac1.MfuncC2 = (int32)mod1.Counter<<20;
      PWMDAC_MACRO(7,pwmdac1)                           // PWMDAC 7A, 7B

// ------------------------------------------------------------------------------
//    Connect inputs of the DATALOG module
// ------------------------------------------------------------------------------
      DlogCh2 = (int16)mod1.Counter;
      DlogCh1 = (int16)hall1.HallGpioAccepted;
      DlogCh3 = (int16)_IQtoIQ15(BemfA);
      DlogCh4 = (int16)_IQtoIQ15(BemfB);*/


#endif // (BUILDLEVEL==LEVEL3)

#if (BUILDLEVEL==BLDC_LEVEL4)

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP3 module and call the Ramp control 3 macro.
// ------------------------------------------------------------------------------
      rmp3.DesiredInput = CmtnPeriodTarget;
      rmp3.Ramp3Delay = RampDelay;
      RC3_MACRO(rmp3)

// ------------------------------------------------------------------------------
//    Connect inputs of the IMPULSE module and call the Impulse macro.
// ------------------------------------------------------------------------------
      impl1.Period = rmp3.Out;
      IMPULSE_MACRO(impl1)

// ------------------------------------------------------------------------------
//    Connect inputs of the HALL module and call the Hall sensor read macro.
// ------------------------------------------------------------------------------
      hall1.HallMapPointer = (int16)mod1.Counter;
      HALL3_READ_MACRO(hall1)

      if (hall1.Revolutions>=0)
         ClosedFlag=TRUE;

// ------------------------------------------------------------------------------
//    Connect inputs of the MOD6 module and call the Modulo 6 counter macro.
// ------------------------------------------------------------------------------
       if (ClosedFlag==FALSE) {
          mod1.TrigInput = impl1.Out;
          mod1.Counter   = (int32)hall1.HallMapPointer;
       }
       else
          mod1.TrigInput = (int32)hall1.CmtnTrigHall;

       MOD6CNT_MACRO(mod1)

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP2 module and call the Ramp control 2 macro.
// ------------------------------------------------------------------------------
      rmp2.DesiredInput = (int32)DFuncDesired;
      RC2_MACRO(rmp2)

// ------------------------------------------------------------------------------
//    Connect inputs of the PI module and call the PID controller macro.
// ------------------------------------------------------------------------------
      tempIdc=pid1_idc.Fbk;
      pid1_idc.Ref = CurrentSet;
      pid1_idc.Fbk = _IQ12toIQ(AdcResult.ADCRESULT4)-_IQ(0.5);

      if(pid1_idc.Fbk<0) pid1_idc.Fbk=tempIdc; // Eliminate negative values
      PI_MACRO(pid1_idc)

// ------------------------------------------------------------------------------
//    Connect inputs of the PWM_DRV module and call the PWM signal generation
//    update macro.
// ------------------------------------------------------------------------------
// Switch from fixed duty-cycle or controlled Idc duty-cycle by ILoopFlag variable

      if (ILoopFlag == FALSE)
        { pwm1.DutyFunc = (int16)rmp2.Out;                 // fixed duty-cycle
          pid1_idc.ui=0;
          pid1_idc.i1=0;
        }
      else
         pwm1.DutyFunc = (int16)_IQtoIQ15(pid1_idc.Out);   // controlled Idc duty-cycle

    if (ClosedFlag==TRUE)
    {
     if (hall1.CmtnTrigHall==0x7FFF)
     {

      PreviousState = pwm1.CmtnPointer;


// Comment the following if-else-if statements in case of
// inverted Hall logics for commutation states. r
      if (hall1.HallGpioAccepted==5)
        pwm1.CmtnPointer = 0;

      else if (hall1.HallGpioAccepted==1)
        pwm1.CmtnPointer = 1;

      else if (hall1.HallGpioAccepted==3)
        pwm1.CmtnPointer = 2;

      else if (hall1.HallGpioAccepted==2)
        pwm1.CmtnPointer = 3;

      else if (hall1.HallGpioAccepted==6)
        pwm1.CmtnPointer = 4;

      else if (hall1.HallGpioAccepted==4)
        pwm1.CmtnPointer = 5;
/*

// Comment the following if-else-if statements in case of
// non-inverted Hall logics for commutation states.

      if (hall1.HallGpioAccepted==2)
        pwm1.CmtnPointer = 0;

      else if (hall1.HallGpioAccepted==6)
        pwm1.CmtnPointer = 1;

      else if (hall1.HallGpioAccepted==4)
        pwm1.CmtnPointer = 2;

      else if (hall1.HallGpioAccepted==5)
        pwm1.CmtnPointer = 3;

      else if (hall1.HallGpioAccepted==1)
        pwm1.CmtnPointer = 4;

      else if (hall1.HallGpioAccepted==3)
        pwm1.CmtnPointer = 5;
*/
    }    // Hall1.CmtnTrigHall == 0x7FFF
  }      // ClosedFlag==TRUE

  else

      pwm1.CmtnPointer = (int16)mod1.Counter;
      BLDCPWM_MACRO(1,2,3,pwm1)

// ------------------------------------------------------------------------------
//    Connect inputs of the SPEED_PR module and call the speed calculation macro.
// ------------------------------------------------------------------------------
      if ((pwm1.CmtnPointer==5)&&(PreviousState==4)&&(hall1.CmtnTrigHall==0x7FFF))
      {
         speed1.TimeStamp = VirtualTimer;
         SPEED_PR_MACRO(speed1)
      }

// ------------------------------------------------------------------------------
//    Connect inputs of the PWMDAC module
// ------------------------------------------------------------------------------
      pwmdac1.MfuncC1 = (int32)hall1.HallGpioAccepted<<20;
      pwmdac1.MfuncC2 = speed1.Speed;
      PWMDAC_MACRO(6,pwmdac1)                           // PWMDAC 6A, 6B

      pwmdac1.MfuncC1 = pid1_idc.Fbk;
      pwmdac1.MfuncC2 = (int32)mod1.Counter<<20;
      PWMDAC_MACRO(7,pwmdac1)                           // PWMDAC 7A, 7B

// ------------------------------------------------------------------------------
//    Connect inputs of the DATALOG module
// ------------------------------------------------------------------------------
      DlogCh1 = pwm1.CmtnPointer;
      DlogCh2 = hall1.HallGpioAccepted;
      DlogCh3 = (int16)_IQtoIQ15(speed1.Speed);
      DlogCh4 = (int16)_IQtoIQ15(pid1_idc.Fbk);

#endif // (BUILDLEVEL==LEVEL4)

#if (BUILDLEVEL==BLDC_LEVEL5)

// ------------------------------------------------------------------------------
//    ADC conversion and offset adjustment (observing back-emfs is optinal for this prj.)
// ------------------------------------------------------------------------------
      BemfA =  _IQ12toIQ(AdcResult.ADCRESULT1);
      BemfB =  _IQ12toIQ(AdcResult.ADCRESULT2);
      BemfC =  _IQ12toIQ(AdcResult.ADCRESULT3);
      DCbus_current = _IQ12toIQ(AdcResult.ADCRESULT4)-_IQ(0.5); //1.65V offset added on HVDMC board.

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP module and call the Ramp control macro.
// ------------------------------------------------------------------------------
      rc1.TargetValue = SpeedRef;
      RC_MACRO(rc1)

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP3 module and call the Ramp control 3 macro.
// ------------------------------------------------------------------------------
      rmp3.DesiredInput = CmtnPeriodTarget;
      rmp3.Ramp3Delay   = RampDelay;
      RC3_MACRO(rmp3)

// ------------------------------------------------------------------------------
//    Connect inputs of the IMPULSE module and call the Impulse macro.
// ------------------------------------------------------------------------------
      impl1.Period = rmp3.Out;
      IMPULSE_MACRO(impl1)

// ------------------------------------------------------------------------------
//    Connect inputs of the HALL module and call the Hall sensor read macro.
// ------------------------------------------------------------------------------
      hall1.HallMapPointer = (int16)mod1.Counter;
      HALL3_READ_MACRO(hall1)

      if (hall1.Revolutions>=0)
         ClosedFlag=TRUE;

// ------------------------------------------------------------------------------
//    Connect inputs of the MOD6 module and call the Modulo 6 counter macro.
// ------------------------------------------------------------------------------
   if (ClosedFlag==FALSE)
   {
      mod1.TrigInput = impl1.Out;
      mod1.Counter = (int32)hall1.HallMapPointer;
   }
   else
      mod1.TrigInput = (int32)hall1.CmtnTrigHall;

      MOD6CNT_MACRO(mod1)

// ------------------------------------------------------------------------------
//    Connect inputs of the RMP2 module and call the Ramp control 2 macro.
// ------------------------------------------------------------------------------
      rmp2.DesiredInput = (int32)DFuncDesired;
      RC2_MACRO(rmp2)

// ------------------------------------------------------------------------------
//    Connect inputs of the PI module and call the PID speed controller
//    macro.
// ------------------------------------------------------------------------------
      pid1_spd.Ref = rc1.SetpointValue;
      pid1_spd.Fbk = speed1.Speed;
      PI_MACRO(pid1_spd)

// ------------------------------------------------------------------------------
//    Set the speed closed loop flag once the speed is built up to a desired value.
// ------------------------------------------------------------------------------
      if ( rc1.EqualFlag == 0x7FFFFFFF )
         { SpeedLoopFlag = TRUE;}

// ------------------------------------------------------------------------------
//    Connect inputs of the PWM_DRV module and call the PWM signal generation
//    update macro.
// ------------------------------------------------------------------------------
// Switch from fixed duty-cycle or controlled Speed duty-cycle by SpeedLoopFlag variable
      if (SpeedLoopFlag == FALSE)
         {
          pwm1.DutyFunc = (int16)rmp2.Out;                 // fixed duty-cycle
          pid1_spd.ui=0;
          pid1_spd.i1=0;
         }
      else
         pwm1.DutyFunc = (int16)_IQtoIQ15(pid1_spd.Out);   // controlled speed duty-cycle

   if (ClosedFlag==TRUE)
   {
     if (hall1.CmtnTrigHall==0x7FFF)
     {

      PreviousState = pwm1.CmtnPointer;


// Comment the following if-else-if statements in case of
// inverted Hall logics for commutation states.
      if (hall1.HallGpioAccepted==5)
        pwm1.CmtnPointer = 0;

      else if (hall1.HallGpioAccepted==1)
        pwm1.CmtnPointer = 1;

      else if (hall1.HallGpioAccepted==3)
        pwm1.CmtnPointer = 2;

      else if (hall1.HallGpioAccepted==2)
        pwm1.CmtnPointer = 3;

      else if (hall1.HallGpioAccepted==6)
        pwm1.CmtnPointer = 4;

      else if (hall1.HallGpioAccepted==4)
        pwm1.CmtnPointer = 5;

/*

// Comment the following if-else-if statements in case of
// non-inverted Hall logics for commutation states.
      if (hall1.HallGpioAccepted==2)
        pwm1.CmtnPointer = 0;

      else if (hall1.HallGpioAccepted==6)
        pwm1.CmtnPointer = 1;

      else if (hall1.HallGpioAccepted==4)
        pwm1.CmtnPointer = 2;

      else if (hall1.HallGpioAccepted==5)
        pwm1.CmtnPointer = 3;

      else if (hall1.HallGpioAccepted==1)
        pwm1.CmtnPointer = 4;

      else if (hall1.HallGpioAccepted==3)
        pwm1.CmtnPointer = 5;
*/

    }    // Hall1.CmtnTrigHall == 0x7FFF
  }      // ClosedFlag==TRUE

  else

      pwm1.CmtnPointer = (int16)mod1.Counter;
      BLDCPWM_MACRO(1,2,3,pwm1)

// ------------------------------------------------------------------------------
//    Connect inputs of the SPEED_PR module and call the speed calculation macro.
// ------------------------------------------------------------------------------
      if ((pwm1.CmtnPointer==5)&&(PreviousState==4)&&(hall1.CmtnTrigHall==0x7FFF))
      {
         speed1.TimeStamp = VirtualTimer;
         SPEED_PR_MACRO(speed1)
      }

// ------------------------------------------------------------------------------
//    Connect inputs of the PWMDAC module
// ------------------------------------------------------------------------------
      pwmdac1.MfuncC1 = BemfA;
      pwmdac1.MfuncC2 = BemfB;
      PWMDAC_MACRO(6,pwmdac1)                           // PWMDAC 6A, 6B

      pwmdac1.MfuncC1 = DCbus_current;
      pwmdac1.MfuncC2 = mod1.Counter<<20;
      PWMDAC_MACRO(7,pwmdac1)                           // PWMDAC 7A, 7B

// ------------------------------------------------------------------------------
//    Connect inputs of the DATALOG module
// ------------------------------------------------------------------------------

      DlogCh1 = pwm1.CmtnPointer;
      DlogCh2 = (int16)_IQtoIQ15(BemfA);
      DlogCh3 = (int16)_IQtoIQ15(BemfB);
      DlogCh4 = (int16)_IQtoIQ15(BemfC);


#endif // (BUILDLEVEL==LEVEL5)

// ------------------------------------------------------------------------------
//    Increase virtual timer and force 15 bit wrap around
// ------------------------------------------------------------------------------
      VirtualTimer++;
      VirtualTimer &= 0x00007FFF;

}
