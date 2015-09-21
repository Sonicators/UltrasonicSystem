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
//      PrintLD(Value,#)            // => printf of (long) value
//
//      PrintH(Byte);               // => printf("%02X",Byte);
//      PrintB(Byte);               // => printf("%08B",Byte);
//      PrintH2(Value);             // => printf("%02X",Value);
//
//      PrintChar(SomeChar);        // => printf("%c",SomeChar);
//
//      PrintCRLF();                // => printf("/r/n");
//
//      static const prog_char String1[] = "...";
//
//      PrintStringP(String1);      // => printf("%s",String);
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
//      The PrintD function does not use divide or modulo, which might
//        otherwise require a large [and slow] library call.
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

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#include <avr/pgmspace.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintString - Print out a serial string
//
// Inputs:      Text string to print
//
// Outputs:     None.
//
void PrintString(const char *String);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintStringP - Print out a serial PSTR string
//
// Inputs:      Text string to print
//
// Outputs:     None.
//
void PrintStringP(PGM_P String);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintD  - Printf short integer with %d format
// PrintLD - Printf long  integer with %d format
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
void PrintD (uint16_t Value,int8_t Width);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintH  - Printf byte with %x format
// PrintB  - Printf byte with %b format
//
// Inputs:      Short int to print
//
// Outputs:     None.
//
void PrintH(uint8_t Byte);
void PrintB(uint8_t Byte);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintH2 - Printf integer with %d format
//
// Inputs:      Integer to convert
//
// Outputs:     None.
//
#define PrintH2(_x_) { PrintH(((_x_) >> 8) & 0xFF);PrintH(((_x_) >> 0) & 0xFF); }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintChar - Print out a single char
//
// Inputs:      Char to print
//
// Outputs:     None.
//
void PrintChar(char Char);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintCRLF - Print out CR/LF
//
// Inputs:      None.
//
// Outputs:     None.
//
void PrintCRLF(void);


#endif  // SERIAL_H - entire file
