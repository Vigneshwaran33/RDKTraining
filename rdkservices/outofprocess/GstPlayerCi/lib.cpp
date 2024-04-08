#include<lib.h>

void CustomData::setPath(CustomData *data,string url)
{
    gst_init(NULL,NULL);
    data->file = "playbin uri=" + url;
    data->pipeline = gst_parse_launch(data->file.c_str(),nullptr);
    westeros_sink = gst_element_factory_make("westerossink", NULL);
    if (!westeros_sink) {
        g_printerr("Failed to create westerossink element. Exiting.\n");
    }
    g_object_set(G_OBJECT(data->pipeline), "video-sink", westeros_sink, NULL);
    //gst_element_set_state(data->pipeline,GST_STATE_PLAYING);
    std::cout<<"TESTPLAYING";
}

void CustomData::play(CustomData *data)
{
    data->FastForward(data,1);
    gst_element_set_state(data->pipeline,GST_STATE_PLAYING);
    std::cout<<"TESTPLAYING";
}

void CustomData::pause(CustomData *data)
{
    gst_element_set_state(data->pipeline,GST_STATE_PAUSED);
    std::cout<<"TESTPAUSE";
}

void CustomData::FastForward(CustomData *data,int speed)
{
  data->playback_speed = speed;
  gint64 position; 
  GstEvent *seek_event; 
  if (!gst_element_query_position (data->pipeline, GST_FORMAT_TIME, &position)) { 
    std::cout<<"DBG[VR] Unable to retrieve current position.\n";
    return; 
  } 
  if (data->playback_speed > 0) { 
    seek_event = 
        gst_event_new_seek (data->playback_speed, GST_FORMAT_TIME, 
        static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE), GST_SEEK_TYPE_SET, 
        position, GST_SEEK_TYPE_END, 0);
  } else { 
    seek_event = 
        gst_event_new_seek (data->playback_speed, GST_FORMAT_TIME, 
        static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE), GST_SEEK_TYPE_SET, 0, 
        GST_SEEK_TYPE_SET, position);  
  } 
   
  gst_element_send_event (data->westeros_sink, seek_event); 
  g_print ("Current rate: %g\n", data->playback_speed); 
}

void CustomData::loop(CustomData *data)
{
   
}

void CustomData::exit(CustomData *data)
{   
    gst_element_set_state(data->pipeline,GST_STATE_NULL);
    gst_object_unref (data->pipeline);
}

