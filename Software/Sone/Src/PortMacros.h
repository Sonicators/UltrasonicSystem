//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      PortMacros.h
//
//  SYNOPSIS
//
//      #define IO_PORT    D                    // Use PORTD
//
//      _PORT(IO_PORT) = Some_Val;              // => PORTD = Some_Val
//      _PIN(IO_PORT)  = Some_Val;              // => PIND  = Some_Val
//      _DDR(IO_PORT)  = Some_Val;              // => DRD   = Some_Val
//
//      _SET_MASK(_PORT(IO_PORT),0x30)          // => PORTD |=  0x30
//      _CLR_MASK(_PORT(IO_PORT),0x30);         // => PORTD &= ~0x30
//      _CHG_MASK(_PORT(IO_PORT),0x30)          // => PORTD ^= ~0x30
//
//      #define IO_IPIN    3                    // Input is on pin 3
//
//      _CLR_BIT(_PORT(IO_PORT),IO_IPIN);       // Clear  pin 3 in PORTD
//      _SET_BIT(_PORT(IO_PORT),IO_IPIN)        // Set    pin 3 in PORTD
//      _CHG_BIT(_PORT(IO_PORT),IO_IPIN)        // Toggle pin 3 in PORTD
//
//      _BIT_ON (_PORT(IO_PORT),IO_IPIN)        // TRUE if PORTD.3 is ON
//      _BIT_OFF(_PORT(IO_PORT),IO_IPIN)        // TRUE if PORTD.3 is OFF
//
//      #define IO_INT      0                   // Use INT0
//
//      ISR(_VECT(INT,IO_INT)) { ... }          // => INT0_vect
//
//      #define _MYREG(_x_)  _JOIN(MYREG,_x_)   // Define a local macro
//
//      _MYREG(IO_PORT) = Some_Val;             // => MYREGD = Some_Val
//
//      uint16_t Debug3 NOINIT;                 // Non-initialized memory
//
//  DESCRIPTION
//      
//      These macros allow interface code to depend on #included definitions 
//        for port and pin assignments.
//
//      Instead of writing:
//
//          PORTD &= ~(1 << 3)      // Clear pin 3
//
//      Use this:
//
//          #define IO_PORT    D                    // Use PORTD
//          #define IO_IPIN    3                    // Input is on pin 3
//
//          _CLR_BIT(_PORT(IO_PORT),IO_IPIN);       // Clear  pin 3 in PORTD
//
//      Now changing the pin and port assignments will cause code to automatically
//        adjust.
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

#ifndef PORTMACROS_H
#define PORTMACROS_H

#include <avr/pgmspace.h>

#define _STR(_str_)             #_str_
#define _JOIN(_x_,_y_)          _x_##_y_
#define _JOIN3(_x_,_y_,_z_)     _x_##_y_##_z_
#define _JOIN4(_x_,_y_,_z_,_w_) _x_##_y_##_z_##_w_

//
// _PORT   - Generate PORTx lvalue
// _PIN    - Generate PINx  lvalue
// _DDR    - Generate DDRx  lvalue
//
// Example:
//
//          #define LED_PORT    D       // Use PORTD
//
//          _PORT(LED_PORT) |= 0x10;    // PORTD |= 0x10
//          _PIN(LED_PORT)  |= 0x10;    // ...etc
//          _DDR(LED_PORT)  |= 0x10;
//
#define _PORT(_x_)  _JOIN(PORT,_x_)
#define _PIN(_x_)   _JOIN(PIN,_x_)
#define _DDR(_x_)   _JOIN(DDR,_x_)

//
// _SET_MASK - Turn bits ON  in a register, by mask
// _CLR_MASK - Turn bits OFF in a register, by mask
// _CHG_MASK - Change bits   in a register, by mask
// _GET_MASK - Return bits   in a register, by mask
//
// Example:     _SET_MASK(PORTD,0x30);  // PORTD |= 0x30
//
#define _SET_MASK(_port_,_mask_)   {  (_port_) |=  (_mask_);  }
#define _CLR_MASK(_port_,_mask_)   {  (_port_) &= ~(_mask_);  }
#define _CHG_MASK(_port_,_mask_)   {  (_port_) ^=  (_mask_);  }
#define _GET_MASK(_port_,_mask_)   { ((_port_)  &  (_mask_)); }

//
// _SET_BIT  - Turn numbered bit ON  in a register
// _CLR_BIT  - Turn numbered bit OFF in a register
// _CHG_BIT  - Change numbered bit   in a register
//
// Example:     _BIT_OFF(PORTD,3);      // PORTD &= ~(1 << 3)
//
#define _PIN_MASK(_x_)  (1 << (_x_))

#define _SET_BIT(_port_,_bit_)      { (_port_) |=  (_PIN_MASK(_bit_)); }
#define _CLR_BIT(_port_,_bit_)      { (_port_) &= ~(_PIN_MASK(_bit_)); }
#define _CHG_BIT(_port_,_bit_)      { (_port_) ^=  (_PIN_MASK(_bit_)); }

//
// BIT_ON  - Return TRUE if bit is set in a register
// BIT_OFF - Return TRUE if bit is clr in a register
//
// Example:     if( _BIT_OFF(PORTD,3) ) ...
//
#define _BIT_ON(_port_,_bit_)       ( ((_port_) & _PIN_MASK(_bit_)))
#define _BIT_OFF(_port_,_bit_)      (!((_port_) & _PIN_MASK(_bit_)))

//
// Interrupt vector
//
// Example:
//
//          #define X10_INT     0           // == INT0
//
//          ISR(_VECT(INT,X10_INT)) {...    // => INT0_vect
//
#define _VECT(_vect_,_index_)       _JOIN3(_vect_,_index_,_vect)

//
// NOINIT - Flag for non-initialized memory
//
// Tells the compiler not to initialize the memory at startup (code
//   will do that)
//
// Example:
//
//      uint16_t SomeVar NOINIT;
//
#define NOINIT      __attribute__ ((section (".noinit")))

//
// This fixes minor GCC bug
//
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))

//
// NUMOF - Return number of elements in array
//
// Example:
//
//      for( i=0; i<NUMOF(array); i++ )
//
#define NUMOF(_x_)  (sizeof(_x_)/sizeof((_x_)[0]))

//
// SCALE_RATIO - Sclae a number by a ratio
//
// Example:
//
//      mm = SCALE_RATIO(inches,254,10) => ((Inches+(10/2))*254)/10
//
#define SCALE_RATIO(_x_,_n_,_d_)    (((_x_)*(_n_)+((_d_)/2))/(_d_))

#endif  // PORTMACROS_H - whole file
