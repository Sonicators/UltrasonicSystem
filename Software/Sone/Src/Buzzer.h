//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Buzzer.h
//
//  SYNOPSIS
//
//
//      //////////////////////////////////////
//      //
//      // In Buzzer.h
//      //
//      ...Choose port and pin for buzzer (Default: PortD.4)
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      BuzzerInit();
//          :
//
//      BuzzerSet(bool Value);              // Turn buzzer on or off
//
//  DESCRIPTION
//
//      Buzzer processing
//
//      Manage buzzer
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

#ifndef BUZZER_H
#define BUZZER_H

#include <stdbool.h>

#include <avr\io.h>
#include <avr\interrupt.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// The port and bit which represents the buzzer
//
//#define BUZZER_PORT     C               // PortC      // See Config.h
//#define BUZZER_BIT      4               // Pin 4      // See config.h

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define BuzzerGet  _BIT_ON(_PORT(BUZZER_PORT),BUZZER_BIT)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// BuzzerInit - Initialize buzzer
//
// Inputs:      None.
//
// Outputs:     None.
//
void BuzzerInit(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// BuzzerSet - Turn on buzzer
//
// Inputs:      TRUE  to set buzzer ON
//              FALSE otherwise
//
// Outputs:     None.
//
void BuzzerSet(bool Set);


#endif  // BUZZER_H - entire file
