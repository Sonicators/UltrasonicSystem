//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Transducer.h - Transducer driver interface
//
//  SYNOPSIS
//
//      The transducer driver is a finite state machine that runs in the background
//        independently of the user interface. For example, in RUN_TIMED mode when the
//        transducer is turned ON the output will automatically countdown and turn OFF
//        again without further attention from the interface.
//
//      The driver is based on two structs: TransducerSet has settings requested by the
//        user, and TransducerCurr contains measurements made by the driver.
//
//      Thus, TransducerSet.Power is the power requested by the user, and
//        TransducerCurr.Power is the power delivered to the transducer, as measured by
//        the driver.
//
//      //////////////////////////////////////
//      //
//      // In Transducer.h
//      //
//      Choose a PWMPot  PORT and Bit settings (see below)
//      Choose a FreqPot PORT and Bit settings (see below)
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      TransducerInit();                   // Called once at startup
//          :
//
//      TransducerEStop(bool);              // Set/clear EStop
//
//      TransducerFreq(28000);              // Set output frequency in Hz
//      TransducerPower(100);               // Set output power, in Watts*10
//      TransducerRunMode(RUN_TIMED,10);    // Run for 10 ticks when engaged
//      TransducerCtlMode(CTL_CONST_FREQ);  // Run at constant frequency
//      TransducerOn(bool);                 // Turn transducer on
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

#ifndef TRANSDUCER_H
#define TRANSDUCER_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PWMPot - 10K ohm MCP4161 on PortD.2
//
//#define PWMPot_PORT                             D     // See Config.h
//#define PWMPot_BIT                              2
#define PWMPot_MAXR                         10000
#define PWMPot_MAX_WIPER                      255

//
// Uncomment this to print single-chars that show the frequency tuning
//
//#define SHOW_FREQ_TUNING

//
// Uncomment this to print single-chars that show the power tuning
//
#define SHOW_PWR_TUNING

//
// Uncomment this to disable power tracking and allow direct wiper setting
//   commands
//
#define USE_WIPER_CMDS

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// Project parameters
//
#define TRANSDUCER_MIN_FREQ     20000       // Minimum frequency we allow
#define TRANSDUCER_DEF_FREQ     28000       // Default frequency at startup
#define TRANSDUCER_MAX_FREQ     35000       // Maximum frequency we allow

#define TRANSDUCER_MIN_POWER    0           // Minimum power we allow
#define TRANSDUCER_MAX_POWER    (100*10)    // Maximum power we allow (in watts x 10)

#define TRANSDUCER_DRIVE_VOLTS  12          // Volts in driver circuit

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// RunMode
//
typedef enum {
    RUN_CONTINUOUS = 100,           // On stays on
    RUN_TIMED,                      // Timed run
    } TRANSDUCER_RUN_MODE;

#define NUM_RUN_MODES   ( RUN_TIMED - RUN_CONTINUOUS + 1 )

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ControlMode
//
typedef enum {
    CTL_CONST_FREQ = 200,           // Constant freq and power
    CTL_MAX_EFF,                    // Constant power, max efficiency
    } TRANSDUCER_CTL_MODE;

#define NUM_CTL_MODES     ( CTL_MAX_EFF - CTL_CONST_FREQ + 1 )
#define IDX_CTL_MODE(_x_) (_x_ - CTL_CONST_FREQ)        // Index of 1st power mode

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Input information
//
//      Action
//      MaxPower    Maximum power this transducer can sustain (watts)
//
typedef enum {
    INPUT_UNUSED = 300,     // Input is unused
    INPUT_XCTRL,            // Xducer control: on while pushed
    INPUT_XPOPO,            // Xducer control: Push on/push off
    INPUT_ESTOP,            // ESTOP when triggered
    } INPUT_ACTION;

#define NUM_ACTIONS     ( INPUT_ESTOP - INPUT_UNUSED + 1 )
#define IDX_ACTION(_x_) (_x_ - INPUT_UNUSED)            // Index of 1st input action

typedef struct {
    INPUT_ACTION    Action; // What to do when activated
    bool            Print;  // TRUE if should print msg when activated
    } INPUT;


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerSet - Parameters set by the user, the driver will try to keep these settings
//
typedef struct {
    uint16_t            Freq;       // Requested target frequency (Hz)
    uint16_t            Power;      // Requested target power     (watts x 10)

    TRANSDUCER_RUN_MODE RunMode;    // Mode, when running
    uint16_t            RunTimer;   // Countdown timer, when in RUN_TIMED mode

    TRANSDUCER_CTL_MODE CtlMode;    // Control output mode

    INPUT               Input1;     // Input actions
    INPUT               Input2;
    } TRANSDUCER_SET;

extern TRANSDUCER_SET TransducerSet;

//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerCurr - Actual current parameters, measured by the driver
//
typedef struct {
    uint16_t    Freq;       // Current frequency
    uint16_t    Power;      // Transducer power, in watts x 10
    uint16_t    RunTimer;   // Countdown timer, when in RUN_TIMED mode
    uint16_t    Current;    // Current (amps)

    uint16_t    PWM;        // PWM, in     % x 10
    uint16_t    PWMWiper;   // Current PWM         wiper

    bool        EStop;      // TRUE if we are in EStop
    bool        On;         // TRUE if transducer is turned ON
    } TRANSDUCER_CURR;

extern TRANSDUCER_CURR TransducerCurr;

//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PWMPot setup
//
#include "MCP4161.h"

#define PWMPotInit              MCP4161Init(PWMPot_PORT,PWMPot_BIT)
#define PWMPotSetWiper(_w_)     MCP4161SetWiper(PWMPot_PORT,PWMPot_BIT,_w_)
#define PWMPotSetResist(_r_)    MCP4161SetResist(PWMPot_PORT,PWMPot_BIT,PWMPot_MAXR,_r_)
#define PWMPotIncr              MCP4161Incr(PWMPot_PORT,PWMPot_BIT)
#define PWMPotDecr              MCP4161Decr(PWMPot_PORT,PWMPot_BIT)

#define PWMPotR2W(_r_)          MCP4161_R2W(PWMPot_MAXR,_r_)
#define PWMPotW2R(_w_)          MCP4161_W2R(PWMPot_MAXR,_w_)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// FreqPot setup
//
#include "MCP4131.h"

#define FreqPotInit             MCP4161Init(FreqPot_PORT,FreqPot_BIT)
#define FreqPotSetWiper(_w_)    MCP4161SetWiper(FreqPot_PORT,FreqPot_BIT,_w_)
#define FreqPotSetResist(_r_)   MCP4161SetResist(FreqPot_PORT,FreqPot_BIT,FreqPot_MAXR,_r_)
#define FreqPotIncr             MCP4161Incr(FreqPot_PORT,FreqPot_BIT)
#define FreqPotDecr             MCP4161Decr(FreqPot_PORT,FreqPot_BIT)

#define FreqPotR2W(_r_)         MCP4161_R2W(FreqPot_MAXR,_r_)
#define FreqPotW2R(_w_)         MCP4161_W2R(FreqPot_MAXR,_w_)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerInit - Initialize the driver interface
//
// Inputs:      None.
//
// Outputs:     None.
//
void TransducerInit(void);
                                                                                        \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerEStop - Set EStop on/off
//
// Inputs:      TRUE  if     in EStop, output is stopped
//              FALSE if not in EStop, clear to operate transducer
//
// Outputs:     None.
//
void TransducerEStop(bool EStop);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerOn - Enable/Disable output
//
// Inputs:      TRUE  if output should be enabled
//              FALSE otherwise
//
// Outputs:     None.
//
// NOTE: If TRANSDUCERCurr.RunMode == MODE_TIMED, will set timer and turn off output
//         when timer expires
//
void TransducerOn(bool On);


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
void TransducerFreq(uint16_t Freq);


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
void TransducerPower(uint16_t Power);


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
void TransducerRunMode(TRANSDUCER_RUN_MODE RunMode,uint16_t Ticks);


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
void TransducerCtlMode(TRANSDUCER_CTL_MODE CtlMode);


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
void TransducerSetup(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerUpdate - Update the driver system
//
// Inputs:      None
//
// Outputs:     None.
//
// NOTE: Called by timer update, not for public consumption
//
void TransducerUpdate(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TransducerCmd - Manage typed commands affecting at the transducer driver
//
// Inputs:      Command to interpret
//
// Outputs:     TRUE  if we understood and processed command
//              FALSE if command isn't ours, belongs to another system
//
bool TransducerCmd(char *Command);


#endif  // DRIVER_H - entire file
