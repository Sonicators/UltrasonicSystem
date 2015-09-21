//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      EEProm.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In EEPROM.h
//      //
//      ...Choose a version number
//      ...Define the EEPROM struct
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      EEPROMInit();                       // Initialize RAM copy
//          :
//
//      EEPROM.Value1 = NewValue1;          // Set new local values
//      EEPROM.Value2 = NewValue2;
//          :
//
//      EEPROMWrite();                      // Write new local values in EEPROM
//      
//
//  DESCRIPTION
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

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

#include "Setup.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EEPROM memory layout
//
//#define EEPROM_CURR_VERSION 6       // See Config.h

typedef struct {
    //
    // Start with the version number of the EEPROM layout
    //
    uint8_t     Version;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // User defined vars go here
    //
    SETUP       Setups[MAX_SETUPS];

    //////////////////////////////////////////////////////////////////////////////////////
    } EEPROM_T;

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

extern EEPROM_T    EEPROM;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EEPROMInit - Initialize RAM copy of EEPROM 
//
// Inputs:      None.
//
// Outputs:     None.
//
void EEPROMInit(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EEPROMWrite - Write new EEPROM values from RAM
//
// Inputs:      None.
//
// Outputs:     None.
//
void EEPROMWrite(void);


#endif  // EEPROM_H - entire file
