//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Setup.c
//
//  SYNOPSIS
//
//  DESCRIPTION
//
//      Definition of a "setup" in the Sone software
//
//      A "setup" is a set of configuration choices can be saved and restored from
//        EEPROM.
//
//      For example, a setup specific for welding might indicate the transducer to use,
//        the power to generate when "on", and the generation time when triggered.
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

#include "Setup.h"
#include "EEPROM.h"

#include "Serial.h"
#include "Command.h"
#include "Parse.h"
#include "MAScreen.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

uint8_t CurrSetup NOINIT;

PROGMEM SETUP SetupDefaults = {
    { 28000, 20,                // Default output freq, power
      RUN_CONTINUOUS, 0,        // Default run mode and run timer
      CTL_CONST_FREQ,           // Constant frequency
      { INPUT_UNUSED, 0 },      // Default action for Input1
      { INPUT_UNUSED, 0 },      // Default action for Input2
      }
    };


//
// Per the WINAVR definition of PROGMEM, we must explicitly put each string into PROGMEM
//   within an array separately
//
static char IAT1[] PROGMEM = "Unused";
static char IAT2[] PROGMEM = "Switch transducer";
static char IAT3[] PROGMEM = "Push on/Push off transducer";
static char IAT4[] PROGMEM = "EStop";

static char *InputActionText[NUM_ACTIONS] = {
    IAT1, IAT2, IAT3, IAT4
    };

static char PMT1[] PROGMEM = "Constant freq";
static char PMT2[] PROGMEM = "Max efficiency";

static char *CtlModeText[NUM_CTL_MODES]= {
    PMT1, PMT2
    };

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetupInit - Initialize setup at system start
//
// Inputs:      None.
//
// Outputs:     None.
//
void SetupInit(void) {

    EEPROMInit();

    //
    // If uninitialized, or if version mismatch (we're the newer version), initialize
    //   with defaults
    //
    if( EEPROM.Version != EEPROM_CURR_VERSION ) {
        for( CurrSetup = 0; CurrSetup < MAX_SETUPS; CurrSetup++ )
            memcpy_P(&EEPROM.Setups[CurrSetup],&SetupDefaults,sizeof(SetupDefaults));
        EEPROM.Version = EEPROM_CURR_VERSION;
        EEPROMWrite();
        }

    LoadSetup(0);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LoadSetup - Load a setup from EEPROM
//
// Inputs:      Index of setup to load
//
// Outputs:     None.
//
void LoadSetup(uint8_t Setup) {

    CurrSetup = Setup;
    TransducerSet = EEPROM.Setups[Setup].Setup;
    TransducerSetup();
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SaveSetup - Save a setup to EEPROM
//
// Inputs:      Index of setup to save
//
// Outputs:     None.
//
void SaveSetup(uint8_t Setup) {

    EEPROM.Setups[Setup].Setup = TransducerSet;
    EEPROMWrite();
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintInputMode - Print out input mode
//
// Inputs:      Input of note (1 or 2)
//              Input to print
//
// Outputs:     None.
//
static void PrintInputMode(uint8_t InputID,INPUT *Input) {

    StartMsg();
    PrintStringP(PSTR("Mode I"));
    PrintChar('0' + InputID);
    PrintStringP(PSTR(": "));
    PrintStringP(InputActionText[IDX_ACTION(Input->Action)]);
    if( Input->Print )
        PrintStringP(PSTR(" +print"));
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintSetup - Print out setup information
//
// Inputs:      Index of setup (-1 == current working set)
//              Address of TRANSDUCER_SET to print
//
// Outputs:     None.
//
static void PrintSetup(uint8_t SetupID,TRANSDUCER_SET *Setup) {

    StartMsg();
    PrintStringP(PSTR("Setup "));
    if( SetupID == -1 ) PrintStringP(PSTR(" (working)"));
    else {
        PrintD(SetupID,0);                  // == %d
        if( SetupID == CurrSetup )
            PrintStringP(PSTR(" (current)"));
        }
    PrintStringP(PSTR(":\r\n"));

    PrintStringP(PSTR("Freq: "));
    PrintD(Setup->Freq,5);
    PrintStringP(PSTR("Hz, "));
    PrintD(Setup->Power,3);
    PrintStringP(PSTR(" Watts\r\n"));

    PrintStringP(PSTR("Output: "));
    if( Setup->RunMode == RUN_CONTINUOUS )
        PrintStringP(PSTR("Continuous\r\n"));
    else {
        PrintStringP(PSTR("Timed "));
        PrintD(Setup->RunTimer,5);
        PrintCRLF();
        }

    PrintStringP(PSTR("Control: "));
    PrintStringP(CtlModeText[IDX_CTL_MODE(Setup->CtlMode)]);

    PrintInputMode(1,&Setup->Input1);
    PrintInputMode(2,&Setup->Input2);
    PrintCRLF();
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ModeInputCmd - Interpret "MO Ix" command arguments
//
// Inputs:      Input of note (1 or 2)
//              Token after "MO Ix"
//
// Outputs:     None.
//
static void ModeInputCmd(uint8_t InputID,char *Command) {
    INPUT          *Input;
    INPUT_ACTION    Action;
    bool            Print;

    //
    // Note - Caller will error if anything other than I1 or I2 is used.
    //
    if ( InputID == 1 ) Input = &TransducerSet.Input1;
    else                Input = &TransducerSet.Input2;

    //
    // Accept blank "MO Ix" command as a request to print current mode
    //
    if( !strlen(Command) ) {
        PrintInputMode(InputID,Input);
        return;
        }

    bool    ArgOK = false;

    //
    // U - Input is unused
    //
    if( StrEQ(Command,"U") ) {
        Action = INPUT_UNUSED;
        ArgOK  = true;
        }

    //
    // XC - Input is unused
    //
    if( StrEQ(Command,"XC") ) {
        Action = INPUT_XCTRL;
        ArgOK  = true;
        }

    //
    // PO - Input is unused
    //
    if( StrEQ(Command,"PO") ) {
        Action = INPUT_XPOPO;
        ArgOK  = true;
        }

    //
    // ES - Input is unused
    //
    if( StrEQ(Command,"ES") ) {
        Action = INPUT_ESTOP;
        ArgOK  = true;
        }

    StartMsg();
    if( !ArgOK ) {
        PrintStringP(PSTR("Unrecognized input mode ("));
        PrintString(Command);
        PrintStringP(PSTR("), must be U, XC, PO, or ES.\r\n"));
        PrintStringP(PSTR("Type '?' for help\r\n"));
        return;
        }

    //
    // See if "P" appended to mode
    //
    Command = ParseToken();
    Print   = false;

    if( strlen(Command) > 0 ) {
        if( StrEQ(Command,"P") ) Print = true;
        else {
            PrintStringP(PSTR("Unrecognized input mode ("));
            PrintString(Command);
            PrintStringP(PSTR("), must be P or nothing.\r\n"));
            PrintStringP(PSTR("Type '?' for help\r\n"));
            return;
            }
        }

    Input->Action = Action;
    Input->Print  = Print;

    PrintInputMode(InputID,Input);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ModeCmd - Interpret "MO" command arguments
//
// Inputs:      Token after "MO"
//
// Outputs:     None.
//
static void ModeCmd(char *Command) {

    //
    // Accept blank "MO" command as a request to print current mode
    //
    if( !strlen(Command) ) {
        StartMsg();
        PrintStringP(PSTR("Current mode: "));
        PrintStringP(CtlModeText[IDX_CTL_MODE(TransducerSet.CtlMode)]);
        return;
        }

    //
    // R - Set run mode
    //
    if( StrEQ(Command,"R") ) {
        StartMsg();
        PrintStringP(PSTR("Run continuous"));
        TransducerRunMode(RUN_CONTINUOUS,0);
        return;
        }

    //
    // RT - Set timed run mode
    //
    if( StrEQ(Command,"RT") ) {
        char *TimeText  = ParseToken();
        int   TimeTicks = atoi(TimeText);

        if( !strlen(TimeText) ||
            TimeTicks < 0      ) {
            StartMsg();
            PrintStringP(PSTR("Bad or out of range run time ("));
            PrintString(TimeText);
            PrintStringP(PSTR("), must be positive number.\r\n"));
            PrintStringP(PSTR("Type '?' for help\r\n"));
            return;
            }

        StartMsg();
        PrintStringP(PSTR("Run for "));
        PrintD(TimeTicks,0);
        PrintStringP(PSTR(" ticks."));
        TransducerRunMode(RUN_TIMED,TimeTicks);
        return;
        }


    //
    // CF - Constant frequency
    //
    if( StrEQ(Command,"CF") ) {
        StartMsg();
        PrintStringP(PSTR("Constant frequency mode"));
        TransducerCtlMode(CTL_CONST_FREQ);
        return;
        }

    //
    // Ix - Set input action
    //
    if( StrEQ(Command,"I1") ||
        StrEQ(Command,"I2") ) {
        ModeInputCmd(Command[1] - '0',ParseToken());
        return;
        }

    StartMsg();
    PrintStringP(PSTR("Unrecognized mode argument ("));
    PrintString(Command);
    PrintStringP(PSTR(")\r\n"));
    PrintStringP(PSTR("Type '?' for help\r\n"));
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetupCmd - Interpret typed commands for setup processing
//
// Inputs:      Command to interpret
//
// Outputs:     TRUE  if we understood and processed command
//              FALSE if command isn't ours, belongs to another system
//
bool SetupCmd(char *Command) {

    //
    // MO - Mode command
    //
    if( StrEQ(Command,"MO") ) {
        ModeCmd(ParseToken());
        return true;
        }


    //
    // LS - Save setup
    //
    if( StrEQ(Command,"LS") ) {
        char *SetupText = ParseToken();
        int   SetupNum  = atoi(SetupText);

        if( !strlen(SetupText) )
            SetupNum = CurrSetup;

        LoadSetup(SetupNum);

        StartMsg();
        PrintStringP(PSTR("Loaded setup "));
        PrintD(CurrSetup,0);            // == %d
        return true;
        }

    //
    // PS - Print current setup
    //
    if( StrEQ(Command,"PS") ) {
        char *SetupText = ParseToken();
        int   SetupNum  = atoi(SetupText);

        if( !strlen(SetupText) ) PrintSetup(-1      ,&TransducerSet);
        else                     PrintSetup(SetupNum,&EEPROM.Setups[SetupNum].Setup);
        return true;
        }

    //
    // SS - Save current setup
    //
    if( StrEQ(Command,"SS") ) {
        char *SetupText = ParseToken();
        int   SetupNum  = atoi(SetupText);

        if( !strlen(SetupText) )
            SetupNum = CurrSetup;

        SaveSetup(SetupNum);

        StartMsg();
        PrintStringP(PSTR("Saved as setup "));
        PrintD(CurrSetup,SetupNum);            // == %d
        return true;
        }

    return false;
    }
