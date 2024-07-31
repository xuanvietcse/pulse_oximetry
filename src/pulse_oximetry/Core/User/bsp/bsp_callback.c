/**
 * @file       bso_callback.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-27
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      None
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "bsp_adc.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  bsp_adc_conv_cplt_callback(hadc);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  bsp_pwm_pulse_finished_handler(htim);
}
/* Private definitions ------------------------------------------------ */
/* End of file -------------------------------------------------------- */
