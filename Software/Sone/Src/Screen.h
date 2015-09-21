//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Screen.h
//
//  SYNOPSIS
//
//      ...MAScreen
//      ...HEScreen
//      ...CustomScreens
//      (Other screens, as needed)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      ///////////////////////////////////////////////////////////////////////////
//      //
//      // All done with init, OK to enable interrupts and go
//      // 
//      sei();                              // Enable interrupts
//
//      ScreenInit();                       // NOTE: After interrupts enabled!
//      CommandInit();                      // NOTE: After interrupts enabled!
//
//      while(1) {
//
//          //
//          // Update screen display
//          //
//          ScreenUpdate();
//          } 
//
//  DESCRIPTION
//
//      Command processing
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

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>

//
// Uncomment these next to use the pre-built screens
//
#define USE_MAIN_SCREEN
#define USE_HELP_SCREEN
#define USE_DEBUG_SCREEN
//#define USE_MEMORY_SCREEN
//#define USE_EEPROM_SCREEN

//#define USE_CUSTOM_SCREENS

//
// Determine if pre-built screens should have command processing
//
#define USE_MAIN_SCREEN_CMDS
//#define USE_HELP_SCREEN_CMDS
//#define USE_DEBUG_SCREEN_CMDS
//#define USE_MEMORY_SCREEN_CMDS
//#define USE_EEPROM_SCREEN_CMDS

//#define USE_CUSTOM_SCREEN_CMDS


//
// Position of user input, on screen
//
#define INPUT_COL   1
#define INPUT_ROW   21

//
// Position of error msgs, if any
//
#define ERROR_COL   1
#define ERROR_ROW   19

//
// Static layout of the help screen
//
#define HELP_SCREEN "\
-- ADNS2610 Test program --\r\n\
\r\n\
MA - Show the  main screen\r\n\
DE - Show the  debug screen\r\n\
EE - Dump the  EEPROM memory\r\n\
ME - Dump the  RAM memory\r\n\
HE - Show this help panel\r\n\
?  - Show this help panel\r\n\
\r\n\
PW   #  Set power       wiper\r\n\
ON      Turn transducer on\r\n\
OF      Turn transducer off\r\n\
";

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

extern int  SelectedScreen;

#ifdef USE_MAIN_SCREEN
#   include "MAScreen.h"
#   endif

#ifdef USE_DEBUG_SCREEN
#   include "DEScreen.h"
#   endif

#ifdef USE_HELP_SCREEN
#   include "HEScreen.h"
#   endif

#ifdef USE_MEMORY_SCREEN
#   include "MEScreen.h"
#   endif

#ifdef USE_EEPROM_SCREEN
#   include "EEScreen.h"
#   endif

#ifdef USE_CUSTOM_SCREENS
#   include "CustomScreens.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ScreenInit - Initialize status screen display
//
// Inputs:      None
//
// Outputs:     None.
//
void ScreenInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ShowScreen - Show status screen
//
// Inputs:      Type of screen to show
//
// Outputs:     None.
//
void ShowScreen(int ScreenType);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ScreenUpdate - Update the status screen information
//
// Inputs:      None
//
// Outputs:     None.
//
void ScreenUpdate(void);

#endif  // SCREEN_H - entire file
