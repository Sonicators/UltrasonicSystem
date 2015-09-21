//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Setup.h
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

#ifndef SETUP_H
#define SETUP_H

#include <stdint.h>
#include <stdbool.h>

#include "Transducer.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup information
//
#define MAX_SETUPS 10

typedef struct {
    TRANSDUCER_SET  Setup;      // Saved mode, freq, and power
    } SETUP;

extern uint8_t  CurrSetup;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetupInit - Initialize setup at system start
//
// Inputs:      None.
//
// Outputs:     None.
//
void SetupInit(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LoadSetup - Load a setup from EEPROM
//
// Inputs:      Index of setup to load
//
// Outputs:     None.
//
void LoadSetup(uint8_t Setup);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SaveSetup - Save a setup to EEPROM
//
// Inputs:      Index of setup to save
//
// Outputs:     None.
//
void SaveSetup(uint8_t Setup);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetupCmd - Parse setup management commands
//
// Inputs:      Command to process
//
// Outputs:     TRUE  if we understood and processed command
//              FALSE if command isn't ours, belongs to another subsystem
//
bool SetupCmd(char *Command);


#endif  // SETUP_H - entire file
