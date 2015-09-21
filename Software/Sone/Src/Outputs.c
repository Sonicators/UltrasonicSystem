//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Outputs.c
//
//  DESCRIPTION
//
//      Output processing
//
//      See Outputs.h for an in-depth description
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


#include "Outputs.h"
#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// OutputsInit - Initialize outputs
//
// Inputs:      None.
//
// Outputs:     None.
//
void OutputsInit(void) {

    _SET_BIT( _DDR(OUTPUT1_PORT),OUTPUT1_BIT);    // Set as output
    _CLR_BIT(_PORT(OUTPUT1_PORT),OUTPUT1_BIT);    // Set low for now

    _SET_BIT( _DDR(OUTPUT2_PORT),OUTPUT2_BIT);    // Set as output
    _CLR_BIT(_PORT(OUTPUT2_PORT),OUTPUT2_BIT);    // Set low for now
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Output1Set - Set output1
//
// Inputs:      TRUE  to set output ON
//              FALSE otherwise
//
// Outputs:     None.
//
void Output1Set(bool Set) {

    if( Set ) { _SET_BIT(_PORT(OUTPUT1_PORT),OUTPUT1_BIT); }
    else      { _CLR_BIT(_PORT(OUTPUT1_PORT),OUTPUT1_BIT); }
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Output2Set - Set output2
//
// Inputs:      TRUE  to set output ON
//              FALSE otherwise
//
// Outputs:     None.
//
void Output2Set(bool Set) {

    if( Set ) { _SET_BIT(_PORT(OUTPUT2_PORT),OUTPUT2_BIT); }
    else      { _CLR_BIT(_PORT(OUTPUT2_PORT),OUTPUT2_BIT); }
    }
