/**
 * @file       drv_buzzer.C
 * @copyright
 * @license
 * @version    1.0.0
 * @date       31/07/2024
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      Driver for interfacing with the buzzer
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "drv_buzzer.h"
#include "common.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
static drv_buzzer_callback drv_buzzer_callback_pulse_finished = NULL;
static drv_buzzer_callback drv_buzzer_callback_period_elapsed = NULL;

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
drv_buzzer_status_t drv_buzzer_init(drv_buzzer_t *buzzer,
                                    TIM_HandleTypeDef *buzzer_htim,
                                    uint32_t buzzer_htim_prescaler,
                                    uint32_t buzzer_pwm_channel,
                                    uint16_t buzzer_period,
                                    uint16_t buzzer_duty_cycle)
{
  __ASSERT(buzzer != NULL, DRV_BUZZER_ERROR);
  __ASSERT(buzzer_htim != NULL, DRV_BUZZER_ERROR);

  buzzer->htim = buzzer_htim;
  buzzer->pwm_channel = buzzer_pwm_channel;
  buzzer->prescaler = buzzer_htim_prescaler;
  buzzer->period = buzzer_period;
  buzzer->duty_cycle = buzzer_duty_cycle;

  bsp_timer_status_t ret = BSP_TIMER_OK;

  ret = bsp_timer_set_prescaler(buzzer_htim, buzzer_htim_prescaler);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  ret = bsp_timer_set_autoreload(buzzer_htim, buzzer_period);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  ret = bsp_timer_set_output_compare(buzzer_htim, buzzer_pwm_channel, buzzer_duty_cycle);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  return DRV_BUZZER_OK;
}

drv_buzzer_status_t drv_buzzer_set_period(drv_buzzer_t *buzzer, uint16_t buzzer_period)
{
  __ASSERT(buzzer != NULL, DRV_BUZZER_ERROR);

  bsp_timer_status_t ret = BSP_TIMER_OK;

  ret = bsp_timer_set_autoreload(buzzer->htim, buzzer_period);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  return DRV_BUZZER_OK;
}

drv_buzzer_status_t drv_buzzer_set_duty_cycle(drv_buzzer_t *buzzer, uint16_t buzzer_on_time)
{
  __ASSERT(buzzer != NULL, DRV_BUZZER_ERROR);

  bsp_timer_status_t ret = BSP_TIMER_OK;

  ret = bsp_timer_set_output_compare(buzzer->htim, buzzer->pwm_channel, buzzer_on_time);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  return DRV_BUZZER_OK;
}

drv_buzzer_status_t drv_buzzer_enable(drv_buzzer_t *buzzer)
{
  __ASSERT(buzzer != NULL, DRV_BUZZER_ERROR);

  bsp_timer_status_t ret = BSP_TIMER_OK;

  ret = bsp_pwm_start_it(buzzer->htim, buzzer->pwm_channel);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  return DRV_BUZZER_OK;
}

drv_buzzer_status_t drv_buzzer_disable(drv_buzzer_t *buzzer)
{
  __ASSERT(buzzer != NULL, DRV_BUZZER_ERROR);

  bsp_timer_status_t ret = BSP_TIMER_OK;

  ret = bsp_pwm_stop_it(buzzer->htim, buzzer->pwm_channel);
  __ASSERT(ret == BSP_TIMER_OK, DRV_BUZZER_FAIL);

  return DRV_BUZZER_OK;
}

drv_buzzer_status_t drv_buzzer_register_callback(drv_buzzer_callback pulse_finished_callback,
                                                 drv_buzzer_callback period_elapsed_callback)
{
  drv_buzzer_callback_pulse_finished = pulse_finished_callback;
  drv_buzzer_callback_period_elapsed = period_elapsed_callback;

  return DRV_BUZZER_OK;
}

drv_buzzer_status_t drv_buzzer_pulse_finished_handler(TIM_HandleTypeDef *htim)
{
  __ASSERT(htim != NULL, DRV_BUZZER_ERROR);

  __CALLBACK(drv_buzzer_callback_pulse_finished, htim);

  return DRV_BUZZER_OK;
}

/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
