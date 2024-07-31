/**
 * @file       drv_buzzer.h
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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef USER_DRV_DRV_BUZZER_H_
#define USER_DRV_DRV_BUZZER_H_

/* Includes ----------------------------------------------------------- */
#include "bsp_gpio.h"
#include "bsp_timer.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  DRV_BUZZER_FAIL = 0xFFFFFFFF,
  DRV_BUZZER_ERROR = 0x7FFFFFFF,
  DRV_BUZZER_OK = 0x3FFFFFFF
} drv_buzzer_status_t;

typedef struct __attribute__((__packed__))
{
  TIM_HandleTypeDef *htim;
  uint32_t pwm_channel;
  uint32_t prescaler;
  uint16_t period;
  uint16_t duty_cycle;
} drv_buzzer_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */
typedef void (*drv_buzzer_callback)(TIM_HandleTypeDef *);

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Inititialize the buzzer.
 *
 * @param[in]   buzzer                Pointer to buzzer handler.
 * @param[in]   buzzer_htim           Pointer of timer buzzer handler.
 * @param[in]   buzzer_htim_prescaler Prescaler for timer clock source.
 * @param[in]   buzzer_pwm_channel    Channel PWM connect to buzzer.
 * @param[in]   buzzer_period         buzzer blink period.
 * @param[in]   buzzer_duty_cycle     buzzer on-time.
 *
 * @attention   the valid buzzer period and duty cycle.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to initialize buzzer,
 *              0 if initialize buzzer successfully.
 */
drv_buzzer_status_t drv_buzzer_init(drv_buzzer_t *buzzer,
                                    TIM_HandleTypeDef *buzzer_htim,
                                    uint32_t buzzer_htim_prescaler,
                                    uint32_t buzzer_pwm_channel,
                                    uint16_t buzzer_period,
                                    uint16_t buzzer_duty_cycle);

/**
 * @brief       Set the frequency for blinking buzzer.
 *
 * @param[in]   buzzer                Pointer to buzzer handler.
 * @param[in]   buzzer_frequency      Desired frequency of buzzer.
 *
 * @attention   the range of the frequency and valid frequency value,
 *              remember to determine the clock source.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to set frequency,
 *              0 if set frequency successfully.
 */
drv_buzzer_status_t drv_buzzer_set_period(drv_buzzer_t *buzzer, uint16_t buzzer_period);

/**
 * @brief       Set the buzzer on-time.
 *
 * @param[in]   buzzer                Pointer to buzzer handler.
 * @param[in]   buzzer_frequency      Desired frequency of buzzer.
 *
 * @note        Duty cycle equal to buzzer_on_time divided by buzzer_period.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to set duty cycle,
 *              0 if set duty cycle successfully.
 */
drv_buzzer_status_t drv_buzzer_set_duty_cycle(drv_buzzer_t *buzzer, uint16_t buzzer_on_time);

/**
 * @brief       Enable PWM channel connect to the buzzer.
 *
 * @param[in]   buzzer                Pointer to buzzer handler.
 *
 * @note        Should be configured before use this function.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to enable this buzzer,
 *              0 if enable this buzzer successfully.
 */
drv_buzzer_status_t drv_buzzer_enable(drv_buzzer_t *buzzer);

/**
 * @brief       Disable PWM channel connect to the buzzer.
 *
 * @param[in]   buzzer                Pointer to buzzer handler.
 *
 * @note        Should be configuared before use this function.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to disable this buzzer,
 *              0 if disable this buzzer successfully.
 */
drv_buzzer_status_t drv_buzzer_disable(drv_buzzer_t *buzzer);

/**
 * @brief       Register PWM period elapsed callback.
 *
 * @param[in]   pulse_finished_callback Function pointer point to pulse finished callback function.
 * @param[in]   period_elapsed_callback Function pointer point to period elapsed callback function.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to register this callback,
 *              0 if register this callback successfully.
 */
drv_buzzer_status_t drv_buzzer_register_callback(drv_buzzer_callback pulse_finished_callback,
                                                 drv_buzzer_callback period_elapsed_callback);

/**
 * @brief       Function to PWM period elapsed callback.
 *
 * @param[in]   htim          Pointer of timer handler.
 *
 * @return      -2 if input parameters error,
 *              -1 if fail to register this callback,
 *              0 if register this callback successfully.
 */
drv_buzzer_status_t drv_buzzer_pulse_finished_handler(TIM_HandleTypeDef *htim);

#endif /* USER_DRV_DRV_BUZZER_H_ */