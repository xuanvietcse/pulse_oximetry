/**
 * @file       sys_protocol.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    0.0.1
 * @date       2024-07-29
 * @authors    Viet Hoang Xuan
 *             Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *
 * @brief      <A brief description of the content of the file>
 *
 * @note
 * @example
 * @example
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_PROTOCOL_H
#define __SYS_PROTOCOL_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  SYS_PROTOCOL_ERROR = 0xFFFFFFFF,
  SYS_PROTOCOL_FAILED = 0x7FFFFFFF,
  SYS_PROTOCOL_OK = 0x3FFFFFFF
} sys_protocol_status_t;
typedef struct
{
  uint8_t command;
  uint32_t data;
  uint8_t threshold_level;
} sys_protocol_pkt_t;
/* Public macros ------------------------------------------------------ */
#define SOF_FIELD ((0xF0) << 5)
#define COMMAND_FIELD ((0x0F) << 5)
#define DATA_FIELD ((0xFFFFFFFF) << 3)
#define TH_LEVEL_FIELD ((0x0F) << 1)
#define EOF_FIELD (0x0F)
/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

#endif // __SYS_PROTOCOL_H

/* End of file -------------------------------------------------------- */
