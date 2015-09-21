//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Parse.h
//
//  DESCRIPTION
//
//      Input (serial) line parsing
//
//      Pull the next token from the supplied input line
//
//  NOTE
//
//      These functions work from static variables, so can only parse commands
//        serially in order. Cannot parse 2 lines at once.
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

#ifndef PARSE_H
#define PARSE_H

//////////////////////////////////////////////////////////////////////////////////////////
//
// ParseInit - Initialize command line parsing
//
// Inputs:      COmmand line to examine
//
// Outputs:     None.
//
void ParseInit(char *Buffer);

//////////////////////////////////////////////////////////////////////////////////////////
//
// ParseToken - Return next token in command buffer
//
// Inputs:      None.
//
// Outputs:     Ptr to next token in command buffer
//              Empty string if no more tokens
//
char *ParseToken(void);

#endif  // PARSE_H - Entire file 
