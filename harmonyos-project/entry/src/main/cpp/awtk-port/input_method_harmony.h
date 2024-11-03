/**
 * File:   input_method_harmony.h
 * Author: AWTK Develop Team
 * Brief:  input_method_harmony
 *
 * Copyright (c) 2024 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-11-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_INPUT_METHOD_HARMONY_H
#define TK_INPUT_METHOD_HARMONY_H

#include "base/input_method.h"

BEGIN_C_DECLS

/**
 * @class input_method_harmony_t
 * @parent input_method_t
 *
 * harmony输入法实现。
 *
 */
typedef struct _input_method_harmony_t {
    input_method_t input_method;
    InputMethod_AttachOptions *options;
    InputMethod_TextEditorProxy *textEditorProxy;
    InputMethod_InputMethodProxy *inputMethodProxy;
} input_method_harmony_t;

/**
 * @method input_method_create
 * @annotation ["constructor"]
 *
 * 创建harmony输入法对象。
 *
 * @return {input_method_t*} 返回输入法对象。
 *
 */
input_method_t *input_method_create(void);

END_C_DECLS

#endif /*TK_INPUT_METHOD_HARMONY_H*/
