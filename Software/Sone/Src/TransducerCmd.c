//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TransducerCmd.c - Command interface to the transducer driver
//
//  SYNOPSIS
//
//      See Transducer.h for details
//
//  DESCRIPTION
//
//      The Transducer module was getting a little big, so the command processing was
//        moved to this file.
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

#include <stdlib.h>
#include <string.h>

#include "Transducer.h"
#include "Command.h"
#include "Serial.h"
#include "MAScreen.h"
#include "Parse.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

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
bool TransducerCmd(char *Command) {

    //
    // OFF - Turn transducer output off
    //
    if( StrEQ(Command,"OF" ) ||
        StrEQ(Command,"OFF")) {
        TransducerOn(false);
StartMsg();
PrintStringP(PSTR("Transducer OFF"));
        return true;
        }


    //
    // ON - Turn transducer output on
    //
    if( StrEQ(Command,"ON") ) {
        TransducerOn(true);

        //
        // If EStopped, let the user know we can't start.
        //
        if( TransducerCurr.EStop ) {
            StartMsg();
            PrintStringP(PSTR("Can't output while EStopped\007"));
            return true;
            }

StartMsg();
PrintStringP(PSTR("Transducer ON"));
        return true;
        }


    //
    // ES - Set estop
    //
    if( StrEQ(Command,"ES") ) {
        TransducerEStop(true);
StartMsg();
PrintStringP(PSTR("Reset: EStop ON"));
        return true;
        }


    //
    // RE - Reset estop
    //
    if( StrEQ(Command,"RE") ) {
        TransducerEStop(false);
StartMsg();
PrintStringP(PSTR("Reset: EStop OFF"));
        return true;
        }


    //
    // FR - Set frequency
    //
    if( StrEQ(Command,"FR") ) {
        char *FreqText = ParseToken();

        //
        // Accept a blank FR command as a request to print the frequency
        //
        if( !strlen(FreqText) ) {
StartMsg();
PrintStringP(PSTR("Freq: "));
PrintD(TransducerSet.Freq,0);
            return true;
            }


        int   FreqNum  = atoi(FreqText);
        if( FreqNum < TRANSDUCER_MIN_FREQ ||
            FreqNum > TRANSDUCER_MAX_FREQ ) {
            StartMsg();
            PrintStringP(PSTR("Bad or out of range frequency ("));
            PrintString(FreqText);
            PrintStringP(PSTR("), must be "));
            PrintD(TRANSDUCER_MIN_FREQ,0);
            PrintStringP(PSTR(" to "));
            PrintD(TRANSDUCER_MAX_FREQ,0);
            PrintCRLF();
            PrintStringP(PSTR("Type '?' for help\r\n"));
            return true;
            }

        TransducerFreq(FreqNum);
        return true;
        }

    //
    // PO - Set power
    //
    if( StrEQ(Command,"PO") ) {
        char *PowerText = ParseToken();
        int   PowerNum  = atoi(PowerText);
        if( PowerNum < TRANSDUCER_MIN_POWER ||
            PowerNum > TRANSDUCER_MAX_POWER ) {
            StartMsg();
            PrintStringP(PSTR("Bad or out of range power ("));
            PrintString(PowerText);
            PrintStringP(PSTR("), must be "));
            PrintD(TRANSDUCER_MIN_POWER,0);
            PrintStringP(PSTR(" to "));
            PrintD(TRANSDUCER_MAX_POWER,0);
            PrintCRLF();
            PrintStringP(PSTR("Type '?' for help\r\n"));
            return true;
            }

        TransducerPower(PowerNum);
        return true;
        }

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // ADJ COMMANDS
    //
    // For debugging, allow the user to directly bump the power/freq
    //

    //
    // L - Louder (more power)
    //
    if( StrEQ(Command,"L" ) ) {
        TransducerPower(TransducerSet.Power+10);
        return true;
        }


    //
    // S - Softer (less power)
    //
    if( StrEQ(Command,"S" ) ) {
        TransducerPower(TransducerSet.Power-10);
        return true;
        }

    //
    // U - Bump the frequency up 100 Hz
    //
    if( StrEQ(Command,"U" ) ) {
        TransducerFreq(TransducerSet.Freq+100);
        return true;
        }


    //
    // D - Bump the frequency down 100 Hz
    //
    if( StrEQ(Command,"D" ) ) {
        TransducerFreq(TransducerSet.Freq-100);
        return true;
        }


    //
    // + - Make frequency go up by a little
    //
    if( StrEQ(Command,"+" ) ) {
        TransducerFreq(TransducerSet.Freq+20);
        return true;
        }


    //
    // - - Make frequency go down by a little
    //
    if( StrEQ(Command,"-" ) ) {
        TransducerFreq(TransducerSet.Freq-20);
        return true;
        }


#ifdef USE_WIPER_CMDS
    //////////////////////////////////////////////////////////////////////////////////////
    //
    // WIPER COMMANDS
    //
    // For debugging, allow the user to directly set the wiper positions
    //

    //
    // PW - Set transducer power wiper
    //
    if( StrEQ(Command,"PW") ) {
        char *PowerText = ParseToken();
        int   PowerNum  = atoi(PowerText);
        if( PowerNum < 0 ||
            PowerNum > PWMPot_MAX_WIPER ) {
            StartMsg();
            PrintStringP(PSTR("Bad or out of range wiper ("));
            PrintString(PowerText);
            PrintStringP(PSTR("), must be 0 to "));
            PrintD(PWMPot_MAX_WIPER,0);
            PrintCRLF();
            PrintStringP(PSTR("Type '?' for help\r\n"));
            PrintCRLF();
            return true;
            }

        PWMPotSetWiper(TransducerCurr.PWMWiper = PowerNum);
        return true;
        }
#endif // USE_WIPER_CMDS


    return false;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ProcessInputX - Process input trigger events
//
// Inputs:      Trigger event to process (1 == button pressed)
//
// Outputs:     None.
//
// NOTE: See Inputs.h for an explanation of this linkage
//
void ProcessInput1(bool Input1On) {

    if( TransducerSet.Input1.Print ) {
        StartMsg();
        PrintStringP(PSTR("Input1 "));
        if( Input1On ) PrintStringP(PSTR("ON\r\n" ));
        else           PrintStringP(PSTR("OFF\r\n"));
        }

    switch( TransducerSet.Input1.Action ) {

        //
        // Input is unused - just ignore
        //
        case INPUT_UNUSED:
            break;

        //
        // Direct xducer control - Turn xducer ON when button pressed
        //
        case INPUT_XCTRL:
            TransducerOn(Input1On);
            break;

        //
        // Push on/Push off - Toggle current state when button pressed
        //
        case INPUT_XPOPO:
            if( Input1On )
                TransducerOn(!TransducerCurr.On);
            break;

        //
        // ESTOP when triggered
        //
        case INPUT_ESTOP:
            if( Input1On )
                TransducerEStop(true);
            break;
        }
    }


void ProcessInput2(bool Input2On) {

    if( TransducerSet.Input1.Print ) {
        StartMsg();
        PrintStringP(PSTR("Input2 "));
        if( Input2On ) PrintStringP(PSTR("ON\r\n" ));
        else           PrintStringP(PSTR("OFF\r\n"));
        }

    switch( TransducerSet.Input2.Action ) {

        //
        // Input is unused - just ignore
        //
        case INPUT_UNUSED:
            break;

        //
        // Direct xducer control - Turn xducer ON when button pressed
        //
        case INPUT_XCTRL:
            TransducerOn(Input2On);
            break;

        //
        // Push on/Push off - Toggle current state when button pressed
        //
        case INPUT_XPOPO:
            if( Input2On )
                TransducerOn(!TransducerCurr.On);
            break;

        //
        // ESTOP when triggered
        //
        case INPUT_ESTOP:
            if( Input2On )
                TransducerEStop(true);
            break;
        }
    }

