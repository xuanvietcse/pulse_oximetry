/**
 * @file       bsp_flash.h
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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __USER_BSP_FLASH_H
#define __USER_BSP_FLASH_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
enum bsp_flash_status_t
{
  BSP_FLASH_ERROR = 0xFFFFFFFF,
  BSP_FLASH_FAIL = 0x7FFFFFF,
  BSP_FLASH_OK = 0x3FFFFFF
};

enum bsp_flash_sector_num_t
{
  BSP_FLASH_SECTOR_0 = 0,
  BSP_FLASH_SECTOR_1,
  BSP_FLASH_SECTOR_2,
  BSP_FLASH_SECTOR_3,
  BSP_FLASH_SECTOR_4,
  BSP_FLASH_SECTOR_5,
  BSP_FLASH_SECTOR_6,
  BSP_FLASH_SECTOR_7
};

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Unlock the FLASH memory
 *
 * @return
 *  - 0xFFFFFFFF: Error
 *  - 0x7FFFFFFF: Fail
 *  - 0x3FFFFFFF: Success
 */
uint32_t bsp_flash_unlock(void);

/**
 * @brief  Lock the FLASH memory
 *
 * @return
 *  - 0xFFFFFFFF: Error
 *  - 0x7FFFFFFF: Fail
 *  - 0x3FFFFFFF: Success
 */
uint32_t bsp_flash_lock(void);

/**
 * @brief  Erase byte(s) from address
 *
 * @param[in]   address   Address that start the erase operation.
 * @param[in]   nbytes   Number of bytes want to be erased.
 *
 * @return
 *  - 0xFFFFFFFF: Error
 *  - 0x7FFFFFFF: Fail
 *  - 0x3FFFFFFF: Success
 */
uint32_t bsp_flash_erase_address(uint32_t address, uint32_t nbytes);

/**
 * @brief  Erase whole data in selected sector
 *
 * @param[in]   sector_num   Sector want to be erased.
 *
 * @return
 *  - 0xFFFFFFFF: Error
 *  - 0x7FFFFFFF: Fail
 *  - 0x3FFFFFFF: Success
 */
uint32_t bsp_flash_erase_sector(uint32_t sector_num);

/**
 * @brief  Write data from address in FLASH
 *
 * @param[in]   address      Address that start the write operation.
 * @param[in]   data_buf     Pointer to data will be written.
 * @param[in]   nbytes       Size of written data in byte.
 *
 * @return
 *  - 0xFFFFFFFF: Error
 *  - 0x7FFFFFFF: Fail
 *  - 0x3FFFFFFF: Success
 */
uint32_t bsp_flash_write(uint32_t address, void *data_buf, uint32_t nbytes);

/**
 * @brief  Read data from address in FLASH
 *
 * @param[in]   address      Address that start the read operation.
 * @param[in]   data_buf     Pointer to data will be read.
 * @param[in]   nbytes       Size of read data in byte.
 *
 * @return
 *  - 0xFFFFFFFF: Error
 *  - 0x7FFFFFFF: Fail
 *  - 0x3FFFFFFF: Success
 */
uint32_t bsp_flash_read(uint32_t address, void *data_buf, uint32_t nbytes);

#endif // __USER_BSP_FLASH_H

/* End of file -------------------------------------------------------- */
