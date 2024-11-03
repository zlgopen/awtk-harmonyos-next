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

#include <cstdio>
#include <js_native_api.h>
#include <js_native_api_types.h>
#include <cstdint>

#include "native_common.h"
#include "log.h"
#include "napi_manager.h"

#include "awtk_app.h"
#include "app_napi.h"
#include "rawfile/raw_file_manager.h"
#include "rawfile/raw_file.h"
#include "rawfile/raw_dir.h"

std::unordered_map<std::string, AppNapi *> AppNapi::instance_;
OH_NativeXComponent_Callback AppNapi::callback_;
static AwtkApp *awtk_app_;

uint32_t AppNapi::isCreated_ = 0;
uint32_t AppNapi::xcHeight_ = 0;
uint32_t AppNapi::xcWidth_ = 0;
double AppNapi::off_x = 0;
double AppNapi::off_y = 0;
uint32_t AppNapi::toolType_ = 5;
uint32_t AppNapi::mousecallback_ = 0;
float AppNapi::tiltX_ = 0;
float AppNapi::tiltY_ = 0;
uint32_t AppNapi::touchType = 4;
OH_NativeXComponent_TouchEvent AppNapi::testTouchEvent_;
OH_NativeXComponent_MouseEvent AppNapi::testMouseEvent_;
OH_NativeXComponent_MouseEvent_Callback AppNapi::mouseEventcallback_;

std::unique_ptr<uint8_t[]> AppNapi::assetsData;

AwtkApp *AppNapi::getAwtkApp(void) { return awtk_app_; }

static int Normalize(int angle) {
    int ret = angle % CIRCUMFERENCE_DEGREE;
    if (ret < 0) {
        ret += CIRCUMFERENCE_DEGREE;
    }

    return ret;
}

static void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window) {
    LOGE("OnSurfaceCreatedCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    std::string id(idStr);
    auto instance = AppNapi::GetInstance(id);
    instance->OnSurfaceCreated(component, window);
}

static void OnSurfaceChangedCB(OH_NativeXComponent *component, void *window) {
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    std::string id(idStr);
    auto instance = AppNapi::GetInstance(id);
    instance->OnSurfaceChanged(component, window);
}

static void OnSurfaceDestroyedCB(OH_NativeXComponent *component, void *window) {
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    std::string id(idStr);
    auto instance = AppNapi::GetInstance(id);
    awtk_app_->Quit();
    instance->OnSurfaceDestroyed(component, window);
}

static void DispatchTouchEventCB(OH_NativeXComponent *component, void *window) {
    LOGE("DispatchTouchEventCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    std::string id(idStr);
    auto instance = AppNapi::GetInstance(id);
    instance->DispatchTouchEvent(component, window);
}

static void DispatchMouseEventCB(OH_NativeXComponent *component, void *window) {
    LOGD("DispatchMouseEventCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    std::string id(idStr);
    auto instance = AppNapi::GetInstance(id);
    instance->DispatchMouseEvent(component, window);
}

AppNapi::AppNapi(std::string &id) {
    id_ = id;
    component_ = nullptr;
    auto appCallback = AppNapi::GetNXComponentCallback();
    appCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
    appCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    appCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    appCallback->DispatchTouchEvent = DispatchTouchEventCB;
    auto appMouseEventCallback = AppNapi::GetNXComponentMouseEventCallback();
    appMouseEventCallback->DispatchMouseEvent = DispatchMouseEventCB;

    awtk_app_ = new AwtkApp(id);
}

AppNapi *AppNapi::GetInstance(std::string &id) {
    if (instance_.find(id) == instance_.end()) {
        AppNapi *instance = new AppNapi(id);
        instance_[id] = instance;
        return instance;
    } else {
        return instance_[id];
    }
}

OH_NativeXComponent_Callback *AppNapi::GetNXComponentCallback() { return &AppNapi::callback_; }

OH_NativeXComponent_MouseEvent_Callback *AppNapi::GetNXComponentMouseEventCallback() {
    return &AppNapi::mouseEventcallback_;
}

void AppNapi::SetNativeXComponent(OH_NativeXComponent *component) {
    component_ = component;
    OH_NativeXComponent_RegisterCallback(component_, &AppNapi::callback_);
    uint32_t mousecallback = OH_NativeXComponent_RegisterMouseEventCallback(component_, &AppNapi::mouseEventcallback_);
    mousecallback_ = mousecallback;
}

void AppNapi::OnSurfaceCreated(OH_NativeXComponent *component, void *window) {
    LOGE("AppNapi::OnSurfaceCreated");

    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);

    LOGE("Offset : x = %{public}f, y = %{public}f ", x_, y_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        awtk_app_->PreInit(window, width_, height_);
        awtk_app_->Update();
        isCreated_++;
        xcHeight_ = height_;
        xcWidth_ = width_;

        LOGE("AppNapi::OnSurfaceCreated success ");
    } else {
        LOGE("AppNapi::OnSurfaceCreated failed");
    }
}

void AppNapi::OnSurfaceChanged(OH_NativeXComponent *component, void *window) {
    LOGE("AppNapi::OnSurfaceChanged");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    int32_t ret1;
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        xcHeight_ = height_;
        xcWidth_ = width_;
        LOGE("after width = %{public}d, height = %{public}d", xcWidth_, xcHeight_);
        ret1 = OH_NativeXComponent_GetXComponentOffset(component, window, &x_, &y_);
        off_x = x_;
        off_y = y_;

        if (ret1 == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
            LOGE("Offset : x = %{public}lf, y = %{public}lf ", off_x, off_y);
        } else {
            LOGE("Offset get failed");
        }

        LOGE("AppNapi::GetOffset ");
        LOGE("Offset : x = %{public}lf, y = %{public}lf ", off_x, off_y);
    }
}

void AppNapi::OnSurfaceDestroyed(OH_NativeXComponent *component, void *window) {
    LOGE("AppNapi::OnSurfaceDestroyed");
    isCreated_--;
    LOGE("AppNapi::OnSurfaceDestroyed iscreated %{public}d", isCreated_);
}

void AppNapi::DispatchTouchEvent(OH_NativeXComponent *component, void *window) {
    int32_t ret = OH_NativeXComponent_GetTouchEvent(component, window, &touchEvent_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        double dpr = awtk_app_->GetDevicePixelRatio();
        int x = touchEvent_.x / dpr;
        int y = touchEvent_.y / dpr;
        int button = 0;

        switch (touchEvent_.type) {
        case OH_NATIVEXCOMPONENT_DOWN: {
            awtk_app_->DispatchPointerDown(button, x, y);
            break;
        }
        case OH_NATIVEXCOMPONENT_MOVE: {
            awtk_app_->DispatchPointerMove(button, x, y);
            break;
        }
        case OH_NATIVEXCOMPONENT_UP: {
            awtk_app_->DispatchPointerUp(button, x, y);
            break;
        }
        default: {
            break;
        }
        }
    } else {
        LOGE("Touch fail");
    }
}

void AppNapi::DispatchMouseEvent(OH_NativeXComponent *component, void *window) {
    int32_t ret = OH_NativeXComponent_GetMouseEvent(component, window, &mouseEvent_);
    LOGE("Mouse Info DispatchMouseEvent");
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        double dpr = awtk_app_->GetDevicePixelRatio();
        int x = mouseEvent_.x / dpr;
        int y = mouseEvent_.y / dpr;
        int button = mouseEvent_.button;

        switch (mouseEvent_.action) {
        case OH_NATIVEXCOMPONENT_MOUSE_PRESS: {
            awtk_app_->DispatchPointerDown(button, x, y);
            break;
        }
        case OH_NATIVEXCOMPONENT_MOUSE_MOVE: {
            awtk_app_->DispatchPointerMove(button, x, y);
            break;
        }
        case OH_NATIVEXCOMPONENT_MOUSE_RELEASE: {
            awtk_app_->DispatchPointerUp(button, x, y);
            break;
        }
        default: {
            break;
        }
        }
    } else {
        LOGE("Mouse Info fail");
    }
    awtk_app_->Update();
}

napi_value AppNapi::Init(napi_env env, napi_callback_info info) {
    double screenDensity = 640;
    double fontScale = 1;
    void *assetsZipContent = NULL;
    uint32_t assetsZipSize = 0;

    size_t argc = 6;
    napi_value argv[6] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    napi_valuetype valueType;
    napi_typeof(env, argv[0], &valueType);
    NativeResourceManager *mNativeResMgr = OH_ResourceManager_InitNativeResourceManager(env, argv[0]);

    size_t strSize;
    char strBuf[MAX_PATH + 1] = {0};
    napi_get_value_string_utf8(env, argv[1], strBuf, sizeof(strBuf), &strSize);
    std::string filesDir(strBuf, strSize);

    napi_get_value_string_utf8(env, argv[2], strBuf, sizeof(strBuf), &strSize);
    std::string tempDir(strBuf, strSize);

    napi_get_value_double(env, argv[3], &screenDensity);
    napi_get_value_double(env, argv[4], &fontScale);

    napi_get_value_string_utf8(env, argv[5], strBuf, sizeof(strBuf), &strSize);
    std::string language(strBuf, strSize);

    std::string filename = "";
    RawDir *rawDir = OH_ResourceManager_OpenRawDir(mNativeResMgr, filename.c_str());
    int count = OH_ResourceManager_GetRawFileCount(rawDir);
    std::vector<std::string> tempArray;
    for (int i = 0; i < count; i++) {
        std::string filename = OH_ResourceManager_GetRawFileName(rawDir, i);
        tempArray.emplace_back(filename);
    }

    filename = "assets.zip";
    RawFile *rawFile = OH_ResourceManager_OpenRawFile(mNativeResMgr, filename.c_str());
    if (rawFile != nullptr) {
    }
    long len = OH_ResourceManager_GetRawFileSize(rawFile);
    assetsData = std::make_unique<uint8_t[]>(len);

    int res = OH_ResourceManager_ReadRawFile(rawFile, assetsData.get(), len);

    OH_ResourceManager_CloseRawFile(rawFile);
    OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);

    assetsZipContent = assetsData.get();
    assetsZipSize = len;
    awtk_app_->Init(filesDir.c_str(), tempDir.c_str(), screenDensity, fontScale, language.c_str(), assetsZipContent, assetsZipSize);

    napi_value ret;
    napi_create_int32(env, 0, &ret);

    return ret;
}

napi_value AppNapi::Update(napi_env env, napi_callback_info info) {
    napi_value ret;
    awtk_app_->Update();
    napi_create_int32(env, 0, &ret);

    return ret;
}

napi_value AppNapi::Quit(napi_env env, napi_callback_info info) {
    LOGE("AppNapi -> Quit");
    napi_value exportInstance;
    napi_value thisArg;
    napi_status status;
    OH_NativeXComponent *nativeXComponent = nullptr;

    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;

    awtk_app_->Quit();

    NAPI_CALL(env, napi_get_cb_info(env, info, NULL, NULL, &thisArg, NULL));

    status = napi_get_named_property(env, thisArg, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return nullptr;
    }

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent));
    if (status != napi_ok) {
        return nullptr;
    }

    ret = OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return nullptr;
    }

    return 0;
}

napi_value AppNapi::GetXComponentId(napi_env env, napi_callback_info info) {
    napi_value thisArg;
    napi_status status;
    napi_value exportInstance;
    OH_NativeXComponent *nativeXComponent = nullptr;

    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;

    NAPI_CALL(env, napi_get_cb_info(env, info, NULL, NULL, &thisArg, NULL));
    status = napi_get_named_property(env, thisArg, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance);
    status = napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent));
    ret = OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return nullptr;
    }

    std::string id(idStr);

    napi_value output;
    NAPI_CALL(env, napi_create_string_utf8(env, idStr, id.length(), &output));

    return output;
}

napi_value AppNapi::GetXComponentSize_Height(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentSize_Height");
    napi_value output;
    NAPI_CALL(env, napi_create_uint32(env, xcHeight_, &output));
    LOGE(" GetXComponentSize_Height %{public}d ", xcHeight_);
    return output;
}

napi_value AppNapi::GetXComponentSize_Width(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentSize_Width");
    napi_value output;
    NAPI_CALL(env, napi_create_uint32(env, xcWidth_, &output));
    LOGE(" GetXComponentSize_Width %{public}d ", xcWidth_);
    return output;
}

napi_value AppNapi::GetXComponentOffset_x(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentOffset_x");

    napi_value output;
    NAPI_CALL(env, napi_create_double(env, off_x, &output));
    LOGE("GetXComponentOffset_x : %{public}f", off_x);

    return output;
}

napi_value AppNapi::GetXComponentOffset_y(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentOffset_y");

    napi_value output;
    NAPI_CALL(env, napi_create_double(env, off_y, &output));
    LOGE("GetXComponentOffset_y : %{public}f", off_y);

    return output;
}

napi_value AppNapi::GetXComponentpointtool_tiltx(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentpointtool_tiltx");

    napi_value output;
    NAPI_CALL(env, napi_create_double(env, tiltX_, &output));
    LOGE("GetXComponentpointtool_tiltx : %{public}f", tiltX_);

    return output;
}

napi_value AppNapi::GetXComponentpointtool_tilty(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentpointtool_tilty");

    napi_value output;
    NAPI_CALL(env, napi_create_double(env, tiltY_, &output));
    LOGE("GetXComponentpointtool_tilty : %{public}f", tiltY_);

    return output;
}

napi_value AppNapi::GetXComponentpointtool_type(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponentpointtool_type");

    napi_value output;
    NAPI_CALL(env, napi_create_double(env, toolType_, &output));
    LOGE("GetXComponentpointtool_type : %{public}u", toolType_);

    return output;
}

napi_value AppNapi::GetXComponent_TouchEvent(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponent_TouchEvent");

    napi_value surf_x;
    napi_value surf_y;
    napi_value t_type;

    NAPI_CALL(env, napi_create_double(env, testTouchEvent_.x, &(surf_x)));
    NAPI_CALL(env, napi_create_double(env, testTouchEvent_.y, &(surf_y)));
    NAPI_CALL(env, napi_create_uint32(env, testTouchEvent_.type, &(t_type)));

    napi_value obj;
    NAPI_CALL(env, napi_create_object(env, &obj));
    NAPI_CALL(env, napi_set_named_property(env, obj, "surface_X", surf_x));
    NAPI_CALL(env, napi_set_named_property(env, obj, "surface_Y", surf_y));
    NAPI_CALL(env, napi_set_named_property(env, obj, "touchType", t_type));

    return obj;
}

napi_value AppNapi::GetXComponent_MouseEvent(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponent_MouseEvent");

    napi_value surf_x;
    napi_value surf_y;
    napi_value t_button;

    NAPI_CALL(env, napi_create_double(env, testMouseEvent_.x, &(surf_x)));
    NAPI_CALL(env, napi_create_double(env, testMouseEvent_.y, &(surf_y)));
    NAPI_CALL(env, napi_create_uint32(env, testMouseEvent_.button, &(t_button)));

    napi_value obj;
    NAPI_CALL(env, napi_create_object(env, &obj));
    NAPI_CALL(env, napi_set_named_property(env, obj, "surface_X1", surf_x));    // float x
    NAPI_CALL(env, napi_set_named_property(env, obj, "surface_Y1", surf_y));    // float y
    NAPI_CALL(env, napi_set_named_property(env, obj, "mousebutton", t_button)); // int32_t

    return obj;
}

napi_value AppNapi::GetXComponent_RegisterMouseEventCallback(napi_env env, napi_callback_info info) {
    LOGE("running AppNapi::GetXComponent_RegisterMouseEventCallback");

    napi_value callback_;
    NAPI_CALL(env, napi_create_double(env, mousecallback_, &(callback_)));

    napi_value obj;
    NAPI_CALL(env, napi_create_object(env, &obj));
    NAPI_CALL(env, napi_set_named_property(env, obj, "MouseCallback_", callback_)); // float x

    return obj;
}

napi_value AppNapi::Export(napi_env env, napi_value exports) {
    LOGE("AppNapi::Export");
    // Register NAPI
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("Quit", AppNapi::Quit),
        DECLARE_NAPI_FUNCTION("GetXComponentId", AppNapi::GetXComponentId),
        DECLARE_NAPI_FUNCTION("GetXComponentSize_Height", AppNapi::GetXComponentSize_Height),
        DECLARE_NAPI_FUNCTION("GetXComponentSize_Width", AppNapi::GetXComponentSize_Width),
        DECLARE_NAPI_FUNCTION("GetXComponentOffset_x", AppNapi::GetXComponentOffset_x),
        DECLARE_NAPI_FUNCTION("GetXComponentOffset_y", AppNapi::GetXComponentOffset_y),
        DECLARE_NAPI_FUNCTION("GetXComponent_TouchEvent", AppNapi::GetXComponent_TouchEvent),
        DECLARE_NAPI_FUNCTION("GetXComponent_MouseEvent", AppNapi::GetXComponent_MouseEvent),
        DECLARE_NAPI_FUNCTION("GetXComponentpointtool_tilty", AppNapi::GetXComponentpointtool_tilty),
        DECLARE_NAPI_FUNCTION("GetXComponentpointtool_type", AppNapi::GetXComponentpointtool_type),
        DECLARE_NAPI_FUNCTION("GetXComponentpointtool_tiltx", AppNapi::GetXComponentpointtool_tiltx),
        DECLARE_NAPI_FUNCTION("GetXComponent_RegisterMouseEventCallback",
                              AppNapi::GetXComponent_RegisterMouseEventCallback),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}
