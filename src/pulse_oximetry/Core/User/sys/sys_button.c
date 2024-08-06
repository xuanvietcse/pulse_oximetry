/**
 * @file       sys_button.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    v1.0.0
 * @date       2024-07-13
 * @author     Giang Phan Truong
 *             Khanh Nguyen Ngoc
 *             Viet Hoang Xuan
 *
 * @brief      Layer controls button events
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "sys_button.h"
#include "bsp_timer.h"
#include "bsp_utils.h"
#include "common.h"
#include <stdbool.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
sys_button_t sbutton;

/* Private variables -------------------------------------------------- */
sys_button_evt_on_power s_on_power_cb;
sys_button_evt_off_power s_off_power_cb;

bool s_button_is_power_off_flag = false;

/* Private function prototypes ---------------------------------------- */
static void sys_button_detect_edge(uint16_t exti_line);

/* Function definitions ----------------------------------------------- */
sys_button_status_t sys_button_init(GPIO_TypeDef *gpio, uint16_t pin, uint32_t button_active_level)
{
  drv_button_t dbutton;
  drv_button_status_t ret = DRV_BUTTON_OK;

  ret = drv_button_init(&dbutton, gpio, pin, button_active_level);
  __ASSERT(ret == DRV_BUTTON_OK, SYS_BUTTON_FAIL);

  sbutton.dbutton = dbutton;
  sbutton.transient_state = SYS_BUTTON_STATE_STABLE;
  sbutton.fsm_state = SYS_BUTTON_FSM_STATE_IDLE;
  drv_button_register_callback(sys_button_detect_edge);

  return SYS_BUTTON_OK;
}

sys_button_status_t sys_button_manage()
{
  if (sbutton.transient_state == SYS_BUTTON_STATE_DEBOUNCE)
  {
    if (HAL_GetTick() - sbutton.dbutton.time_debounce >= BUTTON_DEBOUNCE_TIME)
    {
      uint32_t button_current_state = bsp_gpio_read_pin(sbutton.dbutton.port, sbutton.dbutton.pin);
      if (button_current_state != sbutton.dbutton.current_state)
      {
        sbutton.dbutton.current_state = button_current_state;
      }
      sbutton.transient_state = SYS_BUTTON_STATE_STABLE;
    }
  }

  switch (sbutton.fsm_state)
  {
  case SYS_BUTTON_FSM_STATE_IDLE:
  {
    if (sbutton.dbutton.current_state == sbutton.dbutton.active_level)
    {
      sbutton.fsm_state = SYS_BUTTON_FSM_STATE_PRESS;
      sbutton.dbutton.time_change = bsp_utils_get_tick();
    }
    break;
  }

  case SYS_BUTTON_FSM_STATE_PRESS:
  {
    uint32_t current_tick = bsp_utils_get_tick();
    if (sbutton.dbutton.current_state != sbutton.dbutton.active_level)
    {
      if (current_tick - sbutton.dbutton.time_change < BUTTON_SINGLE_CLICK_TIME)
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_WAIT_SINGLE_CLICK;
        sbutton.dbutton.time_change = bsp_utils_get_tick();
      }
      else
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_IDLE;
      }
    }
    else if ((sbutton.dbutton.current_state == sbutton.dbutton.active_level) &&
             (current_tick - sbutton.dbutton.time_change >= BUTTON_SINGLE_CLICK_TIME))
    {
      sbutton.fsm_state = SYS_BUTTON_FSM_STATE_WAIT_HOLD;
    }
    break;
  }

  case SYS_BUTTON_FSM_STATE_WAIT_SINGLE_CLICK:
  {
    uint32_t current_tick = bsp_utils_get_tick();
    if (current_tick - sbutton.dbutton.time_change >= BUTTON_RELEASE_TIME)
    {
      if (sbutton.dbutton.current_state != sbutton.dbutton.active_level)
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_SINGLE_CLICK;
      }
    }
    else if ((sbutton.dbutton.current_state == sbutton.dbutton.active_level) &&
             (current_tick - sbutton.dbutton.time_change < BUTTON_RELEASE_TIME))
    {
      sbutton.fsm_state = SYS_BUTTON_FSM_STATE_WAIT_DOUBLE_CLICK;
      sbutton.dbutton.time_change = bsp_utils_get_tick();
    }
    break;
  }

  case SYS_BUTTON_FSM_STATE_WAIT_DOUBLE_CLICK:
  {
    uint32_t current_tick = bsp_utils_get_tick();
    if (sbutton.dbutton.current_state != sbutton.dbutton.active_level)
    {
      if (current_tick - sbutton.dbutton.time_change < BUTTON_SINGLE_CLICK_TIME)
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_DOUBLE_CLICK;
      }
      else
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_IDLE;
      }
    }
    else if ((sbutton.dbutton.current_state == sbutton.dbutton.active_level) &&
             (current_tick - sbutton.dbutton.time_change >= BUTTON_SINGLE_CLICK_TIME))
    {
      sbutton.fsm_state = SYS_BUTTON_FSM_STATE_IDLE;
    }
    break;
  }

  case SYS_BUTTON_FSM_STATE_WAIT_HOLD:
  {
    uint32_t current_tick = bsp_utils_get_tick();
    if (current_tick - sbutton.dbutton.time_change >= BUTTON_HOLD_TIME)
    {
      if (sbutton.dbutton.current_state == sbutton.dbutton.active_level)
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_HOLD;
      }
      else
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_IDLE;
      }
    }
    break;
  }

  case SYS_BUTTON_FSM_STATE_WAIT_RELEASE:
  {
    uint32_t current_tick = bsp_utils_get_tick();
    if (sbutton.dbutton.current_state != sbutton.dbutton.active_level)
    {
      if (current_tick - sbutton.dbutton.time_change >= BUTTON_RELEASE_TIME)
      {
        sbutton.fsm_state = SYS_BUTTON_FSM_STATE_RELEASE;
      }
    }
    break;
  }

  case SYS_BUTTON_FSM_STATE_HOLD:
  {
    sbutton.fsm_state = SYS_BUTTON_FSM_STATE_WAIT_RELEASE;
    sbutton.dbutton.time_change = bsp_utils_get_tick();
  }

  default:
  {
    sbutton.fsm_state = SYS_BUTTON_FSM_STATE_IDLE;
    break;
  }
  }

  switch (sbutton.fsm_state)
  {
  case SYS_BUTTON_FSM_STATE_HOLD:
  {
    if (s_button_is_power_off_flag == false)
    {
      s_button_is_power_off_flag = true;
      __CALLBACK(s_off_power_cb);
    }
    else if (s_button_is_power_off_flag == true)
    {
      s_button_is_power_off_flag = false;
      __CALLBACK(s_on_power_cb);
    }
    break;
  }

  default:
  {
    break;
  }
  }
  return SYS_BUTTON_OK;
}

sys_button_status_t sys_button_register_cb_function(sys_button_evt_on_power on_power_cb,
                                                    sys_button_evt_off_power off_power_cb)
{
  __ASSERT(on_power_cb != NULL, SYS_BUTTON_ERROR);
  __ASSERT(off_power_cb != NULL, SYS_BUTTON_ERROR);

  s_on_power_cb = on_power_cb;
  s_off_power_cb = off_power_cb;

  return SYS_BUTTON_OK;
}
/* Private definitions ------------------------------------------------ */
static void sys_button_detect_edge(uint16_t exti_line)
{
  if ((sbutton.dbutton.pin == exti_line) &&
      (sbutton.transient_state == SYS_BUTTON_STATE_STABLE))
  {
    sbutton.transient_state = SYS_BUTTON_STATE_DEBOUNCE;
    sbutton.dbutton.time_debounce = bsp_get_systick();
  }
}
/* End of file -------------------------------------------------------- */
