/**
 * File:   native_window_harmony.hpp
 * Author: AWTK Develop Team
 * Brief:  native window harmony
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
 * 2024-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NATIVE_WINDOW_HARMONY_H
#define TK_NATIVE_WINDOW_HARMONY_H

#include "base/lcd.h"
#include "base/native_window.h"

BEGIN_C_DECLS

class AwtkApp;

/**
 * @class native_window_harmony_t
 * @annotation ["fake"]
 * harmony native window
 */

/**
 * @method native_window_harmony_init
 * @annotation ["constructor"]
 * @param {RenderWindow*} win
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_harmony_init(AwtkApp* app);

/**
 * @method native_window_harmony_deinit
 * @annotation ["destructor"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_harmony_deinit(void);

END_C_DECLS

#endif /*TK_NATIVE_WINDOW_HARMONY_H*/
