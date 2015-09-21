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

#ifndef SERIALLONG_H
#define SERIALLong_H

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
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
void PrintLD(uint32_t Value,int8_t Width);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintLH  - Printf long with %x format
// PrintLB  - Printf long with %b format
//
// Inputs:      Short int to print
//
// Outputs:     None.
//
void PrintLH(uint32_t Byte);
void PrintLB(uint32_t Byte);


#endif  // SERIALLONG_H - entire file
