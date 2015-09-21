//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      VT100.h
//
//  DESCRIPTION
//      
//      Definitions for VT100 escape code functions. These are all ANSII
//        standard and widely published.
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

#ifndef VT100_H
#define VT100_H

#include "Serial.h"

#ifndef __xstr__
#define __str__(_x_)   #_x_
#define __xstr__(_x_)  __str__(_x_)
#endif

#define ClearScreen         PrintStringP(PSTR("\033[2J"));
#define ClearEOS            PrintStringP(PSTR("\033[J" ));
#define ClearEOL            PrintStringP(PSTR("\033[K" ));
#define CursorHome          PrintStringP(PSTR("\033[H" ));
#define CursorPos(_x_,_y_)  PrintStringP(PSTR("\033[" __xstr__(_y_) ";" __xstr__(_x_) "H"))

#endif  // VT100_H - Entire file 
