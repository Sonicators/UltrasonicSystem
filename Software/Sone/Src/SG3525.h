//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SG3525.h - Interface to the SG3525 SMPS controller
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In SG3525.h
//      //
//      ...Set chip select port and bit    (Default: PortD.7)
//      ...Set PWM pot     port and bit    (Default: PortD.7)
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      SG3525Init();                       // Called once at startup
//
//      SG3525Enb(bool);                    // Turn device on/off
//
//  DESCRIPTION
//
//      A simple driver module for the SG3525 digital potentiometer.
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

#ifndef SG3525_H
#define SG3525_H

#include <stdbool.h>

#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// The driver will pull this CS line low while communicating with the chip.
//
// See Config.h
//
//#define SG3525_CS_PORT                          D       // Chip select: Port D
//#define SG3525_CS_BIT                           7       // Chip select: Control pin

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// Convenience macros
//
#define SG3525_ON           _CLR_BIT(_PORT(SG3525_CS_PORT),SG3525_CS_BIT)
#define SG3525_OFF          _SET_BIT(_PORT(SG3525_CS_PORT),SG3525_CS_BIT)
#define SG3525_IS_ON        _BIT_OFF(_PORT(SG3525_CS_PORT),SG3525_CS_BIT)

#define SG3525Init(_p_,_b_) {                                                           \
    SG3525_OFF;                                                                         \
    _SET_BIT(_DDR(SG3525_CS_PORT),SG3525_CS_BIT);       /* CS   is an output */         \
    }                                                                                   \

#define SG3525_INIT SG3525Init(SG3525_CS_PORT,SG3525_CS_BIT)

#endif  // SG3525_H - entire file
