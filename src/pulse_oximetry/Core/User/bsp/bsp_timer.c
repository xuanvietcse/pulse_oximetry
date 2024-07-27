/**
 * @file       button.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       06/07/2024
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      APIs definition of bsp_timer.h
 */
/* Includes ----------------------------------------------------------- */
#include "bsp_timer.h"
#include "common.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
uint32_t bsp_timer_set_prescaler(bsp_tim_typedef_t *htim, uint32_t prescaler)
{
  __ASSERT(htim != NULL, BSP_TIMER_ERROR);
  __ASSERT((prescaler > 0) && (prescaler < 65536), BSP_TIMER_ERROR);

  __HAL_TIM_SET_PRESCALER(htim, prescaler);

  return BSP_TIMER_OK;
}

uint32_t bsp_timer_set_autoreload(bsp_tim_typedef_t *htim, uint32_t autoreload)
{
  __ASSERT(htim != NULL, BSP_TIMER_ERROR);
  __ASSERT(autoreload > 0, BSP_TIMER_ERROR);

  __HAL_TIM_SET_AUTORELOAD(htim, autoreload);

  return BSP_TIMER_OK;
}

uint32_t bsp_timer_start(bsp_tim_typedef_t *htim)
{
  __ASSERT(htim != NULL, BSP_TIMER_ERROR);

  HAL_StatusTypeDef btim_err = HAL_OK;
  btim_err = HAL_TIM_Base_Start(htim);

  __ASSERT(btim_err == HAL_OK, BSP_TIMER_FAIL);
  return BSP_TIMER_OK;
}

uint32_t bsp_timer_start_it(bsp_tim_typedef_t *htim)
{
  __ASSERT(htim != NULL, BSP_TIMER_ERROR);

  HAL_StatusTypeDef btim_err = HAL_OK;
  btim_err = HAL_TIM_Base_Start_IT(htim);

  __ASSERT(btim_err == HAL_OK, BSP_TIMER_FAIL);
  return BSP_TIMER_OK;
}

uint32_t bsp_timer_stop(bsp_tim_typedef_t *htim)
{
  __ASSERT(htim != NULL, BSP_TIMER_ERROR);

  HAL_StatusTypeDef btim_err = HAL_OK;
  btim_err = HAL_TIM_Base_Stop(htim);

  __ASSERT(btim_err == HAL_OK, BSP_TIMER_FAIL);
  return BSP_TIMER_OK;
}

uint32_t bsp_timer_stop_it(bsp_tim_typedef_t *htim)
{
  __ASSERT(htim != NULL, BSP_TIMER_ERROR);

  HAL_StatusTypeDef btim_err = HAL_OK;
  btim_err = HAL_TIM_Base_Stop_IT(htim);

  __ASSERT(btim_err == HAL_OK, BSP_TIMER_FAIL);
  return BSP_TIMER_OK;
}
/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
