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

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

// Coeffi in s-domain
double a_s[NUM_OF_COEFFS] = {1, 
                                28.74438523, 
                                413.1198405, 
                                3478.070613, 
                                14641};

double b_s[NUM_OF_COEFFS] = {0, 
                                0, 
                                0, 
                                0, 
                                14641};

// Coeffi in z-domain
double a_z[NUM_OF_COEFFS] = {1, 
                                -3.7128, 
                                5.1789, 
                                -3.2162, 
                                0.7502};

double b_z[NUM_OF_COEFFS] = {7.92609259e-06, 
                                3.17043703e-05, 
                                4.75565555e-05, 
                                3.17043704e-05, 
                                7.92609258e-06};

// NUM_OF_COEFFS most recent input and output
double recent_input[NUM_OF_COEFFS] = {0};
double recent_output[NUM_OF_COEFFS] = {0};

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

// For testing, not for realtime data
filter_status_t apply_filter(double *input_array, 
                             double *output_array)
{
  __ASSERT(input_array != NULL, FILTER_ERROR);
  __ASSERT(output_array != NULL, FILTER_ERROR);

  for (int i = 0; i < 100; i++)
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


filter_status_t apply_filter_realtime(double *current_input_data,
                                      double *current_output_data)
{
  // Dịch các giá trị trước đó trong x và y sang phải
  for (int i = NUM_OF_COEFFS - 1; i > 0; --i) 
  {
    recent_input[i] = recent_input[i - 1];
    recent_output[i] = recent_output[i - 1];
  }

  // Đặt giá trị hiện tại của tín hiệu đầu vào vào vị trí đầu tiên của mảng x
  recent_input[0] = *current_input_data;

  // Tính toán giá trị đầu ra hiện tại
  *current_output_data = b_z[0] * recent_input[0];

  for (int j = 1; j < NUM_OF_COEFFS; ++j) 
  {
      *current_output_data += b_z[j] * recent_input[j];
  }

  for (int j = 1; j < NUM_OF_COEFFS; ++j) 
  {
      *current_output_data -= a_z[j] * recent_output[j];
  }

  // Đặt giá trị đầu ra hiện tại vào vị trí đầu tiên của mảng y
  recent_output[0] = *current_output_data;

  return FILTER_OK;
}

/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
