/**
 * @file       sys_measure.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-29
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
#ifndef __USER_SYS_MEASURE_H
#define __USER_SYS_MEASURE_H

/* Includes ----------------------------------------------------------- */
#include "drv_hr.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
enum sys_measure_status_t
{
  SYS_MEASURE_ERROR = 0xFFFFFFFF,
  SYS_MEASURE_FAILED = 0x7FFFFFFF,
  SYS_MEASURE_OK = 0x3FFFFFFF
};

typedef struct
{
  drv_hr_t *dev;
  drv_hr_sampling_rate_t *sampling_rate;
  float *filtered_data;
  uint32_t sample_nums;
} sys_measure_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Initialize the system measurement
 *
 * @param[in]     signal      The type of signal
 * @param[in]     dev         Sensor
 * @param[in]     tim         Timer to trigerr ADC Conversion
 * @param[in]     prescaler   Prescale the clock timer source
 * @param[in]     autoreload   Set the top of counter
 *
 * @return
 *  - (0): Error
 *  - (1): Fail
 *  - (2) : Success
 */
uint32_t sys_measure_init(sys_measure_t *signal,
                          drv_hr_t *dev,
                          bsp_tim_typedef_t *tim,
                          uint32_t prescaler,
                          uint32_t autoreload,
                          uint16_t *data_buf);

/**
 * @brief  Collect the data from source buffer
 *
 * @param[in]     signal      The type of signal
 * @param[in]     src_data    Source of data
 * @param[in]     size        Size of data buffer
 *
 * @return
 *  - (0): Error
 *  - (1): Fail
 *  - (2) : Success
 */
uint32_t sys_measure_collect_data(sys_measure_t *signal, uint16_t *src_data, uint32_t size);

/**
 * @brief Process
 *
 * @param[in]     signal      The signal need to be processed
 * @param[in]     output      Pointer to output buffer
 * @param[in]     heart_rate  Heart rate if can be measured
 *
 * @return
 *  - (0): Error
 *  - (1): Fail
 *  - (2) : Success
 */
uint32_t sys_measure_process_data(sys_measure_t *signal);

#endif // __USER_SYS_MEASURE_H

/* End of file -------------------------------------------------------- */
