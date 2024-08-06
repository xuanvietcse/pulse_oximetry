/**
 * @file       sys_manage.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-08-06
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      Service manage all tasks of system
 *
 * @note
 * @example    None
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
static drv_ds1307_t ds1307;
/* Private function prototypes ---------------------------------------- */

/**
 * @brief       Sleep down the system service.
 *
 * @return
 * -  None
 */
static void sys_manage_sleep();

/**
 * @brief       Wake up the system service.
 *
 * @return
 * -  None
 */
static void sys_manage_wakeup();

/**
 * @brief       Record the current heart rate.
 *
 * @return
 * -  None
 */
static void sys_manage_record_heart_rate();
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
  ret = sys_button_register_cb_function(sys_manage_wakeup,
                                        sys_manage_sleep,
                                        sys_manage_record_heart_rate);
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

uint32_t sys_manage_start_rtc(bsp_i2c_handle_t *i2c)
{
  uint32_t ret = SYS_TIME_OK;
  ret = sys_time_init(i2c, &ds1307);
  __ASSERT(ret == SYS_TIME_OK, SYS_MANAGE_ERROR);

  return SYS_MANAGE_OK;
}

uint32_t sys_manage_start_buzzer(bsp_tim_typedef_t *tim, uint32_t pwm_channel)
{
  uint32_t ret = DRV_BUZZER_OK;
  ret = drv_buzzer_init(&passive_buzzer, tim, pwm_channel);
  __ASSERT(ret == DRV_BUTTON_OK, SYS_MANAGE_ERROR);

  return SYS_MANAGE_OK;
}

uint32_t sys_manage_loop()
{
  sys_button_manage();
  sys_measure_process_data(&ppg_signal);
  sys_display_update_heart_rate(&oled_screen, ppg_signal.heart_rate);
  sys_display_update_ppg_signal(&oled_screen, &(ppg_signal.filtered_data));
}
/* Private definitions ------------------------------------------------ */
static void sys_manage_sleep()
{
  // Do something stuffs
  drv_hr_sleep(&(ppg_signal.dev));
}

static void sys_manage_wakeup()
{
  // Do something stuffs
  drv_hr_wakeup(&(ppg_signal.dev));
}

static void sys_manage_record_heart_rate()
{
  // Waiting for Khanh
}
/* End of file -------------------------------------------------------- */
