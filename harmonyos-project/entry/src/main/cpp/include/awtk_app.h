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

#ifndef AWTK_APP_H
#define AWTK_APP_H

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <string>
#include <stdint.h>
#include "awtk.h"

class AwtkApp {
public:
    explicit AwtkApp(std::string &id)
        : id(id), mWidth(0), mHeight(0), mScreenDensity(0), mPressed(false), mDevicePixelRatio(0){};
    int32_t PreInit(void *windowHandle, int windowWidth, int windowHeight);
    int32_t Init(const char *filesDir, const char *tempDir, int32_t screenDensity, double fontScale,
                 const char *language, const void *assetsZipContent, uint32_t assetsZipSize);

    void Update(void);
    void Flush(void);
    int32_t Quit(void);

    void DispatchPointerDown(int button, int x, int y);
    void DispatchPointerMove(int button, int x, int y);
    void DispatchPointerUp(int button, int x, int y);

public:
    double GetDevicePixelRatio();
    bool GetMetrics(int &x, int &y, uint32_t &width, uint32_t &height, float &devicePixelRatio);

public:
    std::string id;

private:
    int mWidth;
    int mHeight;
    int mScreenDensity;
    bool_t mPressed;
    double mDevicePixelRatio;

    EGLNativeWindowType mEglWindow;
    EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
    EGLConfig mEGLConfig = nullptr;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    EGLContext mSharedEGLContext = EGL_NO_CONTEXT;
    EGLSurface mEGLSurface = nullptr;
};

#endif /* AWTK_APP_H */
