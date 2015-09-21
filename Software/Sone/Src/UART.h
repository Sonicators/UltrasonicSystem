//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      UART.h
//
//  SYNOPSIS
//
//      UARTInit();                         // Called once at startup
//
//      char InChar = GetUARTByte();        // == 0 if no chars available
//
//      bool Success = PutUARTByte('A');    // == FALSE if buffer was full
//
//      PutUARTByteW('A');                  // Block until complete
//
//      If( UARTBusy() ) ...                // TRUE if sending something
//
//  DESCRIPTION
//
//      A simple serial Rx/Tx driver module for interrupt driven communications
//        on an AVR processor.
//
//      Sometimes you just don't need all the structure and complexity of stdio.
//        Many projects need only fixed text output, or simple char-by-char
//        input processing.
//
//      (Also, this module can be stepped through and debugged more readily than
//         the stdio system.)
//
//      The baud rate and FIFO sizes are set in the UART.h file.
//
//  NOTES:
//
//      This interface WILL NOT receive a NUL character (ascii 0). This is on
//        purpose, to make for a simple interface.
//
//      This interface DOES NOT detect UART or overflow errors.
//
//      These are not the putc() and getc() functions required for stdio
//        by WinAVR. See serial.h for those.
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

#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <avr/wdt.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef BAUD
#define BAUD            19200
#endif

//
// The serial FIFO's must be a power of two long each, since the code
//   uses binary wraparounds to access. This system has NO error detection
//   and NO XON/XOFF processing.
//
#ifndef IFIFO_SIZE
#define IFIFO_SIZE      (1 << 3)        // == 8  char Rx FIFO
#endif

#ifndef OFIFO_SIZE
#define OFIFO_SIZE      (1 << 6)        // == 64 chars Tx FIFO
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// UARTInit - Initialize UART
//
// This routine initializes the UART based on the settings above. Called from
//   init. Also enables the Rx interrupts, disables the Tx interrupt and
//   clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
void UARTInit(void);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// PutUARTByte - Send one char out the serial port
//
// Send a char out the serial port. We stuff the char into the FIFO and enable
//   interrupts - at some point the interrupt handler will get serviced and
//   send the char out for us.
//
// Inputs:      Byte to send
//
// Outputs:     TRUE  if char was sent OK,
//              FALSE if buffer full
//
bool PutUARTByte(char OutChar);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// PutUARTByteW - Send one char out the serial port, wait for completion
//
// Like PutUARTByte, but will block [if no FIFO space] until complete.
//
// Inputs:      Byte to send
//
// Outputs:     None.
//
#define PutUARTByteW(_OutChar_) { while(!PutUARTByte(_OutChar_)); }

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// GetUARTByte - Get one char from the serial port
//
// Get a char from the UART. The interrupt handler already received the
//   character for us, so this just pulls the char out of the receive FIFO.
//
// Inputs:      None.
//
// Outputs:     ASCII char, if one was available
//              NUL   (binary value = 0) if no chars available
//
char GetUARTByte(void);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// UARTBusy - Return TRUE if UART is busy sending output
//
// Inputs:      None.
//
// Outputs:     TRUE  if UART is busy sending output
//              FALSE if UART is idle
//
bool UARTBusy(void);

#endif // UART_H - entire file
