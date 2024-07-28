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
/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Apply filter to an array of data.
 *
 * @param[in]   current_input_array       Pointer to the current input array of data.
 * @param[out]  current_output_array      Pointer to the current output array of data.
 * @param[in]   size_of_array             Size of input and output array.
 * 
 * @attention   Input array and output array must be the same size.
 * 
 * @return      The status of filter operation.
 */
filter_status_t apply_filter_to_array_of_data(double *current_input_array, 
                                              double *current_output_array,
                                              uint8_t size_of_array);

/**
 * @brief       Apply filter to a single data.
 *
 * @param[in]   current_input_data       Pointer to the current input data.
 * @param[out]  current_output_data      Pointer to the current output data.
 * 
 * @attention   No.
 * 
 * @return      The status of filter operation.
 */
filter_status_t apply_filter_to_single_data(double *current_input_data,
                                            double *current_output_data);

#endif /* USER_MIDDLEWARE_FILTER_H_ */
