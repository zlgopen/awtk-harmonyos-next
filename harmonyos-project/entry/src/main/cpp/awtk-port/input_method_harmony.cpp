/**
 * File:   input_method_harmony.c
 * Author: AWTK Develop Team
 * Brief:  input method harmony
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
 * 2024-11-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */


#include "awtk.h"

#include <inputmethod/inputmethod_controller_capi.h>
#include <inputmethod/inputmethod_types_capi.h>
#include "input_method_harmony.h"

static void input_method_harmony_get_text_config(InputMethod_TextEditorProxy *textEditorProxy,
                                                 InputMethod_TextConfig *config) {
    input_method_t *im = input_method();
    return_if_fail(im != NULL && im->widget != NULL);

    input_type_t type = (input_type_t)widget_get_prop_int(im->widget, WIDGET_PROP_INPUT_TYPE, INPUT_TEXT);
    InputMethod_TextInputType harmony_type = IME_TEXT_INPUT_TYPE_TEXT;

    switch (type) {
    case INPUT_TEXT: {
        harmony_type = IME_TEXT_INPUT_TYPE_TEXT;
        break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
        harmony_type = IME_TEXT_INPUT_TYPE_NUMBER;
        break;
    }
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
        harmony_type = IME_TEXT_INPUT_TYPE_NUMBER_DECIMAL;
        break;
    }
    case INPUT_ASCII: {
        harmony_type = IME_TEXT_INPUT_TYPE_TEXT;
        break;
    }
    case INPUT_DATE:
    case INPUT_TIME:
    case INPUT_CUSTOM:
    case INPUT_TIME_FULL: {
        harmony_type = IME_TEXT_INPUT_TYPE_DATETIME;
        break;
    }
    case INPUT_EMAIL: {
        harmony_type = IME_TEXT_INPUT_TYPE_EMAIL_ADDRESS;
        break;
    }
    case INPUT_CUSTOM_PASSWORD:
    case INPUT_PASSWORD: {
        harmony_type = IME_TEXT_INPUT_TYPE_NEW_PASSWORD;
        break;
    }
    case INPUT_PHONE: {
        harmony_type = IME_TEXT_INPUT_TYPE_PHONE;
        break;
    }
    default: {
        break;
    }
    }
    if (tk_str_eq(im->widget->vt->type, WIDGET_TYPE_MLEDIT)) {
        harmony_type = IME_TEXT_INPUT_TYPE_MULTILINE;
        OH_TextConfig_SetEnterKeyType(config, IME_ENTER_KEY_NEWLINE);
    } else {
        OH_TextConfig_SetEnterKeyType(config, IME_ENTER_KEY_NEXT);
    }
    OH_TextConfig_SetInputType(config, harmony_type);
    OH_TextConfig_SetPreviewTextSupport(config, true);
}


static ret_t wstr_from_char16(wchar_t *wstr, size_t size, const char16_t *text, size_t length) {
    size_t i = 0;
    for (i = 0; i < length && i < size; i++) {
        wstr[i] = text[i];
    }

    return RET_OK;
}

static void input_method_harmony_insert_text(InputMethod_TextEditorProxy *textEditorProxy, const char16_t *text,
                                             size_t length) {
    char str[512] = {0};
    wchar_t wstr[256] = {0};
    input_method_t *im = input_method();
    return_if_fail(im != NULL && im->widget != NULL);

    memset(wstr, 0x00, sizeof(wstr));
    wstr_from_char16(wstr, ARRAY_SIZE(wstr) - 1, text, length);

    tk_utf8_from_utf16(wstr, str, sizeof(str) - 1);

    input_method_commit_text(im, str);
}

static void input_method_harmony_delete_forward(InputMethod_TextEditorProxy *textEditorProxy, int32_t length) {
    input_method_t *im = input_method();
    return_if_fail(im != NULL && im->widget != NULL);
    input_method_dispatch_key(input_method(), TK_KEY_DELETE);
}

static void input_method_harmony_delete_backward(InputMethod_TextEditorProxy *textEditorProxy, int32_t length) {
    input_method_t *im = input_method();
    return_if_fail(im != NULL);
    input_method_dispatch_key(input_method(), TK_KEY_BACKSPACE);
}

static void input_method_harmony_send_keyboard_status(InputMethod_TextEditorProxy *textEditorProxy,
                                                      InputMethod_KeyboardStatus keyboardStatus) {}

static void input_method_harmony_send_enter_key(InputMethod_TextEditorProxy *textEditorProxy,
                                                InputMethod_EnterKeyType enterKeyType) {
    input_method_t *im = input_method();
    return_if_fail(im != NULL && im->widget != NULL);
    switch (enterKeyType) {
    case IME_ENTER_KEY_NEXT: {
        widget_focus_next(im->widget);
        break;
    }
    case IME_ENTER_KEY_PREVIOUS: {
        widget_focus_prev(im->widget);
        break;
    }
    case IME_ENTER_KEY_NEWLINE: {
        input_method_dispatch_key(im, TK_KEY_RETURN);
        break;
    }
    default:
        break;
    }
}

static void input_method_harmony_move_cursor(InputMethod_TextEditorProxy *textEditorProxy,
                                             InputMethod_Direction direction) {}

static void input_method_harmony_handle_set_selection(InputMethod_TextEditorProxy *textEditorProxy, int32_t start,
                                                      int32_t end) {}

static void input_method_harmony_handle_extend_action(InputMethod_TextEditorProxy *textEditorProxy,
                                                      InputMethod_ExtendAction action) {}

static void input_method_harmony_get_left_text_of_cursor(InputMethod_TextEditorProxy *textEditorProxy, int32_t number,
                                                         char16_t text[], size_t *length) {}
static void input_method_harmony_get_right_text_of_cursor(InputMethod_TextEditorProxy *textEditorProxy, int32_t number,
                                                          char16_t text[], size_t *length) {}
static int32_t input_method_harmony_get_text_index_at_cursor(InputMethod_TextEditorProxy *textEditorProxy) { return 0; }
static int32_t input_method_harmony_receive_private_command(InputMethod_TextEditorProxy *textEditorProxy,
                                                            InputMethod_PrivateCommand *privateCommand[], size_t size) {
    return 0;
}

static int32_t input_method_harmony_set_preview_text(InputMethod_TextEditorProxy *textEditorProxy,
                                                     const char16_t text[], size_t length, int32_t start, int32_t end) {
    return 0;
}
static void input_method_harmony_finish_text_preview(InputMethod_TextEditorProxy *textEditorProxy) {}

static InputMethod_TextEditorProxy *OH_TextEditorProxy_CreateAndInit(void) {
    auto textEditorProxy = OH_TextEditorProxy_Create();

    OH_TextEditorProxy_SetGetTextConfigFunc(textEditorProxy, input_method_harmony_get_text_config);
    OH_TextEditorProxy_SetInsertTextFunc(textEditorProxy, input_method_harmony_insert_text);
    OH_TextEditorProxy_SetDeleteBackwardFunc(textEditorProxy, input_method_harmony_delete_backward);
    OH_TextEditorProxy_SetDeleteForwardFunc(textEditorProxy, input_method_harmony_delete_forward);
    OH_TextEditorProxy_SetSendKeyboardStatusFunc(textEditorProxy, input_method_harmony_send_keyboard_status);
    OH_TextEditorProxy_SetSendEnterKeyFunc(textEditorProxy, input_method_harmony_send_enter_key);
    OH_TextEditorProxy_SetMoveCursorFunc(textEditorProxy, input_method_harmony_move_cursor);
    OH_TextEditorProxy_SetHandleSetSelectionFunc(textEditorProxy, input_method_harmony_handle_set_selection);
    OH_TextEditorProxy_SetHandleExtendActionFunc(textEditorProxy, input_method_harmony_handle_extend_action);
    OH_TextEditorProxy_SetGetLeftTextOfCursorFunc(textEditorProxy, input_method_harmony_get_left_text_of_cursor);
    OH_TextEditorProxy_SetGetRightTextOfCursorFunc(textEditorProxy, input_method_harmony_get_right_text_of_cursor);
    OH_TextEditorProxy_SetGetTextIndexAtCursorFunc(textEditorProxy, input_method_harmony_get_text_index_at_cursor);
    OH_TextEditorProxy_SetReceivePrivateCommandFunc(textEditorProxy, input_method_harmony_receive_private_command);
    OH_TextEditorProxy_SetSetPreviewTextFunc(textEditorProxy, input_method_harmony_set_preview_text);
    OH_TextEditorProxy_SetFinishTextPreviewFunc(textEditorProxy, input_method_harmony_finish_text_preview);

    return textEditorProxy;
}

static ret_t input_method_harmony_enable(input_method_t *input_method) {
    InputMethod_ErrorCode code = InputMethod_ErrorCode::IME_ERR_OK;
    input_method_harmony_t *im = (input_method_harmony_t *)input_method;
    return_value_if_fail(im != NULL, RET_BAD_PARAMS);
    auto options = OH_AttachOptions_Create(true);
    auto textEditorProxy = OH_TextEditorProxy_CreateAndInit();
    auto inputMethodProxy = &(im->inputMethodProxy);

    code = OH_InputMethodController_Attach(textEditorProxy, options, inputMethodProxy);
    if (code == InputMethod_ErrorCode::IME_ERR_OK) {
        log_debug("attach input method ok\n");
    } else {
        log_warn("attach input method failed\n");
    }

    im->options = options;
    im->textEditorProxy = textEditorProxy;

    return RET_OK;
}

static ret_t input_method_harmony_disable(input_method_t *input_method) {
    InputMethod_ErrorCode code = InputMethod_ErrorCode::IME_ERR_OK;
    input_method_harmony_t *im = (input_method_harmony_t *)input_method;
    return_value_if_fail(im != NULL, RET_BAD_PARAMS);

    code = OH_InputMethodController_Detach(im->inputMethodProxy);
    if (code != InputMethod_ErrorCode::IME_ERR_OK) {
        log_warn("Detach input method failed!");
    }
    im->inputMethodProxy = nullptr;
    OH_TextEditorProxy_Destroy(im->textEditorProxy);
    im->textEditorProxy = nullptr;
    OH_AttachOptions_Destroy(im->options);
    im->options = nullptr;

    return RET_OK;
}

static ret_t input_method_harmony_request(input_method_t *input_method, widget_t *widget) {
    if (input_method->widget == widget) {
        return RET_OK;
    }
    input_method->widget = widget;

    if (widget != NULL) {
        return input_method_harmony_enable(input_method);
    } else {
        return input_method_harmony_disable(input_method);
    }
}

static ret_t input_method_harmony_destroy(input_method_t *im) {
    return_value_if_fail(im != NULL, RET_BAD_PARAMS);

    emitter_deinit(&(im->emitter));
    TKMEM_FREE(im);

    return RET_OK;
}

input_method_t *input_method_create(void) {
    input_method_harmony_t *im_harmony = TKMEM_ZALLOC(input_method_harmony_t);
    input_method_t *im = (input_method_t *)im_harmony;
    return_value_if_fail(im != NULL, NULL);

    im->request = input_method_harmony_request;
    im->destroy = input_method_harmony_destroy;

    emitter_init(&(im->emitter));

    return im;
}
