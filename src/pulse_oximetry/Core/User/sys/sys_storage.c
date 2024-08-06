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
#define SYS_STORAGE_ID_SIZE       (1)   // 1 byte
#define SYS_STORAGE_ID_MAX        (255)
#define SYS_STORAGE_ID_MIN        (0)
#define SYS_STORAGE_ID_ACTIVE     (1)
#define SYS_STORAGE_ID_INACTIVE   (0)

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static sys_storage_t s_storage_mng[SYS_STORAGE_ID_MAX + 1] = {0};
static uint8_t s_id_mng[SYS_STORAGE_ID_MAX + 1] = {0};

/* Private function prototypes ---------------------------------------- */

/**
 * @brief  Allocate the idenfication for storage segment
 *
 * @note   Can allocate maximum 256 IDs.
 *
 * @return
 *  - the available ID
 */
static uint8_t sys_storage_get_id(void);

/* Function definitions ----------------------------------------------- */
uint32_t sys_storage_init(sys_storage_t *storage, uint32_t start_address, uint32_t size)
{
  __ASSERT(storage != NULL, SYS_STORAGE_ERROR);
  __ASSERT(size < SYS_STORAGE_FLASH_SECTOR_SIZE, SYS_STORAGE_ERROR);
  __ASSERT(size == 0, SYS_STORAGE_ERROR);
  
  storage->id = sys_storage_get_id();

  s_storage_mng[SYS_STORAGE_ID_MIN].id = storage->id;
  s_storage_mng[SYS_STORAGE_ID_MIN].address = start_address;
  s_storage_mng[SYS_STORAGE_ID_MIN].size = size;

  if (storage->id == 0)
  {
    if ((start_address < SYS_STORAGE_FLASH_SECTOR_ADDRESS) ||
       ((start_address + size) >= (SYS_STORAGE_FLASH_SECTOR_ADDRESS + SYS_STORAGE_FLASH_SECTOR_SIZE)))
    {
      s_id_mng[storage->id] = SYS_STORAGE_ID_INACTIVE;
      s_storage_mng[SYS_STORAGE_ID_MIN] = 0;
      return SYS_STORAGE_ERROR;
    }
    else
    {
      sys_storage_quick_sort(s_storage_mng, SYS_STORAGE_ID_MIN, SYS_STORAGE_ID_MAX); 
    }
  }
  else
  {
    sys_storage_quick_sort(s_storage_mng, SYS_STORAGE_ID_MIN, SYS_STORAGE_ID_MAX);
    uint8_t id_curr_pos = sys_storage_get_id_curr_pos_in_arr(storage->id);

    if ((s_storage_mng[id_curr_pos].address 
        < (s_storage_mng[id_curr_pos - 1].address + s_storage_mng[id_curr_pos - 1].size)) ||
       ((s_storage_mng[id_curr_pos].address + s_storage_mng[id_curr_pos].size)
         >= s_storage_mng[id_curr_pos + 1].storage_mng.address))
    {
      s_id_mng[storage->id] = SYS_STORAGE_ID_INACTIVE;
      s_storage_mng[id_curr_pos] = 0;
      sys_storage_quick_sort(s_storage_mng, SYS_STORAGE_ID_MIN, SYS_STORAGE_ID_MAX);
      return SYS_STORAGE_ERROR;
    }
  }

  s_id_mng[storage->id] = SYS_STORAGE_ID_ACTIVE;
  storage->address = start_address;

  uint32_t ret = SYS_STORAGE_OK;
  ret = bsp_flash_write(storage->address, &storage->id, SYS_STORAGE_ID_SIZE);
  __ASSERT(ret == SYS_STORAGE_OK, SYS_STORAGE_FAILED);

  storage->size = size;
  storage->space_left = size - 1;

  return SYS_STORAGE_OK;
}

uint32_t sys_storage_import(sys_storage_t *storage, void *data, uint32_t size)
{
  __ASSERT(storage != NULL, SYS_STORAGE_ERROR);
  __ASSERT(data != NULL, SYS_STORAGE_ERROR);
  __ASSERT(size <= storage->space_left, SYS_STORAGE_ERROR);
  __ASSERT(size == 0, SYS_STORAGE_ERROR);
  __ASSERT(s_id_mng[storage->id] == SYS_STORAGE_ID_ACTIVE, SYS_STORAGE_ERROR);

  uint32_t ret = SYS_STORAGE_OK;
  ret = bsp_flash_write(storage->address, data, size);
  __ASSERT(ret == SYS_STORAGE_OK, SYS_STORAGE_FAILED);

  return SYS_STORAGE_OK;
}

uint32_t sys_storage_export(sys_storage_t *storage, void *data, uint32_t size)
{
  __ASSERT(storage != NULL, SYS_STORAGE_ERROR);
  __ASSERT(data != NULL, SYS_STORAGE_ERROR);
  __ASSERT(size <= (storage->size - storage->space_left), SYS_STORAGE_ERROR);
  __ASSERT(size == 0, SYS_STORAGE_ERROR);
  __ASSERT(s_id_mng[storage->id] == SYS_STORAGE_ID_ACTIVE, SYS_STORAGE_ERROR);

  uint32_t ret = SYS_STORAGE_OK;
  ret = bsp_flash_read(storage->address, data, size);
  __ASSERT(ret == SYS_STORAGE_OK, SYS_STORAGE_FAILED);

  return SYS_STORAGE_OK; 
}

uint32_t sys_storage_fully_clean(sys_storage_t *storage)
{
  __ASSERT(storage != NULL, SYS_STORAGE_ERROR);
  __ASSERT(s_id_mng[storage->id] == SYS_STORAGE_ID_ACTIVE, SYS_STORAGE_ERROR);

  uint32_t ret = SYS_STORAGE_OK;
  ret = 
  __ASSERT(ret == SYS_STORAGE_OK, SYS_STORAGE_FAILED);

  return SYS_STORAGE_OK; 
}

uint32_t sys_storage_deinit(sys_storage_t *storage)
{
  __ASSERT(storage != NULL, SYS_STORAGE_ERROR);
  __ASSERT(s_id_mng[storage->id] == SYS_STORAGE_ID_ACTIVE, SYS_STORAGE_ERROR);

  uint32_t ret = SYS_STORAGE_OK;
  ret = sys_storage_fully_clean(storage);
  __ASSERT(ret == SYS_STORAGE_OK, SYS_STORAGE_FAILED);

  s_id_mng[storage->id] = SYS_STORAGE_ID_INACTIVE;
  
  storage->address = 0;
  storage->size = 0;
  storage->space_left = 0;

  return SYS_STORAGE_OK; 
}

/* Private definitions ------------------------------------------------ */
static uint8_t sys_storage_get_id(void)
{
  for (uint8_t i = SYS_STORAGE_ID_MIN; i <= SYS_STORAGE_ID_MAX; i++)
  {
    if (s_id_mng[i] == SYS_STORAGE_ID_INACTIVE)
    {
      return i;
    }
  }
}

static void sys_storage_swap(sys_storage_t* a, sys_storage_t* b) 
{
  sys_storage_t t = *a;
  *a = *b;
  *b = t;
}

static void sys_storage_partition(uint8_t low, uint8_t high) 
{
    sys_storage_t pivot.storage_mng.address = s_storage_mng[high].storage_mng.address; 
    uint8_t i = (low - 1);

    for (uint8_t j = low; j <= high - 1; j++) 
    {
      if (s_storage_mng[j].storage_mng.address < pivot.storage_mng.address) 
      {
        i++; 
        swap((s_storage_mng + i), (s_storage_mng + j)); 
      }
    }
    swap((s_storage_mng + i + 1), (s_storage_mng + high));
    return (i + 1);
}

// Sort array from smallest to largest
static uint8_t sys_storage_quick_sort(uint8_t low, uint8_t high) 
{
  if (low < high) 
  {
    uint8_t pi = sys_storage_partition(low, high);

    sys_storage_quick_sort(low, pi - 1);
    sys_storage_quick_sort(pi + 1, high);
  }
}

static uint8_t sys_storage_get_id_curr_pos_in_arr(uint8_t id)
{
  for (uint8_t i = SYS_STORAGE_ID_MIN; i <= SYS_STORAGE_ID_MAX, i++)
  {
    if (s_storage_mng[i].storage_mng.id == id)
    {
      return i;
    }
  }
}

/* End of file -------------------------------------------------------- */
