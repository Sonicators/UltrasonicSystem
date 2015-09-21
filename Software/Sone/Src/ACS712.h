//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ACS712.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In ACS712.h
//      //
//      ...Choose AtoD channel              (Default: ADC0)
//      ...Choose skip count                (Default: 10)
//      ...Choose pos or neg mode           (Default: Neg)
//      
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      TimerInit();
//      ACS712Init();                       // Called once at startup
//          :
//
//      while(1) {
//          while( !TimerUpdate() )
//              sleep_cpu();                // Wait for tick to happen
//
//          ACS712Update();                 // Update current calculations
//          }
//
//      Current = ACS712GetCurrent();       // Return avg current from last tick
//
//
//  DESCRIPTION
//
//      Simple ACS712 current measurement
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

#ifndef ACS712_H
#define ACS712_H

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a n AtoD channel to use
//
//#define ACS712_CHANNEL  0     // See Config.h

#define ACS712_SKIP     16                          // Count one out of every 10

//
// Uncomment this next if the current goes forward through the chip in the wrong
//   direction.
//
#define ACS712_REVERSE

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ACS712Init - Initialize ACS712 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void ACS712Init(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ACS712Update - Update ACS712 current values
//
// Inputs:      None
//
// Outputs:     None.
//
void ACS712Update(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ACS712GetCurrent - Return current value
//
// Inputs:      None
//
// Outputs:     Average current since last request
//
uint16_t ACS712GetCurrent(void);

#endif  // ACS712_H - entire file
