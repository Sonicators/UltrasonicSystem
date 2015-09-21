//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Timer.c
//
//  DESCRIPTION
//
//      Timer processing
//
//      Setup a timer as a real time timer, to manage timed events.
//
//      See Timer.h for an in-depth description
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

#include <avr\io.h>
#include <avr\interrupt.h>

#include <Timer.h>
#include <TimerMacros.h>

static struct {
    TIME_T      Seconds;                            // Seconds since init
    uint16_t    MS;                                 // MS within second
    uint16_t    Countdown;                          // Interrupt count
    bool        Changed;                            // Set TRUE at each tick
    } Timer NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx      _PRTIM(TIMER_ID)
#define TIMER_ISR   _TCOMPA_VECT(TIMER_ID)

#define TCNTx       _TCNT(TIMER_ID)
#define TCCRAx      _TCCRA(TIMER_ID)
#define TCCRBx      _TCCRB(TIMER_ID)
#define TIMSKx      _TIMSK(TIMER_ID)
#define OCRAx       _OCRA(TIMER_ID)
#define OCIEAx      _OCIEA(TIMER_ID)

#define DISABLE_INT _CLR_BIT(TIMSKx,OCIEAx)
#define ENABLE_INT  _SET_BIT(TIMSKx,OCIEAx)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerInit(void) {

    memset(&Timer,0,sizeof(Timer));

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = CTC_MODE;              // No output compare functions
    TCCRBx = CLOCK_BITS;            // Set appropriate clock
    TCNTx  = 0;
    OCRAx  = CLOCK_COUNT-1;         // And clock count for ticks

    Timer.Countdown = TIMER_COUNT;

    ENABLE_INT;                     // Allow interrupts
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerUpdate - Update timer, tell if tick has passed
//
// Inputs:      None.
//
// Outputs:     TRUE  if tick has elapsed since last call
//              FALSE otherwise
//
bool TimerUpdate(void) {

    //
    // Return immediately if tick hasn't expired.
    //
    if( !Timer.Changed )
        return(false);

    //
    // Otherwise, update the timer and inform the caller
    //
    Timer.MS += MS_PER_TICK;

    if( Timer.MS >= 1000 ) {
        Timer.MS -= 1000;
        Timer.Seconds++;
        }

    Timer.Changed = false;

    //
    // Call the user's function
    //
#ifdef CALL_TimerISR
    TimerISR();
#endif

    return true;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerGetSeconds - Return seconds since TimerBInit()
// TimerGetMS      - Get milliseconds since previous second
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T TimerGetSeconds(void) {
    TIME_T Rtnval;

    DISABLE_INT;                    // Disable interrupts
    Rtnval = Timer.Seconds;
    ENABLE_INT;                     // Allow interrupts

    return Rtnval;
    }

uint16_t TimerGetMS(void) {
    TIME_T Rtnval;

    DISABLE_INT;                    // Disable interrupts
    Rtnval = Timer.MS;
    ENABLE_INT;                     // Allow interrupts

    return Rtnval;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPA_vect - Compare A causes a timer tick
//
// This defines the "tick" of the system clock. Increment the global
//   clock counter, call the user's function (if defined) and return.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_ISR,ISR_NOBLOCK) {

    if( --Timer.Countdown != 0 )
        return;

    Timer.Countdown = TIMER_COUNT;
    Timer.Changed   = true;
    }
