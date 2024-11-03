/**
 * File:   lcd_harmony.inc
 * Author: AWTK Develop Team
 * Brief:  lcd harmony
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/lcd.h"
#include "base/vgcanvas.h"
#include "lcd/lcd_nanovg.h"
#include "lcd/lcd_vgcanvas.inc"

bool_t lcd_harmony_is_support_dirty_rect(lcd_t* lcd) {
  return FALSE;
}

lcd_t* lcd_harmony_init(native_window_t* window) {
  lcd_t* lcd = NULL;
  vgcanvas_t* vg = NULL;
  native_window_info_t info;
  return_value_if_fail(window != NULL, NULL);
  return_value_if_fail(native_window_get_info(window, &info) == RET_OK, NULL);

  vg = vgcanvas_create(info.w, info.h, 0, BITMAP_FMT_NONE, window);
  return_value_if_fail(vg != NULL, NULL);

  lcd = lcd_vgcanvas_init(info.w, info.h, vg);
  return_value_if_fail(lcd != NULL, NULL);
  lcd->is_support_dirty_rect = lcd_harmony_is_support_dirty_rect;

  return lcd;
}
