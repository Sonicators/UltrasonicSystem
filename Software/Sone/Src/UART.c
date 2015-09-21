//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      UART.c
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
//      The baud rate and FIFO sizes can be set in the UART.h file
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

#include <stdint.h>
#include <string.h>

#include <avr/interrupt.h>

#include "PortMacros.h"
#include "UART.h"

//////////////////////////////////////////////////////////////////////////////////////////

#define IFIFO_WRAP  (IFIFO_SIZE-1)      // Wraparound mask for Rx
#define OFIFO_WRAP  (OFIFO_SIZE-1)      // Wraparound mask for Tx

static struct {
    char    Rx_FIFO[IFIFO_SIZE];
    char    Tx_FIFO[OFIFO_SIZE];

    uint8_t Tx_FIFO_In;                 // FIFO input  pointer
    uint8_t Tx_FIFO_Out;                // FIFO output pointer
    uint8_t Rx_FIFO_In;                 // FIFO input  pointer
    uint8_t Rx_FIFO_Out;                // FIFO output pointer
    } UART NOINIT;


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// UARTInit - Initialize serial port
//
// This routine initializes the UART to BAUDRATE/7,1,n. Called from init. Also
//   enables the Rx interrupts, disables the Tx interrupt and clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
void UARTInit(void) {

    memset(&UART,0,sizeof(UART));

    _CLR_BIT(PRR,PRUSART0);             // Power up the UART

    //
    // Set the baud rate
    //
#include <util/setbaud.h>

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    _SET_BIT(UCSR0A,U2X0);
#else
    _CLR_BIT(UCSR0A,U2X0);
#endif

    //
    // Enable port I/O and Rx interrupt
    //
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);    // 8,n,1
    UCSR0B = (1<< RXEN0) | (1<< TXEN0) | (1<<RXCIE0);

    //
    // Enable internal pull-up resistor on pin D0 (RX), to supress line noise
    //
    _CLR_BIT( DDRD,PIND0);
    _SET_BIT(PORTD,PIND0);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
bool PutUARTByte(char OutChar) {
    uint8_t NewIn;
    bool    Success = false;

    _CLR_BIT(UCSR0B,UDRIE0);                    // Disable UART interrupts

    //
    // If there's room in the buffer, add the new char
    //
    NewIn = (UART.Tx_FIFO_In+1) & OFIFO_WRAP;

    if( NewIn != UART.Tx_FIFO_Out ) {
        UART.Tx_FIFO[UART.Tx_FIFO_In] = OutChar;
        UART.Tx_FIFO_In               = NewIn;
        Success = true;
        }

    _SET_BIT(UCSR0B,UDRIE0);                    // Enable UART interrupts

    return(Success);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetUARTByte - Get one char from the serial port
//
// Get a char from the serial port. The interrupt handler already received the
//   character for us, so this just pulls the char out of the receive FIFO.
//
// Inputs:      None
//
// Outputs:     ASCII char, if one was available
//              NUL   (binary value = 0) if no chars available
//
char GetUARTByte(void) {
    char    OutChar = 0;

    _CLR_BIT(UCSR0B,RXCIE0);                     // Disable UART interrupts

    if( UART.Rx_FIFO_In != UART.Rx_FIFO_Out ) {
        OutChar = UART.Rx_FIFO[UART.Rx_FIFO_Out];
        UART.Rx_FIFO_Out = (UART.Rx_FIFO_Out+1) & IFIFO_WRAP;
        }

    _SET_BIT(UCSR0B,RXCIE0);                    // Enable UART interrupts

    return(OutChar);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// UARTBusy - Return TRUE if UART is busy sending output
//
// Inputs:      None
//
// Outputs:     TRUE  if UART is busy sending output
//              FALSE if UART is idle
//
bool UARTBusy(void) { return( UART.Tx_FIFO_In != UART.Tx_FIFO_Out ); }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// USART_RX_vect - Handle input received chars
//
// Get the input character and place it into the Rx_FIFO.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(USART_RX_vect) {
    uint8_t NewIn;
    char    NewChar;

    NewChar = UDR0;                         // Get data, clear errors

    //
    // If there's room in the buffer, add the new char
    //
    NewIn = (UART.Rx_FIFO_In+1) & IFIFO_WRAP;

    if( NewIn != UART.Rx_FIFO_Out ) {
        UART.Rx_FIFO[UART.Rx_FIFO_In] = NewChar;
        UART.Rx_FIFO_In               = NewIn;
        }

    //
    // No room - Drop the character
    //
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// USART_UDRE_vect - Queue up another char to be transmitted
//
// Pull the next character to be sent from the TX_FIFO and send it. If no
//   more, turn off interrupt.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(USART_UDRE_vect) {

    //
    // If more chars are available, queue one up.
    //
    if( UART.Tx_FIFO_In != UART.Tx_FIFO_Out ) {
        UDR0             = UART.Tx_FIFO[UART.Tx_FIFO_Out];
        UART.Tx_FIFO_Out = (UART.Tx_FIFO_Out+1) & OFIFO_WRAP;
        }

    //
    // Else turn off interrupts, for now.
    //
    else _CLR_BIT(UCSR0B,UDRIE0);       // Disable buffer empty interrupt
    }
