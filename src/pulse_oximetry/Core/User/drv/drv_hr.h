/**
 * @file       drv_hr.h
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
 * @note
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __USER_DRV_HR_H
#define __USER_DRV_HR_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "bsp_adc.h"
#include "bsp_timer.h"
#include <stdbool.h>
/* Public defines ----------------------------------------------------- */
typedef void (*drv_hr_cb_t)(bsp_adc_typedef_t *adc_sen);
/* Public enumerate/structure ----------------------------------------- */
enum drv_hr_status_t
{
  DRV_HR_ERROR = -2,
  DRV_HR_FAIL,
  DRV_HR_OK
};

typedef struct
{
  bsp_tim_typedef_t *timer;
  uint32_t prescaler;
  uint32_t autoreload;
} drv_hr_sampling_rate_t;

typedef struct __attribute((__packed__))
{
  bsp_adc_typedef_t *adc;
  drv_hr_sampling_rate_t *sampling_rate;
  uint32_t *converted_data_buf;
  uint32_t buf_size;
  drv_hr_cb_t buf_full_cb;
  bool active;
} drv_hr_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize the Heart Rate sensor
 *
 * @param[in]     hr_sen  pointer to a drv_hr_t structure that store the information of Heart Rate sensor
 * @param[in]     sen_adc pointer to a ADC structure of Heart Rate Sensor.
 * @param[in]     hr_sampling_rate pointer to Timer trigger ADC conversion.
 * @param[in]     converted_data_buf  pointer to a buffer that store the converted data from Heart Rate sensor
 * @param[in]     buf_size  Size of buffer that store the converted data.
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_hr_init(drv_hr_t *hr_sen,
                     bsp_adc_typedef_t *sen_adc,
                     drv_hr_sampling_rate_t *hr_sampling_rate,
                     uint32_t *converted_data_buf,
                     uint32_t buf_size);

/**
 * @brief  Register the upper-layer function to handler when buffer of Heart Rate sensor is full.
 *
 * @param[in]     hr_sen  pointer to a drv_hr_t structure that store the information of Heart Rate sensor
 * @param[in]     full_buf_handler  pointer to a function that handle the data in buffer
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_hr_register_handler(drv_hr_t *hr_sen, drv_hr_cb_t full_buf_handler);

/**
 * @brief  Halt the Heart Rate sensor, can not collect data
 *
 * @param[in]     hr_sen  pointer to a drv_hr_t structure that store the information of Heart Rate sensor
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_hr_sleep(drv_hr_t *hr_sen);

/**
 * @brief  Wakeup the Heart Rate sensor, continue to collect data
 *
 * @param[in]     hr_sen  pointer to a drv_hr_t structure that store the information of Heart Rate sensor
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
uint32_t drv_hr_wakeup(drv_hr_t *hr_sen);

#endif // __USER_DRV_HR_H

/* End of file -------------------------------------------------------- */
