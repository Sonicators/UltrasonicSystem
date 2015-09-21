//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MCP4131.h - SPI Interface to the MCP4131 50K digital potentiometer
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In TestPot.h
//      //
//      #include <MCP4131.h>
//
//      #define TestPot_PORT                    D
//      #define TestPot_BIT                     3
//      #define TestPot_MAXR                50000
//      #define TestPot_MAX_WIPER   MCP4131_STEPS
//
//      #define TestPotInit             MCP4131Init(TestPot_PORT,TestPot_BIT)
//      #define TestPotSetWiper(_w_)    MCP4131SetWiper(TestPot_PORT,TestPot_BIT,_w_)
//      #define TestPotSetResist(_r_)   MCP4131SetResist(TestPot_PORT,TestPot_BIT,TestPot_MAXR,_r_)
//      #define TestPotIncr             MCP4131Incr(TestPot_PORT,TestPot_BIT)
//      #define TestPotDecr             MCP4131Decr(TestPot_PORT,TestPot_BIT)
//
//      #define TestPotR2W(_r_)         MCP4131_R2W(TestPot_MAXR,_r_)
//      #define TestPotW2R(_w_)         MCP4131_W2R(TestPot_MAXR,_w_)
//
//      //////////////////////////////////////
//      //
//      // In Circuit
//      //
//      Connect MCP4131 CS  (pin 1) to PORT/BIT specified in TestPot.h
//      Connect MCP4131 SDO (pin 3) to PortB.4 (MISO) of CPU
//      Connect MCP4131 SDI (pin 3) to PortB.3 (MOSI) of CPU
//      Connect MCP4131 SCK (pin 2) to PortB.5 (SCK)  of CPU
//      
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      TestPotInit();                      // Called once at startup
//
//      TestPotSetWiper(uint8_t);           // Set wiper value in steps
//      TestPotSetResist(uint8_t);          // Set wiper value in ohms
//      TestPotInc;                         // Increment wiper value
//      TestPotDec;                         // Decrement wiper value
//
//  DESCRIPTION
//
//      A simple driver module for the MCP4131 digital potentiometer.
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

#ifndef MCP4131_H
#define MCP4131_H

#include <stdint.h>

#include "SPIInline.h"

#define MCP4131_STEPS           129

#define MCP4131_R2W(_M_,_R_)    ((uint16_t) SCALE_RATIO(((uint32_t) (_R_)),MCP4131_STEPS,_M_))
#define MCP4131_W2R(_M_,_W_)    ((uint16_t) SCALE_RATIO(((uint32_t) (_W_)),_M_,MCP4131_STEPS))

#define MCP4131_WRITE           0x00
#define MCP4131_INCR            0x04
#define MCP4131_DECR            0x08
#define MCP4131_READ            0x0C

#define MCP4131_PULLUP          0x03

//
// Definitions for entire MCP series, some are invalid in some devices
//
#define MCP4131_VWIPER0         0x00
#define MCP4131_NVWIPER0        0x01
#define MCP4131_VWIPER1         0x02
#define MCP4131_NVWIPER2        0x03
#define MCP4131_TCON            0x04
#define MCP4131_STATUS          0x05
#define MCP4131_EEPROM          0x06

#define MCP4131_SELECT(_p_,_b_)     _CLR_BIT(_PORT(_p_),_b_)
#define MCP4131_DESELECT(_p_,_b_)   _SET_BIT(_PORT(_p_),_b_)

#define MCP4131_R1HW    0x80
#define MCP4131_R1A     0x40
#define MCP4131_R1W     0x20
#define MCP4131_R1B     0x10
#define MCP4131_R0HW    0x08
#define MCP4131_R0A     0x04
#define MCP4131_R0W     0x02
#define MCP4131_R0B     0x01

#define MCP4131_R0ENB   (MCP4131_R0HW | MCP4131_R0A | MCP4131_R0W | MCP4131_R0B)
#define MCP4131_R1ENB   (MCP4131_R1HW | MCP4131_R1A | MCP4131_R1W | MCP4131_R1B)

//////////////////////////////////////////////////////////////////////////////////////////
//
// Convenience macros
//
#define MCP4131Write(_p_,_b_,_Addr_,_OutByte_) {                                        \
    MCP4131_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4131_WRITE);                                        \
    PutSPIByte(_OutByte_);                                                              \
    MCP4131_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

#define MCP4131Incr(_p_,_b_,_Addr_) {                                                   \
    MCP4131_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4131_INCR);                                         \
    MCP4131_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

#define MCP4131Decr(_p_,_b_,_Addr_) {                                                   \
    MCP4131_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4131_DECR);                                         \
    MCP4131_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131Init - Initialize the MCP4131 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define MCP4131Init(_p_,_b_) {                                                          \
                                                                                        \
    MCP4131_DESELECT(_p_,_b_);                          /* Set CS high to start */      \
    _SET_BIT(_DDR(_p_),_b_);                            /* CS   is an output    */      \
                                                                                        \
    MCP4131Write(_p_,_b_,MCP4131_TCON,MCP4131_R0ENB);                                   \
    MCP4131SetWiper(_p_,_b_,MCP4131_STEPS/2)                                            \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131SetWiper - Set wiper to specified value
//
// Inputs:      Wiper value to set (0 .. 255)
//
// Outputs:     None.
//
#define MCP4131SetWiper(_p_,_b_,_W_) {                                                  \
    MCP4131Write(_p_,_b_,MCP4131_VWIPER0,_W_);                                          \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131SetResist - Set resistance value
//
// Inputs:      Resistance (0 .. 1000)
//
// Outputs:     None.
//
#define MCP4131SetResist(_p_,_b_,_MAXR_,_R_) {                                          \
    MCP4131Write(_p_,_b_,MCP4131_VWIPER0,MCP4131_R2W(_MAXR_,_R_));                      \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131Inc - Increment wiper value
//
// Inputs:      Resistance (0 .. 1000)
//
// Outputs:     None.
//
#define MCP4131Inc(_p_,_b_) {                                                           \
    MCP4131Incr(_p_,_b_,MCP4131_VWIPER0);                                               \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131Dec - Increment wiper value
//
// Inputs:      Resistance (0 .. 1000)
//
// Outputs:     None.
//
#define MCP4131Dec(_p_,_b_) {                                                           \
    MCP4131Decr(_p_,_b_,MCP4131_VWIPER0);                                               \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131GetReg - Return register value
//
// Inputs:      Address to get
//
// Outputs:     Value of register (9 bits)
//
#define MCP4131GetReg(_p_,_b_,_Addr_,_Dest_) {                                          \
    uint8_t Left, Right;                                                                \
                                                                                        \
    MCP4131_SELECT(_p_,_b_);                                                            \
    GetSPIByte(((_Addr_) << 4) | MCP4131_READ | MCP4131_PULLUP,Left);                   \
    GetSPIByte(0xFF,Right);                                                             \
    MCP4131_DESELECT(_p_,_b_);                                                          \
                                                                                        \
    _Dest_ = ((Left << 8) | Right) & 0x3FF;                                             \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131PutReg - Set register value
//
// Inputs:      Address to set
//
// Outputs:     Value to set (9 bits)
//
#define MCP4131PutReg(_p_,_b_,_Addr_,_Value_) {                                         \
    MCP4131_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4131_WRITE | (((_Value_) >> 8) & 0x03));            \
    PutSPIByte((_Value_) & 0xFF);                                                       \
    MCP4131_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

#endif  // MCP4131_H - entire file
