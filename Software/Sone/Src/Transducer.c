//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Transducer.c - Transducer driver interface
//
//  SYNOPSIS
//
//      See Transducer.h for details
//
//  DESCRIPTION
//
//      A driver module for transducer power output
//
//////////////////////////////////////////////////////////////////////////////////////////
//
//  MIT LICENSE
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//    this software and associated documentation files (the "Software"), to deal in
//    the Software without restriction, including without limitation the rights to
//    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//    of the Software, and to permit persons to whom the Software is furnished to do
//    so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//    all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "Transducer.h"
#include "AD9833.h"
#include "ACS712.h"
#include "SPIInline.h"
#include "SG3525.h"
#include "PWM.h"
#include "Inputs.h"
#include "Outputs.h"

#ifdef SHOW_PWR_TUNING
#include "Serial.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

TRANSDUCER_SET  TransducerSet  NOINIT;
TRANSDUCER_CURR TransducerCurr NOINIT;

PROGMEM TRANSDUCER_SET TransducerDefaults = {
    TRANSDUCER_DEF_FREQ, 20,    // Default output freq, power
    RUN_CONTINUOUS, 0,          // Default run mode and run timer
    CTL_CONST_FREQ,             // Constant frequency
    { INPUT_UNUSED, 0 },        // Default action for Input1
    { INPUT_UNUSED, 0 },        // Default action for Input2
    };

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerInit - Initialize the driver interface
//
// Inputs:      None.
//
// Outputs:     None.
//
void TransducerInit(void) {

    //
    // Initialize sub components to this module
    //
    SPIInit;
    SG3525_INIT;
    AD9833Init();
    ACS712Init();
    PWMPotInit;
    PWMInit();
    InputsInit();
    OutputsInit();

    //
    // See initialization struct above
    //
    // TransducerSet.Freq      = SG3525_DEF_FREQ;
    // TransducerSet.Power     = SG3525_MIN_POWER;
    // TransducerSet.RunMode   = RUN_CONTINUOUS;
    // TransducerSet.RunTimer  = 0;
    // TransducerSet.CtlMode   = PWR_CONST_FREQ;
    //
    memcpy_P(&TransducerSet,&TransducerDefaults,sizeof(TransducerSet));

    //
    // TransducerCurr.Freq     = 0;
    // TransducerCurr.Power    = 0;
    // TransducerCurr.RunTimer = 0;
    // TransducerCurr.Current  = 0;
    // TransducerCurr.PWM      = 0;
    // TransducerCurr.PWMWiper = 0;
    // TransducerCurr.EStop    = false;
    // TransducerCurr.On       = false;
    //
    memset(&TransducerCurr,0,sizeof(TransducerCurr));

    TransducerSetup();
    TransducerOn(false);
    TransducerEStop(true);

    //
    // The default power wiper aetting was determined experimentally, and
    //   represents a reasonably small initial PWM (24%) as a starting point.
    //
    TransducerCurr.PWMWiper = 30;
    PWMPotSetWiper(TransducerCurr.PWMWiper);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerEStop - Set/clear EStop
//
// Inputs:      TRUE  if     in EStop, output is stopped
//              FALSE if not in EStop, clear to operate transducer
//
// Outputs:     None.
//
void TransducerEStop(bool EStop) {

    TransducerCurr.EStop = EStop;

    //
    // If EStop, turn off transducer
    //
    if( TransducerCurr.EStop )
        TransducerOn(false);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerOn - Enable/Disable transducer output
//
// Inputs:      TRUE  if output is ON
//              FALSE otherwise
//
// Outputs:     None.
//
void TransducerOn(bool On) {

    if( !On ) {
        TransducerCurr.RunTimer = 0;
        TransducerCurr.On       = false;
        SG3525_OFF;
        return;
        }

    //
    // Don't allow turn ON in EStop
    //
    if( TransducerCurr.EStop )
        return;

    //
    // Set the run timer if needed
    //
    if( TransducerSet.RunMode == RUN_TIMED )
        TransducerCurr.RunTimer = TransducerSet.RunTimer;

    TransducerCurr.On = true;
    SG3525_ON;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerRunMode - Set transducer run mode
//
// Inputs:      Run mode (RUN_CONTINUOUS/RUN_TIMED
//              Ticks to run (in 1/25ths of a second)
//
// Outputs:     None.
//
//
void TransducerRunMode(TRANSDUCER_RUN_MODE RunMode,uint16_t Ticks) {

    TransducerSet.RunMode  = RunMode;
    TransducerSet.RunTimer = Ticks;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerCtlMode - Set transducer ctl mode
//
// Inputs:      Ctl mode (CTL_CONST_FREQ,CTL_MAX_EFF)
//
// Outputs:     None.
//
//
void TransducerCtlMode(TRANSDUCER_CTL_MODE CtlMode) {

    TransducerSet.CtlMode  = CtlMode;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerFreq - Set transducer output frequency
//
// Inputs:      Frequency to set (Hz)
//
// Outputs:     None.
//
//
void TransducerFreq(uint16_t Freq) {

    TransducerSet.Freq = Freq;

    AD9833Output(AD9833_SQ,TransducerSet.Freq);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerPower - Set transducer output power
//
// Inputs:      Power to set, in watts*10 (ie: 315 means 31.5 watts)
//
// Outputs:     None.
//
//
void TransducerPower(uint16_t Power) {

    TransducerSet.Power = Power;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AdjustPower - Station keeping for power setpoint
//
// Inputs:      None. Called periodically by the update program
//
// Outputs:     None.
//
static void AdjustPower(void) {

    //
    // If we're not actually on, then adjusting power will have no effect.
    //
    if( !TransducerCurr.On )
        return;

    //
    // Otherwise, adjust the power wiper until the desired power output
    //
    if( TransducerCurr.Power > TransducerSet.Power ) {
        //
        // If the PWM is already minimum, then adjusting will have
        //   no effect.
        //
        if( TransducerCurr.PWM == 0 )
            return;

        if( TransducerCurr.PWMWiper == 0 )
            return;

        PWMPotSetWiper(--TransducerCurr.PWMWiper);
#       ifdef SHOW_PWR_TUNING
        PrintChar('<');
#       endif
        return;
        }

    if( TransducerCurr.Power < TransducerSet.Power ) {
        //
        // If the PWM is already maximum, then adjusting will have
        //   no effect.
        //
        // Note that by design, the SG3525 controller only goes to 98% PWM
        //   due to dead time.
        //
        if( TransducerCurr.PWM > 96*10 )
            return;

        if( TransducerCurr.PWMWiper >= PWMPot_MAX_WIPER )
            return;

        PWMPotSetWiper(++TransducerCurr.PWMWiper);
#       ifdef SHOW_PWR_TUNING
        PrintChar('>');
#       endif

        return;
        }
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerSetup - Load settings from TransducerSet variable
//
// Inputs:      None.
//
// Outputs:     None.
//
//
void TransducerSetup(void) {

    TransducerFreq   (TransducerSet.Freq);
    TransducerPower  (TransducerSet.Power);
    TransducerRunMode(TransducerSet.RunMode,TransducerSet.RunTimer);
    TransducerCtlMode(TransducerSet.CtlMode);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerUpdate - Update the transducer driver
//
// Inputs:      None
//
// Outputs:     None.
//
// NOTE: Called by timer update, not for public consumption
//
void TransducerUpdate(void) {

    //
    // Update all subordinate components
    //
    PWMUpdate();
    ACS712Update();
    InputsUpdate();

    //
    // If we're running, use the PWM version of frequency since it's the most accurate.
    //   Otherwise the PWM is offline so use the less-accurate count to keep us in the
    //   ballpark.
    //
    // Note: SG3525 counted output is twice the actual frequency
    //
    TransducerCurr.Freq    = GetPWMFreq();
    TransducerCurr.PWM     = GetPWM()*2;
    TransducerCurr.Current = ACS712GetCurrent();

    //
    // PWM is % x 10, Current is in Amps x 10, and drive is 12 volts
    //
    // Multiply everything together and divide by 10 to get power in Watts x 10
    //
    uint32_t    PwrTemp;

    PwrTemp  = TransducerCurr.Current*TRANSDUCER_DRIVE_VOLTS;
    PwrTemp *= TransducerCurr.PWM;
    PwrTemp /= 1000;
    TransducerCurr.Power = (uint16_t) PwrTemp;

    //
    // If we're running on timer, decrement and possibly stop
    //
    if( TransducerCurr.On && TransducerSet.RunMode == RUN_TIMED ) {
        if( --TransducerCurr.RunTimer == 0 )
            TransducerOn(false);
        }

    switch(TransducerSet.CtlMode) {

        //////////////////////////////////////////////////////////////////////////////////
        //
        // CTL_CONST_FREQ - Set constant frequency and power
        //
        case CTL_CONST_FREQ:
//            AdjustPower();
            break;


        //////////////////////////////////////////////////////////////////////////////////
        //
        // CTL_MAX_EFF - Keep maximum efficiency
        //
        case CTL_MAX_EFF:
            break;


        default:        
            break;
        }

    }
