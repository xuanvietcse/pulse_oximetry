/**
 * @file       <file_name>.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    major.minor.patch
 * @date       yyyy-mm-dd
 * @author     <first_name_1> <last_name_1>
 * @author     <first_name_2> <last_name_2>
 *
 * @brief      <A brief description of the content of the file>
 *
 * @note
 * @example    example_file_1.c
 *             Example_1 description
 * @example    example_file_2.c
 *             Example_2 description
 */

/* Includes ----------------------------------------------------------- */
#include "sys_manage.h"
#include "common.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static sys_measure_t ppg_signal;
static sys_display_t oled_screen;
static drv_buzzer_t passive_buzzer;
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
uint32_t sys_manage_start_display(bsp_i2c_handle_t *i2c, uint8_t *dev_buffer)
{
  uint32_t ret = SYS_DISPLAY_OK;
  ret = sys_display_init(&oled_screen, i2c, dev_buffer);
  __ASSERT(ret == SYS_DISPLAY_OK, SYS_MANAGE_ERROR);

  return SYS_MANAGE_OK;
}

uint32_t sys_manage_start_measure(bsp_adc_typedef_t *adc,
                                  bsp_tim_typedef_t *tim,
                                  uint32_t prescaler,
                                  uint32_t autoreload,
                                  double *data_buf)
{
  uint32_t ret = SYS_MEASURE_OK;
  ret = sys_measure_init(&ppg_signal, adc, tim, prescaler, autoreload, data_buf);
  __ASSERT(ret == SYS_MEASURE_OK, SYS_MANAGE_FAILED);

  return SYS_MANAGE_OK;
}

uint32_t sys_manage_start_button(GPIO_TypeDef *gpio, uint16_t pin, uint32_t button_active_level)
{
  uint32_t ret = SYS_BUTTON_OK;
  ret = sys_button_init(gpio, pin, button_active_level);
  __ASSERT(ret == SYS_BUTTON_OK, SYS_MANAGE_ERROR);

  return SYS_MANAGE_OK;
}

uint32_t sys_manage_start_protocol(UART_HandleTypeDef *uart)
{
  uint32_t ret = SYS_PROTOCOL_OK;
  ret = sys_protocol_init(uart);
  __ASSERT(ret == SYS_PROTOCOL_OK, SYS_MANAGE_ERROR);

  return SYS_MANAGE_OK;
}

uint32_t sys_manage_start_buzzer(bsp_tim_typedef_t *tim, uint32_t pwm_channel)
{
  uint32_t ret = DRV_BUZZER_OK;
  ret = drv_buzzer_init(&passive_buzzer, tim, pwm_channel);
  __ASSERT(ret == DRV_BUTTON_OK, SYS_MANAGE_ERROR);

  return SYS_MANAGE_OK;
}

/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
