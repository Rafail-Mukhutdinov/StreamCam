#pragma once
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
}

class device
{
private:
     const char* name_devace;


private:
    bool openInputDevice(AVFormatContext** in_context);   

    void readAndPrintPacketData(AVFormatContext* context, AVPacket* packet); 

public:
    void output_data_as_text(AVFormatContext* in_context, AVPacket* in_packet);

    device(const char* n_device):name_devace(n_device){};
    //~device();
};

