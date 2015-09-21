//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Debug.c
//
//  SYNOPSIS
//
//      //////////////////////////////////////////////////////////////////////////////
//      //
//      // In Main
//      //
//      DebugInit();
//
//      //////////////////////////////////////////////////////////////////////////////
//      //
//      // All done with init, OK to enable interrupts and go
//      // 
//      sei();                              // Enable interrupts
//
//      while(1) {
//
//          //
//          // Dump debug values on command
//          //
//          if( Some-Trigger ) 
//              PrintDebug();
//          } 
//
//  DESCRIPTION
//
//      Functions to aid debug
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

#include "Debug.h"
#include "Serial.h"
#include "PortMacros.h"

#ifdef DEBUG_CPU_COUNT
#include "SerialLong.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef USE_DEBUG_ARRAY
//
// The GCC compiler ignores "__attribute__ ((aligned (8)))", so use the assembler
//   macros to achieve 16-byte alignment. (GCC bug)
//
//uint16_t DebugArray[DEBUG_SIZE] __attribute__ ((aligned (8)));
//
asm("           .global DebugArray");
asm("           .data             ");
asm("           .align           8");   // 8 words, 16bytes
asm("DebugArray:.byte          128");   // **** MUST MATCH DEBUG_ARRAY!!! ****
asm("           .text             ");

       uint16_t DebugIndex    NOINIT;   // Wraparound index into DebugArray
static uint8_t  DebugTrigFlag NOINIT;   // Trigger flag

#define DEBUG_WRAP  (DEBUG_SIZE-1)      // Wraparound mask for DebugIndex
#endif

#ifdef DEBUG_CPU_COUNT
uint32_t DebugCPUCounter NOINIT;
#endif

uint16_t Debug1 NOINIT;                 // Utility probes
uint16_t Debug2 NOINIT;
uint16_t Debug3 NOINIT;
uint16_t Debug4 NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// DebugInit - Initialize debugging code
//
// Inputs:      None.
//
// Outputs:     None.
//
void DebugInit(void) {

    Debug1 = 0;
    Debug2 = 0;
    Debug3 = 0;
    Debug4 = 0;

#ifdef USE_DEBUG_ARRAY
    for( DebugIndex = 0; DebugIndex < DEBUG_SIZE; DebugIndex ++ )
        DebugArray[DebugIndex] = 0;

    DebugIndex    = 0;
    DebugTrigFlag = DEBUG_TRIG_SET;
#endif

#ifdef DEBUG_CPU_COUNT
    DebugCPUCounter = 0;
#endif
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintDebug - Print out the debugging code
//
// Inputs:      None.
//
// Outputs:     None.
//
void DebugPrint(void) {

    PrintStringP(PSTR("DBG1 "));PrintD(Debug1,-6);PrintCRLF();
    PrintStringP(PSTR("DBG2 "));PrintD(Debug2,-6);PrintCRLF();
    PrintStringP(PSTR("DBG3 "));PrintD(Debug3,-6);PrintCRLF();
    PrintStringP(PSTR("DBG4 "));PrintD(Debug4,-6);PrintCRLF();

#ifdef DEBUG_CPU_COUNT
    PrintStringP(PSTR("CNTR "));PrintLD(DebugCPUCounter,8);PrintCRLF();
    DebugCPUCounter = 0;
#endif  // DEBUG_CPU_COUNT
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DEBUG_ARRAY

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// DebugLog - Log data value to debugging array
//
// Inputs:      Value to log
//
// Outputs:     None.
//
void DebugLog(uint16_t Value) {

    //
    // Return immediately if triggered and done logging
    //
    if( DebugTrigFlag == DEBUG_TRIG_OFF )
        return;

    //
    // Log values
    //
    DebugArray[DebugIndex] = Value;
    DebugIndex = (DebugIndex+1) & DEBUG_WRAP;

    //
    // Wind down to DEBUG_STOP if triggered
    //
    if( DebugTrigFlag != DEBUG_TRIG_SET )
        DebugTrigFlag--;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// DebugTrigger - Trigger the debugging array and stop logging
//
// Inputs:      -1 Trigger is reset, waiting for trigger
//               0 Trigger is complete, data logging is stopped
//              >0 Triggered, waiting for final log values
//
// Outputs:     None.
//
void DebugTrigger(void) {

    DebugTrigFlag = DEBUG_TRIG_ON;
    }

#endif  // USE_DEBUG_ARRAY

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_CPU_COUNT

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// DebugCPUCount - Count CPU cycles per call
//
// Inputs:      None.
//
// Outputs:     None.
//
void DebugCPUCount(void) { DebugCPUCounter++; }

#endif  // DEBUG_CPU_COUNT
