/**
 * @file       drv_hr.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-27
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
#include "drv_hr.h"
#include "common.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static drv_hr_cb_t d_full_buf_cb = NULL;
/* Private function prototypes ---------------------------------------- */
/**
 * @brief  Start the peripherals that use for sensor
 *
 * @param[in]     hr_sen  pointer to a drv_hr_t structure that store the information of Heart Rate sensor
 *
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
static uint32_t drv_hr_peri_init(drv_hr_t *hr_sen);

/**
 * @brief  Handler the DMA buffer full event
 *
 * @param[in]     adc_sen  pointer to a ADC_HandleTypeDef structure that contains
 *         the configuration information for the specified ADC
 * @return
 *  - (-2): Error
 *  - (-1): Fail
 *  - (0) : Success
 */
static uint32_t drv_hr_buffer_full_callback(bsp_adc_typedef_t *adc_sen);
/* Function definitions ----------------------------------------------- */
uint32_t drv_hr_init(drv_hr_t *hr_sen,
                     bsp_adc_typedef_t *sen_adc,
                     drv_hr_sampling_rate_t *hr_sampling_rate,
                     uint32_t *converted_data_buf,
                     uint32_t buf_size)
{
  __ASSERT(hr_sen != NULL, DRV_HR_ERROR);
  __ASSERT(sen_adc != NULL, DRV_HR_ERROR);
  __ASSERT(hr_sampling_rate != NULL, DRV_HR_ERROR);
  __ASSERT(converted_data_buf != NULL, DRV_HR_ERROR);
  __ASSERT(buf_size > 0, DRV_HR_ERROR);

  hr_sen->adc = sen_adc;
  hr_sen->sampling_rate = hr_sampling_rate;
  hr_sen->converted_data_buf = converted_data_buf;
  hr_sen->buf_size = buf_size;

  uint32_t ret = DRV_HR_OK;
  ret = drv_hr_peri_init(hr_sen);
  __ASSERT(ret == DRV_HR_OK, DRV_HR_FAIL);

  ret = bsp_adc_register_handler(drv_hr_buffer_full_callback);
  __ASSERT(ret == BSP_ADC_OK, DRV_HR_FAIL);

  hr_sen->active = true;

  return DRV_HR_OK;
}

uint32_t drv_hr_register_handler(drv_hr_t *hr_sen, drv_hr_cb_t full_buf_handler)
{
  __ASSERT(hr_sen != NULL, DRV_HR_ERROR);
  __ASSERT(full_buf_handler != NULL, DRV_HR_ERROR);

  hr_sen->buf_full_cb = full_buf_handler;
  d_full_buf_cb = full_buf_handler;

  return DRV_HR_OK;
}

uint32_t drv_hr_sleep(drv_hr_t *hr_sen)
{
  __ASSERT(hr_sen != NULL, DRV_HR_ERROR);

  uint32_t ret = BSP_TIMER_OK;
  ret = bsp_timer_stop_it(hr_sen->sampling_rate->timer);
  __ASSERT(ret == BSP_TIMER_OK, DRV_HR_FAIL);
  ret = bsp_adc_stop_dma(hr_sen->adc);
  __ASSERT(ret == BSP_ADC_OK, DRV_HR_FAIL);

  hr_sen->active = false;

  return DRV_HR_OK;
}

uint32_t drv_hr_wakeup(drv_hr_t *hr_sen)
{
  __ASSERT(hr_sen != NULL, DRV_HR_ERROR);

  uint32_t ret = BSP_ADC_OK;
  ret = bsp_adc_start_dma(hr_sen->adc, hr_sen->converted_data_buf, hr_sen->buf_size);
  __ASSERT(ret == BSP_ADC_OK, DRV_HR_FAIL);
  ret = bsp_timer_start_it(hr_sen->sampling_rate->timer);
  __ASSERT(ret == BSP_TIMER_OK, DRV_HR_FAIL);

  hr_sen->active = true;

  return DRV_HR_OK;
}

/* Private definitions ------------------------------------------------ */
static uint32_t drv_hr_peri_init(drv_hr_t *hr_sen)
{
  __ASSERT(hr_sen != NULL, DRV_HR_ERROR);

  uint32_t ret = BSP_TIMER_OK;
  ret = bsp_timer_set_prescaler(hr_sen->sampling_rate->timer, hr_sen->sampling_rate->prescaler);
  __ASSERT(ret == BSP_TIMER_OK, DRV_HR_FAIL);

  ret = bsp_timer_set_autoreload(hr_sen->sampling_rate->timer, hr_sen->sampling_rate->autoreload);
  __ASSERT(ret == BSP_TIMER_OK, DRV_HR_FAIL);

  ret = bsp_timer_start_it(hr_sen->sampling_rate->timer);
  __ASSERT(ret == BSP_ADC_OK, DRV_HR_FAIL);

  ret = bsp_adc_start_dma(hr_sen->adc, hr_sen->converted_data_buf, hr_sen->buf_size);
  __ASSERT(ret == BSP_ADC_OK, DRV_HR_FAIL);

  return DRV_HR_OK;
}

static uint32_t drv_hr_buffer_full_callback(bsp_adc_typedef_t *adc_sen)
{
  __CALLBACK(d_full_buf_cb, adc_sen);
  return DRV_HR_OK;
}

/* End of file -------------------------------------------------------- */
