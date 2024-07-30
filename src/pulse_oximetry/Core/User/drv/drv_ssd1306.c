/**
 * @file       drv_ssd1306.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v0.0.1
 * @date       2024-07-30
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
#include "drv_ssd1306.h"
#include "common.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

uint32_t drv_ssd1306_write_command(drv_ssd1306_t *dev, uint8_t command);
/* Function definitions ----------------------------------------------- */

uint32_t drv_ssd1306_init(drv_ssd1306_t *dev,
                          bsp_i2c_handle_t *dev_i2c,
                          uint16_t dev_address,
                          uint8_t *dev_buffer,
                          uint8_t dev_width,
                          uint8_t dev_height)
{
  // Check parameters
  __ASSERT((dev != NULL), DRV_SSD1306_ERROR);
  __ASSERT((dev_i2c != NULL), DRV_SSD1306_ERROR);
  __ASSERT((dev_address < 256), DRV_SSD1306_ERROR);
  __ASSERT((dev_buffer != NULL), DRV_SSD1306_ERROR);
  __ASSERT((dev_width <= 128), DRV_SSD1306_ERROR);
  __ASSERT((dev_height <= 64), DRV_SSD1306_ERROR);
  // Pass values
  dev->i2c = dev_i2c;
  dev->buffer = dev_buffer;
  dev->size.width = dev_width;
  dev->size.height = dev_height;
  // Init sequence
  drv_ssd1306_set_display(dev, DRV_SSD1306_DISPLAY_OFF);
  // Set display clock divide ratio/oscillator frequency
  drv_ssd1306_write_command(dev, 0xD5);
  drv_ssd1306_write_command(dev, 0x80);
  // Set multiplex ratio
  drv_ssd1306_write_command(dev, 0xA8);
  drv_ssd1306_write_command(dev, 0x3F);
  // Set display offset
  drv_ssd1306_write_command(dev, 0xD3);
  drv_ssd1306_write_command(dev, 0x00);
  // Set display start line
  drv_ssd1306_write_command(dev, 0x40);
  // Set charge pump
  drv_ssd1306_write_command(dev, 0x8D);
  drv_ssd1306_write_command(dev, 0x14);
  // Set segment remap
  drv_ssd1306_write_command(dev, 0xA1);
  // Set COM output scan direction
  drv_ssd1306_write_command(dev, 0xC8);
  // Set COM pins hardware configuration
  drv_ssd1306_write_command(dev, 0xDA);
  drv_ssd1306_write_command(dev, 0x12);
  // Set contrast control
  drv_ssd1306_write_command(dev, 0x81);
  drv_ssd1306_write_command(dev, 0xCF);
  // Set pre-charge period
  drv_ssd1306_write_command(dev, 0xD9);
  drv_ssd1306_write_command(dev, 0xF1);
  // Set VCOMH deselect level
  drv_ssd1306_write_command(dev, 0xDB);
  drv_ssd1306_write_command(dev, 0x40);
  // Set entire display on/off
  drv_ssd1306_write_command(dev, 0xA4);
  // Set normal/inverse display
  drv_ssd1306_write_command(dev, 0xA6);
  // Clear display
  drv_ssd1306_fill_screen(dev, DRV_SSD1306_COLOR_BLACK);
  // Set display ON
  drv_ssd1306_set_display(dev, DRV_SSD1306_DISPLAY_ON);
  // Return
  return DRV_SSD1306_OK;
}

uint32_t drv_ssd1306_set_display(drv_ssd1306_t *dev,
                                 drv_ssd1306_display_t state)
{
  uint8_t value = 0x00;
  switch (state)
  {
  case DRV_SSD1306_DISPLAY_ON:
    value = 0xAF;
    break;
  case DRV_SSD1306_DISPLAY_OFF:
    value = 0xAE;
    break;
  default:
    // Do nothing
    break;
  }
  drv_ssd1306_write_command(dev, value);
  return DRV_SSD1306_OK;
}
/* Private definitions ----------------------------------------------- */

uint32_t drv_ssd1306_write_command(drv_ssd1306_t *dev, uint8_t command)
{
  uint32_t ret = DRV_SSD1306_OK;
  ret = bsp_i2c_mem_write(dev->i2c,
                          (dev->address) << 1,
                          0x00,
                          1,
                          &command,
                          1,
                          HAL_MAX_DELAY);
  __ASSERT((ret == DRV_SSD1306_OK), DRV_SSD1306_FAILED);
  return DRV_SSD1306_OK;
}
/* End of file -------------------------------------------------------- */
