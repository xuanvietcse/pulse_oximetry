/**
 * @file       drv_ssd1306.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-28
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      None
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __USER_DRV_SSD1306_H
#define __USER_DRV_SSD1306_H

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
enum drv_ssd1306_status_t
{
  DRV_SSD1306_ERROR = -2,
  DRV_SSD1306_FAILED,
  DRV_SSD1306_OK
};

typedef struct
{
  bsp_i2c_handle_t *i2c;
  uint16_t address;
  uint8_t *buffer;
} drv_ssd1306_t;

enum drv_ssd1306_color_t
{
  DRV_SSD1306_COLOR_BLACK = 0x00, // Turn off all pixel
  DRV_SSD1306_COLOR_WHITE = 0x01  // Turn on all pixel, color depends on device
};
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief         Initialize the SSD1306 OLED screen
 *
 * @param[in]     dev             Pointer to drv_ssd1306_t struct to manage device
 * @param[in]     dev_i2c         Pointer to device I2C_HandleTypeDef
 * @param[in]     dev_address     Device address. Shift left 1 bit before calling this API
 * @param[in]     dev_buffer      Buffer contains the screen data.
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_ssd1306_init(drv_ssd1306_t *dev, bsp_i2c_handle_t *dev_i2c, uint16_t dev_address, uint8_t *dev_buffer);

/**
 * @brief         Initialize the SSD1306 OLED screen
 *
 * @param[in]     dev             Pointer to drv_ssd1306_t struct to manage device
 * @param[in]     color           Colors list in drv_ssd1306_color_t
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_ssd1306_fill_screen(drv_ssd1306_t *dev, uint32_t color);

/**
 * @brief         Initialize the SSD1306 OLED screen
 *
 * @param[in]     dev             Pointer to drv_ssd1306_t struct to manage device
 * @param[in]     pos_x, pos_y    Position of pixel on the device
 * @param[in]     color           Colors list in drv_ssd1306_color_t
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_ssd1306_draw_pixel(drv_ssd1306_t *dev, uint8_t pos_x, uint8_t pos_y, uint32_t color);

#endif // __USER_DRV_SSD1306_H

/* End of file -------------------------------------------------------- */
