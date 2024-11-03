import os
import sys
import glob
import json
import shutil

AWTK_ROOT = os.path.normpath(os.path.abspath(os.path.join(os.getcwd(), "../awtk")))
sys.path.append(os.path.join(AWTK_ROOT, "scripts"))

import mobile_project_helper as helper

WORK_DIR = os.path.abspath(os.path.normpath(os.getcwd()))
BUILD_DIR = helper.join_path(WORK_DIR, "build")
TEMPLATE_DIR = helper.join_path(WORK_DIR, "harmonyos-project")

print("WORK_DIR:" + WORK_DIR)


def replace_title(app_root_dst, title):
    for lang in title:
        value = title[lang]
        lang_file = helper.join_path(
            app_root_dst, "entry/src/main/resources/" + lang + "/element/string.json"
        )
        if os.path.exists(lang_file):
            helper.file_replace(lang_file, "awtktitle", value)
        if lang == "en_US":
            lang_file = helper.join_path(
                app_root_dst, "entry/src/main/resources/base/element/string.json"
            )
            if os.path.exists(lang_file):
                helper.file_replace(lang_file, "awtktitle", value)


def replace_files_content(app_root_dst, config):
    files = ["AppScope/app.json5"]
    helper.files_replace_with_config(files, app_root_dst, config)
    if "title" in config:
        replace_title(app_root_dst, config["title"])


def show_result(app_name):
    print("====================================================")
    print("project is created at: build/" + app_name)
    print("====================================================")


def copy_app_icon(app_icon, app_root_dst):
    if os.path.exists(app_icon):
        helper.copy_file(
            app_icon,
            helper.join_path(
                app_root_dst, "entry/src/main/resources/base/media/icon.png"
            ),
        )
        helper.copy_file(
            app_icon,
            helper.join_path(
                app_root_dst, "entry/src/main/resources/base/media/startIcon.png"
            ),
        )
        helper.copy_file(
            app_icon,
            helper.join_path(
                app_root_dst, "AppScope/resources/base/media/app_icon.png"
            ),
        )
    else:
        print("app_icon not exist:" + app_icon)


def create_project(config, app_root_src):
    app_name = helper.config_get_app_name(config)
    app_root_dst = helper.join_path(BUILD_DIR, app_name)
    app_icon = helper.join_path(app_root_src, helper.config_get_app_icon(config))
    awtk_dst_source_dir = helper.join_path(app_root_dst, "entry/src/main/cpp/awtk")
    app_dst_source_dir = helper.join_path(app_root_dst, "entry/src/main/cpp/app")
    assets_dst_dir = "entry/src/main/resources/rawfile/assets.zip"

    copy_app_icon(app_icon, app_root_dst)
    helper.copy_folder(TEMPLATE_DIR, app_root_dst)
    replace_files_content(app_root_dst, config)
    helper.copy_awtk_files(awtk_dst_source_dir)
    helper.copy_file(helper.join_path(AWTK_ROOT, "scripts/awtk_mobile_common.mk"), helper.join_path(app_root_dst, "entry/src/main/cpp/awtk_common.mk"))
    helper.copy_app_sources(config, app_dst_source_dir, app_root_src)
    helper.create_assets_zip(app_root_src, app_root_dst, assets_dst_dir)
    helper.update_cmake_file(config, helper.join_path(app_root_dst, "entry/src/main/cpp/CMakeLists.txt"))

    show_result(app_name)


app_json = ""
if len(sys.argv) < 2:
    helper.show_usage()
else:
    app_json = os.path.abspath(sys.argv[1])

config = helper.load_config(app_json, "harmonyos")
create_project(config, os.path.dirname(app_json))
