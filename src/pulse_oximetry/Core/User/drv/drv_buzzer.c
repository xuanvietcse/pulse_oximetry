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
                         uint32_t pwm_channel)
{
  __ASSERT(buzzer != NULL, DRV_BUZZER_ERROR);
  __ASSERT(tim != NULL, DRV_BUZZER_ERROR);

  buzzer->config.htim = tim;
  buzzer->config.pwm_channel = pwm_channel;

  const uint32_t init_effect[] = {1000, 3000, 6000, 9000, 12000};
  const uint32_t note_effect_duration = 250;

  return DRV_BUZZER_OK;
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
