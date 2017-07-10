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

/**************************************************************************************************
 *  1. PRIVATE INCLUDE FILES                                                                      *
 **************************************************************************************************/
#include "max7219.h"

#include "nrf_delay.h"
#include "nrf_drv_spi.h"
#include "nrf_log.h"

/**************************************************************************************************
 *  2. PRIVATE DEFINES                                                                            *
 **************************************************************************************************/
#define MAX7219_ROW_COUNT        8UL
#define MAX7219_COL_COUNT        8UL

#define MAX7219_REG_NOOP         0x00
#define MAX7219_REG_DIGIT0       0x01
#define MAX7219_REG_DIGIT1       0x02
#define MAX7219_REG_DIGIT2       0x03
#define MAX7219_REG_DIGIT3       0x04
#define MAX7219_REG_DIGIT4       0x05
#define MAX7219_REG_DIGIT5       0x06
#define MAX7219_REG_DIGIT6       0x07
#define MAX7219_REG_DIGIT7       0x08
#define MAX7219_REG_DECODEMODE   0x09
#define MAX7219_REG_INTENSITY    0x0A
#define MAX7219_REG_SCANLIMIT    0x0B
#define MAX7219_REG_SHUTDOWN     0x0C
#define MAX7219_REG_DISPLAYTEST  0x0F

/**************************************************************************************************
 *  3. PRIVATE MACROS                                                                             *
 **************************************************************************************************/

/**************************************************************************************************
 *  4. PRIVATE ENUMERATIONS                                                                       *
 **************************************************************************************************/

/**************************************************************************************************
 *  5. PRIVATE STRUCTURES                                                                         *
 **************************************************************************************************/

/**************************************************************************************************
 *  6. PRIVATE TYPES                                                                              *
 **************************************************************************************************/

/**************************************************************************************************
 *  7. PRIVATE (STATIC) FUNCTION PROTOTYPES                                                       *
 **************************************************************************************************/
static uint32_t MAX7219_SendCommand(uint8_t reg_address, const uint8_t data);

/**************************************************************************************************
 *  8. PRIVATE (STATIC) VARIABLES                                                                 *
 **************************************************************************************************/
static const nrf_drv_spi_t m_spi_master_0 = NRF_DRV_SPI_INSTANCE(0);

/**************************************************************************************************
 *  9. PUBLIC VARIABLES                                                                           *
 **************************************************************************************************/

/**************************************************************************************************
 * 10. FUNCTION DEFINITIONS                                                                       *
 **************************************************************************************************/

/**@brief Function for init the MAX7219 driver.
 *
 * @details This function initializes the Nordic SPI driver. Currently the library
 *          is using the NRF_DRV_SPI_INSTANCE(0) SPI instance. The SPI speed is
 *          1 MHz and the bit order is MSB first.
 *
 * @param[in] pin_mosi           MOSI Pin, also known as DIN.
 * @param[in] pin_clock          Clock Pin.
 * @param[in] pin_slave_select   Slave select Pin, also known as circuit enable.
 */
bool MAX7219_Init(uint8_t pin_mosi, uint8_t pin_clock, uint8_t pin_slave_select)
{
    uint32_t err_code;

    nrf_drv_spi_config_t config = NRF_DRV_SPI_DEFAULT_CONFIG;
    config.ss_pin    = pin_slave_select;
    config.mosi_pin  = pin_mosi;
    config.sck_pin   = pin_clock;
    config.frequency = NRF_DRV_SPI_FREQ_1M;
    config.mode      = NRF_DRV_SPI_MODE_0;
    config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;

    /* Init SPI driver */
    err_code = nrf_drv_spi_init(&m_spi_master_0, &config, NULL);

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("[ERROR] MAX7219_Init: cannot init SPI driver\r\n");
        return false;
    }

    /* Let the SPI driver to setup */
    nrf_delay_ms(500);

    /* Init MAX7219 */
    err_code = MAX7219_SendCommand(MAX7219_REG_SCANLIMIT, 7);      // show all 8 digits
    err_code |= MAX7219_SendCommand(MAX7219_REG_DECODEMODE, 0);    // use matrix (not digits)
    err_code |= MAX7219_SendCommand(MAX7219_REG_DISPLAYTEST, 0);   // no display test
    err_code |= MAX7219_SendCommand(MAX7219_REG_SHUTDOWN, 1);      // not shutdown mode
    // TODO: write function for brightness
    err_code |= MAX7219_SendCommand(MAX7219_REG_INTENSITY, 7);     // set brightness

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("[ERROR] MAX7219_Init: cannot configure MAX7219\r\n");
        return false;
    }

    return MAX7219_Clear();
}

/**@brief Function for testing the MAX7219 driver.
 *
 * @details This function displays a little animatio on the LED matrix.
 *          Firstly, is lighting up cols 0, 2, 4 and 6 from top to bottom at minimum
 *          brightness. Secondly, is lighting up cols 1, 3, 5 and 7 from top to bottom
 *          at maximum brightness. Finally, is lighting up all the LEDs and it
 *          does a fade in, fade out effect.
 */
bool MAX7219_Test(void)
{
    uint32_t err_code = NRF_SUCCESS;

    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_LOW);
    for (uint8_t segment=0; segment<8; segment++)
    {
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT0, (1 << segment));
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT2, (1 << segment));
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT4, (1 << segment));
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT6, (1 << segment));
        nrf_delay_ms(100);
    }
    MAX7219_Clear();

    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_HIG);
    for (uint8_t segment=0; segment<8; segment++)
    {
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT1, (1 << segment));
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT3, (1 << segment));
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT5, (1 << segment));
        err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT7, (1 << segment));
        nrf_delay_ms(100);
    }
    MAX7219_Clear();

    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT0, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT1, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT2, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT3, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT4, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT5, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT6, 0xff);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT7, 0xff);
    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_LOW);
    nrf_delay_ms(200);
    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_MID);
    nrf_delay_ms(200);
    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_HIG);
    nrf_delay_ms(200);
    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_MID);
    nrf_delay_ms(200);
    MAX7219_SetBrightness(MAX7219_BRIGHTNESS_LOW);
    nrf_delay_ms(200);
    MAX7219_Clear();

    if (err_code != NRF_SUCCESS)
    {
        return false;
    }
    return true;
}

/**@brief Function for setting a pixel value.
 *
 * @details This function sets the value of a single LED in the 8x8 LEDs matrix.
 *
 * @param[in] x      Pixel X coordinate (it corresponds to the column index).
 * @param[in] y      Pixel y coordinate (it corresponds to the row index).
 * @param[in] value  One of the following MAX7219_PIXEL_POWER_ON, MAX7219_PIXEL_POWER_OFF.
 */
bool MAX7219_SetPixel(int x, int y, const uint8_t value)
{
    uint32_t err_code;

    if (x < 0 || y < 0 || x >= MAX7219_COL_COUNT || y >= MAX7219_ROW_COUNT)
    {
        NRF_LOG_ERROR("[ERROR] MAX7219_SetPixel: pixel coordinate (%d,%d) out of range\r\n", x, y);
        return false;
    }

    uint8_t reg_add;
    uint8_t reg_val = (value << y);

    switch(x)
    {
    case 0:
        reg_add = MAX7219_REG_DIGIT0;
        break;
    case 1:
        reg_add = MAX7219_REG_DIGIT1;
        break;
    case 2:
        reg_add = MAX7219_REG_DIGIT2;
        break;
    case 3:
        reg_add = MAX7219_REG_DIGIT3;
        break;
    case 4:
        reg_add = MAX7219_REG_DIGIT4;
        break;
    case 5:
        reg_add = MAX7219_REG_DIGIT5;
        break;
    case 6:
        reg_add = MAX7219_REG_DIGIT6;
        break;
    case 7:
        reg_add = MAX7219_REG_DIGIT7;
        break;
    default:
        /* It should never happen however, in case it happens, use the digit 0. */
        reg_add = MAX7219_REG_DIGIT0;
        break;
    }

    err_code = MAX7219_SendCommand(reg_add, reg_val);
    if (err_code != NRF_SUCCESS)
    {
        return false;
    }

    return true;
}

/**@brief Function for shutting down all the matrix LEDs.
 *
 * @details This function shuts down al the LEDs.
 */
bool MAX7219_Clear(void)
{
    uint32_t err_code;

    err_code  = MAX7219_SendCommand(MAX7219_REG_DIGIT0, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT1, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT2, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT3, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT4, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT5, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT6, MAX7219_PIXEL_POWER_OFF);
    err_code |= MAX7219_SendCommand(MAX7219_REG_DIGIT7, MAX7219_PIXEL_POWER_OFF);

    if (err_code != NRF_SUCCESS)
    {
        return false;
    }

    return true;
}

/**@brief Function for setting the LEDs brightness.
 *
 * @details This function sets the LEDs brightness.
 *
 * @param[in] brightness  	The brightness to set. Use one of the following values:
 *                          - MAX7219_BRIGHTNESS_LOW,
 *                          - MAX7219_BRIGHTNESS_MID,
 *                          - MAX7219_BRIGHTNESS_HIG.
 */
bool MAX7219_SetBrightness(const uint8_t brightness)
{
    uint32_t err_code;

    err_code  = MAX7219_SendCommand(MAX7219_REG_INTENSITY, brightness);
    if (err_code != NRF_SUCCESS)
    {
        return false;
    }

    return true;
}

/**************************************************************************************************
 * 11. PRIVATE (STATIC) FUNCTION DEFINITIONS                                                      *
 **************************************************************************************************/
static uint32_t MAX7219_SendCommand(uint8_t reg_address, const uint8_t value)
{
    uint8_t tx_buf[] = {
            reg_address,
            value
    };

    return nrf_drv_spi_transfer(&m_spi_master_0, tx_buf, 2, NULL, 0);
}
