#
#  Copyright (c) 2023 Huawei Device Co., Ltd.
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)

# project
project(awtk_napi)

set(NATIVE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(awtk_common.mk)

# include directory
include_directories(${NATIVE_ROOT_PATH}
  ${AWTK_COMMON_INCLUDES}
  ${AWTK_NANOVG_PLUS_INCLUDES}
  ${NATIVE_ROOT_PATH}/include
  ${NATIVE_ROOT_PATH}/include/util
  EXTRA_INCLUDES
  )


set(AWTK_HARMONY_OS_FLAGS " -DHARMONY_OS -DLINUX -DWITHOUT_AWTK_MAIN ")
set(AWTK_HARMONY_OS_FLAGS "${AWTK_HARMONY_OS_FLAGS} -DWITH_ASSET_LOADER_ZIP ${AWTK_VGCANVAS_NANOVG_PLUS_GLES3_FLAGS}")

set(AWTK_FLAGS "${AWTK_COMMON_FLAGS} ${AWTK_HARMONY_OS_FLAGS}")
set(AWTK_SOURCE_FILES ${AWTK_COMMON_SOURCE_FILES} ${AWTK_VGCANVAS_NANOVG_PLUS_SOURCE_FILES})

file(GLOB AWTK_PORT_SOURCE_FILES 
  awtk-port/*.cpp
)

set(AWTK_APP_FLAGS "-DUSE_GUI_MAIN -DAWTK_DEFAULT_FONT=\\\"AWTK_DEFAULT_FONT_NAME\\\" ")
set(CMAKE_C_FLAGS "${AWTK_FLAGS} ${AWTK_APP_FLAGS} EXTRA_CFLAGS")
set(CMAKE_CXX_FLAGS "${AWTK_FLAGS} ${AWTK_APP_FLAGS} EXTRA_CPPFLAGS")

# build shared library
add_library(awtk_napi SHARED
            ${AWTK_SOURCE_FILES}
            ${AWTK_PORT_SOURCE_FILES}
            ${AWTK_APP_SOURCE_FILES}
            module.cpp
            app_napi.cpp
            awtk_app.cpp
            napi_manager.cpp
            napi_util.cpp)

# link OpenHarmony native library
target_link_libraries(awtk_napi PUBLIC EGL)
target_link_libraries(awtk_napi PUBLIC GLESv3)
target_link_libraries(awtk_napi PUBLIC hilog_ndk.z)
target_link_libraries(awtk_napi PUBLIC ace_ndk.z)
target_link_libraries(awtk_napi PUBLIC ace_napi.z)
target_link_libraries(awtk_napi PUBLIC libc++.a)
target_link_libraries(awtk_napi PUBLIC z)
target_link_libraries(awtk_napi PUBLIC uv)
target_link_libraries(awtk_napi PUBLIC libace_napi.z.so)
target_link_libraries(awtk_napi PUBLIC librawfile.z.so)
target_link_libraries(awtk_napi PUBLIC libohinputmethod.so)
