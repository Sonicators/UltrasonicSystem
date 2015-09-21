//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Command.c
//
//  SYNOPSIS
//
//      ...MAScreen.c
//      ...HEScreen.c
//      (Other screens, as needed)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      TimerInit();
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
//          // Process serial commands, if any
//          //
//          ProcessSerialInput(GetUARTByte());
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
#include <string.h>
#include <ctype.h>

#include "PortMacros.h"
#include "VT100.h"

#include "Command.h"
#include "Screen.h"
#include "Parse.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define BEEP    "\007"

//
// Backspace and ESC character
//
#define BACKSPACE   '\b'

//
// IsDelimiter() macro
//
#define IsDelimiter(__char__)   strchr(DELIMITERS,__char__)

static char     CommandBuffer[MAX_CMD_LENGTH+1] NOINIT;
static uint8_t  nChars                          NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// StrEQ - Compare two strings without regard to case
//
// Inputs:      Char string to check
//              Char string to check against
//
// Outputs:     TRUE  if strings are equal
//              FALSE otherwise
//
bool StrEQ(const char *String1, const char *String2) {

    while( *String1 != 0 ) {
        char    c1 = *String1++;
        char    c2 = *String2++;

        if( c1 == ' ' )
            return(true);

        if( isalpha(c1) ) c1 = toupper(c1);
        if( isalpha(c2) ) c2 = toupper(c2);

        if( c1 != c2 ) 
            return(false);
        }

    //
    // Note that if strlen(String2) < strlen(String1), the NUL at the end of String2
    //   will cause a preemptive return here before we access off the end of the string.
    //
    if( *String2 != 0 ) 
        return(false);

    return(true);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// InitCommandBuffer - Initialize command buffer for next command
//
// Inputs:      None.
//
// Outputs:     None.
//
static void InitCommandBuffer() {

    memset(CommandBuffer,0,sizeof(CommandBuffer));
    nChars = 0;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CommandInit - Command initialization
//
// Inputs:      None.
//
// Outputs:     None.
//
void CommandInit(void) {

    InitCommandBuffer();
    Prompt();
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Command - Process text command
//
// Inputs:      Line command to process
//
// Outputs:     None.
//
void Command(char *Buffer) {

    ParseInit(Buffer);

    char *Command = ParseToken();

#ifdef USE_MAIN_SCREEN
    //
    // MA - Show the main screen
    //
    if( StrEQ(Command,"MA") ) {
        ShowScreen('MA');
        return;
        }

#ifdef USE_MAIN_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'MA' ) {
        if( MAScreenCommand(Command) )
            return;
        }

    //
    // Universally accept <ESC> as an escape to the main screen.
    //
    if( StrEQ(Command,ESC_CMD) ) {
        ShowScreen('MA');
        return;
        }
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_HELP_SCREEN
    //
    // HE - Help screen
    //
    if( StrEQ(Command,"HE") ||
        StrEQ(Command,"?" ) ) {
        ShowScreen('HE');
        return;
        }

#ifdef USE_HELP_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'HE' ) {
        if( HEScreenCommand(Command) )
            return;
        }
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DEBUG_SCREEN
    //
    // DE - Show the debug screen
    //
    if( StrEQ(Command,"DE") ) {
        ShowScreen('DE');
        return;
        }

#ifdef USE_DEBUG_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'DE' ) {
        if( DEScreenCommand(Command) )
            return;
        }
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_MEMORY_SCREEN
    //
    // ME - Show the memory screen
    //
    if( StrEQ(Command,"ME") ) {
        ShowScreen('ME');
        return;
        }

#ifdef USE_MEMORY_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'ME' ) {
        if( MEScreenCommand(Command) )
            return;
        }
#endif
#endif

#ifdef USE_EEPROM_SCREEN
    //
    // EE - Show the memory screen
    //
    if( StrEQ(Command,"EE") ) {
        ShowScreen('EE');
        return;
        }

#ifdef USE_EEPROM_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'EE' ) {
        if( EEScreenCommand(Command) )
            return;
        }
#endif
#endif

    //
    // Not a recognized command. Let the user know he goofed.
    //
    CursorPos(1,ERROR_ROW);
    ClearEOL;
    PrintString("Unrecognized command (");
    PrintString(Command);
    PrintString(")\r\n");

    PrintStringP(PSTR(BEEP));
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Prompt - Print out a prompt
//
// Inputs:      None.
//
// Outputs:     None.
//
void Prompt(void) {
    CursorPos(INPUT_COL,INPUT_ROW);
    ClearEOL;
    PrintString(PROMPT);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PlotInput - Put cursor in correct position for input
//
// Inputs:      None.
//
// Outputs:     None.
//
void PlotInput(void) {

    PrintStringP(PSTR("\033["));
    PrintD(INPUT_ROW,0);
    PrintStringP(PSTR(";"));
    PrintD(INPUT_COL+strlen(CommandBuffer)+strlen(PROMPT),0);
    PrintStringP(PSTR("H"));
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PlotError - Print an error message
//
// Inputs:      Message to print
//
// Outputs:     None.
//
void PlotError(char *Error) {

    CursorPos(ERROR_COL,ERROR_ROW);

    PrintString(Error);
    PrintStringP(PSTR(")" BEEP));
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ProcessSerialInput - Parse serial command input chars
//
// Collect chars until a terminator is seen, then pass the input 
//   buffer to SystemCommand().
//
// Inputs:      Serial input char to process
//
// Outputs:     None.
//
void ProcessSerialInput(char InChar) {

    //
    // A short buffer for echoed characters. Since some characters echo as more than one
    //   output char (BS -> BS-SP-BS), we need space for a couple of chars and the
    //   terminating NUL.
    //
    char    TempOutLine[6];

    //
    // Always ignore NUL characters. This can be noise on the serial line, or (more
    //   probably) the caller not bothering to check the return value from the input
    //   routine.
    //
    if( InChar == 0 )
        return;

    //
    // Always accept BS character as "Erase previous character"
    //
    if( InChar == BACKSPACE ) {
        if( nChars != 0 ) {        // If not at start of command
            PlotInput();
            CommandBuffer[--nChars] = 0;
            TempOutLine[0] = BACKSPACE;
            TempOutLine[1] = ' ';
            TempOutLine[2] = BACKSPACE;
            TempOutLine[3] = 0;
            PrintString(TempOutLine);
            }
        return;
        }

    //
    // Setup to echo the characters.
    //
    if( InChar == '\r' ) {              // Switch \r to \r\n
        TempOutLine[0] = '\r';
        TempOutLine[1] = '\n';
        TempOutLine[2] = 0;
        }
    else if( InChar != '\n' ) {         // And print nothing for \n
        TempOutLine[0] = InChar;
        TempOutLine[1] = 0;
        }
    else TempOutLine[0] = 0;

    PlotInput();
    PrintString(TempOutLine);

    //
    // In the lingo of the system, a '\r' indicates EOL
    //
    if( InChar == '\r' || InChar == ESC ) {

        //
        // ESC causes us to clear the command buffer and process ESC
        //   as a command.
        //
        if( InChar == ESC ) 
            strcpy(CommandBuffer,ESC_CMD);

        Command(CommandBuffer);
        InitCommandBuffer();
        Prompt();
        return;
        }
    else if (InChar == '\n')               // Do not add \n characters to
        return;                            // CommandBuffer.


    //
    // Not a TERMINATOR character, must be part of a COMMAND.  Add it to the command
    //   buffer string if there is room.
    //
    if( nChars < MAX_CMD_LENGTH )
        CommandBuffer[nChars++] = InChar;
    }

