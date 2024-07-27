/**
 * @file       bsp_adc.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-27
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      <None
 *
 * @note       None
 * @example    Noone
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_adc.h"
#include "common.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static bsp_adc_cb_t b_adc_conv_cplt = NULL;
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
uint32_t bsp_adc_start(bsp_adc_typedef_t *badc)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);

  hal_status_t ret = HAL_OK;
  ret = HAL_ADC_Start(badc);

  __ASSERT(ret == HAL_OK, BSP_ADC_FAILED);

  return BSP_ADC_OK;
}

uint32_t bsp_adc_start_it(bsp_adc_typedef_t *badc)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);

  hal_status_t ret = HAL_OK;
  ret = HAL_ADC_Start_IT(badc);

  __ASSERT(ret == HAL_OK, BSP_ADC_FAILED);

  return BSP_ADC_OK;
}

uint32_t bsp_adc_start_dma(bsp_adc_typedef_t *badc, uint32_t *dma_buf, uint32_t length)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);
  __ASSERT(dma_buf != NULL, BSP_ADC_ERROR);
  __ASSERT(length > 0, BSP_ADC_ERROR);

  hal_status_t ret = HAL_OK;
  ret = HAL_ADC_Start_DMA(badc, dma_buf, length);

  __ASSERT(ret == HAL_OK, BSP_ADC_FAILED);

  return BSP_ADC_OK;
}

uint32_t bsp_adc_stop(bsp_adc_typedef_t *badc)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);

  hal_status_t ret = HAL_OK;
  ret = HAL_ADC_Stop(badc);

  __ASSERT(ret == HAL_OK, BSP_ADC_FAILED);

  return BSP_ADC_OK;
}

uint32_t bsp_adc_stop_it(bsp_adc_typedef_t *badc)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);

  hal_status_t ret = HAL_OK;
  ret = HAL_ADC_Stop_IT(badc);

  __ASSERT(ret == HAL_OK, BSP_ADC_FAILED);

  return BSP_ADC_OK;
}

uint32_t bsp_adc_stop_dma(bsp_adc_typedef_t *badc)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);

  hal_status_t ret = HAL_OK;
  ret = HAL_ADC_Stop_DMA(badc);

  __ASSERT(ret == HAL_OK, BSP_ADC_FAILED);

  return BSP_ADC_OK;
}

uint32_t bsp_adc_get_value(bsp_adc_typedef_t *badc)
{
  __ASSERT(badc != NULL, BSP_ADC_ERROR);

  return HAL_ADC_GetValue(badc);
}

uint32_t bsp_adc_register_handler(bsp_adc_cb_t bsp_adc_cb)
{
  __ASSERT(bsp_adc_cb != NULL, BSP_ADC_ERROR);

  b_adc_conv_cplt = bsp_adc_cb;

  return BSP_ADC_OK;
}

void bsp_adc_conv_cplt_callback(bsp_adc_typedef_t *badc)
{
  __CALLBACK(b_adc_conv_cplt, badc);
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
