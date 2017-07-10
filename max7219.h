/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef MAX7219_H__
#define MAX7219_H__

/**************************************************************************************************
 *  1.  INCLUDE FILES                                                                             *
 **************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/**************************************************************************************************
 *  2.  PUBLIC DEFINES                                                                            *
 **************************************************************************************************/
#define MAX7219_PIXEL_POWER_ON    1
#define MAX7219_PIXEL_POWER_OFF   0

#define MAX7219_BRIGHTNESS_LOW    0x01U
#define MAX7219_BRIGHTNESS_MID    0x07U
#define MAX7219_BRIGHTNESS_HIG    0x0FU

/**************************************************************************************************
 *  3.  PUBLIC MACROS                                                                             *
 **************************************************************************************************/

/**************************************************************************************************
 *  4.  PUBLIC ENUMERATIONS                                                                       *
 **************************************************************************************************/

/**************************************************************************************************
 *  5.  PUBLIC STRUCTURES                                                                         *
 **************************************************************************************************/

/**************************************************************************************************
 *  6.  PUBLIC TYPES                                                                              *
 **************************************************************************************************/

/**************************************************************************************************
 *  7.  EXPORTED (PUBLIC) VARIABLES                                                               *
 **************************************************************************************************/

/**************************************************************************************************
 *  8.  FUNCTION PROTOTYPES                                                                       *
 **************************************************************************************************/
bool MAX7219_Init(uint8_t pin_mosi, uint8_t pin_clock, uint8_t pin_slave_select);
bool MAX7219_Test(void);
bool MAX7219_SetBrightness(const uint8_t brightness);
bool MAX7219_SetPixel(int x, int y, const uint8_t value);
bool MAX7219_Clear(void);

#endif // MAX7219_H__

/** @} */
