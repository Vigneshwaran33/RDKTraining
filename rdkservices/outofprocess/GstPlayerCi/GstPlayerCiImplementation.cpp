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
 
#include "GstPlayerCiImplementation.h"

#include <gst/gst.h>
#include <string>
#include <iostream>

using namespace std;
namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(GstPlayerCiImplementation, 1, 0);

    GstPlayerCiImplementation* GstPlayerCiImplementation::s_instance = nullptr;


    GstPlayerCiImplementation::~GstPlayerCiImplementation()
    {
        _servicePI->Release();
        _servicePI = nullptr;
    }

        
        static void essosKeyPressed( void *userData, unsigned int key )
        {
          std::cout<<"DBG[VR] key press 200 ok : playerci"<<key<<std::endl;
          if(GstPlayerCiImplementation::s_instance)
          {
          if(key==28)
          GstPlayerCiImplementation::s_instance->play();
          else if(key==13)
          GstPlayerCiImplementation::s_instance->pause();
          else if(key==106)
          GstPlayerCiImplementation::s_instance->FastForward();
          else if(key==105)
          GstPlayerCiImplementation::s_instance->loop();  
          else if(key==38)
          GstPlayerCiImplementation::s_instance->exit();
          }
          else
          std::cout<<"DBG[VR] s_instance is null";
        }

        static void essosKeyReleased( void *userData, unsigned int key )
        {
          std::cout<<"DBG[VR] key release 200 ok: palyerci"<<key<<std::endl;
        }
        



    void GstPlayerCiImplementation::InitThread()
    {
        static EssKeyListener essosKeyListener=
        {
         essosKeyPressed,
         essosKeyReleased
        };
        
        std::cout<<"DBG[VR] InitThread"<<std::endl;
        //Create Display

        //Key handler
        //mUseWayland=false;
        
        std::cout<<"DBG[VR] Keylistener Implementation"<<std::endl;
        mEssosContext = EssContextCreate();

        if(mEssosContext){
       /* if ( !EssContextSetUseWayland( mEssosContext, mUseWayland ) )
            {
               std::cout<<"DBG[VR] setusewayland could not be created"<<std::endl;
            } */
            
        if( !EssContextSetKeyListener( mEssosContext, 0, &essosKeyListener ))
           {
            std::cout<<"DBG[VR] setkeylistener could not be created"<<std::endl;
           }
 
        if ( !EssContextStart( mEssosContext ) )
            {
             std::cout<<"DBG[VR] contextstart could not be created"<<std::endl;
            }
        }
        else
        std::cout<<"DBG[VR] couldn't create mEssosContext";
        
        flag=true;
        while(flag)
            {
            std::cout<<"DBG[VR] inside Run loop"<<std::endl;
            //EssContextUpdateDisplay(mEssosContext);
            EssContextRunEventLoopOnce(mEssosContext);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  
            }

    }

    uint32_t GstPlayerCiImplementation::Initialize()
    {
          
        std::cout<<"DBG[VR] Initialize1"<<std::endl;
        mEventLoopThread = std::thread(&GstPlayerCiImplementation::InitThread,this);
        return Core::ERROR_NONE;
    }

    uint32_t GstPlayerCiImplementation::Deinitialize()
    {
        flag=false;
        mEventLoopThread.join();
        std::cout<<"DBG[VR] Deinitialize"<<std::endl;
        return Core::ERROR_NONE;
    }

    void GstPlayerCiImplementation::Register()
    {
    }

    void GstPlayerCiImplementation::Unregister()
    {
    }

    bool GstPlayerCiImplementation::setfile(string url)
    {
        data->setPath(data,url);
        cout<<"IMPTESTPLAYING";
        return true;
    }
    
    bool GstPlayerCiImplementation::play()
    {
        data->play(data);
        return true;
    }

   bool GstPlayerCiImplementation::pause()
    {
        data->pause(data);
        return true;
    }

   bool GstPlayerCiImplementation::FastForward()
    {
        data->FastForward(data,2);
        std::cout<<"TESTFF"<<std::endl;
        return true;
    }

    bool GstPlayerCiImplementation::loop()
    {
        data->loop(data);
        return true;
    }

    bool GstPlayerCiImplementation::exit()
    {
        data->exit(data);
        return true;
    }

   

}  // namespace Plugin
}  // namespace WPEFramework
