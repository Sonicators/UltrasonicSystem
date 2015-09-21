//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Outputs.h
//
//  SYNOPSIS
//
//
//      //////////////////////////////////////
//      //
//      // In Outputs.h
//      //
//      ...Choose port and pins for output (Default: PortC.2/PortC.3)
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      OutputsInit();
//          :
//
//      Output1Set(bool Value);             // Turn output 1 on or off
//      Output2Set(bool Value);             // Turn output 2 on or off
//
//      if( Output1Get ) ...do something    // Recall current output setting
//      if( Output2Get ) ...do something    // Recall current output setting
//
//  DESCRIPTION
//
//      Output processing
//
//      Manage outputs
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

#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <stdbool.h>

#include <avr\io.h>
#include <avr\interrupt.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// The port and bit which represents the input
//
//#define OUTPUT1_PORT    C               // PortC    // See Config.h
//#define OUTPUT1_BIT     2               // Pin 2

//#define OUTPUT2_PORT    C               // PortC    // See Config.h
//#define OUTPUT2_BIT     3               // Pin 3

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define Output1Get  _BIT_ON(_PORT(OUTPUT1_PORT),OUTPUT1_BIT)
#define Output2Get  _BIT_ON(_PORT(OUTPUT2_PORT),OUTPUT2_BIT)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// OutputsInit - Initialize outputs
//
// Inputs:      None.
//
// Outputs:     None.
//
void OutputsInit(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Output1Set - Set output1
//
// Inputs:      TRUE  to set output ON
//              FALSE otherwise
//
// Outputs:     None.
//
void Output1Set(bool Set);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Output2Set - Set output2
//
// Inputs:      TRUE  to set output ON
//              FALSE otherwise
//
// Outputs:     None.
//
void Output2Set(bool Set);

#endif  // OUTPUTS_H - entire file
