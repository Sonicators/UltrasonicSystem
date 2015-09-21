//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      PWM.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // On Circuitboard
//      //
//      Hook up a PWM Oscillator to ICP (PortB.0)
//
//      //////////////////////////////////////
//      //
//      // In Timer.h
//      //
//      ...Choose a timer                  (Default: Timer2)
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//      ...Choose tick duration            (Default: 40ms)
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      TimerInit();
//      PWMInit();                          // Called once at startup
//          :
//
//      while(1) {
//          while( !TimerUpdate() )
//              sleep_cpu();                // Wait for tick to happen
//
//          PWMUpdate();                    // Update frequency calculations
//
//          uint16_t PWM = GetPWM();        // Get PWM over last second
//          }
//
//  DESCRIPTION
//
//      PWM processing
//
//  EXAMPLE
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

#ifndef PWM_H
#define PWM_H

#include "Timer.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// The only ICP on the Arduino is on Timer1
//

//
// No user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PWMInit - Initialize PWM system
//
// Inputs:      None.
//
// Outputs:     None.
//
void PWMInit(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PWMUpdate - Update PWM measurement
//
// Inputs:      None.
//
// Outputs:     None.
//
void PWMUpdate(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetPWM     - Return currently measured PWM
// GetPWMFreq - Return currently measured Frequency
//
// Inputs:      None.
//
// Outputs:     Measured PWM, in % x 10 (ex: 35% -> 350)
// Outputs:     Measured Frequency, in Hz
//
uint16_t GetPWM (void);
uint16_t GetPWMFreq(void);


#endif  // PWM_H - entire file
