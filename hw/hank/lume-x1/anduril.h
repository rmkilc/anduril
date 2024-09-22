// Hank Emisar/Noctigon Lume-X1 config options for Anduril
// Copyright (C) 2018-2024 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// For flashlights using the Loneoceans Lume-X1-40W boost driver (AVR32DD20)
// - Same firmware for 6V, 9V, or 12V configs
// same as loneoceans lume-x1-avr32dd20 but with Hank-specific defaults

#define HWDEF_H hank/lume-x1/hwdef.h
#include "hank/anduril.h"



//***************************************
//**    RAMP TABLE AND OPERATIONS      **
//***************************************

#define RAMP_SIZE 150   // Change to 160 as well as table and values below if desired

// PWM1: DAC Data
// UDR x^4 curves specifically for Lume-X1 (AVR32DD20)

// levels 1 to 5:
//   ./bin/level_calc.py 5.2 1 5 7135 100 0.01 0.1 --pwm 2500
//   (with the last 2 values divided to fit)
// levels 6 to 150:
//   ./bin/dac-scale.py $( ./bin/level_calc.py 5.3 1 145 7135 17 0.2 3000 --pwm 240000 | grep PWM1 | cut -d : -f 2- )
// top level for each "gear": 3 5 / 39 50 / 123 150
#define PWM1_LEVELS \
         100, 359, 790, 588,1023, \
          17,  21,  24,  29,  34,  39,  46,  54,  62,  71,  82,  94, 107, 122, 138, 157, 177, 199, 223, 250, 279, 311, 346, 385, 426, 471, 520, 573, 630, 692, 758, 830, 907, 990, \
         441, 480, 522, 567, 615, 666, 720, 778, 840, 905, 974, \
          26,  28,  30,  33,  35,  37,  40,  43,  46,  49,  52,  56,  59,  63,  67,  71,  76,  80,  85,  90,  96, 101, 107, 113, 120, 126, 133, 141, 148, 156, 165, 173, 182, 192, 202, 212, 222, 234, 245, 257, 269, 282, 296, 310, 324, 339, 355, 371, 387, 405, 422, 441, 460, 480, 501, 522, 544, 567, 590, 614, 640, 665, 692, 720, 748, 778, 808, 839, 872, 905, 939, 975,1011, \
         429, 445, 461, 478, 495, 513, 531, 550, 570, 589, 610, 631, 653, 675, 698, 721, 745, 770, 795, 821, 848, 875, 903, 932, 962, 992,1023
#define PWM2_LEVELS \
         V10, V10, V10, V25, V25, \
         V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
         V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
         V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
         V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25

#define LED_PATH1_PIN_LEVEL_MIN   1
#define LED_PATH2_PIN_LEVEL_MIN   6
#define LED_PATH3_PIN_LEVEL_MIN  51

#define HALFSPEED_LEVEL     20
#define QUARTERSPEED_LEVEL  10

#define DEFAULT_LEVEL  50
#define MAX_1x7135     49

#define RAMP_SMOOTH_FLOOR    1
#define RAMP_SMOOTH_CEIL   130

#define RAMP_DISCRETE_FLOOR   10
#define RAMP_DISCRETE_CEIL   130
#define RAMP_DISCRETE_STEPS    7

#define SIMPLE_UI_FLOOR   10
#define SIMPLE_UI_CEIL   110      // about ~12W, (119 for 160 levels)
#define SIMPLE_UI_STEPS    5

#if 0
// 150 levels:
#define PWM1_LEVELS 1, 2, 10, 31, 77, 159, 295, 504, 807, 504, 737, 1023, 14, 19, 25, 32, 41, 52, 65, 79, 96, 116, 139, 164, 194, 226, 263, 305, 351, 401, 458, 520, 588, 662, 744, 832, 929, 423, 470, 520, 574, 632, 694, 761, 832, 909, 991, 25, 27, 30, 33, 36, 39, 42, 45, 48, 51, 55, 59, 63, 67, 71, 76, 81, 86, 91, 96, 102, 108, 114, 121, 128, 135, 142, 149, 157, 165, 174, 183, 192, 201, 211, 221, 232, 242, 254, 265, 277, 289, 302, 315, 329, 343, 357, 372, 387, 403, 419, 435, 452, 470, 488, 506, 525, 545, 565, 586, 607, 628, 651, 673, 697, 721, 745, 770, 796, 822, 849, 877, 905, 934, 963, 994, 420, 432, 446, 459, 473, 487, 501, 516, 531, 546, 562, 578, 594, 610, 627, 644, 662, 680, 698, 716, 735, 754, 774, 794, 814, 835, 856
// 160 levels:
// #define PWM1_LEVELS 1, 2, 8, 24, 59, 123, 228, 389, 623, 950, 569, 807, 11, 15, 19, 25, 32, 40, 50, 61, 74, 90, 107, 127, 150, 175, 203, 235, 271, 310, 354, 401, 454, 512, 575, 643, 718, 798, 886, 980, 443, 488, 536, 588, 643, 702, 765, 832, 904, 980, 28, 30, 32, 35, 37, 40, 43, 46, 49, 52, 56, 59, 63, 67, 71, 76, 80, 85, 90, 95, 100, 106, 111, 117, 123, 130, 137, 144, 151, 158, 166, 174, 182, 191, 199, 208, 218, 227, 237, 248, 258, 269, 281, 292, 304, 316, 329, 342, 355, 369, 383, 398, 413, 428, 444, 460, 476, 493, 510, 528, 546, 565, 584, 603, 623, 644, 665, 686, 708, 730, 753, 777, 800, 825, 850, 875, 901, 927, 954, 982, 1010, 425, 437, 450, 462, 475, 487, 501, 514, 528, 541, 556, 570, 585, 599, 615, 630, 646, 662, 678, 694, 711, 728, 745, 763, 780, 799, 817, 836, 856

// PWM2_LEVELS - used to set the internal voltage reference for the DAC
//  Makes use the of concept of dynamic Vref for higher resolution output
//  despite DAC being only 10 bits. 
// Bits 2:0 of register ADC0REF set the reference voltage:
//  0x0 - 1.024V - ..000
//  0x1 - 2.048V - ..001
//  0x2 - 4.096V - ..010
//  0x3 - 2.500V-  ..011
//  0x5 - VDD   -  ..101
//  0x6 - VREFA -  ..111
// Bit 7 controls if the reference is always on or not. 
// Hence set PWM2_LEVELS to be 1xxx000 (0d64) for 1.024V DAC vref (V10)
//       set PWM2_LEVELS to be 1xxx011 (0d67) for 2.500V DAC vref (V25)
// Other references are not so important since we have a 10-bit DAC

// 150 levels:
#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25
// 160 levels:
// #define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25

// Enable usage of multiple power paths 
//#define USE_MULTIPLE_POWER_PATH (no longer required, defined in hwdef.c for >2023 Anduril)
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 13
#define LED_PATH3_PIN_LEVEL_MIN 48 // 51 for 160 levels

#define HALFSPEED_LEVEL 50
#define QUARTERSPEED_LEVEL 40   // check with TK about the mechanics of this

#define DEFAULT_LEVEL 59        // about same as 1 AMC7135, (61 for 160 levels)
#define MAX_1x7135 59           // about same as 1 AMC7135 
// MIN_THERM_STEPDOWN defined as MAX_1x7135 in ramp-mode.h if not otherwise defined

#define RAMP_SMOOTH_FLOOR 8
#define RAMP_SMOOTH_CEIL 124    // about ~18-20W (Turbo ~40W), (132 for 160 levels)

#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 6

#define SIMPLE_UI_FLOOR 13
#define SIMPLE_UI_CEIL 112      // about ~12W, (119 for 160 levels)
#define SIMPLE_UI_STEPS 5
#endif

// don't blink mid-ramp
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

//***************************************
//**       THERMAL SETTINGS            **
//***************************************

// stop panicking at 6W (not sure of this numbers yet since it depends on the host..)
#define THERM_FASTER_LEVEL 98          // about 6W
#define MIN_THERM_STEPDOWN 62           // similar to single amc7135 in a 3V light

// extra boost during turbo, lower when temp = temp_ceil + extra
//#define TURBO_TEMP_EXTRA 0

//***************************************
//**          USER INTERFACE           **
//***************************************

// Allow 3C in Simple UI for switching between smooth and stepped ramping
#define USE_SIMPLE_UI_RAMPING_TOGGLE

// double click while on goes to full-power turbo, not ramp ceiling
#define DEFAULT_2C_STYLE 1

#define BLINK_BRIGHTNESS (MAX_LEVEL/5)  // increase blink brightness from max/6
#define BLINK_ONCE_TIME 32              // increase from 10 to make brighter

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

#define USE_BUTTON_LED

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS

// show each channel while it scroll by in the menu
#define USE_CONFIG_COLORS

// blink numbers on the main LEDs by default (but allow user to change it)
#define DEFAULT_BLINK_CHANNEL  CM_MAIN

// use aux red + aux blue for police strobe
#define USE_POLICE_COLOR_STROBE_MODE
#define POLICE_STROBE_USES_AUX
#define POLICE_COLOR_STROBE_CH1        CM_AUXRED
#define POLICE_COLOR_STROBE_CH2        CM_AUXBLU

// the aux LEDs are front-facing, so turn them off while main LEDs are on
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif

#define PARTY_STROBE_ONTIME 4  // slow down party strobe
#define STROBE_OFF_LEVEL 1  // keep the regulator chips on between pulses

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 30 // default 25

// avr32dd20 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// added for convenience
#define USE_SOFT_FACTORY_RESET

// enable long-blink as negative sign
#define USE_LONG_BLINK_FOR_NEGATIVE_SIGN

// Uncomment to use Microphone mode
//#define USE_MICROPHONE_MODE
//#define MICROPHONE_SCALER 10

