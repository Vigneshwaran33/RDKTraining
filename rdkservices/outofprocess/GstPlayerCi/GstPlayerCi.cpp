/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
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
**/

#include "GstPlayerCi.h"
#include "UtilsJsonRpc.h"
#include <iostream>
#include <glib.h>
#include <glib/gstdio.h>
#include <essos.h>

#define API_VERSION_NUMBER_MAJOR 1
#define API_VERSION_NUMBER_MINOR 0
#define API_VERSION_NUMBER_PATCH 0

using namespace std;

namespace WPEFramework {

    namespace {

        static Plugin::Metadata<Plugin::GstPlayerCi> metadata(
            // Version (Major, Minor, Patch)
            API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {}
        );
    }

    namespace Plugin {

        SERVICE_REGISTRATION(GstPlayerCi, API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH);
        

		const string GstPlayerCi::Initialize(PluginHost::IShell* service) {
        ASSERT (_service == nullptr);
        ASSERT (service != nullptr);

        setenv("WAYLAND_DISPLAY","wst-GstPlayerCi",1);
        const char* wd = getenv("WAYLAND_DISPLAY");
        std::cout<<wd<<":DBG[VR]"<<std::endl;
        
        _service = service;
        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());
         string result;
        _implementation = _service->Root<Exchange::IGstPlayerCi>(_connectionId, 2000, _T("GstPlayerCiImplementation"));
         if (_implementation == nullptr) {
            result = _T("Couldn't create GstPlayerCi instance");
         }
         std::cout<<"DBG[VR] GstPlayerCi::Initialize test1"<<std::endl;
         //_implementation->Initialize(); 
        return (result);
    }

    void GstPlayerCi::Deinitialize(PluginHost::IShell* service)
    {
        //_implementation->Deinitialize();
        ASSERT(_service == service);

        if (_implementation->Release() != Core::ERROR_DESTRUCTION_SUCCEEDED) {

            ASSERT(_connectionId != 0);

            RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));

            // The process can disappear in the meantime...
            if (connection != nullptr) {

                // But if it did not dissapear in the meantime, forcefully terminate it. Shoot to kill :-)
                connection->Terminate();
                connection->Release();
            }
        }

        _service = nullptr;
        _implementation = nullptr;
    }
        uint32_t GstPlayerCi::setfilein(const JsonObject& parameters, JsonObject& response)
        {
            bool result = true;
            std::string url = parameters["url"].String();
            result = _implementation->setfile(url);
            if (!result)
            {
                response["message"] = "failed to set file path";
            }
            response["message"]="File path has been setted and video is playing";
            return Core::ERROR_NONE;
        }

        uint32_t GstPlayerCi::pausein(const JsonObject& parameters, JsonObject& response)
		{
		    bool result = true;
			result = _implementation->pause();
			if(!result)
			{
				response["message"] = "failed to set playbin to pause state";
			}
			response["playstate"]="paused";
            response["message"]="Video is paused";
			return Core::ERROR_NONE;
		}
        
        uint32_t GstPlayerCi::exitin(const JsonObject& parameters, JsonObject& response)
        {
            bool result = true;
            result = _implementation->exit();
            if(!result)
            {
                response["message"] = "failed to exit";
            }
            response["message"]="Video is stopped";
            return Core::ERROR_NONE;
        }
		

        string GstPlayerCi::Information() const
        {
            return (string());
        }


    void GstPlayerCi::Deactivated(RPC::IRemoteConnection* connection)
    {
        if (connection->Id() == _connectionId) {
            ASSERT(_service != nullptr);
            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service,
                PluginHost::IShell::DEACTIVATED,
                PluginHost::IShell::FAILURE));
        }
    }
	
	}
}
