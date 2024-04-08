#include <gst/gst.h>
//#include<graphviz/cgraph.h>
#include <curses.h>
#include <pthread.h>
#include<stdio.h>

typedef struct CustomData {
  GstElement *pipeline;
  GstElement *source;
  GstElement *demux;
  GstElement *queue_video;
  GstElement *h264parse;
  GstElement *avdec_h264;
  GstElement *videoscale;
  GstElement *videoconvert;
  GstElement *autovideosink;
  GstElement *queue_audio;
  GstElement *aacparse;
  GstElement *faad;
  GstElement *audioconvert;
  GstElement *autoaudiosink;
  volatile gboolean terminate;
  gdouble playback_speed;
} CustomData;

void *user_input_thread(void *data) {
    CustomData *custom_data = (CustomData *)data;
    while (!custom_data->terminate) {
        initscr();
        char c = getch();
        endwin();
        int new_state = -1;
        float speed=1;
        switch (c) {
            case 'n': // NULL
                new_state = GST_STATE_NULL;
                printf("%d",new_state);
                break;
            case 'p': // PAUSE
                new_state = GST_STATE_PAUSED;
                printf("%d",new_state);
                break;
            case 's': // PLAY
                new_state = GST_STATE_PLAYING;
                printf("%d",new_state);
                break;
            case 'f':
                printf("Enter + to speedup and - to slowdown");
                char ss;
                scanf("%c",&ss);
                printf("Enter the playback speed");
                scanf("%f",&speed);
                if(ss=='+')
                custom_data->playback_speed *= speed;
                else
                custom_data->playback_speed /= speed;
                send_seek_event (custom_data);
                break;
            default:
                printf("Please enter 'n for null', 'p for pause', 's for play','+ to increase playback speed','- to decrease playback speed'.\n");
                break;
        }

        if (new_state >= 0) {
            gst_element_set_state(custom_data->pipeline, new_state);
        }
    }
    return NULL;
}

 void send_seek_event (CustomData * data) 
{ 
  gint64 position; 
  GstEvent *seek_event; 
  if (!gst_element_query_position (data->pipeline, GST_FORMAT_TIME, &position)) { 
    g_printerr ("Unable to retrieve current position.\n"); 
    return; 
  } 
  if (data->playback_speed > 0) { 
    seek_event = 
        gst_event_new_seek (data->playback_speed, GST_FORMAT_TIME, 
        GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, GST_SEEK_TYPE_SET, 
        position, GST_SEEK_TYPE_END, 0); 
  } else { 
    seek_event = 
        gst_event_new_seek (data->playback_speed, GST_FORMAT_TIME, 
        GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, GST_SEEK_TYPE_SET, 0, 
        GST_SEEK_TYPE_SET, position); 
  } 
  if (data->autovideosink == NULL) { 
    g_object_get (data->pipeline, "video-sink", &data->autovideosink, NULL); 
  } 
  gst_element_send_event (data->autovideosink, seek_event); 
  g_print ("Current rate: %g\n", data->playback_speed); 
} 


static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data);

int main(int argc, char *argv[]) {
//setenv("GST_DEBUG_DUMP_DOT_DIR","/home/vicky/Desktop/",1);
  CustomData data;
  GstBus *bus;
  GstMessage *msg;
  GstStateChangeReturn ret;
  gboolean terminate = FALSE;
  data.terminate = FALSE;
  data.playback_speed = 1.0;

  /* Initialize GStreamer */
  gst_init(&argc, &argv);

  /* Create the elements */
  data.source = gst_element_factory_make("filesrc", "file-source");
  data.demux = gst_element_factory_make("qtdemux", "demux");
  data.queue_video = gst_element_factory_make("queue", "queue-video");
  data.h264parse = gst_element_factory_make("h264parse", "h264-parser");
  data.avdec_h264 = gst_element_factory_make("avdec_h264", "h264-decoder");
  data.videoscale=gst_element_factory_make("videoscale","video-scale");
  data.videoconvert = gst_element_factory_make("videoconvert", "video-converter");
  data.autovideosink = gst_element_factory_make("autovideosink", "video-sink");
  data.queue_audio = gst_element_factory_make("queue", "queue-audio");
  data.aacparse = gst_element_factory_make("aacparse", "aac-parser");
  data.faad = gst_element_factory_make("faad", "faad-decoder");
  data.audioconvert = gst_element_factory_make("audioconvert", "audio-converter");
  data.autoaudiosink = gst_element_factory_make("autoaudiosink", "audio-sink");

  /* Create the empty pipeline */
  data.pipeline = gst_pipeline_new("test-pipeline");

  if (!data.pipeline || !data.source || !data.demux || !data.queue_video || !data.h264parse || !data.avdec_h264 || !data.videoscale|| !data.videoconvert || !data.autovideosink ||
      !data.queue_audio || !data.aacparse || !data.faad || !data.audioconvert || !data.autoaudiosink) {
    g_printerr("Not all elements could be created.\n");
    return 1;
  }

  /* Build the pipeline. Note that we are NOT linking the source at this
   * point. We will do it later. */
  gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.demux, data.queue_video, data.h264parse, data.avdec_h264,data.videoscale, data.videoconvert, data.autovideosink,
                   data.queue_audio, data.aacparse, data.faad, data.audioconvert, data.autoaudiosink, NULL);

  /* Set the file location to play */
  g_object_set(data.source, "location", "/home/vicky/Desktop/gstreamer/haikyuu.mp4", NULL);

  // width height
   g_object_set(data.videoscale, "method", 0, "add-borders", TRUE, "width", 640, "height", 480, NULL);

  /* Connect to the pad-added signal */
  g_signal_connect(data.demux, "pad-added", G_CALLBACK(pad_added_handler), &data);

  /* Link the elements */
  if (!gst_element_link(data.source, data.demux)) {
    g_printerr("Source and demux could not be linked.\n");
    gst_object_unref(data.pipeline);
    return 1;
  }

  if (!gst_element_link_many(data.queue_video, data.h264parse, data.avdec_h264, data.videoscale,data.videoconvert, data.autovideosink, NULL) ||
      !gst_element_link_many(data.queue_audio, data.aacparse, data.faad, data.audioconvert, data.autoaudiosink, NULL)) {
    g_printerr("Elements could not be linked.\n");
    gst_object_unref(data.pipeline);
    return 1;
  }

  g_object_set(data.avdec_h264, "speed", data.playback_speed, NULL);


//GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(data.pipeline,GST_DEBUG_GRAPH_SHOW_ALL,"Image");

pthread_t thread;
pthread_create(&thread, NULL, user_input_thread, &data);

  /* Start playing */
  ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    gst_object_unref(data.pipeline);
    return 1;
  }

  /* Listen to the bus */
  bus = gst_element_get_bus(data.pipeline);
  do {
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Parse message */
    if (msg != NULL) {
      GError *err;
      gchar *debug_info;

      switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR:
          gst_message_parse_error(msg, &err, &debug_info);
          g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
          g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
          g_clear_error(&err);
          g_free(debug_info);
          terminate = TRUE;
          break;
        case GST_MESSAGE_EOS:
          g_print("End-Of-Stream reached.\n");
          terminate = TRUE;
          break;
        case GST_MESSAGE_STATE_CHANGED:
          /* We are only interested in state-changed messages from the pipeline */
          if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data.pipeline)) {
            GstState old_state, new_state, pending_state;
            gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
            g_print("Pipeline state changed from %s to %s:\n",
                    gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
          }
          break;
        default:
          /* We should not reach here */
          g_printerr("Unexpected message received.\n");
          break;
      }
      gst_message_unref(msg);
    }
  } while (!terminate);

  /* Free resources */
  gst_object_unref(bus);
  gst_element_set_state(data.pipeline, GST_STATE_NULL);
  gst_object_unref(data.pipeline);
  data.terminate = TRUE;  // Signal thread to terminate
  pthread_join(thread, NULL);  // Wait for thread to finish
  return 0;
}

/* This function will be called by the pad-added signal */
static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data) {
  gchar *new_pad_type = NULL;
  GstPad *sink_pad = NULL;
  GstElement *convert_element = NULL;

  g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src));

  /* Determine whether it's an audio or video pad */
  GstCaps *new_pad_caps = gst_pad_get_current_caps(new_pad);
  GstStructure *new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
  new_pad_type = g_strdup(gst_structure_get_name(new_pad_struct));

g_print("Content of new_pad_caps: %s\n", gst_caps_to_string(new_pad_caps));

// Assuming GstStructure has a field named 'some_field'
g_print("Content of new_pad_struct: %s\n", gst_structure_to_string(new_pad_struct));
g_print("new_pad_type: %s\n", new_pad_type);


  if (g_str_has_prefix(new_pad_type, "video/")) {
    sink_pad = gst_element_get_static_pad(data->queue_video, "sink");
    convert_element = data->queue_video;
  } else if (g_str_has_prefix(new_pad_type, "audio/")) {
    sink_pad = gst_element_get_static_pad(data->queue_audio, "sink");
    convert_element = data->queue_audio;
  }

  /* If our converter is already linked, we have nothing to do here */
  /* if (gst_pad_is_linked(sink_pad)) {
    g_print("We are already linked. Ignoring.\n");
    g_free(new_pad_type);
    gst_caps_unref(new_pad_caps);
    return;
  } */

  /* Attempt the link */
  GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
  if (GST_PAD_LINK_FAILED(ret)) {
    g_print("Link failed.\n");
  } else {
    g_print("Link succeeded (type '%s').\n", new_pad_type);
    //gst_element_link_pads(src, GST_PAD_NAME(new_pad), convert_element, "sink");
  } 

  g_free(new_pad_type);
  gst_caps_unref(new_pad_caps);
  gst_object_unref(sink_pad);
}

