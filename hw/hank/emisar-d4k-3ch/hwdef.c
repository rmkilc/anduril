// Emisar D4K 3-channel hwdef
// Copyright (C) 2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "fsm/chan-rgbaux.c"

void set_level_zero();

void set_level_main2(uint8_t level);
void set_level_led3(uint8_t level);
void set_level_led4(uint8_t level);

bool gradual_tick_main2(uint8_t gt);
bool gradual_tick_led3(uint8_t gt);
bool gradual_tick_led4(uint8_t gt);


Channel channels[] = {
    { // main 2 LEDs only
        .set_level    = set_level_main2,
        .gradual_tick = gradual_tick_main2,
        .has_args     = 0
    },
    { // 3rd LED only
        .set_level    = set_level_led3,
        .gradual_tick = gradual_tick_led3,
        .has_args     = 0
    },
    { // 4th LED only
        .set_level    = set_level_led4,
        .gradual_tick = gradual_tick_led4,
        .has_args     = 0
    },
    RGB_AUX_CHANNELS
};

void set_level_zero() {
    // disable timer overflow interrupt
    // (helps improve button press handling from Off state)
    DSM_INTCTRL &= ~DSM_OVF_bm;

    // turn off all LEDs
    MAIN2_ENABLE_PORT &= ~(1 << MAIN2_ENABLE_PIN);
    LED3_ENABLE_PORT  &= ~(1 << LED3_ENABLE_PIN );
    LED4_ENABLE_PORT  &= ~(1 << LED4_ENABLE_PIN );
    main2_dsm_lvl = 0;
    led3_dsm_lvl  = 0;
    led4_dsm_lvl  = 0;
    MAIN2_PWM_LVL = 0;
    LED3_PWM_LVL  = 0;
    LED4_PWM_LVL  = 0;
    PWM_CNT       = 0;
    //PWM_TOP       = PWM_TOP_INIT;
}

// wrap setting the dsm vars, to get a faster response
// (just setting *_dsm_lvl doesn't work well for strobes)
void set_hw_levels(PWM_DATATYPE main2, // brightness, 0 to DSM_TOP
                   PWM_DATATYPE led3,
                   PWM_DATATYPE led4,
                   bool main2_en,      // enable even at PWM=0?
                   bool led3_en,
                   bool led4_en
                   ) {

    // enable/disable LED power channels
    if (main2 | main2_en)
         MAIN2_ENABLE_PORT |=  (1 << MAIN2_ENABLE_PIN);
    else MAIN2_ENABLE_PORT &= ~(1 << MAIN2_ENABLE_PIN);

    if (led3  | led3_en )
         LED3_ENABLE_PORT  |=  (1 << LED3_ENABLE_PIN);
    else LED3_ENABLE_PORT  &= ~(1 << LED3_ENABLE_PIN);

    if (led4  | led4_en )
         LED4_ENABLE_PORT  |=  (1 << LED4_ENABLE_PIN);
    else LED4_ENABLE_PORT  &= ~(1 << LED4_ENABLE_PIN);

    // set delta-sigma soft levels
    main2_dsm_lvl = main2;
    led3_dsm_lvl  = led3;
    led4_dsm_lvl  = led4;
    // set hardware PWM levels and init dsm loop
    MAIN2_PWM_LVL = main2_pwm = main2 >> 7;
    LED3_PWM_LVL  = led3_pwm  = led3  >> 7;
    LED4_PWM_LVL  = led4_pwm  = led4  >> 7;

    // enable timer overflow interrupt so DSM can work
    DSM_INTCTRL |= DSM_OVF_bm;

    // force phase reset
    PWM_CNT       = PWM_CNT2  = 0;
}

// delta-sigma modulation of PWM outputs
// happens on each Timer overflow (every 512 cpu clock cycles)
// uses 8-bit pwm w/ 7-bit dsm (0b 0PPP PPPP PDDD DDDD)
ISR(DSM_vect) {
    // set new hardware values first,
    // for best timing (reduce effect of interrupt jitter)
    MAIN2_PWM_LVL = main2_pwm;
    LED3_PWM_LVL  = led3_pwm;
    LED4_PWM_LVL  = led4_pwm;

    // calculate next values, now that timing matters less

    // accumulate error
    main2_dsm += (main2_dsm_lvl & 0x007f);
    // next PWM = base PWM value + carry bit
    main2_pwm  = (main2_dsm_lvl >> 7) + (main2_dsm > 0x7f);
    // clear carry bit
    main2_dsm &= 0x7f;

    // repeat for other channels

    led3_dsm += (led3_dsm_lvl & 0x007f);
    led3_pwm  = (led3_dsm_lvl >> 7) + (led3_dsm > 0x7f);
    led3_dsm &= 0x7f;

    led4_dsm += (led4_dsm_lvl & 0x007f);
    led4_pwm  = (led4_dsm_lvl >> 7) + (led4_dsm > 0x7f);
    led4_dsm &= 0x7f;
}

// LEDs 1+2 are 8-bit
// this 8-bit channel may be LEDs 1+2 or LED 4, depending on wiring
void set_level_main2(uint8_t level) {
    set_hw_levels(PWM_GET(pwm1_levels, level), 0, 0,
                  1,                           0, 0);
}

// LED 3 is 16-bit
void set_level_led3(uint8_t level) {
    set_hw_levels(0, PWM_GET(pwm1_levels, level), 0,
                  0, 1,                           0);
}

// this 16-bit channel may be LED 4 or LEDs 1+2, depending on wiring
void set_level_led4(uint8_t level) {
    set_hw_levels(0, 0, PWM_GET(pwm1_levels, level),
                  0, 0, 1);
}

///// "gradual tick" functions for smooth thermal regulation /////
// (and other smooth adjustments)

bool gradual_adjust(PWM_DATATYPE main2, PWM_DATATYPE led3, PWM_DATATYPE led4) {
    // adjust multiple times based on current brightness
    // (so it adjusts faster/coarser when bright, slower/finer when dim)

    // higher shift = slower/finer adjustments
    const uint8_t shift = 9;  // ((255 << 7) >> 9) = 63 max
    uint8_t steps;

    steps = main2_dsm_lvl >> shift;
    for (uint8_t i=0; i<=steps; i++)
        GRADUAL_ADJUST_SIMPLE(main2, main2_dsm_lvl);

    steps = led3_dsm_lvl  >> shift;
    for (uint8_t i=0; i<=steps; i++)
        GRADUAL_ADJUST_SIMPLE(led3,  led3_dsm_lvl );

    steps = led4_dsm_lvl  >> shift;
    for (uint8_t i=0; i<=steps; i++)
        GRADUAL_ADJUST_SIMPLE(led4,  led4_dsm_lvl );

    if ((main2 == main2_dsm_lvl)
     && (led3  == led3_dsm_lvl )
     && (led4  == led4_dsm_lvl )) {
        return true;  // done
    }
    return false;  // not done yet
}

bool gradual_tick_main2(uint8_t gt) {
    PWM_DATATYPE main2 = PWM_GET(pwm1_levels, gt);
    return gradual_adjust(main2, 0, 0);
}

bool gradual_tick_led3(uint8_t gt) {
    PWM_DATATYPE led3 = PWM_GET(pwm1_levels, gt);
    return gradual_adjust(0, led3, 0);
}

bool gradual_tick_led4(uint8_t gt) {
    PWM_DATATYPE led4 = PWM_GET(pwm1_levels, gt);
    return gradual_adjust(0, 0, led4);
}

