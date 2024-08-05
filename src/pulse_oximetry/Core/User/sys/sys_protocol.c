/**
 * @file       sys_protocol.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    0.0.1
 * @date       2024-08-05
 * @author     Viet Hoang Xuan
 *             Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *
 * @brief      <A brief description of the content of the file>
 *
 * @note
 * @example
 * @example
 */

/* Includes ----------------------------------------------------------- */
#include "sys_protocol.h"
#include "common.h"
#include "drv_serial.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
const uint8_t s_protocol_msg[] = "\r\nStart the System Protocol finished SUCCESSFULLY!\r\n";
static bool s_packet_cplt_flag = false;
static cbuffer_t *s_protocol_node[SYS_PROTOCOL_MAX_NODE] = {NULL};
/* Private function prototypes ---------------------------------------- */
static void sys_protocol_receive_packet_cplt_handler();
/* Function definitions ----------------------------------------------- */
uint32_t sys_protocol_init(UART_HandleTypeDef *uart)
{
  // Check parameters
  __ASSERT((uart != NULL), SYS_PROTOCOL_ERROR);
  // Operation
  // Initialize UART
  uint32_t ret = drv_serial_init(uart);
  __ASSERT((ret == SYS_PROTOCOL_OK), SYS_PROTOCOL_FAILED);
  // Register callback function
  ret = drv_serial_register_cb_function(sys_protocol_receive_packet_cplt_handler);
  __ASSERT((ret == SYS_PROTOCOL_OK), SYS_PROTOCOL_FAILED);
  // Send sample message to UART to inform status
  ret = drv_serial_transmit(s_protocol_msg, __SIZE_OF(s_protocol_msg));
  __ASSERT((ret == SYS_PROTOCOL_OK), SYS_PROTOCOL_FAILED);
  // Return
  return SYS_PROTOCOL_OK;
}
uint32_t sys_protocol_register_node_to_send(sys_protocol_node_t rx_node, cbuffer_t *rx_cb)
{
  // Check parameters
  __ASSERT((rx_node < SYS_PROTOCOL_MAX_NODE), SYS_PROTOCOL_ERROR);
  __ASSERT((rx_cb != NULL), SYS_PROTOCOL_ERROR);
  // Operation
  s_protocol_node[rx_node] = rx_cb;
  // Return
  return SYS_PROTOCOL_OK;
}
uint32_t sys_protocol_send_pkt_to_node(sys_protocol_node_t rx_node, sys_protocol_pkt_t pkt)
{
  // Check parameters
  __ASSERT((rx_node < SYS_PROTOCOL_MAX_NODE), SYS_PROTOCOL_ERROR);
  // Operations
  // Check if there is enough space
  if (cb_space_count(s_protocol_node[rx_node]) < 6)
  {
    return SYS_PROTOCOL_FAILED;
  }
  // Send CMD
  cb_write(s_protocol_node[rx_node], &(pkt.command), 1);
  // Send DATA
  cb_write(s_protocol_node[rx_node], &(pkt.data), 4);
  // Send TH_LEVEL
  cb_write(s_protocol_node[rx_node], &(pkt.threshold_level), 1);
  // Return
  return SYS_PROTOCOL_OK;
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
