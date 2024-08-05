/**
 * @file       sys_storage.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-08-05
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      System sevice take responsibility for storing and managing data in FLASH memory
 *
 * @note
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "sys_storage.h"
#include "common.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/**
 * @brief  Allocate the idenfication for storage segment
 *
 * @note  Can allocate maximum 256 IDs.
 *
 * @return
 *  - the available ID
 */
static uint8_t sys_storage_get_id(void);
/* Function definitions ----------------------------------------------- */
uint32_t sys_storage_init(sys_storage_t *storage, uint32_t start_address, uint32_t size)
{
  __ASSERT(storage != NULL, SYS_STORAGE_ERROR);
  __ASSERT(size < SYS_STORAGE_FLASH_SECTOR, SYS_STORAGE_ERROR);
  __ASSERT(start_address > BSP_FLASH_SECTOR_7_ADDRESS, SYS_STORAGE_ERROR);
  __ASSERT(start_address < (BSP_FLASH_SECTOR_7_ADDRESS + BSP_FLASH_SECTOR_7_SIZE - size), SYS_STORAGE_ERROR);

  storage->address = start_address;
  storage->size = size;
  storage->space_left = size;

  return SYS_STORAGE_OK;
}
/* Private definitions ------------------------------------------------ */
static uint8_t sys_storage_get_id(void)
{
}
/* End of file -------------------------------------------------------- */
