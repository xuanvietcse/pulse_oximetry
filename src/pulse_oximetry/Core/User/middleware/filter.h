/**
 * @file       filter.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       28/07/2024
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      Filter for PPG signal
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef USER_MIDDLEWARE_FILTER_H_
#define USER_MIDDLEWARE_FILTER_H_

/* Includes ----------------------------------------------------------- */
#include "main.h"
//#include "arm_math.h"

/* Public defines ----------------------------------------------------- */
#define NUM_OF_COEFFS       (5)   // 4-order filter
#define SAMPLING_FREQUENCY  (100) // Hz

/* Public enumerate/structure ----------------------------------------- */
typedef enum 
{
  FILTER_ERROR = -2,
  FILTER_FAIL,
  FILTER_OK
} filter_status_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

// For testing, not for realtime data
filter_status_t apply_filter(double *input_array, 
                             double *output_array);

// For realtime data
filter_status_t apply_filter_realtime(double *current_input_data,
                                      double *current_output_data);

#endif /* USER_MIDDLEWARE_FILTER_H_ */
