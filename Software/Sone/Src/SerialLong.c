//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SerialLong.h
//
//  SYNOPSIS
//
//      uint32_t Value;
//
//      PrintLD(Value,  0);          // => printf(  "%d",Value);
//      PrintLD(Value,  3);          // => printf( "%3d",Value);
//      PrintLD(Value,103);          // => printf("%03d",Value);
//      PrintLD(Value, -3);          // => printf("%+3d",Value);
//
//      PrintLH(Value);              // => printf("%08X",Long);
//      PrintLB(Value);              // => printf("%032B",Long);
//
//  DESCRIPTION
//
//      Simple UART serial interface for long (32-bit) values
//
//      The arguments to PrintLD(Value,Width):
//
//          If Width is 0       The output is unpadded, as in %d
//                      n       The output is right justified in n spaces
//                      -n      The output is left  justified in n spaces
//                      100+n   Like n, with lead zeroes
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

#include <stddef.h>
#include <avr/pgmspace.h>

#include "SerialLong.h"
#include "Serial.h"
#include "UART.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintLD - Printf long integer with %d format
//
// Inputs:      Integer to convert
//              Width of field:
//
//                  0       The output is unpadded, as in %d
//                  n       The output is right justified in n spaces
//                  -n      The output is left  justified in n spaces
//                  100+n   Like n, with lead zeroes
//
// Outputs:     None.
//
static long Divisors[] PROGMEM = { 1000000000, 100000000, 10000000, 1000000,
                                  100000, 10000, 1000, 100, 10 };

void PrintLD(uint32_t Value,int8_t Width) {
    uint8_t CharsPrinted = 0;
    uint8_t Index;
    char    PadChar = ' ';

    //
    // If the Width field is > 100, then it's a signal to pad the
    //   field with zeroes instead of spaces.
    //
    if( Width > 100 ) {
        Width -= 100;
        PadChar = '0';
        }

    for( Index = 0; Index < 9; Index++ ) {
        uint32_t    Divisor = pgm_read_dword(&Divisors[Index]);
        char        OutChar = '0';

        while( Value >= Divisor ) {
            Value -= Divisor;
            OutChar++;
            }

        if( OutChar != '0' || CharsPrinted ) {
            if( CharsPrinted == 0 && Width > 0 ) {
                uint8_t Chars = 5-Index;
                if( Width < Chars )
                    Width = Chars;
                while( Width > Chars ) {
                    PrintChar(PadChar);
                    Width--;
                    }
                }

            PrintChar(OutChar);
            CharsPrinted++;
            }
        }

    //
    // Print out the last character.
    //
    if( CharsPrinted == 0 && Width > 0 )
        while( --Width ) {
            PrintChar(PadChar);
            }

    PrintChar('0' + Value);

    //
    // If we were left justified, pad out the rest of the field.
    //
    if( Width < 0 )
        while( ++CharsPrinted < -Width )
            PrintChar(PadChar);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintLH - Printf long with %x format
//
// Inputs:      Long int to print
//
// Outputs:     None.
//
static char HexChars[] PROGMEM = { 
    '0', '1', '2', '3', 
    '4', '5', '6', '7', 
    '8', '9', 'A', 'B', 
    'C', 'D', 'E', 'F' };

void PrintLH(uint32_t DWord) {

    PrintChar(pgm_read_byte(HexChars + ((DWord >> 28) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >> 24) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >> 20) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >> 16) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >> 12) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >>  8) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >>  4) & 0x0F)));
    PrintChar(pgm_read_byte(HexChars + ((DWord >>  0) & 0x0F)));
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintLB - Printf long with %b format
//
// Inputs:      Long int to print
//
// Outputs:     None.
//
void PrintLB(uint32_t DWord) {
    int Bit;

    for( Bit = 0; Bit < 32; Bit++ ) {
        PrintChar((DWord & 0x800000) == 0 ? '0' : '1');
        DWord <<= 1;
        }
    }
