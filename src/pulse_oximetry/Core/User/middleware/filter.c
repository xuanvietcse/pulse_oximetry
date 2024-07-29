/**
 * @file       filter.C
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

/* Includes ----------------------------------------------------------- */
#include "filter.h"
#include "common.h"
#include <stdint.h>
#include <math.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
// Sample coeffi in s-domain
// double a_s[NUM_OF_COEFFS] = {1, 28.74438523, 413.1198405, 3478.070613, 14641};
// double b_s[NUM_OF_COEFFS] = {0, 0, 0, 0, 14641};

filter_t *filter_internal = NULL;

/* Private function prototypes ---------------------------------------- */
/**
 * @brief        Convert transfer function's coefficients from s-domain to z-domain.
 *
 * @param[in]    filter      Pointer to the filter_t variable.
 * 
 * @attention    Only for 4-order butterworth filter.
 * 
 * @return       The status of filter operation.
 */
static filter_status_t filter_btw_four_order_bilinear_transform(filter_t *filter);

/* Function definitions ----------------------------------------------- */
filter_status_t filter_btw_init(filter_t *filter)
{
  __ASSERT(filter->a_s != NULL, FILTER_ERROR);
  __ASSERT(filter->b_s != NULL, FILTER_ERROR);

  __ASSERT(filter->a_z != NULL, FILTER_ERROR);
  __ASSERT(filter->b_z != NULL, FILTER_ERROR);

  __ASSERT(filter->current_input_array != NULL, FILTER_ERROR);
  __ASSERT(filter->current_output_array != NULL, FILTER_ERROR);

  __ASSERT(filter->size_of_array != 0, FILTER_ERROR);
  __ASSERT(filter->sampling_frequency != 0, FILTER_ERROR);

  uint8_t ret = filter_btw_four_order_bilinear_transform(filter);
  __ASSERT(ret == FILTER_OK, FILTER_FAIL);

  filter_internal = filter;

  return FILTER_OK;
}

filter_status_t filter_btw_process(filter_t *filter)
{
  __ASSERT(filter_internal == filter, FILTER_ERROR);
  __ASSERT(filter_internal != NULL, FILTER_ERROR);

  for (int i = 0; i < filter_internal->size_of_array; i++)
  {
    filter_internal->current_output_array[i] = (filter_internal->b_z[0]) * filter_internal->current_input_array[i];
    
    for (int j = 1; j < NUM_OF_COEFFS; j++)
    {
      if ((i - j) >= 0)
      {
        filter_internal->current_output_array[i] += (filter_internal->b_z[j]) * filter_internal->current_input_array[i - j];
      }
    }

    for (int j = 1; j < NUM_OF_COEFFS; j++)
    {
      if ((i - j) >= 0)
      {
        filter_internal->current_output_array[i] -= (filter_internal->a_z[j]) * filter_internal->current_output_array[i - j];
      }
    }
  }

  return FILTER_OK;
}

/* Private definitions ------------------------------------------------ */
static filter_status_t filter_btw_four_order_bilinear_transform(filter_t *filter)
{
  // +-1, +-2, +-4, +-6 are the coefficients obtained when expanding an algebraic identity.

  double free_order_coeffi_in_b_z = (filter->b_s[4])/(pow(2*filter->sampling_frequency, NUM_OF_COEFFS - 1));

  double four_order_coeffi_in_a_z = 1 + (filter->a_s[1]/pow((2*filter->sampling_frequency), FILTER_ORDER - 3)) 
                                      + (filter->a_s[2]/pow((2*filter->sampling_frequency), FILTER_ORDER - 2)) 
                                      + (filter->a_s[3]/pow((2*filter->sampling_frequency), FILTER_ORDER - 1)) 
                                      + (filter->a_s[4]/pow((2*filter->sampling_frequency), FILTER_ORDER));

  filter->b_z[0] = free_order_coeffi_in_b_z / four_order_coeffi_in_a_z;
  filter->b_z[4] = free_order_coeffi_in_b_z / four_order_coeffi_in_a_z;
  filter->b_z[1] = (4*free_order_coeffi_in_b_z) / four_order_coeffi_in_a_z;
  filter->b_z[3] = (4*free_order_coeffi_in_b_z) / four_order_coeffi_in_a_z;
  filter->b_z[2] = (6*free_order_coeffi_in_b_z) / four_order_coeffi_in_a_z;

  filter->a_z[0] = (1 + (filter->a_s[1]/pow((2*filter->sampling_frequency), FILTER_ORDER - 3)) 
                      + (filter->a_s[2]/pow((2*filter->sampling_frequency), FILTER_ORDER - 2)) 
                      + (filter->a_s[3]/pow((2*filter->sampling_frequency), FILTER_ORDER - 1)) 
                      + (filter->a_s[4]/pow((2*filter->sampling_frequency), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  filter->a_z[1] = (-4 + -2*(filter->a_s[1]/pow((2*filter->sampling_frequency), FILTER_ORDER - 3)) 
                       + 2*(filter->a_s[3]/pow((2*filter->sampling_frequency), FILTER_ORDER - 1))
                       + 4*(filter->a_s[4]/pow((2*filter->sampling_frequency), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  filter->a_z[2] = (6 + 2*(filter->a_s[2]/pow((2*filter->sampling_frequency), FILTER_ORDER - 2)) 
                      + 6*(filter->a_s[4]/pow((2*filter->sampling_frequency), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  filter->a_z[3] = (-4 + 2*(filter->a_s[1]/pow((2*filter->sampling_frequency), FILTER_ORDER - 3)) 
                       + 2*(filter->a_s[3]/pow((2*filter->sampling_frequency), FILTER_ORDER - 1))
                       + 4*(filter->a_s[4]/pow((2*filter->sampling_frequency), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  filter->a_z[4] = (1 - (filter->a_s[1]/pow((2*filter->sampling_frequency), FILTER_ORDER - 3)) 
                      + (filter->a_s[2]/pow((2*filter->sampling_frequency), FILTER_ORDER - 2))
                      - (filter->a_s[3]/pow((2*filter->sampling_frequency), FILTER_ORDER - 1)) 
                      + (filter->a_s[4]/pow((2*filter->sampling_frequency), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  return FILTER_OK;
}

/* End of file -------------------------------------------------------- */