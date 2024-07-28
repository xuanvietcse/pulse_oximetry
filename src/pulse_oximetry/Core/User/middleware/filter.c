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
// Coeffi in z-domain
double a_z[NUM_OF_COEFFS] = {0};
double b_z[NUM_OF_COEFFS] = {0};

/* Private function prototypes ---------------------------------------- */
/**
 * @brief        Convert transfer function's coefficients from s-domain to z-domain.
 *
 * @param[in]    a_s      Pointer to the numerator coeffi of the transfer function in the s-domain.
 * @param[in]    b_s      Pointer to the denominator coeffi of the transfer function in the s-domain.
 * @param[out]   a_z      Pointer to the numerator coeffi of the transfer function in the z-domain.
 * @param[out]   b_z      Pointer to the denominator coeffi of the transfer function in the z-domain.
 * 
 * @attention    Only for 4-order butterworth filter.
 * 
 * @return       The status of filter operation.
 */
static filter_status_t filter_btw_four_order_bilinear_transform(double *a_s, double *b_s, 
                                                                double *a_z, double *b_z);

/* Function definitions ----------------------------------------------- */
filter_status_t filter_btw_init(void *a_s, void *b_s)
{
  __ASSERT(a_s != NULL, FILTER_ERROR);
  __ASSERT(b_s != NULL, FILTER_ERROR);

  double *a_s_temp = (double *)a_s;
  double *b_s_temp = (double *)b_s;
  
  uint8_t ret = filter_btw_four_order_bilinear_transform(a_s_temp, b_s_temp, a_z, b_z);
  __ASSERT(ret == FILTER_OK, FILTER_FAIL);

  return FILTER_OK;
}

filter_status_t filter_btw_process(void *current_input_array, 
                                   void *current_output_array,
                                   uint8_t size_of_array)
{
  __ASSERT(current_input_array != NULL, FILTER_ERROR);
  __ASSERT(current_output_array != NULL, FILTER_ERROR);

  __ASSERT(a_z != NULL, FILTER_ERROR);
  __ASSERT(b_z != NULL, FILTER_ERROR);

  double *input_array = (double *)current_input_array;
  double *output_array = (double *)current_output_array;

  for (int i = 0; i < size_of_array; i++)
  {
    *(output_array + i) = (b_z[0]) * (*(input_array + i));
    
    for (int j = 1; j < NUM_OF_COEFFS; j++)
    {
      if ((i - j) >= 0)
      {
        *(output_array + i) += (b_z[j]) * (*(input_array + i - j));
      }
    }

    for (int j = 1; j < NUM_OF_COEFFS; j++)
    {
      if ((i - j) >= 0)
      {
        *(output_array + i) -= (a_z[j]) * (*(output_array + i - j));
      }
    }
  }

  return FILTER_OK;
}

/* Private definitions ------------------------------------------------ */
static filter_status_t filter_btw_four_order_bilinear_transform(double *a_s, double *b_s, 
                                                                double *a_z, double *b_z)
{
  __ASSERT(a_s != NULL, FILTER_ERROR);
  __ASSERT(b_s != NULL, FILTER_ERROR);

  __ASSERT(a_z != NULL, FILTER_ERROR);
  __ASSERT(b_z != NULL, FILTER_ERROR);
  
  // +-1, +-2, +-4, +-6 are the coefficients obtained when expanding an algebraic identity.

  double free_order_coeffi_in_b_z = (b_s[4])/(pow(2*SAMPLING_FREQUENCY, NUM_OF_COEFFS - 1));

  double four_order_coeffi_in_a_z = 1 + (a_s[1]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 3)) 
                                      + (a_s[2]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 2)) 
                                      + (a_s[3]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 1)) 
                                      + (a_s[4]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER));

  b_z[0] = free_order_coeffi_in_b_z / four_order_coeffi_in_a_z;
  b_z[4] = free_order_coeffi_in_b_z / four_order_coeffi_in_a_z;
  b_z[1] = (4*free_order_coeffi_in_b_z) / four_order_coeffi_in_a_z;
  b_z[3] = (4*free_order_coeffi_in_b_z) / four_order_coeffi_in_a_z;
  b_z[2] = (6*free_order_coeffi_in_b_z) / four_order_coeffi_in_a_z;

  a_z[0] = (1 + (a_s[1]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 3)) 
              + (a_s[2]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 2)) 
              + (a_s[3]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 1)) 
              + (a_s[4]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  a_z[1] = (-4 + -2*(a_s[1]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 3)) 
               + 2*(a_s[3]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 1))
               + 4*(a_s[4]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  a_z[2] = (6 + 2*(a_s[2]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 2)) 
              + 6*(a_s[4]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  a_z[3] = (-4 + 2*(a_s[1]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 3)) 
               + 2*(a_s[3]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 1))
               + 4*(a_s[4]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  a_z[4] = (1 - (a_s[1]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 3)) 
              + (a_s[2]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 2))
              - (a_s[3]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER - 1)) 
              + (a_s[4]/pow((2*SAMPLING_FREQUENCY), FILTER_ORDER))) / four_order_coeffi_in_a_z;

  return FILTER_OK;
}

/* End of file -------------------------------------------------------- */