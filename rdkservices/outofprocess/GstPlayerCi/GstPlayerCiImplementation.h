/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#pragma once

#include "Module.h"
#include <interfaces/IGstPlayerCi.h>
#include <gst/gst.h>
#include <lib.h>
#include <essos.h>
#include <thread>
#include <chrono>

#include <list>
#include <string>



namespace WPEFramework {
namespace Plugin {

    class GstPlayerCiImplementation : public Exchange::IGstPlayerCi {
    public:
        GstPlayerCiImplementation(const GstPlayerCiImplementation&) = delete;
        GstPlayerCiImplementation& operator=(const GstPlayerCiImplementation&) = delete;

        static GstPlayerCiImplementation* s_instance;


        
        public:
            GstPlayerCiImplementation()
                : _adminLock()
                , _servicePI(nullptr)
            {
                s_instance = this;
            }

            ~GstPlayerCiImplementation();


        BEGIN_INTERFACE_MAP(GstPlayerCiImplementation)
            INTERFACE_ENTRY(Exchange::IGstPlayerCi)
        END_INTERFACE_MAP

        //   IPackager methods
        uint32_t Initialize();
        uint32_t Deinitialize();
        void InitThread();
        void Register();
        void Unregister();
        bool setfile(string url);
        bool play();
        bool pause();
        bool FastForward();
        bool loop();
        bool exit();

    private:
        Core::CriticalSection _adminLock;
        PluginHost::IShell* _servicePI;
        EssCtx * mEssosContext;
        bool mUseWayland;
        bool flag;
        std::thread mEventLoopThread;

    public:
        CustomData* data = new CustomData();
    };

};  // namespace Plugin
}  // namespace WPEFramework
