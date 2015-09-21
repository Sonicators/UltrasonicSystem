//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MAScreen.c
//
//  DESCRIPTION
//
//      Main screen for the Sone ultrasonic transducer control system
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

#include "Screen.h"

#ifdef USE_MAIN_SCREEN

#include "Transducer.h"
#include "Setup.h"

#include <stdlib.h>
#include <string.h>

#include "Command.h"
#include "Parse.h"
#include "Serial.h"
#include "VT100.h"

#include "Debug.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static bool PromptNeeded;           // TRUE if need to replot the prompt

static TRANSDUCER_SET   PrevSet;    // Previous shown values
static TRANSDUCER_CURR  PrevCurr;

//
// Static layout of the main screen
//
static char MAScreenText[] PROGMEM = "\
EStop :  ---- |  Out:   ---\r\n\
Freq  : ----- | Freq: -----\r\n\
Pwr   :  ---- |  Pwr:  ----\r\n\
Ctrl  :    -- |\r\n\
Run   : ----- |\r\n\
RunTim:   --- |\r\n\
==============+============\r\n\
Amps  :  ---- \r\n\
PWM   :  ----\r\n\
PWip  :   ---\r\n\
\r\n\
";

#define POS_ESTOP   CursorPos(10,1)
#define POS_OUT     CursorPos(25,1)

#define POS_FREQS   CursorPos(9,2)
#define POS_FREQM   CursorPos(23,2)

#define POS_PWRS    CursorPos(9,3)
#define POS_PWRM    CursorPos(23,3)

#define POS_CMODE   CursorPos(12,4)
#define POS_RMODE   CursorPos(9,5)
#define POS_RTIME   CursorPos(9,6)

#define POS_AMPS    CursorPos(9,8)
#define POS_PWM     CursorPos(9,9)
#define POS_PWW     CursorPos(11,10)

#define POS_MSG     CursorPos(1,14)

#define DEBUG_ROW   14

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Unset - Make one struct not equal to another
//
// Inputs:      Src struct to be unset
//              Cmp struct compared against
//              Number of bytes in Src struct
//
// Outputs:     None.
//
static void Unset(void *Src,void *Cmp,uint8_t nBytes) {

    while( nBytes-- )
        * (uint8_t *) Src++ = * (uint8_t *) Cmp++ ^ 0xFF;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ShowMAScreen - Display the main screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void ShowMAScreen(void) {

    CursorHome;
    ClearScreen;
    PrintStringP(MAScreenText);

    PromptNeeded = true;

    //
    // This uncopy/set will invalidate all the saved values and force a redraw
    //
    Unset(&PrevSet ,&TransducerSet ,sizeof(PrevSet));
    Unset(&PrevCurr,&TransducerCurr,sizeof(PrevCurr));

    UpdateMAScreen();
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintX10 - Print out x10 decimal value with decimal point
//
// Inputs:      Value to print
//
// Outputs:     None.
//
void PrintX10(uint16_t Value) {

    PrintD(Value/10,3);
    PrintChar('.');
    PrintChar('0' + (Value%10));
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// UpdateMAScreen - Display the main screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void UpdateMAScreen(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Screen-specific display fields
    //
    if( PrevCurr.EStop != TransducerCurr.EStop ) {
        PrevCurr.EStop  = TransducerCurr.EStop;
        POS_ESTOP;
        if( PrevCurr.EStop ) PrintStringP(PSTR("Stop"));
        else                 PrintStringP(PSTR(" Run"));
        }

    if( PrevCurr.On != TransducerCurr.On ) {
        PrevCurr.On  = TransducerCurr.On;
        POS_OUT;
        if( PrevCurr.On ) PrintStringP(PSTR(" On"));
        else              PrintStringP(PSTR("Off"));
        }

    if( PrevSet.Freq != TransducerSet.Freq ) {
        PrevSet.Freq  = TransducerSet.Freq;
        POS_FREQS;
        PrintD(PrevSet.Freq,5);
        }

    if( PrevCurr.Freq != TransducerCurr.Freq ) {
        PrevCurr.Freq  = TransducerCurr.Freq;
        POS_FREQM;
        PrintD(PrevCurr.Freq,5);
        }

    if( PrevSet.Power != TransducerSet.Power ) {
        PrevSet.Power  = TransducerSet.Power;
        POS_PWRS;
        PrintX10(PrevSet.Power);
        }

    if( PrevCurr.Power != TransducerCurr.Power ) {
        PrevCurr.Power  = TransducerCurr.Power;
        POS_PWRM;
        PrintX10(PrevCurr.Power);
        }

    if( PrevSet.CtlMode != TransducerSet.CtlMode ) {
        PrevSet.CtlMode  = TransducerSet.CtlMode;
        POS_CMODE;
        if( PrevSet.CtlMode == CTL_CONST_FREQ ) PrintStringP(PSTR("CF"));
        if( PrevSet.CtlMode == CTL_MAX_EFF    ) PrintStringP(PSTR("ME"));
        }

    if( PrevSet.RunMode != TransducerSet.RunMode ) {
        PrevSet.RunMode  = TransducerSet.RunMode;
        POS_RMODE;
        if( PrevSet.RunMode == RUN_CONTINUOUS ) PrintStringP(PSTR("Contn"));
        if( PrevSet.RunMode == RUN_TIMED      ) PrintStringP(PSTR("Timed"));
        }

    if( PrevSet.RunTimer != TransducerSet.RunTimer ) {
        PrevSet.RunTimer  = TransducerSet.RunTimer;
        POS_RTIME;
        PrintD(PrevSet.RunTimer,5);
        }

    if( PrevCurr.Current != TransducerCurr.Current ) {
        PrevCurr.Current  = TransducerCurr.Current;
        POS_AMPS;
        PrintX10(PrevCurr.Current);
        }

    if( PrevCurr.PWM != TransducerCurr.PWM ) {
        PrevCurr.PWM  = TransducerCurr.PWM;
        POS_PWM;
        PrintX10(PrevCurr.PWM);
        }

    if( PrevCurr.PWMWiper != TransducerCurr.PWMWiper ) {
        PrevCurr.PWMWiper  = TransducerCurr.PWMWiper;
        POS_PWW;
        PrintD(PrevCurr.PWMWiper,3);
        }

    CursorPos(1,DEBUG_ROW);
    DebugPrint();

    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////////////

    if( PromptNeeded )
        Prompt();
    PromptNeeded = false;
    
    PlotInput();
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAScreenCommand
//
// Inputs:      Command entered at screen
//
// Outputs:     TRUE  if we understood and processed command
//              FALSE if command isn't ours, belongs to another system
//
#ifdef USE_MAIN_SCREEN_CMDS

bool MAScreenCommand(char *Command) {

    //
    // Accept blank commands and commands beginning with "#" as comments
    //
    if( strlen(Command) == 0 ||
        Command[0]      == '#' )
        return true;

    if( SetupCmd(Command) )
        return true;

    if( TransducerCmd(Command) )
        return true;

    //
    // CL - Clear the message area
    //
    if( StrEQ(Command,"CL") ) {
        StartMsg();
        return true;
        }

    //
    // Command was unrecognized by this panel - caller must take responsibility
    //
    return false;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// StartMsg - Setup for message reporting
//
// Inputs:      None.
//
// Outputs:     None.
//
void StartMsg(void) {

    POS_MSG;
    ClearEOS;

    PromptNeeded = true;
    }


#endif // USE_MAIN_SCREEN_CMDS

#endif // USE_MAIN_SCREEN
