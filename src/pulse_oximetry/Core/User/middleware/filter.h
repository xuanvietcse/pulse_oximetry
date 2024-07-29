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

/* Public defines ----------------------------------------------------- */
#define FILTER_ORDER        (4)  
#define NUM_OF_COEFFS       (5)   // 4-order filter has 5 coeffi       
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
// Coeffi in s-domain
//double a_s[NUM_OF_COEFFS] = {1, 28.74438523, 413.1198405, 3478.070613, 14641};
//double b_s[NUM_OF_COEFFS] = {0, 0, 0, 0, 14641};

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Init filter.
 *
 * @param[in]   a_s      Pointer to the numerator coeffi of the transfer function in the s-domain.
 * @param[in]   b_s      Pointer to the denominator coeffi of the transfer function in the s-domain.
 * 
 * @attention   This function should be called first.
 * 
 * @return      The status of filter operation.
 */
filter_status_t filter_btw_init(void *a_s, void *b_s);

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
filter_status_t filter_btw_process(void *current_input_array, 
                                   void *current_output_array,
                                   uint8_t size_of_array);

#endif /* USER_MIDDLEWARE_FILTER_H_ */
