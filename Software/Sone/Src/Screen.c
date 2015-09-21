//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Screen.c
//
//  SYNOPSIS
//
//      ...MAScreen
//      ...HEScreen
//      (Other screens, as needed)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//          :
//
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
//          UpdateScreen();
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

#include <stdint.h>

#include "PortMacros.h"
#include "Screen.h"
#include "Command.h"
#include "Timer.h"
#include "Serial.h"

       int      SelectedScreen  NOINIT;
static TIME_T   NextDisplayTime NOINIT;

#define BEEP    "\007"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ScreenInit - Initialize screen display
//
// Inputs:      None
//
// Outputs:     None.
//
void ScreenInit(void) {

    //
    // The main screen is shown by default
    //
    NextDisplayTime = 0;
    ShowScreen('MA');
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// BadScreen - Print out message and beep the user
//
// Inputs:      Bad screen ID seen
//
// Outputs:     None.
//
static void BadScreen(int ScreenType) {

    PlotError("Bad screen selected ("); 
    PrintChar((char) ((ScreenType >> 8) & 0xff));
    PrintChar((char) ((ScreenType >> 0) & 0xff));
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ShowScreen - Display the selected screen
//
// Inputs:      Two char abbreviation of screen (ie - 'DE')
//
// Outputs:     None.
//
void ShowScreen(int ScreenType) {

    SelectedScreen = ScreenType;

    switch(SelectedScreen) {

#ifdef USE_MAIN_SCREEN
        //
        //  - Show the main screen
        //
        case 'MA':
            ShowMAScreen();
            return;
#endif

#ifdef USE_HELP_SCREEN
        //
        // HE - Show the help screen
        //
        case 'HE':
            ShowHEScreen();
            return;
#endif

#ifdef USE_DEBUG_SCREEN
        //
        // DE - Show the debug screen
        //
        case 'DE':
            ShowDEScreen();
            return;
#endif

#ifdef USE_MEMORY_SCREEN
        //
        // ME - Show the memory screen
        //
        case 'ME':
            ShowMEScreen();
            return;
#endif

#ifdef USE_EEPROM_SCREEN
        //
        // EE - Show the memory screen
        //
        case 'EE':
            ShowEEScreen();
            return;
#endif
        }

#ifdef USE_USER_SCREENS
    if( ShowUserScreen(SelectedScreen) )
        return;
#endif

    BadScreen(SelectedScreen);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ScreenUpdate - Update the status screen information
//
// Inputs:      None
//
// Outputs:     None.
//
void ScreenUpdate(void) {
    TIME_T CurrentTime = TimerGetSeconds();

    //
    // Wait until at least 1 second has elapsed before we do anything.
    //
    if( CurrentTime < NextDisplayTime )
        return;

    NextDisplayTime = CurrentTime + 1;

    switch(SelectedScreen) {

#ifdef USE_MAIN_SCREEN
        //
        //  - Show the main screen
        //
        case 'MA':
            UpdateMAScreen();
            return;
#endif

#ifdef USE_HELP_SCREEN
        //
        // HE - Update the help screen
        //
        case 'HE':
            UpdateHEScreen();
            return;
#endif

#ifdef USE_DEBUG_SCREEN
        //
        // DE - Update the debug screen
        //
        case 'DE':
            UpdateDEScreen();
            return;
#endif

#ifdef USE_MEMORY_SCREEN
        //
        // ME - Update the memory screen
        //
        case 'ME':
            UpdateMEScreen();
            return;
#endif

#ifdef USE_EEPROM_SCREEN
        //
        // EE - Update the memory screen
        //
        case 'EE':
            UpdateEEScreen();
            return;
#endif
        }

#ifdef USE_USER_SCREENS
    if( UpdateUserScreen(SelectedScreen) )
        return;
#endif

    BadScreen(SelectedScreen);
    }
