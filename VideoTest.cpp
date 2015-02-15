//Primary author: Jonathan Bedard
//Confirmed working: 11/5/2014

#ifndef VIDEO_TEST_CPP
#define VIDEO_TEST_CPP

/*#include "libavcodec/avcodec.h"
#include "libavcodec/avdevice.h"
#include "libavcodec/avfft.h"
#include "libavcodec/dv_profile.h"
#include "libavcodec/dxva2.h"
#include "libavcodec/old_codec_ids.h"
#include "libavcodec/vaapi.h"
#include "libavcodec/version.h"

#include "libavdevice/avdevice.h"
#include "libavdevice/version.h"

#include "libavfilter/asrc_abuffer.h"
#include "libavfilter/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/version.h"

#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavformat/version.h"

#include "libpostproc/postprocess.h"
#include "libpostproc/version.h"

#include "libswresample/swresample.h"
#include "libswresample/version.h"

#include "libswscale/swscale.h"
#include "libswscale/version.h"*/

#include <stdio.h>
#include <iostream>

#include <stdlib.h>
#include <string.h>
 
#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif
 
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
 
#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
using namespace std;

#ifndef MAIN
#define MAIN

inline int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}

inline int c99_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                      char *filename)
{
     FILE *f;
     int i;
 
     fopen_s(&f,filename,"w");
     fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
	 for(i=0;i<ysize;i++)
         fwrite(buf + i * wrap,1,xsize,f);
     fclose(f);
}

int main() {
	 char* filename = (char*) "test.mp4";
	 char* outfilename = (char*) "test.mpg";
     AVCodec *codec;
     AVCodecContext *c= NULL;
     int frame, got_picture, len;
     FILE *f;
     AVFrame *picture;
     uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
     char buf[1024];
     AVPacket avpkt;
 
     av_init_packet(&avpkt);
 
     /* set end of buffer to 0 (this ensures that no overreading happens for damaged mpeg streams) */
     memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);
 
     printf("Video decoding\n");
 
     /* find the mpeg1 video decoder */
	 codec = avcodec_find_decoder(CODEC_ID_MP4ALS);
     if (!codec) {
         fprintf(stderr, "codec not found\n");
         exit(1);
     }
 
     c= avcodec_alloc_context3(codec);
     picture= av_frame_alloc();
 
     if(codec->capabilities&CODEC_CAP_TRUNCATED)
         c->flags|= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */
 
     /* For some codecs, such as msmpeg4 and mpeg4, width and height
        MUST be initialized there because this information is not
        available in the bitstream. */
 
     /* open it */
     if (avcodec_open2(c, codec,NULL) < 0) {
         fprintf(stderr, "could not open codec\n");
         exit(1);
     }

     /* the codec gives us the frame size, in samples */
 
     fopen_s(&f,filename, "rb");
     if (!f) {
         fprintf(stderr, "could not open %s\n", filename);
         exit(1);
     }
 
     frame = 0;
     for(;;) {
         avpkt.size = fread(inbuf, 1, INBUF_SIZE, f);
         if (avpkt.size == 0)
             break;

         avpkt.data = inbuf;
         while (avpkt.size > 0) {
             len = avcodec_decode_video2(c, picture, &got_picture, &avpkt);
             if (len < 0) {
                 fprintf(stderr, "Error while decoding frame %d\n", frame);
                 exit(1);
             }
             if (got_picture) {
                 printf("saving frame %3d\n", frame);
                 fflush(stdout);
 
                 /* the picture is allocated by the decoder. no need to
                    free it */
                 c99_snprintf(buf, sizeof(buf), outfilename, frame);
                 pgm_save(picture->data[0], picture->linesize[0],
                          c->width, c->height, buf);
                 frame++;
             }
             avpkt.size -= len;
             avpkt.data += len;
         }
     }
 
     /* some codecs, such as MPEG, transmit the I and P frame with a
        latency of one frame. You must do the following to have a
        chance to get the last frame of the video */
     avpkt.data = NULL;
     avpkt.size = 0;
     len = avcodec_decode_video2(c, picture, &got_picture, &avpkt);
     if (got_picture) {
         printf("saving last frame %3d\n", frame);
         fflush(stdout);
 
         /* the picture is allocated by the decoder. no need to
            free it */
         c99_snprintf(buf, sizeof(buf), outfilename, frame);
         pgm_save(picture->data[0], picture->linesize[0],
                  c->width, c->height, buf);
         frame++;
     }
 
     fclose(f);
 
     avcodec_close(c);
     av_free(c);
     av_free(picture);
     printf("\n");
 }

#endif

#endif