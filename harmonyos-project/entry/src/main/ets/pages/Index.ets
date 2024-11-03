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

import { Logger } from '../utils/Logger';
import awtk_napi from 'libawtk_napi.so'
import { AbilityConstant } from '@kit.AbilityKit';
import UIAbility from '@ohos.app.ability.UIAbility';
import { common } from '@kit.AbilityKit';

@Entry
@Component
struct Index {
  private xcomponentId = 'awtk_app';
  private refreshTimerID: number = 0;

  async aboutToAppear() {
    this.getUIContext()
    Logger.info('aboutToAppear');
  }

  aboutToDisappear() {
    console.info('IndexComponent aboutToDisappear');
  }

  onPageShow() {
    this.refreshTimerID = setInterval(() => {
      awtk_napi.update();
    }, 16);
  }

  onPageHide() {
    clearTimeout(this.refreshTimerID);
    this.refreshTimerID = 0;
  }

  build() {
    Column() {
      XComponent({ id: this.xcomponentId, type: 'surface', libraryname: 'awtk_napi' })
        .onLoad(() => {
          let context = getContext(this) as common.UIAbilityContext;
          let filesDir: string = context.filesDir;
          let tempDir: string = context.tempDir;
          let resourceManager = context.resourceManager;
          let resourceDir = context.resourceDir;
          let screenDensity: number = context.config.screenDensity || 640;
          let fontScale:number = context.config.fontSizeScale || 1;
          let language:string = context.config.language || "zh_CN";
          awtk_napi.init(resourceManager, filesDir, tempDir, screenDensity, fontScale, language);
        })
        .width('100%')
        .height('100%')
        .key('awtk_app')
        .onDestroy(() => {
          Logger.info('onDestroy');
        })
        .id('xComponent')
    }
    .width('100%')
    .height('100%')
  }
}
