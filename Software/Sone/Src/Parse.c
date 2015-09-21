//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Parse.c
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

#include <string.h>

#include <PortMacros.h>

#include <Parse.h>

//
// Maximum size of an input token. In other words, the maximum
//   number of characters in a command, or between two delimiters
//   or the maximum number of digits in an input number
//
#define MAX_TOKEN_LENGTH    10

//
// Any character in the following is a delimiter character.
//   Delimiters come between tokens in a command.
//
#define DELIMITERS  " \t"

//
// IsDelimiter() macro
//
#define IsDelimiter(__char__)   strchr(DELIMITERS,__char__)

static char *LineBuffer              NOINIT;
static char  Token[MAX_TOKEN_LENGTH] NOINIT;

/////////////////////////////////////////////////////////////////////////////////
//
// ParseInit - Initialize command line parsing
//
// Inputs:      COmmand line to examine
//
// Outputs:     None.
//
void ParseInit(char *Buffer) {

    LineBuffer = Buffer;
    }

/////////////////////////////////////////////////////////////////////////////////
//
// ParseToken - Return next token in command buffer
//
// Inputs:      None.
//
// Outputs:     Ptr to next token in command buffer
//              Empty string if no more tokens
//
char *ParseToken() {
    char    *TokenPtr = Token;

    memset(Token,0,sizeof(Token));

    //
    // Start by skipping over any existing delimiters.
    //
    // Note that strchr (in IsDelimiter) considers the NUL a searchable
    //   character, so we have to make a special case for it.
    //
    while( *LineBuffer != 0 && IsDelimiter(*LineBuffer) )
        LineBuffer++;

    //
    // Now move token chars into Token array, while keeping track of
    //   lengths and such
    //
    while( *LineBuffer != 0                 &&
           !IsDelimiter(*LineBuffer)        &&
            strlen(Token) < MAX_TOKEN_LENGTH ) {
        *TokenPtr++ = *LineBuffer++;
        }

    return(Token);
    }
