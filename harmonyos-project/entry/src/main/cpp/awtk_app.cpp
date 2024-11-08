/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <GLES3/gl31.h>
#include <bits/alltypes.h>
#include <cmath>
#include "log.h"
#include "awtk_app.h"
#include "awtk-port/native_window_harmony.hpp"
#include "main_loop/main_loop_simple.h"
#include "base/asset_loader_zip.h"

extern "C" ret_t application_init(void);
extern "C" ret_t application_exit(void);

namespace {
EGLConfig getConfig(int version, EGLDisplay eglDisplay) {
    int attribList[] = {EGL_SURFACE_TYPE,
                        EGL_WINDOW_BIT,
                        EGL_RED_SIZE,
                        8,
                        EGL_GREEN_SIZE,
                        8,
                        EGL_BLUE_SIZE,
                        8,
                        EGL_ALPHA_SIZE,
                        8,
                        EGL_RENDERABLE_TYPE,
                        EGL_OPENGL_ES2_BIT,
                        EGL_NONE};
    EGLConfig configs = NULL;
    int configsNum;

    if (!eglChooseConfig(eglDisplay, attribList, &configs, 1, &configsNum)) {
        LOGE("eglChooseConfig ERROR");
        return NULL;
    }

    return configs;
}
} // namespace

static ret_t locale_info_change_simple(const char *name) {
    char language[32] = {0};
    char country_or_region[32] = {0};

    const char *p = NULL;
    return_value_if_fail(name != NULL, RET_BAD_PARAMS);

    p = strchr(name, '-');
    if (p == NULL) {
        locale_info_change(locale_info(), name, NULL);
        return RET_OK;
    }

    tk_strncpy_s(language, sizeof(language) - 1, name, p - name);

    p = strrchr(name, '-');
    tk_strncpy(country_or_region, p + 1, sizeof(country_or_region) - 1);


    return locale_info_change(locale_info(), language, country_or_region);
}

extern "C" main_loop_t* main_loop_init(int w, int h) {
    assert(!"should not be called!");
    return NULL;
}

int32_t AwtkApp::Init(const char *filesDir, const char *tempDir, int32_t screenDensity, double fontScale,
                      const char *language, const void *assetsZipContent, uint32_t assetsZipSize) {
    const char *app_name = "harmony";
    const char *app_root = "./";
    char mem_url[MAX_PATH + 1] = {0};

    data_reader_mem_build_url(assetsZipContent, assetsZipSize, mem_url);
    data_reader_t *reader = data_reader_mem_create(mem_url);
    asset_loader_t *loader = asset_loader_zip_create_with_reader(reader, TRUE);
    return_value_if_fail(tk_pre_init() == RET_OK, RET_FAIL);
    ENSURE(system_info_init(APP_MOBILE, app_name, app_root) == RET_OK);
    return_value_if_fail(tk_init_internal() == RET_OK, RET_FAIL);

    mScreenDensity = screenDensity;
    mDevicePixelRatio = screenDensity / 160;
    
    assets_manager_set_loader(assets_manager(), loader);

    locale_info_change_simple(language);
    system_info_set_font_scale(system_info(), fontScale);
    system_info_set_default_font(system_info(), AWTK_DEFAULT_FONT);

    tk_ext_widgets_init();
    native_window_harmony_init(this);
    main_loop_simple_init(mWidth, mHeight, NULL, NULL);

    application_init();

    return 0;
}

int32_t AwtkApp::PreInit(void *window, int32_t width, int32_t height) {
    LOGI("Init window = %{public}p, w = %{public}d, h = %{public}d.", window, width, height);
    mEglWindow = reinterpret_cast<EGLNativeWindowType>(window);

    mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEGLDisplay == EGL_NO_DISPLAY) {
        LOGE("unable to get EGL display.");
        return -1;
    }

    EGLint eglMajVers, eglMinVers;
    if (!eglInitialize(mEGLDisplay, &eglMajVers, &eglMinVers)) {
        mEGLDisplay = EGL_NO_DISPLAY;
        LOGE("unable to initialize display");
        return -1;
    }

    int version = 3;
    mEGLConfig = getConfig(version, mEGLDisplay);
    if (mEGLConfig == nullptr) {
        LOGE("GLContextInit config ERROR");
        return -1;
    }

    EGLint winAttribs[] = {EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_SRGB_KHR, EGL_NONE};
    if (mEglWindow) {
        mEGLSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, mEglWindow, winAttribs);
        if (mEGLSurface == nullptr) {
            LOGE("eglCreateContext eglSurface is null");
            return -1;
        }
    }

    /* Create EGLContext from */
    int attrib3_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    mEGLContext = eglCreateContext(mEGLDisplay, mEGLConfig, mSharedEGLContext, attrib3_list);
    if (!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext)) {
        LOGE("eglMakeCurrent error = %{public}d", eglGetError());
    }

    mWidth = width;
    mHeight = height;

    return 0;
}

void AwtkApp::Update(void) {
    if (mWidth > 0 && mHeight > 0) {
        main_loop_step(main_loop());
    }
}


void AwtkApp::DispatchPointerDown(int button, int x, int y) {
    mPressed = true;

    pointer_event_t event;
    widget_t *widget = window_manager();
    pointer_event_init(&event, EVT_POINTER_DOWN, widget, x, y);
    event.pressed = mPressed;
    event.button = button;

    event_queue_req_t r;
    memset(&r, 0x00, sizeof(r));
    r.pointer_event = event;
    main_loop_queue_event(main_loop(), &r);
}

void AwtkApp::DispatchPointerMove(int button, int x, int y) {
    pointer_event_t event;
    widget_t *widget = window_manager();
    pointer_event_init(&event, EVT_POINTER_MOVE, widget, x, y);
    event.pressed = mPressed;
    event.button = button;

    event_queue_req_t r;
    memset(&r, 0x00, sizeof(r));
    r.pointer_event = event;
    main_loop_queue_event(main_loop(), &r);
}

void AwtkApp::DispatchPointerUp(int button, int x, int y) {
    pointer_event_t event;
    widget_t *widget = window_manager();
    pointer_event_init(&event, EVT_POINTER_UP, widget, x, y);
    event.pressed = mPressed;
    event.button = button;

    event_queue_req_t r;
    memset(&r, 0x00, sizeof(r));
    r.pointer_event = event;
    main_loop_queue_event(main_loop(), &r);

    mPressed = false;
}

void AwtkApp::Flush(void) { eglSwapBuffers(mEGLDisplay, mEGLSurface); }

int32_t AwtkApp::Quit(void) {
    EGLBoolean ret = eglDestroySurface(mEGLDisplay, mEGLSurface);
    if (!ret) {
        LOGW("eglDestroySurface failure.");
    }

    ret = eglDestroyContext(mEGLDisplay, mEGLContext);
    if (!ret) {
        LOGW("eglDestroyContext failure.");
    }

    ret = eglTerminate(mEGLDisplay);
    if (!ret) {
        LOGW("eglTerminate failure.");
    }

    mEGLSurface = NULL;
    mEGLContext = NULL;
    mEGLDisplay = NULL;
    application_exit();
    native_window_harmony_deinit();
    LOGE("Quit success.");
    return 0;
}

double AwtkApp::GetDevicePixelRatio() { return mDevicePixelRatio; }

bool AwtkApp::GetMetrics(int &x, int &y, uint32_t &width, uint32_t &height, float &devicePixelRatio) {
    x = 0;
    y = 0;
    width = mWidth;
    height = mHeight;
    devicePixelRatio = mDevicePixelRatio;

    return true;
}
