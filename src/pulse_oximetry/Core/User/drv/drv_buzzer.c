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

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
uint32_t drv_buzzer_init(drv_buzzer_t *buzzer,
                         bsp_tim_typedef_t *tim,
                         uint32_t prescaler,
                         uint32_t pwm_channel,
                         uint16_t period,
                         uint16_t duty_cycle)
{
  return DRV_BUZZER_OK;
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
