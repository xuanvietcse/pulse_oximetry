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
#define SYS_MEASURE_MAX_SAMPLES_PROCESS (200)
#define SYS_MEASURE_FILTER_NUM_OF_COEFFS (5)
#define SYS_MEASURE_SAMPLING_RATE (100.0)
/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
static uint16_t s_adc_val_buf[SYS_MEASURE_MAX_SAMPLES_PROCESS] = {0};
/* Private variables -------------------------------------------------- */

// Coeffi in z-domain
// Numerator
double a_z[SYS_MEASURE_FILTER_NUM_OF_COEFFS] = {1,
                                                -3.71800951758217,
                                                5.19310810021585,
                                                -3.22909001061018,
                                                0.754109572133735};

// Denominator
double b_z[SYS_MEASURE_FILTER_NUM_OF_COEFFS] = {0,
                                                5.35617889287357e-06,
                                                5.56603268403827e-05,
                                                5.26057994909257e-05,
                                                4.52185201307133e-06};

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
static uint32_t sys_measure_filter_data(sys_measure_t *signal);

/**
 * @brief  Detect the peak in dataset of signal
 *
 * @param[in]     signal  The signal object
 * @param[in]     data    Data need to be detected the peak
 *
 * @return
 *  - the number of peaks
 */
static uint32_t sys_measure_peak_detector(sys_measure_t *signal);
/* Function definitions ----------------------------------------------- */
uint32_t sys_measure_init(sys_measure_t *signal,
                          bsp_adc_typedef_t *adc,
                          bsp_tim_typedef_t *tim,
                          uint32_t prescaler,
                          uint32_t autoreload,
                          double *data_buf)
{
  __ASSERT(signal != NULL, SYS_MEASURE_ERROR);
  __ASSERT(data_buf != NULL, SYS_MEASURE_ERROR);

  cb_init(&(signal->dev.adc_conv), s_adc_val_buf, sizeof(s_adc_val_buf));
  cb_init(&(signal->filtered_data), data_buf, SYS_MEASURE_MAX_SAMPLES_PROCESS * sizeof(double));

  signal->heart_rate = 0;
  drv_hr_init(&(signal->dev), adc, tim, prescaler, autoreload);

  return SYS_MEASURE_OK;
}

uint32_t sys_measure_process_data(sys_measure_t *signal)
{
  __ASSERT(signal != NULL, SYS_MEASURE_ERROR);
  __ASSERT(signal->dev.active == true, SYS_MEASURE_ERROR);

  sys_measure_filter_data(signal);
  if (cb_space_count(&(signal->filtered_data)) == 0)
  {
    sys_measure_peak_detector(signal);
  }

  return SYS_MEASURE_OK;
}

/* Private definitions ------------------------------------------------ */
static uint32_t sys_measure_filter_data(sys_measure_t *signal)
{
  __ASSERT(signal != NULL, SYS_MEASURE_ERROR);

  while (cb_data_count(&(signal->dev.adc_conv)) != 0)
  {
    static double recent_input[SYS_MEASURE_FILTER_NUM_OF_COEFFS] = {0};
    static double recent_output[SYS_MEASURE_FILTER_NUM_OF_COEFFS] = {0};

    // Shift recent value to the right
    for (int i = SYS_MEASURE_FILTER_NUM_OF_COEFFS - 1; i > 0; --i)
    {
      recent_input[i] = recent_input[i - 1];
      recent_output[i] = recent_output[i - 1];
    }

    // Put the current value of the input signal into the first position of the array
    uint16_t adc_temp;
    cb_read(&(signal->dev.adc_conv), &adc_temp, sizeof(adc_temp));
    recent_input[0] = (double)adc_temp;

    // Calculate the current output value
    recent_output[0] = b_z[0] * recent_input[0];

    for (int j = 1; j < SYS_MEASURE_FILTER_NUM_OF_COEFFS; ++j)
    {
      recent_output[0] += b_z[j] * recent_input[j];
    }

    for (int j = 1; j < SYS_MEASURE_FILTER_NUM_OF_COEFFS; ++j)
    {
      recent_output[0] -= a_z[j] * recent_output[j];
    }

    // Place the current output value at the first position of the array
    cb_write(&(signal->filtered_data), &recent_output[0], sizeof(recent_output[0]));
  }
  return SYS_MEASURE_OK;
}

static uint32_t sys_measure_peak_detector(sys_measure_t *signal)
{
  __ASSERT(signal != NULL, SYS_MEASURE_ERROR);
  // Choose the Windows Size W1, W2 in TERMA framework
  int w_cycle = 97,
      w_evt = 13;

  double ma_cycle[SYS_MEASURE_MAX_SAMPLES_PROCESS] = {0},
         ma_evt[SYS_MEASURE_MAX_SAMPLES_PROCESS] = {0};

  double mean_of_signal = 0;
  int i, j;
  double handle_data[SYS_MEASURE_MAX_SAMPLES_PROCESS] = {0};
  cb_read(&(signal->filtered_data), handle_data, sizeof(handle_data));

  // Enhance the signal
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES_PROCESS; i++)
  {
    handle_data[i] = pow(handle_data[i], 2);
  }

  // Calculate the Event Duration Moving Average
  for (i = ((w_evt - 1) / 2); i < __SIZE_OF(handle_data) - ((w_evt - 1) / 2); i++)
  {
    for (j = -((w_evt - 1) / 2); j < ((w_evt - 1) / 2); j++)
    {
      ma_evt[i] += handle_data[i + j];
    }
    ma_evt[i] /= w_evt;
  }

  // Calculate the Event Cycle Moving Average
  for (i = ((w_cycle - 1) / 2); i < __SIZE_OF(handle_data) - ((w_cycle - 1) / 2); i++)
  {
    for (j = -((w_cycle - 1) / 2); j < ((w_evt - 1) / 2); j++)
    {
      ma_cycle[i] += handle_data[i + j];
    }
    ma_cycle[i] /= w_cycle;
  }

  // Calculate the mean of signal
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES_PROCESS; i++)
  {
    mean_of_signal += handle_data[i];
  }
  mean_of_signal /= SYS_MEASURE_MAX_SAMPLES_PROCESS;

  // Calculate the Threshold for generating Block of Interest
  double beta = 0.4;
  double threshold[SYS_MEASURE_MAX_SAMPLES_PROCESS] = {0};

  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES_PROCESS; i++)
  {
    threshold[i] = ma_cycle[i] + beta * mean_of_signal;
  }

  // Generate the Bloock of Interest
  uint8_t block_of_interest[SYS_MEASURE_MAX_SAMPLES_PROCESS] = {0};
  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES_PROCESS; i++)
  {
    if (ma_evt[i] > threshold[i])
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

  double peak = 0;
  uint32_t peak_index = 0;
  double heart_rate_avg = 0;
  uint32_t peak_index_buf[5] = {0};
  uint32_t peak_nums = 0;

  for (i = 0; i < SYS_MEASURE_MAX_SAMPLES_PROCESS - 1; i++)
  {
    if ((block_of_interest[i + 1] - block_of_interest[i]) == 1)
    {
      pos_start_block = i;
    }
    if ((block_of_interest[i] - block_of_interest[i + 1]) == 1)
    {
      pos_stop_block = i;
      if (pos_stop_block - pos_start_block >= w_evt)
      {
        // hmmm, add histogram
        peak = handle_data[pos_start_block];
        for (i = pos_start_block; i <= pos_stop_block; i++)
        {
          if (handle_data[i] > peak)
          {
            peak = handle_data[i];
            peak_index = i;
          }
        }
        peak_index_buf[peak_nums] = peak_index;
        peak_nums++;
      }
    }
  }

  for (uint32_t i = 0; i < peak_nums - 1; i++)
  {
    heart_rate_avg += (peak_index_buf[i + 1] - peak_index_buf[i]);
  }
  // Calculate the average peak interval (in second unit)
  heart_rate_avg /= peak_nums;
  heart_rate_avg *= (1 / SYS_MEASURE_SAMPLING_RATE);
  // Estimate the heart rate (beats per minute unit)
  heart_rate_avg = 60 / heart_rate_avg;
  signal->heart_rate = (uint32_t)heart_rate_avg;
  return SYS_MEASURE_OK;
}
/* End of file -------------------------------------------------------- */
