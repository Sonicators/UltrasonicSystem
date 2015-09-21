//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ACS712.c
//
//  SYNOPSIS
//
//  DESCRIPTION
//
//      Simple AtoD processing
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

#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>

#include "PortMacros.h"
#include "ACS712.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// Full Speed AtoD at 16mHz = 8710 samples per second
//
// With 25 Ticks per second, taking 1 in 10 measurements is about 34 measurements per
//   tick, which gives us 10+5 = 15 bits of resolution.
//
static struct {
    int16_t     Current;                            // Measured current, in Amps*10
    uint16_t    Total;                              // Total AtoD in counted cycles
    uint8_t     Cycles;                             // Number of cycles in total
    uint8_t     SkipCount;                          // Cycles until next measurement
    } ACS712 NOINIT;

#define START_ATOD  { _SET_BIT(ADCSRA,ADSC); }      // Start the AtoD conversion
#define ADMUX_VAL   (_PIN_MASK(REFS0) + ACS712_CHANNEL)

#define DISABLE_INT _CLR_BIT(ADCSRA,_PIN_MASK(ADIE));   // Disable AtoD interrupts
#define ENABLE_INT  _SET_BIT(ADCSRA,_PIN_MASK(ADIE));   // Enable  AtoD interrupts

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ACS712Init - Initialize ACS712 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void ACS712Init(void) {

    memset(&ACS712,0,sizeof(ACS712));

    ACS712.SkipCount = ACS712_SKIP;

    //
    // Setup AtoD channels for input
    //
    _CLR_BIT(PRR,PRADC);                    // Powerup the A/D converter

    DIDR0  = 0;                             // Turn off digital outputs
    ADCSRB = 0;                             // Free running mode
    ADMUX  = ADMUX_VAL;                     // AVCC as ref, number of channels
    ADCSRA = _PIN_MASK(ADPS2) | 
             _PIN_MASK(ADPS1) | 
             _PIN_MASK(ADPS0) |             // Prescale to 150 KHz
             _PIN_MASK(ADEN)  |             // Enable, enable ints
             _PIN_MASK(ADIE);

    //
    // Start the conversions
    //
    START_ATOD;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ACS712GetCurrent - Return ACS712 Current values
//
// Inputs:      None.
//
// Outputs:     ACS712 Current in Amps*10
//
void ACS712Update(void) {

    DISABLE_INT;

    int16_t ACS712Total  = ACS712.Total;
    uint8_t ACS712Cycles = ACS712.Cycles;

    ACS712.Total  = 0;
    ACS712.Cycles = 0;

    ENABLE_INT;

    //
    // The AtoD reading is proportional to 1023 with 

#define MAX_ADC 0x3FF
#define VADC    500             // == 5 volts x 100

    int32_t Voltage = (((int32_t) ACS712Total)*VADC)/(MAX_ADC*ACS712Cycles);

    //
    // In normal  mode, a reading of 512 represents zero and positive current is greater.
    //
    // In reverse mode, 512 represents zero and positive current is lessor
    //
#ifdef ACS712_REVERSE
    ACS712.Current = 250 - Voltage;
#else
    ACS712.Current = Voltage - 250;
#endif
    }



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ACS712GetCurrent - Return ACS712 Current values
//
// Inputs:      None.
//
// Outputs:     ACS712 Current in Amps*10
//
uint16_t ACS712GetCurrent(void) { return ACS712.Current; }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ADC_vect - A/D interrupt processing
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(ADC_vect,ISR_NOBLOCK) {

    //
    // Start the next conversion
    //
    START_ATOD;

    //
    // Save resolution by only measuring 1-in-SKIP_MAX cycles
    //
    if( --ACS712.SkipCount > 0 )
        return;

    ACS712.SkipCount = ACS712_SKIP;

    //
    // Total up the count and return
    //
    ACS712.Cycles++;
    ACS712.Total += ADC;
    }
