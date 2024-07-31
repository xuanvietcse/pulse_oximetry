/**
 * @file       sys_measure.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-30
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      None
 *
 * @note
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "sys_measure.h"
#include "common.h"
#include "math.h"

/* Private defines ---------------------------------------------------- */
#define SYS_MEASURE_MAX_SAMPLES (100)
#define NUM_OF_COEFFS           (5)
/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
// Coeffi in z-domain
// Numerator 
double a_z[NUM_OF_COEFFS] = {1, -3.71800951758217, 5.19310810021585, 
                            -3.22909001061018, 0.754109572133735};

// Denominator
double b_z[NUM_OF_COEFFS] = {0, 5.35617889287357e-06, 5.56603268403827e-05, 
                            5.26057994909257e-05, 4.52185201307133e-06};

/* Private function prototypes ---------------------------------------- */

/**
 * @brief  Filter the interferances of the signal
 *
 * @param[in]     signal  The signal object
 * @param[in]     input   The input signal
 *
 * @return
 *  - the filtered data
 */
static uint32_t sys_measure_filter_data(sys_measure_t *signal, uint16_t input);

/**
 * @brief  Detect the peak in dataset of signal
 *
 * @param[in]     signal  The signal object
 * @param[in]     data    Data need to be detected the peak
 *
 * @return
 *  - the number of peaks
 */
static uint32_t sys_measure_peak_detector(sys_measure_t *signal, double *data);
/* Function definitions ----------------------------------------------- */
void public_function(void)
{
}
/* Private definitions ------------------------------------------------ */
static uint32_t sys_measure_filter_data(sys_measure_t *signal, uint16_t input)
{
  __ASSERT(signal != NULL, SYS_MEASURE_ERROR);

  static double recent_input[NUM_OF_COEFFS] = {0};
  static double recent_output[NUM_OF_COEFFS] = {0};

  // Shift recent value to the right
  for (int i = NUM_OF_COEFFS - 1; i > 0; --i) 
  {
    recent_input[i] = recent_input[i - 1];
    recent_output[i] = recent_output[i - 1];
  }

  // Put the current value of the input signal into the first position of the array
  recent_input[0] = (double)input;

  // Calculate the current output value
  signal->filtered_data[signal->sample_nums] = b_z[0] * recent_input[0];

  for (int j = 1; j < NUM_OF_COEFFS; ++j) 
  {
      signal->filtered_data[signal->sample_nums] += b_z[j] * recent_input[j];
  }

  for (int j = 1; j < NUM_OF_COEFFS; ++j) 
  {
      signal->filtered_data[signal->sample_nums] -= a_z[j] * recent_output[j];
  }

  // Place the current output value at the first position of the array
  recent_output[0] = signal->filtered_data[signal->sample_nums];

  return SYS_MEASURE_OK;
}


static uint32_t sys_measure_peak_detector(sys_measure_t *signal, double *data)
{
  __ASSERT(signal != NULL, SYS_MEASURE_ERROR);
  __ASSERT(data != NULL, SYS_MEASURE_ERROR);
  // Choose the Windows Size W1, W2 in TERMA framework
  uint32_t w_cycle = 301,
           w_evt = 51;

  float ma_cycle[SYS_MEASURE_MAX_SAMPLES] = {0},
        ma_evt[SYS_MEASURE_MAX_SAMPLES] = {0};

  float mean_of_signal = 0;
  uint32_t i, j, k;

  // Enhance the signal
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES; i++)
  {
    data[i] = pow(data[i], 2);
  }

  // Calculate the Event Duration Moving Average
  k = ((w_evt - 1) / 2);
  for (i = ((w_evt - 1) / 2); i < SYS_MEASURE_MAX_SAMPLES - ((w_evt - 1) / 2); i++)
  {
    for (j = -((w_evt - 1) / 2); j < k; j++)
    {
      ma_evt[i] += data[i + j];
    }
    ma_evt[i] /= w_evt;
  }

  // Calculate the Event Cycle Moving Average
  k = ((w_cycle - 1) / 2);
  for (i = ((w_cycle - 1) / 2); i < SYS_MEASURE_MAX_SAMPLES - ((w_cycle - 1) / 2); i++)
  {
    for (j = -((w_cycle - 1) / 2); j < k; j++)
    {
      ma_cycle[i] += data[i + j];
    }
    ma_cycle[i] /= w_cycle;
  }

  // Calculate the mean of signal
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES; i++)
  {
    mean_of_signal += data[i];
  }
  mean_of_signal /= SYS_MEASURE_MAX_SAMPLES;

  // Calculate the Threshold for generating Block of Interest
  float beta = 0.8;
  float threshold_1[SYS_MEASURE_MAX_SAMPLES] = {0};

  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES; i++)
  {
    threshold_1[i] = ma_cycle[i] + beta * mean_of_signal;
  }

  // Generate the Bloock of Interest
  uint8_t block_of_interest[SYS_MEASURE_MAX_SAMPLES] = {0};
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES; i++)
  {
    if (ma_evt[i] > threshold_1[i])
    {
      block_of_interest[i] = 1;
    }
    else
    {
      block_of_interest[i] = 0;
    }
  }

  // Peak detector
  uint32_t pos_start_block = 0;
  uint32_t pos_stop_block = 0;
  uint32_t peak_num = 0;
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES - 1; i++)
  {
    if ((block_of_interest[i + 1] - block_of_interest[i]) == 1)
    {
      pos_start_block = i;
    }
    else if ((block_of_interest[i] - block_of_interest[i + 1]) == 1)
    {
      pos_stop_block = i;
      if (pos_stop_block - pos_start_block >= w_evt)
      {
        // hmmm, add histogram
        peak_num++;
      }
    }
  }
  return peak_num;
}
/* End of file -------------------------------------------------------- */
