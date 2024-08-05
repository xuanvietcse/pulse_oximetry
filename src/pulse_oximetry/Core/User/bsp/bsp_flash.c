/**
 * @file       bsp_flash.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-08-04
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      This BSP supports to interface and transfer data to FLASH easily.
 *
 * @note
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_flash.h"
#include "common.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
uint32_t bsp_flash_unlock(void)
{
  uint32_t ret = HAL_OK;
  ret = HAL_FLASH_Unlock();
  __ASSERT(ret == HAL_OK, BSP_FLASH_FAIL);

  return BSP_FLASH_OK;
}

uint32_t bsp_flash_lock(void)
{
  uint32_t ret = HAL_OK;
  ret = HAL_FLASH_Lock();
  __ASSERT(ret == HAL_OK, BSP_FLASH_FAIL);

  return BSP_FLASH_OK;
}

uint32_t bsp_flash_write(uint32_t address, void *data_buf, uint32_t nbytes)
{
  __ASSERT(nbytes > 0, BSP_FLASH_ERROR);
  __ASSERT((address >= BSP_FLASH_SECTOR_0_ADDRESS) ||
               (address <= (BSP_FLASH_SECTOR_7_ADDRESS + BSP_FLASH_SECTOR_7_SIZE - nbytes)),
           BSP_FLASH_ERROR);
  __ASSERT(data_buf != NULL, BSP_FLASH_ERROR);

  uint32_t ret = BSP_FLASH_OK;
  ret = bsp_flash_unlock();
  __ASSERT(ret == BSP_FLASH_OK, BSP_FLASH_FAIL);

    for (uint32_t i = 0; i < nbytes; i++)
  {
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address + i, *((uint8_t *)data_buf + i));
    __ASSERT(ret == HAL_OK, BSP_FLASH_FAIL);
  }

  ret = bsp_flash_lock();
  __ASSERT(ret == BSP_FLASH_OK, BSP_FLASH_FAIL);

  return BSP_FLASH_OK;
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
