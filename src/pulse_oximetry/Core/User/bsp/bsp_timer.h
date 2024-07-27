/**
 * @file       bsp_timer.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       10/07/2024
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      APIs of BSP timer
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef USER_BSP_BSP_TIMER_H_
#define USER_BSP_BSP_TIMER_H_

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */
typedef TIM_HandleTypeDef bsp_tim_typedef_t;
/* Public enumerate/structure ----------------------------------------- */
enum bsp_timer_status_t
{
  BSP_TIMER_ERROR = -2,
  BSP_TIMER_FAIL,
  BSP_TIMER_OK
};

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Set the prescaler for clock of timer.
 *
 * @param[in]   htim          Pointer of timer handler.
 * @param[in]   prescale      Desired prescaler .
 *
 * @attention   the range of the prescaler and valid precaler value.
 *
 * @return      -2 if error,
 *              -1 if fail to set prescaler,
 *              0 if set prescler successfully.
 */
uint32_t bsp_timer_set_prescaler(bsp_tim_typedef_t *htim, uint32_t prescaler);

/**
 * @brief       Set the counter period value for clock of timer.
 *
 * @param[in]   htim          Pointer of timer handler.
 * @param[in]   autoreload    Desired counter period.
 *
 * @attention   the range of the counter (16-bit).
 *
 * @return      -2 if error,
 *              -1 if fail to set counter,
 *              0 if set counter successfully.
 */
uint32_t bsp_timer_set_autoreload(bsp_tim_typedef_t *htim, uint32_t autoreload);

/**
 * @brief       Start the PWM generator on a channel of timer.
 *
 * @param[in]   htim          Pointer of timer handler.
 *
 * @attention   timer initialize first.
 *
 * @return      -2 if error,
 *              -1 if fail to start PWM,
 *              0 if start PWM successfully.
 */
uint32_t bsp_timer_start(bsp_tim_typedef_t *htim);

/**
 * @brief       Start the PWM generator on a channel of timer and enable this timer interrupt.
 *
 * @param[in]   htim          Pointer of timer handler.
 *
 * @attention   timer initialize first.
 *
 * @return      -2 if error,
 *              -1 if fail to start PWM,
 *              0 if start PWM successfully.
 */
uint32_t bsp_timer_start_it(bsp_tim_typedef_t *htim);

/**
 * @brief       Stop counter on a channel of timer.
 *
 * @param[in]   htim          Pointer of timer handler.
 *
 * @return      -2 if error,
 *              -1 if fail to stop that channel,
 *              0 if stop that channel successfully.
 */
uint32_t bsp_timer_stop(bsp_tim_typedef_t *htim);

/**
 * @brief       Stop counter on a channel of timer in interrupt mode.
 *
 * @param[in]   htim          Pointer of timer handler.
 *
 * @return      -2 if error,
 *              -1 if fail to stop that channel,
 *              0 if stop that channel successfully.
 */
uint32_t bsp_timer_stop_it(bsp_tim_typedef_t *htim);

#endif /* USER_BSP_BSP_TIMER_H_ */
