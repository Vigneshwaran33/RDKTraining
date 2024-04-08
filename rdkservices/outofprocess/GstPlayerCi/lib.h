#include<iostream>
#include<gst/gst.h>
#include<string>

using namespace std;

struct CustomData{
    string file;
    GstElement* pipeline = nullptr;
    GstElement* westeros_sink = nullptr;
    GstEvent* seek_event = nullptr;
    gdouble playback_speed;
    

    void setPath(CustomData* data,string url);
    void play(CustomData* data);
    void pause(CustomData* data);
    void FastForward(CustomData* data,int speed);
    void loop(CustomData* data);
    void exit(CustomData* data);
};



