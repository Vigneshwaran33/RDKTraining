#include<gst/gst.h>

#include<stdio.h>

typedef struct CustomData{

	GstElement *pipeline;

	GstElement *source;

	GstElement *demux;

	GstElement *queue_video;

	GstElement *h264parse;

	GstElement *avdec_h264;

	GstElement *videoscale;

	GstElement *video_sink;

	GstElement *queue_audio;

	GstElement *aacparse;

	GstElement *avdec_aac;

	GstElement *audioconvert;

	GstElement *audioresample;

	GstElement *amlhalasink;

	GThread *input_thread;

	gdouble playback_speed;



} CustomData;





static gboolean handle_input(CustomData *data);

static void pad_handler(GstElement *src,GstPad *new_pad,CustomData *data);

static void send_seek_event (CustomData * data)

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



  if (data->video_sink == NULL) {

    

    g_object_get (data->pipeline, "video-sink", &data->video_sink, NULL);

  }



  gst_element_send_event (data->video_sink, seek_event);

  g_print ("Current rate: %g\n", data->playback_speed);

}



int main(int argc,char *argv[]){

	CustomData data;

	GstBus *bus;

	GstMessage *msg;

	GstStateChangeReturn ret;

	data.playback_speed = 1.0;


	gst_init(&argc,&argv);

	data.source=gst_element_factory_make("filesrc","file-source");

	data.demux=gst_element_factory_make("qtdemux","demux");

	data.queue_video=gst_element_factory_make("queue","queue-video");

	data.h264parse=gst_element_factory_make("h264parse","h264-parser");

	data.avdec_h264=gst_element_factory_make("avdec_h264","h264-decoder");

	data.videoscale = gst_element_factory_make("videoscale","videoscale");

	data.video_sink=gst_element_factory_make("autovideosink","video-sink");

	data.queue_audio=gst_element_factory_make("queue","queue_audio");

	data.aacparse=gst_element_factory_make("aacparse","aac-parser");

	data.avdec_aac=gst_element_factory_make("avdec_aac","avdec_aac");



	data.audioconvert=gst_element_factory_make("audioconvert","audioconverter");

	data.audioresample = gst_element_factory_make("audioresample","audioresample");

	data.amlhalasink=gst_element_factory_make("amlhalasink","audio-sink");





	data.pipeline=gst_pipeline_new("video-pipeline");



	if(!data.pipeline || !data.source || !data.demux ||

	   !data.queue_video || !data.h264parse || !data.avdec_h264 || !data.videoscale || !data.video_sink ||

	   !data.queue_audio || !data.aacparse || !data.avdec_aac || !data.audioconvert || !data.audioresample ||  !data.amlhalasink){
		if(!data.pipeline)
		{
			g_printerr("Pipeline not created");
		}
		if(!data.source)
		{
			g_printerr("source not created");
		}
		if(!data.demux)
		{
			g_printerr("demux not created");
		}
		if(!data.queue_video)
		{
			g_printerr("queue_video not created");
		}
		if(!data.h264parse)
		{
			g_printerr("h264parse not created");
		}
		if(!data.avdec_h264)
		{
			g_printerr("avdec_h264 not created");
		}

		if(!data.videoscale)
		{
			g_printerr("videoscale not created");
		}
		if(!data.video_sink)
		{
			g_printerr("video_sink not created");
		}
		if(!data.queue_audio)
		{
			g_printerr("queue_audio not created");
		}
		if(!data.aacparse)
		{
			g_printerr("aacparser not created");
		}

		if(!data.avdec_aac)
		{
			g_printerr("avdec_aac not created");
		}


		if(!data.audioconvert)
		{
			g_printerr("audioconvert not created");
		}
		if(!data.audioresample)
		{
			g_printerr("resample not created");
		}
		if(!data.amlhalasink)
		{
			g_printerr("amlhalasink not created");
		}
		g_printerr("Some elements are not created");

	    return 1;

	}





	gst_bin_add_many(GST_BIN(data.pipeline),data.source,data.demux,data.queue_video,data.h264parse,data.avdec_h264,data.videoscale,data.video_sink,

		data.queue_audio,data.aacparse,data.avdec_aac,data.audioconvert,data.audioresample,data.amlhalasink,NULL);





	
	g_object_set(data.source, "location", argv[1], NULL);



	g_signal_connect(data.demux,"pad-added",G_CALLBACK(pad_handler),&data);

	data.input_thread = g_thread_new("input-thread", (GThreadFunc)handle_input, &data);





	if(!gst_element_link(data.source,data.demux)){

		g_printerr("source and demux not linked.\n");

		gst_object_unref(data.pipeline);
		return 1;

	}



	if(!gst_element_link_many(data.queue_video,data.h264parse,data.avdec_h264,data.videoscale,data.video_sink,NULL)||

		!gst_element_link_many(data.queue_audio,data.aacparse,data.avdec_aac,data.audioconvert,data.audioresample,data.amlhalasink,NULL)){
		
		if(!gst_element_link_many(data.queue_video,data.h264parse,data.avdec_h264,data.videoscale,data.video_sink,NULL))
			g_printerr("video elements could not be linked.\n");
		if(!gst_element_link_many(data.queue_audio,data.aacparse,data.avdec_aac,data.audioconvert,data.audioresample,data.amlhalasink,NULL))
			g_printerr("audio elements could not be linked.\n");

	    gst_object_unref(data.pipeline);

	    return 1;

	}
	GstCaps *caps;
	caps = gst_caps_new_simple("video/x-raw",
                               "width", G_TYPE_INT, 640,
                               "height", G_TYPE_INT, 480,
                               NULL);
    g_object_set(G_OBJECT(data.videoscale), "caps", caps, NULL);
    

	ret=gst_element_set_state(data.pipeline,GST_STATE_PLAYING);

	if(ret == GST_STATE_CHANGE_FAILURE){

		g_printerr("Unable to set pipeline to playing state");

		return 1;

	}



	bus =gst_element_get_bus(data.pipeline);

	  do{

	    msg = gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE, GST_MESSAGE_ANY);



	    if(msg!=NULL){



	    	const gchar *msg_type_name = GST_MESSAGE_TYPE_NAME(msg);

	    	GError *err;

	    	gchar *debug_info;



	    	switch(GST_MESSAGE_TYPE(msg)){

		    	case GST_MESSAGE_ERROR:

		    		gst_message_parse_error(msg,&err,&debug_info);

		    		g_printerr("Error received from element %s :%s\n",GST_OBJECT_NAME(msg->src),err->message);

		    		g_printerr("Debugging information :%s\n",debug_info?debug_info:"none");

		    		g_clear_error(&err);

		    		g_free(debug_info);

		    		break;

		    	case GST_MESSAGE_EOS:

		    		g_print("End of stream\n");

		    		break;

	    	}

	    	gst_message_unref(msg);

	    }

	    g_usleep(10000);



	} while(TRUE);
  

	    g_thread_join(data.input_thread);

		gst_caps_unref(caps);

	    gst_object_unref(bus);

	    gst_element_set_state(data.pipeline, GST_STATE_NULL);

	    gst_object_unref(data.pipeline);

	return 0;

}

static void pad_handler(GstElement *src,GstPad *new_pad,CustomData *data){

	gchar *new_pad_type=NULL;

	GstPad *sink_pad=NULL;



	g_print("Received new pad '%s' from '%s' :\n",GST_PAD_NAME(new_pad),GST_ELEMENT_NAME(src));





	GstCaps *new_pad_caps=gst_pad_get_current_caps(new_pad);

	GstStructure *new_pad_struct=gst_caps_get_structure(new_pad_caps,0);

	new_pad_type=g_strdup(gst_structure_get_name(new_pad_struct));



	g_print("caps:%s \n",gst_caps_to_string(new_pad_caps));

	g_print("Structure:%s \n",gst_structure_to_string(new_pad_struct));

	g_print("Pad:%s\n",new_pad_type);







	if(g_str_has_prefix(new_pad_type,"video/"))

		sink_pad=gst_element_get_static_pad(data->queue_video,"sink");



	else if(g_str_has_prefix(new_pad_type,"audio/"))

		sink_pad=gst_element_get_static_pad(data->queue_audio,"sink");







	GstPadLinkReturn ret=gst_pad_link(new_pad,sink_pad);

	if(GST_PAD_LINK_FAILED(ret))

		g_print("Link failed \n ");

	else

		g_print("Link succeeded '%s'\n",new_pad_type);

	  	GIOChannel *io_stdin;



	g_free(new_pad_type);

	gst_caps_unref(new_pad_caps);

	gst_object_unref(sink_pad);

}





static gboolean handle_input(CustomData *data) {

    gchar input[256];


    while (TRUE) {

        if (fgets(input, sizeof(input), stdin) != NULL) {



            input[strcspn(input, "\n")] = '\0';


            if (g_strcmp0(input, "pause") == 0) {

                g_print("Pausing the pipeline\n");

                gst_element_set_state(data->pipeline, GST_STATE_PAUSED);

                 

            } else if (g_strcmp0(input, "play") == 0) {

                g_print("Playing the pipeline\n");

                gst_element_set_state(data->pipeline, GST_STATE_PLAYING);

                 
            } else if (g_strcmp0(input, "ready") == 0) {

                g_print("Setting the pipeline to ready state\n");

                gst_element_set_state(data->pipeline, GST_STATE_READY);

                

            }

            else if (g_strcmp0(input, "null") == 0) {

                g_print("Setting the pipeline to NULL state\n");

                gst_element_set_state(data->pipeline, GST_STATE_NULL);

                

            } else if (g_str_has_prefix(input, "speed ") || g_str_has_prefix(input, "Speed ")) {

                gdouble speed = atof(input + 6); 

                if (g_ascii_isupper (input[0])) {

			data->playback_speed *= speed;

	       } else {

			data->playback_speed  /= speed;

	      	}

	      	send_seek_event (data);

	      	

                g_print("Setting playback speed to %.2f\n", speed);

                 

                

            } else {

                g_print("Unknown command: %s\n", input);

            }

        }



        g_usleep(10000);

    }

}





