//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MCP4161.h - SPI Interface to the MCP4161 digital potentiometer
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In TestPot.h
//      //
//      #include <MCP4161.h>
//
//      #define TestPot_PORT                    D
//      #define TestPot_BIT                     3
//      #define TestPot_MAXR                50000
//      #define TestPot_MAX_WIPER   MCP4161_STEPS
//
//      #define TestPotInit             MCP4161Init(TestPot_PORT,TestPot_BIT)
//      #define TestPotSetWiper(_w_)    MCP4161SetWiper(TestPot_PORT,TestPot_BIT,_w_)
//      #define TestPotSetResist(_r_)   MCP4161SetResist(TestPot_PORT,TestPot_BIT,TestPot_MAXR,_r_)
//      #define TestPotIncr             MCP4161Incr(TestPot_PORT,TestPot_BIT)
//      #define TestPotDecr             MCP4161Decr(TestPot_PORT,TestPot_BIT)
//
//      #define TestPotR2W(_r_)         MCP4161_R2W(TestPot_MAXR,_r_)
//      #define TestPotW2R(_w_)         MCP4161_W2R(TestPot_MAXR,_w_)
//
//      //////////////////////////////////////
//      //
//      // In Circuit
//      //
//      Connect MCP4161 CS  (pin 1) to PORT/BIT specified in TestPot.h
//      Connect MCP4161 SDO (pin 3) to PortB.4 (MISO) of CPU
//      Connect MCP4161 SDI (pin 3) to PortB.3 (MOSI) of CPU
//      Connect MCP4161 SCK (pin 2) to PortB.5 (SCK)  of CPU
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
//      A simple driver module for the MCP4161 digital potentiometer.
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

#ifndef MCP4161_H
#define MCP4161_H

#include <stdint.h>

#include "SPIInline.h"

#define MCP4161_STEPS           257

#define MCP4161_R2W(_M_,_R_)    ((uint16_t) SCALE_RATIO(((uint32_t) (_R_)),MCP4161_STEPS,_M_))
#define MCP4161_W2R(_M_,_W_)    ((uint16_t) SCALE_RATIO(((uint32_t) (_W_)),_M_,MCP4161_STEPS))

#define MCP4161_WRITE           0x00
#define MCP4161_INCR            0x04
#define MCP4161_DECR            0x08
#define MCP4161_READ            0x0C

#define MCP4161_PULLUP          0x03

//
// Definitions for entire MCP series, some are invalid in some devices
//
#define MCP4161_VWIPER0         0x00
#define MCP4161_NVWIPER0        0x01
#define MCP4161_VWIPER1         0x02
#define MCP4161_NVWIPER2        0x03
#define MCP4161_TCON            0x04
#define MCP4161_STATUS          0x05
#define MCP4161_EEPROM          0x06

#define MCP4161_SELECT(_p_,_b_)     _CLR_BIT(_PORT(_p_),_b_)
#define MCP4161_DESELECT(_p_,_b_)   _SET_BIT(_PORT(_p_),_b_)

#define MCP4161_R1HW    0x80
#define MCP4161_R1A     0x40
#define MCP4161_R1W     0x20
#define MCP4161_R1B     0x10
#define MCP4161_R0HW    0x08
#define MCP4161_R0A     0x04
#define MCP4161_R0W     0x02
#define MCP4161_R0B     0x01

#define MCP4161_R0ENB   (MCP4161_R0HW | MCP4161_R0A | MCP4161_R0W | MCP4161_R0B)
#define MCP4161_R1ENB   (MCP4161_R1HW | MCP4161_R1A | MCP4161_R1W | MCP4161_R1B)

//////////////////////////////////////////////////////////////////////////////////////////
//
// Convenience macros
//
#define MCP4161Write(_p_,_b_,_Addr_,_OutByte_) {                                        \
    MCP4161_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4161_WRITE);                                        \
    PutSPIByte(_OutByte_);                                                              \
    MCP4161_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

#define MCP4161Incr(_p_,_b_,_Addr_) {                                                   \
    MCP4161_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4161_INCR);                                         \
    MCP4161_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

#define MCP4161Decr(_p_,_b_,_Addr_) {                                                   \
    MCP4161_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4161_DECR);                                         \
    MCP4161_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161Init - Initialize the MCP4161 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define MCP4161Init(_p_,_b_) {                                                          \
                                                                                        \
    MCP4161_DESELECT(_p_,_b_);                          /* Set CS high to start */      \
    _SET_BIT(_DDR(_p_),_b_);                            /* CS   is an output    */      \
                                                                                        \
    MCP4161Write(_p_,_b_,MCP4161_TCON,MCP4161_R0ENB);                                   \
    MCP4161SetWiper(_p_,_b_,MCP4161_STEPS/2)                                            \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161SetWiper - Set wiper to specified value
//
// Inputs:      Wiper value to set (0 .. 255)
//
// Outputs:     None.
//
#define MCP4161SetWiper(_p_,_b_,_W_) {                                                  \
    MCP4161Write(_p_,_b_,MCP4161_VWIPER0,_W_);                                          \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161SetResist - Set resistance value
//
// Inputs:      Resistance (0 .. 1000)
//
// Outputs:     None.
//
#define MCP4161SetResist(_p_,_b_,_MAXR_,_R_) {                                          \
    MCP4161Write(_p_,_b_,MCP4161_VWIPER0,MCP4161_R2W(_MAXR_,_R_));                      \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161Inc - Increment wiper value
//
// Inputs:      Resistance (0 .. 1000)
//
// Outputs:     None.
//
#define MCP4161Inc(_p_,_b_) {                                                           \
    MCP4161Incr(_p_,_b_,MCP4161_VWIPER0);                                               \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161Dec - Increment wiper value
//
// Inputs:      Resistance (0 .. 1000)
//
// Outputs:     None.
//
#define MCP4161Dec(_p_,_b_) {                                                           \
    MCP4161Decr(_p_,_b_,MCP4161_VWIPER0);                                               \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161GetReg - Return register value
//
// Inputs:      Address to get
//
// Outputs:     Value of register (9 bits)
//
#define MCP4161GetReg(_p_,_b_,_Addr_,_Dest_) {                                          \
    uint8_t Left, Right;                                                                \
                                                                                        \
    MCP4161_SELECT(_p_,_b_);                                                            \
    GetSPIByte(((_Addr_) << 4) | MCP4161_READ | MCP4161_PULLUP,Left);                   \
    GetSPIByte(0xFF,Right);                                                             \
    MCP4161_DESELECT(_p_,_b_);                                                          \
                                                                                        \
    _Dest_ = ((Left << 8) | Right) & 0x3FF;                                             \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161PutReg - Set register value
//
// Inputs:      Address to set
//
// Outputs:     Value to set (9 bits)
//
#define MCP4161PutReg(_p_,_b_,_Addr_,_Value_) {                                         \
    MCP4161_SELECT(_p_,_b_);                                                            \
    PutSPIByte(((_Addr_) << 4) | MCP4161_WRITE | (((_Value_) >> 8) & 0x03));            \
    PutSPIByte((_Value_) & 0xFF);                                                       \
    MCP4161_DESELECT(_p_,_b_);                                                          \
    }                                                                                   \

#endif  // MCP4161_H - entire file
