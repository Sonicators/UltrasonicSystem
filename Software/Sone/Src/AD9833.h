//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      AD9833.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AD9833.h
//      //
//      ...Choose FSYNC port and pin        (Default: PORTD.5)
//      
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      AD9833Init();                       // Called once at startup
//          :
//
//      AD9833Enable(bool ON);              // Enable/disable output
//      bool AD9833IsEnabled(bool ON);      // Return whether output is on/off
//
//      AD9833SetFreq(uint16_t Freq);       // Set output frequency
//      uint16_t Freq = AD9833GetFreq();    // Get output frequency
//
//
//  DESCRIPTION
//
//      Simple AD9833 frequency output control
//
//  SYNOPSIS
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

#ifndef AD9833_H
#define AD9833_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the FSYNC output port
//
//#define AD9833_FSYNC_PORT   D         // See Config.h
//#define AD9833_FSYNC_PIN    5

#define AD0933_DEFAULT_FREQ 28000       // Default freq on init
//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
    AD9833_OFF = 300,               // Disable  output
    AD9833_SIN,                     // Sine     output
    AD9833_SQ,                      // Square   output
    AD9833_TRI,                     // Triangle output
    } AD9833_MODE;

#define NUM_AD9833_MODES   ( AD9833-TRI - AD9833_OFF + 1 )

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833Init - Initialize AD9833 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void AD9833Init(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833Output - Set mode and frequency
//
// Inputs:      Mode        AD9833_MODE of output (ie - AD9833_SIN, AD9833_SQ, etc.)
//              Frequency   of output
//
// Outputs:     None.
//
// NOTE: If Mode = AD9833_OFF, frequency is ignored (pass 0, for example).
//
void AD9833Output(AD9833_MODE Mode,uint16_t Freq);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833IsOn - Return whether output is on
//
// Inputs:      None.
//
// Outputs:     TRUE  if output enabled
//              FALSE otherwise
//
bool AD9833IsOn(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833GetFreq - Return current frequency
//
// Inputs:      None
//
// Outputs:     Currently set frequency
//
uint16_t AD9833GetFreq(void);

#endif  // AD9833_H - entire file
