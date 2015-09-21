//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Inputs.h
//
//  SYNOPSIS
//
//
//      //////////////////////////////////////
//      //
//      // In Inputs.h
//      //
//      ...Choose ports and pins for input (Default: PortD.5/PortB.1)
//      ...Choose a debounce time          (Default: 3 ticks)
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      InputsInit();
//          :
//
//      while(1) {
//          while( !TimerUpdate() ) {
//              sleep_cpu();                // Wait for tick to happen
//              }
//
//          InputsUpdate();                 // Update input changes
//          }
//
//      if( Input1Changed ) {
//          Input1Changed = false;          // Reset flag
//          ...Process Input1On             // Process change of state for Input1
//          }
//
//      if( Input2Changed ) {
//          Input2Changed = false;          // Reset flag
//          ...Process Input2On             // Process change of state for Input1
//          }
//
//  DESCRIPTION
//
//      Input processing
//
//      Manage inputs (user button, limit switch, or electronic control)
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

#ifndef INPUTS_H
#define INPUTS_H

#include <stdbool.h>

#include <avr\io.h>
#include <avr\interrupt.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// The port and bit which represents the input
//
//#define INPUT_I1_PORT   C               // PortC      // See Config.h
//#define INPUT_I1_BIT    5               // Pin 5
//
//#define PCIE_I1         PCIE2           // Corresponds to PortD.5 (Datasheet, pg 74)
//#define PCMSK_I1        PCMSK2          // Corresponds to PortD.5 (Datasheet, pg 74)
//#define PCINT_I1        PCINT21         // Corresponds to PortD.5 (Datasheet, pg 74)
//#define INPUT_I1_VECT   PCINT2_vect
//
//#define INPUT_I2_PORT   B               // PortB        // See Config.h
//#define INPUT_I2_BIT    1               // Pin 1
//
//#define PCIE_I2         PCIE0           // Corresponds to PortB.1 (Datasheet, pg 74)
//#define PCMSK_I2        PCMSK0          // Corresponds to PortB.1 (Datasheet, pg 74)
//#define PCINT_I2        PCINT1          // Corresponds to PortB.1 (Datasheet, pg 74)
//#define INPUT_I2_VECT   PCINT0_vect
//
//
// After the input changes, further changes will be ignored for this many Timer cycles
//
#define INPUT_I1_DEBOUNCE 3
#define INPUT_I2_DEBOUNCE 3

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

extern bool Input1Changed;
extern bool Input2Changed;

extern bool Input1On;
extern bool Input2On;

extern void ProcessInput1(bool Input1On);
extern void ProcessInput2(bool Input2On);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// InputsInit - Initialize input1
//
// Inputs:      None.
//
// Outputs:     None.
//
void InputsInit(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// InputsUpdate - Update inputs
//
// Inputs:      None.
//
// Outputs:     None.
//
void InputsUpdate(void);


#endif  // INPUTS_H - entire file
