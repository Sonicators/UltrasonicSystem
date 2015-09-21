//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TimerMacros.h
//
//  SYNOPSIS
//
//      #define TIMER_ID    0                   // Use TIMER0
//      #define OUT_CH      A                   // Use OCRA
//
//      //
//      // Register definitions
//      //
//      _TCNT(TIMER_ID)       = Some_Val;       // => TCNT0  = Some_Val
//      _TCNTH(TIMER_ID)      = Some_Val;       // => TCNTH0 = Some_Val
//      _TCNTL(TIMER_ID)      = Some_Val;       // => TCNTL0 = Some_Val
//
//      _OCRA(TIMER_ID)       = Some_Val;       // => OCRA0  = Some_Val
//      _OCRAH(TIMER_ID)      = Some_Val;       // => OCRAH0 = Some_Val
//      _OCRAL(TIMER_ID)      = Some_Val;       // => OCRAL0 = Some_Val
//
//      _OCRB(TIMER_ID)       = Some_Val;       // => OCRB0  = Some_Val
//      _OCRBH(TIMER_ID)      = Some_Val;       // => OCRBH0 = Some_Val
//      _OCRBL(TIMER_ID)      = Some_Val;       // => OCRBL0 = Some_Val
//
//      _OCR(TIMER_ID,OUT_CH) = Some_Val        // => OCR0A  = Some_Val
//
//      _ICR(TIMER_ID)        = Some_Val;       // => ICR0   = Some_Val
//      _ICRH(TIMER_ID)       = Some_Val;       // => ICRH0  = Some_Val
//      _ICRL(TIMER_ID)       = Some_Val;       // => ICRL0  = Some_Val
//
//      _TCCRA(TIMER_ID)      = Some_Val;       // => TCCRA0 = Some_Val
//      _TCCRB(TIMER_ID)      = Some_Val;       // => TCCRB0 = Some_Val
//      _TIMSK(TIMER_ID)      = Some_Val;       // => TIMSK0 = Some_Val
//      _TIFR(TIMER_ID)       = Some_Val;       // => TIFR0  = Some_Val
//
//      //
//      // Interrupt vectors
//      //
//      _TCOMPA_VECT(TIMER_ID)           // => TIMER0_COMPA_vect
//      _TCOMPB_VECT(TIMER_ID)           // => TIMER0_COMPB_vect
//      _TOVF_VECT(TIMER_ID)             // => TIMER0_OVF_vect
//      _TCAPT_VECT(TIMER_ID)            // => TIMER0_CAPT_vect
//
//      //
//      // Bits within registers
//      //
//      _PRTIM(TIMER_ID)        // == PRTIMx
//
//      _COMA0(TIMER_ID)        // == COM0A0
//      _COMA1(TIMER_ID)        // == COM0A1
//      _COMB0(TIMER_ID)        // == COM0B0
//      _COMB1(TIMER_ID)        // == COM0B1
//
//      _COM0(TIMER_ID,OUT_CH)  // == COMxc0
//      _COM1(TIMER_ID,OUT_CH)  // == COMxc1
//
//      _WGM0(TIMER_ID)         // == WGM00
//      _WGM1(TIMER_ID)         // == WGM01
//      _WGM2(TIMER_ID)         // == WGM02
//      _WGM3(TIMER_ID)         // == WGM03
//
//      _ICNC(TIMER_ID)         // == ICNC0
//      _ICES(TIMER_ID)         // == ICES0
//
//      _CS0(TIMER_ID)          // == CS00
//      _CS1(TIMER_ID)          // == CS01
//      _CS2(TIMER_ID)          // == CS02
//
//      _FOCA(TIMER_ID)         // == FOC0A
//      _FOCB(TIMER_ID)         // == FOC0B
//
//      _TOIE(TIMER_ID)         // == TOIE0
//      _ICIE(TIMER_ID)         // == ICIE0
//      _OCIEA(TIMER_ID)        // == OCIE0A
//      _OCIEB(TIMER_ID)        // == OCIE0B
//
//      _TOV(TIMER_ID)          // == TOV0
//      _ICF(TIMER_ID)          // == ICF0
//      _OCFA(TIMER_ID)         // == OCF0A
//      _OCFB(TIMER_ID)         // == OCF0B
//
//  DESCRIPTION
//      
//      These macros allow interface code to depend on #included definitions 
//        for timer assignments.
//
//      Instead of writing:
//
//          TCCRA0 = (1 << WGM01);
//
//      Use this:
//
//          #define TIMER_ID   0                    // Use Timer 0
//
//          TCCRA(TIMER_ID) = (1 << WGM1(TIMER_ID));
//
//      Now changing the timer assignment will cause code to automatically
//        adjust.
//
//  NOTE
//
//      In practice, AVR timers have small configuration differences which make it
//        impossible to completely abstract the timer using only one #define.
//
//      For example, a clock speed of F_CPU/1024 is 0x05 in Timer0 and 0x07 in Timer2.
//
//      Despite these differences, it is usually possible to completely abstract the
//        timer using these macros, and one or two application-specific #defined values.
//
//      For an example of this abstraction (using 3 #defines), see Timer.h
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

#ifndef TIMERMACROS_H
#define TIMERMACROS_H

#include <avr/pgmspace.h>

#include <PortMacros.h>

//
// Bits within registers which are timer specific
//
// _PRTIM(_x_)  - PRTIMx in PRR
//
// _COMA0(_x_)  - COMxA0
// _COMA1(_x_)  - COMxA1
// _COMB0(_x_)  - COMxB0
// _COMB1(_x_)  - COMxB1
//
// _COM0(_x_,_c_)  - COMxc1
// _COM1(_x_,_c_)  - COMxc1
//
// _WGM0(_x_)   - WGMx0
// _WGM1(_x_)   - WGMx1
// _WGM2(_x_)   - WGMx2
// _WGM3(_x_)   - WGMx3
//
// _ICNC(_x_)   - ICNCx
// _ICES(_x_)   - ICESx
//
// _CS0(_x_)    - CSx0
// _CS1(_x_)    - CSx1
// _CS2(_x_)    - CSx2
//
// _FOCA(_x_)   - FOCxA
// _FOCB(_x_)   - FOCxB
//
// _OCIEA(_x_)  - OCIExA
// _OCIEB(_x_)  - OCIExB
//
// _TOIE(_x_)   - TOIEx
// _ICIE(_x_)   - ICIEx
// _OCIEA(_x_)  - OCIExA
// _OCIEB(_x_)  - OCIExB
// 
// _TOV(_x_)    - TOVx
// _ICF(_x_)    - ICFx
// _OCFA(_x_)   - OCFxA
// _OCFB(_x_)   - OCFxB
//
// Example:
//
//      #define TIMER_ID    0                   // use TIMER0
//
//      _CLR_BIT(PRR,_PRTIM(TIMER_ID));         // Powerup selected clock
//
#define _PRTIM(_x_)     _JOIN(PRTIM,_x_)        // == PRTIMx

#define _COMA0(_x_)     _JOIN3(COM,_x_,A0)      // == COMxA0
#define _COMA1(_x_)     _JOIN3(COM,_x_,A1)      // == COMxA1
#define _COMB0(_x_)     _JOIN3(COM,_x_,B0)      // == COMxB0
#define _COMB1(_x_)     _JOIN3(COM,_x_,B1)      // == COMxB1

#define _COM0(_x_,_c_)  _JOIN4(COM,_x_,_c_,0)   // == COMxc0
#define _COM1(_x_,_c_)  _JOIN4(COM,_x_,_c_,1)   // == COMxc1

#define _WGM0(_x_)      _JOIN3(WGM,_x_,0)       // == WGMx0
#define _WGM1(_x_)      _JOIN3(WGM,_x_,1)       // == WGMx1
#define _WGM2(_x_)      _JOIN3(WGM,_x_,2)       // == WGMx2
#define _WGM3(_x_)      _JOIN3(WGM,_x_,3)       // == WGMx3

#define _ICNC(_x_)      _JOIN(ICNC,_x_)         // == ICNCx
#define _ICES(_x_)      _JOIN(ICES,_x_)         // == ICESx

#define _CS0(_x_)       _JOIN3(CS,_x_,0)        // == CSx0
#define _CS1(_x_)       _JOIN3(CS,_x_,1)        // == CSx1
#define _CS2(_x_)       _JOIN3(CS,_x_,2)        // == CSx2
#define _CS3(_x_)       _JOIN3(CS,_x_,3)        // == CSx3

#define _FOCA(_x_)      _JOIN3(FOC,_x_,A)       // == FOCxA
#define _FOCB(_x_)      _JOIN3(FOC,_x_,B)       // == FOCxB

#define _TOIE(_x_)      _JOIN(TOIE,_x_)         // == TOIEx
#define _ICIE(_x_)      _JOIN(ICIE,_x_)         // == ICIEx
#define _OCIEA(_x_)     _JOIN3(OCIE,_x_,A)      // == OCIExA
#define _OCIEB(_x_)     _JOIN3(OCIE,_x_,B)      // == OCIExB

#define _TOV(_x_)       _JOIN(TOV,_x_)          // == TOVx
#define _ICF(_x_)       _JOIN(ICF,_x_)          // == ICFx
#define _OCFA(_x_)      _JOIN3(OCF,_x_,A)       // == OCFxA
#define _OCFB(_x_)      _JOIN3(OCF,_x_,B)       // == OCFxB

//
// Timer specific registers
//
// _TCNT(_x_)   - Timer counter
// _TCNTH(_x_)  - Counter high byte
// _TCNTL(_x_)  - Counter low byte
//
// _OCRA(_x_)   - OCRxA
// _OCRAH(_x_)  - OCRxAH
// _OCRAL(_x_)  - OCRxAL
//
// _OCRB(_x_)   - OCRxB
// _OCRBH(_x_)  - OCRxBH
// _OCRBL(_x_)  - OCRxBL
//
// _OCR(_x_,_c_) - OCRcx
//
// _ICR(_x_)    - ICRx
// _ICRH(_x_)   - ICRxH
// _ICRL(_x_)   - ICRxL
//
// _TCCRA(_x_)  - TCCRxA
// _TCCRB(_x_)  - TCCRxB
// _TIMSK(_x_)  - TIMSKx
// _TIFR(_x_)   - TIFRx
//
// Example:
//
//      #define TIMER_ID    0                   // use TIMER0
//
//      _SET_BIT(TCCRA(TIMER_ID),COMA1(TIMER_ID));
//
#define _TCNT(_x_)       _JOIN(TCNT,_x_)        // == TCNTx
#define _TCNTH(_x_)      _JOIN3(TCNT,_x_,H)     // == TCNTxH
#define _TCNTL(_x_)      _JOIN3(TCNT,_x_,L)     // == TCNTxL

#define _OCRA(_x_)       _JOIN3(OCR,_x_,A)      // == OCRxA
#define _OCRAH(_x_)      _JOIN3(OCR,_x_,AH)     // == OCRxAH
#define _OCRAL(_x_)      _JOIN3(OCR,_x_,AL)     // == OCRxAL

#define _OCRB(_x_)       _JOIN3(OCR,_x_,B)      // == OCRxB
#define _OCRBH(_x_)      _JOIN3(OCR,_x_,BH)     // == OCRxBH
#define _OCRBL(_x_)      _JOIN3(OCR,_x_,BL)     // == OCRxBL

#define _OCR(_x_,_c_)    _JOIN3(OCR,_x_,_c_)    // == OCRxc

#define _ICR(_x_)        _JOIN(ICR,_x_)         // == ICRx
#define _ICRH(_x_)       _JOIN3(ICR,_x_,H)      // == ICRxH
#define _ICRL(_x_)       _JOIN3(ICR,_x_,L)      // == ICRxL

#define _TCCRA(_x_)      _JOIN3(TCCR,_x_,A)     // == TCCRxA
#define _TCCRB(_x_)      _JOIN3(TCCR,_x_,B)     // == TCCRxB
#define _TIMSK(_x_)      _JOIN(TIMSK,_x_)       // == TIMSKx
#define _TIFR(_x_)       _JOIN(TIFR,_x_)        // == TIFRx

//
// _TCOMPA_VECT - Generate COMPA ISR vector
// _TCOMPB_VECT - Generate COMPB ISR vector
// _TOVF_VECT   - Generate OVF   ISR vector
// _TCAPT_VECT  - Generate CAPT  ISR vector
//
// Example:
//
//      #define TIMER_ID    0                   // use TIMER0
//
//      ISR(_TOVF_VECT(_x_)) { ... }            // == TIMER0_OVF_vect
//
#define _TCOMPA_VECT(_x_)    _JOIN3(TIMER,_x_,_COMPA_vect)
#define _TCOMPB_VECT(_x_)    _JOIN3(TIMER,_x_,_COMPB_vect)
#define _TOVF_VECT(_x_)      _JOIN3(TIMER,_x_,_OVF_vect)
#define _TCAPT_VECT(_x_)     _JOIN3(TIMER,_x_,_CAPT_vect)

#endif  // TIMERMACROS_H - whole file
