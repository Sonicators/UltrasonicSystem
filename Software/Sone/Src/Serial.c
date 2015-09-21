//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Serial.h
//
//  SYNOPSIS
//
//      char     String1[] = "...";
//      uint16_t Value;
//      uint8_t  Byte;
//      char     SomeChar;
//
//      PrintString(String);        // => printf("%s",String);
//
//      PrintD(Value,  0);          // => printf(  "%d",Value);
//      PrintD(Value,  3);          // => printf( "%3d",Value);
//      PrintD(Value,103);          // => printf("%03d",Value);
//      PrintD(Value, -3);          // => printf("%+3d",Value);
//
//      PrintH(Byte);               // => printf("%02X",Byte);
//      PrintB(Byte);               // => printf("%02B",Byte);
//
//      PrintChar(SomeChar);        // => printf("%c",SomeChar);
//
//      PrintCRLF();                // => printf("/r/n");
//
//      static const prog_char String1[] = "...";
//
//      PrintStringP(String1);      // => printf("%s",String);
//
//
//  DESCRIPTION
//      
//      Simple UART serial interface.
//
//      Sometimes you just don't need all the structure and complexity of stdio.
//        Many projects need only fixed text output, or simple char-by-char
//        input processing.
//
//      These functions make static printing trivial. If all you need is
//        static text with simple conversions (hex and decimal), then
//        a series of calls to these functions will take up much less space
//        than the corresponding call to printf().
//
//      The arguments to PrintDValue,Width):
//
//          If Width is 0       The output is unpadded, as in %d
//                      n       The output is right justified in n spaces
//                      -n      The output is left  justified in n spaces
//                      100+n   Like n, with lead zeroes
//      
//      The PrintD function does not use divide or modulo, which might
//        otherwise require a large [and slow] library call.
//      
//  NOTES:
//
//      String literals are normally stored in RAM and therefore take up
//        valuable memory. Storing fixed strings as prog_char puts them in
//        the program space, freeing up more memory for use.
//
//      However, prog_char data requires a special access method. The
//        PrintStringD function deals with this.
//
//      Decimal constants do not have this problem.
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

#include "Serial.h"
#include "UART.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintChar - Print out a single char
//
// Inputs:      Char to print
//
// Outputs:     None.
//
void PrintChar(char Char) { PutUARTByteW(Char); }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintString - Print out a serial string
//
// Inputs:      Text string to print
//
// Outputs:     None.
//
void PrintString(const char *String) {

    while( *String )
        PrintChar(*String++);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintStringP - Print out a serial PSTR string
//
// Inputs:      Text string to print
//
// Outputs:     None.
//
void PrintStringP(const char *String) {
    char    Char;

    while( (Char = pgm_read_byte(String++)) )
        PrintChar(Char);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintCRLF - Print out CR/LF
//
// Inputs:      None.
//
// Outputs:     None.
//
void PrintCRLF(void) { PrintChar('\r'); PrintChar('\n'); }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintD - Printf integer with %d format
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
static int Divisors[] PROGMEM = { 10000, 1000, 100, 10 };

void PrintD(uint16_t Value,int8_t Width) {
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

    for( Index = 0; Index < 4; Index++ ) {
        uint16_t    Divisor = pgm_read_word(&Divisors[Index]);
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
// PrintH - Printf byte with %x format
//
// Inputs:      Short int to print
//
// Outputs:     None.
//
static char HexChars[] PROGMEM = { 
    '0', '1', '2', '3', 
    '4', '5', '6', '7', 
    '8', '9', 'A', 'B', 
    'C', 'D', 'E', 'F' };

void PrintH(uint8_t Byte) {

    PrintChar(pgm_read_byte(HexChars + (Byte >>    4)));
    PrintChar(pgm_read_byte(HexChars + (Byte &  0x0F)));
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintB - Printf byte with %b format
//
// Inputs:      Short int to print
//
// Outputs:     None.
//
void PrintB(uint8_t Byte) {
    int Bit;

    for( Bit = 0; Bit < 8; Bit++ ) {
        PrintChar((Byte & 0x80) == 0 ? '0' : '1');
        Byte <<= 1;
        }
    }
