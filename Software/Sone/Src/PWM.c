//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Freq.c
//
//  DESCRIPTION
//
//      Frequency processing
//
//      Setup a timer as a real-time frequency counter
//
//      See Freq.h for an in-depth description
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

#include <PWM.h>
#include <TimerMacros.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static struct {
    uint16_t    CaptLow;                            // Timer at low  transition
    uint16_t    CaptHigh;                           // Timer at high transition
    uint16_t    FreqTotal;                          // Total     ticks in counted cycles
    uint16_t    FreqCycles;                         // Number of cycles in totals
    uint16_t    PWMTotal;                           // Total PWM ticks in counted cycles
    uint16_t    PWM;                                // Calculated PWM  value
    uint16_t    Freq;                               // Calculated Freq value
    int8_t      SkipCount;                          // Cycles until next measurement
    bool        FreqEnd;                            // TRUE if next meas is end of FREQ
    } PWM NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#undef TIMER_ID                                     // Prevents typos in following

#define PWM_TIMER_ID    1
#define PRTIMx          _PRTIM(PWM_TIMER_ID)
#define PWM_ISR         _TCAPT_VECT(PWM_TIMER_ID)

#define TCCRAx          _TCCRA(PWM_TIMER_ID)
#define TCCRBx          _TCCRB(PWM_TIMER_ID)
#define TIMSKx          _TIMSK(PWM_TIMER_ID)
#define TCNTx           _TCNT(PWM_TIMER_ID)
#define ICIEx           _ICIE(PWM_TIMER_ID)
#define ICRx            _ICR(PWM_TIMER_ID)

#define PWM_FILTER      _PIN_MASK(_ICNC(PWM_TIMER_ID))
#define PWM_MODE        _PIN_MASK(_CS0(PWM_TIMER_ID))

#define RISING_EDGE     _ICES(PWM_TIMER_ID)

#define DISABLE_INT     _CLR_BIT(TIMSKx,ICIEx)
#define ENABLE_INT      _SET_BIT(TIMSKx,ICIEx)

#define SKIP_MAX        10

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PWMInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void PWMInit(void) {

    memset(&PWM,0,sizeof(PWM));

    PWM.SkipCount = SKIP_MAX;

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running
    //
    TCCRAx = 0;                     // Normal counter
    TCCRBx = PWM_MODE;
    TCNTx  = 0;

    ENABLE_INT;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PWMUpdate - Update PWM measurements
//
// Inputs:      None.
//
// Outputs:     None.
//
void PWMUpdate(void) {
    uint16_t    FreqLocal;
    uint16_t    PWMLocal;
    uint16_t    CyclesLocal;

    DISABLE_INT;

    FreqLocal   = PWM.FreqTotal;
    PWMLocal    = PWM.PWMTotal;
    CyclesLocal = PWM.FreqCycles;

    PWM.FreqTotal  = 0;
    PWM.PWMTotal   = 0;
    PWM.FreqCycles = 0;

    ENABLE_INT;

    //
    // If system is off, no frequency or PWM will be seen from last update
    //
    if( FreqLocal == 0 ) {
        PWM.PWM  = 0;
        PWM.Freq = 0;
        }
    else {
        PWM.PWM  = ((uint32_t) PWMLocal*1000)/((uint32_t) FreqLocal);
        PWM.Freq = ((uint32_t) F_CPU*CyclesLocal)/((uint32_t) FreqLocal);
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetPWM - Return currently measured PWM
//
// Inputs:      None.
//
// Outputs:     Measured PWM, in % x 10 (ex: 35% -> 350)
//
uint16_t GetPWM(void) { return PWM.PWM; }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetPWMFreq - Return currently measured frequency
//
// Inputs:      None.
//
// Outputs:     Currently measured frequency
//
uint16_t GetPWMFreq(void) { return PWM.Freq; }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_CAPT_vect - Input capture causes an interrupt
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER1_CAPT_vect,ISR_NOBLOCK) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Save CPU time by only measuring 1-in-10 cycles
    //
    if( --PWM.SkipCount > 0 )
        return;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // FALLING EDGE
    //
    // Note the ICP of the falling edge. Set next round to interrupt on rising edge.
    //
    if( _BIT_OFF(TCCRBx,RISING_EDGE) ) {
        PWM.CaptLow = ICRx;
        PWM.FreqEnd = false;
        _SET_BIT(TCCRBx,RISING_EDGE);
        return;
        }

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // 1ST RISING EDGE
    //
    // Note the ICP of the rising edge. Set next round to interrupt on rising edge.
    //
    if( !PWM.FreqEnd ) {
        PWM.CaptHigh = ICRx;
        PWM.FreqEnd  = true;
        return;
        }

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // 2ND RISING EDGE
    //
    // Total the PWM and FREQ counts
    //
    uint16_t    FreqCount = ICRx - PWM.CaptHigh;

    PWM.FreqTotal += FreqCount;
    PWM.PWMTotal  += FreqCount - (PWM.CaptHigh - PWM.CaptLow);
    PWM.FreqCycles++;
    PWM.SkipCount = SKIP_MAX;

    _CLR_BIT(TCCRBx,RISING_EDGE);
    }
