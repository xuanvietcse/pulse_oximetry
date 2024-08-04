/**
 * @file       sys_display.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    0.0.1
 * @date       2024-08-03
 * @author     Viet Hoang Xuan
 *             Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *
 * @brief      <A brief description of the content of the file>
 *
 * @note
 * @example
 * @example
 */

/* Includes ----------------------------------------------------------- */
#include "sys_display.h"
#include "common.h"
#include "heart_icon.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static drv_ssd1306_t ssd1306;
static uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
uint32_t sys_display_init(sys_display_t *display, bsp_i2c_handle_t *i2c)
{
  // Check parameters
  __ASSERT((display != NULL), SYS_DISPLAY_OK);
  __ASSERT((i2c != NULL), SYS_DISPLAY_OK);
  // Initialize UI
  drv_ssd1306_init(&ssd1306,
                   i2c,
                   SSD1306_I2C_ADDRESS,
                   ssd1306_buffer,
                   SSD1306_WIDTH,
                   SSD1306_HEIGHT);
  drv_ssd1306_write_string(&ssd1306, "Con cac", Font_6x8, DRV_SSD1306_COLOR_WHITE);
  // drv_ssd1306_draw_bitmap(&ssd1306, 121, 0, heart_icon, 9, 9, DRV_SSD1306_COLOR_WHITE);
  // drv_ssd1306_draw_rectangle(&ssd1306, 0, 9, 127, 54, DRV_SSD1306_COLOR_WHITE);
  // drv_ssd1306_set_cursor(&ssd1306, 37, 57);
  // drv_ssd1306_write_string(&ssd1306, "Notifications", Font_6x8, DRV_SSD1306_COLOR_WHITE);
  drv_ssd1306_update_screen(&ssd1306);
  // Return
  return SYS_DISPLAY_OK;
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
