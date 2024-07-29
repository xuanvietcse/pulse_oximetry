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
#define NUM_OF_COEFFS       (5)   // 4-order filter has 5 coefficients       

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Status of filter operation
 */
typedef enum 
{
  FILTER_ERROR = -2,
  FILTER_FAIL,
  FILTER_OK
} filter_status_t;

/**
 * @brief       Filter data type
 * @attention   Coeffi in s-domain are provided by user or another layer (input)
 *              Coeffi in z-domain are converted from coeffi in s-domain by the init function (output)
 */
typedef struct __attribute__((__packed__))
{
  double *a_s;                    // Pointer to the array of numerator coeffi of the transfer function in the s-domain.
  double *b_s;                    // Pointer to the array of denominator coeffi of the transfer function in the s-domain.
  double *a_z;                    // Pointer to the array of numerator coeffi of the transfer function in the z-domain.
  double *b_z;                    // Pointer to the array of denominator coeffi of the transfer function in the z-domain.
  double *current_input_array;    // Pointer to the current input array of data.
  double *current_output_array;   // Pointer to the current output array of data.
  uint8_t size_of_array;          // Size of input and output array.
  uint8_t sampling_frequency;
} filter_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Init filter.
 *
 * @param[in]   filter      Pointer to the filter_t variable.
 * 
 * @attention   This function should be called first.
 * 
 * @return      The status of filter operation.
 */
filter_status_t filter_btw_init(filter_t *filter);

/**
 * @brief       Apply filter to an array of data.
 *
 * @param[in]   filter      Pointer to the filter_t variable.
 * 
 * @attention   Input array and output array must be the same size.
 * 
 * @return      The status of filter operation.
 */
filter_status_t filter_btw_process(filter_t *filter);

#endif /* USER_MIDDLEWARE_FILTER_H_ */
