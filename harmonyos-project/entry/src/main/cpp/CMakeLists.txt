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

set(AWTK_ROOT_PATH ${NATIVE_ROOT_PATH}/awtk)
set(AWTK_3RD_PATH ${AWTK_ROOT_PATH}/3rd)
set(AWTK_SRC_PATH ${AWTK_ROOT_PATH}/src)

# include directory
include_directories(${NATIVE_ROOT_PATH}
  ${AWTK_SRC_PATH}
  ${AWTK_3RD_PATH}
  ${AWTK_3RD_PATH}/glad
  ${AWTK_3RD_PATH}/nanovg
  ${AWTK_3RD_PATH}/nanovg/gl
  ${AWTK_3RD_PATH}/nanovg/base
  ${AWTK_3RD_PATH}/gpinyin/include
  ${AWTK_3RD_PATH}/libunibreak
  ${AWTK_SRC_PATH}/ext_widgets
  ${AWTK_SRC_PATH}/custom_widgets
  ${NATIVE_ROOT_PATH}/include
  ${NATIVE_ROOT_PATH}/include/util)

set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DMOBILE_APP -DWITH_ASSET_LOADER ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_STB_FONT -DWITHOUT_GLAD ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_VGCANVAS -DWITH_UNICODE_BREAK -DWITH_DESKTOP_STYLE ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DHAS_STD_MALLOC -DWITH_FS_RES -DHAS_STDIO ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_NANOVG_GPU -DWITH_VGCANVAS_LCD ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_GPU_GL ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DHAS_PTHREAD -DHAS_GET_TIME_US64=1 ")
set(AWTK_COMMON_FLAGS "${AWTK_COMMON_FLAGS} -DWITH_DATA_READER_WRITER=1 ")

set(AWTK_HARMONY_OS_FLAGS " -DHARMONY_OS -DLINUX -DWITH_NANOVG_GLES3 -DWITH_GPU_GLES3 -DNANOVG_GLES3 ")
set(AWTK_HARMONY_OS_FLAGS "${AWTK_HARMONY_OS_FLAGS} -DWITHOUT_MAIN_LOOP_INIT -DWITHOUT_NATIVE_WINDOW_CREATE ")
set(AWTK_HARMONY_OS_FLAGS "${AWTK_HARMONY_OS_FLAGS} -DWITH_ASSET_LOADER_ZIP ")

set(AWTK_FLAGS "${AWTK_COMMON_FLAGS} ${AWTK_HARMONY_OS_FLAGS}")

file(GLOB AWTK_SOURCE_FILES 
  ${AWTK_3RD_PATH}/libunibreak/*.c
  ${AWTK_3RD_PATH}/miniz/*.c
  ${AWTK_3RD_PATH}/nanovg/base/*.c
  ${AWTK_SRC_PATH}/tkc/*.c
  ${AWTK_SRC_PATH}/base/*.c
  ${AWTK_SRC_PATH}/layouters/*.c
  ${AWTK_SRC_PATH}/widgets/*.c
  ${AWTK_SRC_PATH}/ui_loader/*.c
  ${AWTK_SRC_PATH}/xml/*.c
  ${AWTK_SRC_PATH}/svg/*.c
  ${AWTK_SRC_PATH}/blend/*.c
  ${AWTK_SRC_PATH}/clip_board/*.c
  ${AWTK_SRC_PATH}/font_loader/*.c
  ${AWTK_SRC_PATH}/graphic_buffer/*.c
  ${AWTK_SRC_PATH}/ext_widgets/*.c
  ${AWTK_SRC_PATH}/ext_widgets/*/*.c
  ${AWTK_SRC_PATH}/custom_widgets/*.c
  ${AWTK_SRC_PATH}/custom_widgets/*/*.c
  ${AWTK_SRC_PATH}/custom_widgets/*.cc
  ${AWTK_SRC_PATH}/custom_widgets/*/*.cc
  ${AWTK_SRC_PATH}/custom_widgets/*.cxx
  ${AWTK_SRC_PATH}/custom_widgets/*/*.cxx
  ${AWTK_SRC_PATH}/custom_widgets/*.cpp
  ${AWTK_SRC_PATH}/custom_widgets/*/*.cpp
  ${AWTK_SRC_PATH}/image_loader/*.c
  ${AWTK_SRC_PATH}/widget_animators/*.c
  ${AWTK_SRC_PATH}/window_animators/*.c
  ${AWTK_SRC_PATH}/window_manager/window_manager_default.c
  ${AWTK_SRC_PATH}/dialog_highlighters/*.c
  ${AWTK_SRC_PATH}/*.c
  ${AWTK_SRC_PATH}/ubjson/*.c
  ${AWTK_SRC_PATH}/streams/serial/*.c
  ${AWTK_SRC_PATH}/streams/buffered/*.c
  ${AWTK_SRC_PATH}/streams/mem/*.c
  ${AWTK_SRC_PATH}/streams/file/*.c
  ${AWTK_SRC_PATH}/streams/inet/*.c
  ${AWTK_SRC_PATH}/streams/*.c
  ${AWTK_SRC_PATH}/csv/*.c
  ${AWTK_SRC_PATH}/conf_io/*.c
  ${AWTK_SRC_PATH}/fscript_ext/*.c
  ${AWTK_SRC_PATH}/charset/*.c
  ${AWTK_SRC_PATH}/input_engines/input_engine_pinyin.cpp
  ${AWTK_SRC_PATH}/lcd/lcd_nanovg.c
  ${AWTK_SRC_PATH}/main_loop/main_loop_simple.c
  ${AWTK_SRC_PATH}/main_loop/main_loop_console.c
  ${AWTK_SRC_PATH}/platforms/pc/*.c
  ${AWTK_SRC_PATH}/vgcanvas/vgcanvas_nanovg_gl.c  
  )

file(GLOB AWTK_PORT_SOURCE_FILES 
  awtk-port/*.cpp
)

set(AWTK_APP_FLAGS "-DUSE_GUI_MAIN ")
set(CMAKE_C_FLAGS "${AWTK_FLAGS} ${AWTK_APP_FLAGS}")
set(CMAKE_CXX_FLAGS "${AWTK_FLAGS} ${AWTK_APP_FLAGS}")

file(GLOB AWTK_APP_SOURCE_FILES 
  app/*.c
  app/*.cpp
  app/**/*.c
  app/**/*.cpp
)

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

# link OpenHarmony native library （Use OpenGL）
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
