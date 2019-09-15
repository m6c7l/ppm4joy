/*
 * Copyright (c) 2017, Manfred Constapel
 * This file is licensed under the terms of the MIT license.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ppm4joy.h"

volatile unsigned char chan = 0xff;
volatile unsigned int time = 0;

#define T_BASE (1 * F_CPU / 1000 / 2)
#define T_MIN  (2 * T_BASE)	   // 1.0 ms
#define T_MAX  (4 * T_BASE)	   // 2.0 ms
#define T_SYN  (6 * T_BASE)	   // 3.0 ms
#define T_VAL  ((T_MAX - T_MIN) / 255)

/*
 * convert input (delta time) to output (8-bit value)
 */
unsigned char conv(unsigned int value) {
    unsigned int t = value;
    if (t < T_MIN)
        t = T_MIN;
    else if (t > T_MAX)
        t = T_MAX;
    t = (t - T_MIN) / T_VAL;
    return (unsigned char) t;           // T_MIN .. T_MAX -> 0 .. 255
}

/*
 * Compare Match A: TCNT1 matches OCR1A
 */
ISR (TIMER1_COMPA_vect) {
    chan = 0xff;                        // sync detected iaw OCR1A, reset channel, no timer reset
}

/*
 * Capture Event: ICP event triggered
 */
ISR (TIMER1_CAPT_vect) {
    unsigned int t = ICR1;              // unsigned 16-bit value of ICR1
    OCR1A = t + T_SYN;                  // put new value in OCR1A to compare with TCNT1 (overflowable)
    t = t - time;                       // time passed by since last event (overflowable)
    time = ICR1;                        // remember time for next event
    if (chan < CHANNELS) {              // starting at 0xff = 255
        ppm[chan] = conv(t);            // store value
    }
    chan++;                             // next channel on next event
}

/*
 * Initialize timer
 */
void initialize() {
    TCCR1B |= (1 << CS10);              // timer clock is CPU clock (no prescaling)
    TCCR1B |= (1 << ICNC1);             // enable input noise canceler (four osc cycles for one sample)
    TCCR1B |= (1 << ICES1);             // set capture edge select to "rising edge"
    TIMSK |= (1 << ICIE1);              // input capture interrupt is enabled // @suppress("Symbol is not resolved")
    TIMSK |= (1 << OCIE1A);             // output compare A match interrupt enable
}
