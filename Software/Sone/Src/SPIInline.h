//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SPIInline.h
//
//  SYNOPSIS
//
//      PortB.2 (SS)                // Should be grounded!
//      PoerB.4 (MISO)
//      PortB.3 (MOSI)
//      PortB.5 (SCK)
//      
//      //////////////////////////////////////
//      //
//      // In SPIInline.h
//      //
//      ...Choose a speed                  (Default: FOsc/4)
//      ...Choose clock/data phase         (Default: Mode0)
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      SPIInit;                            // Called once at startup
//
//      PutSPIByte(0x45);                   // [Blocking] Send single byte out SPI port
//
//      GetSPIByte(0x45,SomeVar);           // [Blocking] Transfer/receive byte
//
//  DESCRIPTION
//
//      A simple serial SPI module using polled inline code.
//
//      The SPI interface is inherently fast - enough so that the complexity of interrupt
//        driven processing is a waste of time and effort.
//
//      If you have only a few SPI devices which are programmed largely by single-byte
//        I/O, then the interrupt overhead is unnecessary and inline polled mode is
//        usually just as fast and much simpler to use.
//
//      (Taking an interrupt requires 7 clock cycles. With register saves and restores,
//        the total time load can run several tens of cycles. For comparison, a
//        single-byte SPI transfer at 8MHz will take 16 cycles.)
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

#ifndef SPIInline_H
#define SPIInline_H

#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Choose the speed based on the table in the hardware manual
//
//  SPI2X SPR1 SPR0     Frequency
//  ===== ==== ====     =========
//      0    0    0     F_osc/4
//      0    0    1     F_osc/16
//      0    1    0     F_osc/64
//      0    1    1     F_osc/128
//      1    0    0     F_osc/2
//      1    0    1     F_osc/8
//      1    1    0     F_osc/32
//      1    1    1     F_osc/64
//
#ifndef SPI_SPEED
//#define SPI_SPEED       ((1 << SPR0) | (1 << SPR1))
#define SPI_SPEED       0
#endif

#define SPEED2X
//#define SPI_LSB_FIRST

//
// Choose the clock/data phase based on the table in the hardware manual
//
#ifndef SPI_MODE
//#define SPI_MODE        ((1 << CPOL) | (1 << CPHA))
#define SPI_MODE        0
#endif

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Sigh. The standard Atmega 168/328 file doesn't define these.
//
#ifndef SPI_PORT

#define SPI_PORT    B                   // Port containing MISO, MOSI, &c

#define SS_BIT      2                   // Bits in SPI port
#define MOSI_BIT    3
#define MISO_BIT    4
#define SCK_BIT     5

#endif

#ifdef SPI_LSB_FIRST
#define SPI_DIR     (1 << DORD)
#else
#define SPI_DIR     0
#endif

#ifdef SPEED2X
#define SPEEDX      { SPSR = (1 << SPI2X); }
#else
#define SPEEDX
#endif

//
// Convenience macro
//
#define SPI_WAIT    { while(_BIT_OFF(SPSR,SPIF)); }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SPIInit - Initialize SPI
//
// This routine initializes the SPI based on the settings above. Called from
//   init. Also disables the Tx interrupt and clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
#define SPIInit {                                                                       \
    _CLR_BIT(PRR,PRSPI);                    /* Power up the SPI         */              \
                                                                                        \
    /*                                                                                  \
     * Enable port I/O                                                                  \
     *                                                                                  \
     * NOTE: SS is REQUIRED to be set and low. Don't forget this!!!                     \
     */                                                                                 \
    _SET_BIT( _DDR(SPI_PORT),MOSI_BIT);     /* MOSI is an output        */              \
    _CLR_BIT( _DDR(SPI_PORT),MISO_BIT);     /* MISO is an input         */              \
    _SET_BIT( _DDR(SPI_PORT), SCK_BIT);     /* SCK  is an output        */              \
    _SET_BIT( _DDR(SPI_PORT),  SS_BIT);     /* SS   is an output        */              \
    _CLR_BIT(_PORT(SPI_PORT),  SS_BIT);     /* Set  low for now         */              \
                                                                                        \
    SPCR = (1 << SPE) | (1<<MSTR) | SPI_MODE | SPI_DIR | SPI_SPEED;                     \
    SPEEDX;                                                                             \
    SPDR;                                   /* Clear pending interrupts */              \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SPI_LSB_FIRST - Send the LSB out first
// SPI_MSB_FIRST - Send the MSB out first
//
// Normally this is set and forget, but if you have mixed hardware you can dynamically
//   switch as needed. (The AD9850 takes its data LSB first, for instance.)
//
#define SPI_LSB_FIRST   _SET_BIT(SPCR,DORD)
#define SPI_MSB_FIRST   _CLR_BIT(SPCR,DORD)
        

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PutSPIByte - Send one byte out the SPI port
//
// Send a byte out the SPI port, block until complete
//
// Inputs:      Byte to send
//
// Outputs:     None.
//
#define PutSPIByte(_OutByte_) {                                                         \
        SPDR = _OutByte_;                                                               \
        SPI_WAIT;                                                                       \
        }                                                                               \
        

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetSPIByte - Send one byte out the SPI port, return data
//
// Send a byte out the SPI port, save the response in the specified variable.
//
// Inputs:      Byte to send
//              Var  to receive result
//
// Outputs:     None. (Var is set to SPI result)
//
#define GetSPIByte(_OutByte_,_Result_) {                                                \
        PutSPIByte(_OutByte_);                                                          \
        _Result_ = SPDR;                                                                \
        }                                                                               \

#endif // SPIInline_H - entire file
