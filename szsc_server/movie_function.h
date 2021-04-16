#ifndef INCLUDE_movie_function_H
#define INCLUDE_movie_function_H

#include "start_one_fight.h"
#include <stdio.h>
extern "C"{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>

#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "SDL.h"
};
#pragma  comment(lib,"avcodec.lib")
#pragma  comment(lib,"avdevice.lib")
#pragma  comment(lib,"avfilter.lib")
#pragma  comment(lib,"avformat.lib")
#pragma  comment(lib,"avutil.lib")
#pragma  comment(lib,"postproc.lib")
#pragma  comment(lib,"swresample.lib")
#pragma  comment(lib,"swscale.lib")




static void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
	
    FILE *pFile;//cout<<"存帧开始!\n";
	char szFilename[260]={0};
	addtext(szFilename,"F:\\JY\\www.NewXing.com\\moive\\test\\");
	addtext(szFilename, "frame");addtext(szFilename, iFrame);addtext(szFilename, ".ppm");

    // Open file
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL){cout<<"文件指针创建失败！\n";return;}

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(int y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
    fclose(pFile);
	cout<<"存帧结束！\n";
}

int SaveAsJPEG(AVFrame* pFrame, int width, int height, int index)
{
    // 输出文件路径
    char out_file[MAX_PATH] = {0};
    sprintf_s(out_file, sizeof(out_file), "%s%d.jpg", "F:\\JY\\www.NewXing.com\\moive\\test\\", index);

    // 分配AVFormatContext对象
    AVFormatContext* pFormatCtx = avformat_alloc_context();

    // 设置输出文件格式
    pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);

    // 创建并初始化一个和该url相关的AVIOContext
    if( avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0)
    {printf("无法打开文件夹！");return -1;}

    // 构建一个新stream
    AVStream* pAVStream = avformat_new_stream(pFormatCtx, 0);
    if( pAVStream == NULL ){cout<<"创建流失败！\n";return -1;}
 
    

	
	/*pCodecCtx->thread_count=1;
	//编码器的ID号，这里我们自行指定为264编码器，实际上也可以根据video_st里的codecID 参数赋值
    pCodecCtx->codec_id   = pFormatCtx->oformat->video_codec;
	//编码器编码的数据类型
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt    = AV_PIX_FMT_YUVJ420P;
    pCodecCtx->width      = width;
    pCodecCtx->height     = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;*/
	//pCodecCtx->time_base=(AVRational){1, 25};
	AVCodecParameters *parameters = pAVStream->codecpar;
    parameters->codec_id = pFormatCtx->oformat->video_codec;
    parameters->codec_type = AVMEDIA_TYPE_VIDEO;
    parameters->format = AV_PIX_FMT_YUVJ420P;
    parameters->width = width;
    parameters->height = height;
 
    //打印输出相关信息
    //av_dump_format(pFormatCtx, 0, out_file, 0);
 cout<<"7\n";
    //================================== 查找编码器 ==================================//
    //AVCodec* pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	AVCodec* pCodec = avcodec_find_encoder(pAVStream->codecpar->codec_id);
    if( !pCodec ){printf("无法打开编码器！"); return -1;}

	AVCodecContext* pCodecCtx= avcodec_alloc_context3(NULL);// 设置该stream的信息
	if (pCodecCtx == NULL)  
    {  printf("Could not allocate AVCodecContext\n");  return -1;  }    

	if(avcodec_parameters_from_context(pAVStream->codecpar,pCodecCtx )<0)
	{cout<<"复制编码参数失败！\n";return -1;}
	pCodecCtx->time_base.num=1;
	pCodecCtx->time_base.den=25;
  cout<<"7\n";
    // 设置pCodecCtx的解码器为pCodec
    if( avcodec_open2(pCodecCtx, pCodec, NULL) < 0 )
    {printf("无法打开解码器！\n");return -1;}
  cout<<"8\n";
  
  
    //================================Write Header ===============================//
    if(avformat_write_header(pFormatCtx, NULL)<0)
	{cout<<"头文件写入失败!\n";return -1;}
 
    int y_size = pCodecCtx->width * pCodecCtx->height;
 cout<<"9\n";
    //==================================== 编码 ==================================//
    // 给AVPacket分配足够大的空间
    AVPacket pkt;
    av_new_packet(&pkt, y_size * 3);
  cout<<"10\n";
    //
    int got_picture = 0;
    if(avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture)<0)
    {printf("编码错误！\n");return -1;}
	int ret;
    if( got_picture == 1 )
    {
        pkt.stream_index = pAVStream->index;
		if(av_write_frame(pFormatCtx, &pkt)<0)
		{cout<<"写入失败！\n";}
    }
  cout<<"10\n";
    av_free_packet(&pkt);
 
    //Write Trailer
    av_write_trailer(pFormatCtx);
 
 
    if( pAVStream )
    {
        avcodec_close(pAVStream->codec);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
 
    return 0;
}





void movie_F1(client_member* c1)
{
	special_signal(c1,show_video);//视频功能
	char StrBuf[MAX_PATH]={0};
	AVFormatContext *pFormatCtx = NULL;//分配一个AVFormatContext，FFMPEG所有的操作都要通过这个AVFormatContext来进行
    int             i, videoStream;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame         *pFrame; 
    AVFrame         *pFrameRGB;
    AVPacket        packet;
    int             frameFinished;
    int             numBytes;
    uint8_t         *buffer;

	// Register all formats and codecs
    av_register_all();//初始化FFMPEG  调用了这个才能正常适用编码器和解码器

	const char* filename = "F:\\JY\\www.NewXing.com\\moive\\Wildlife.wmv";// Open video file
	//if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)

	int ii = avformat_open_input(&pFormatCtx, filename, NULL, NULL);//打开文件
    if(ii!=0)
	{
		cout<<"无法打开!\n";
        return ; // Couldn't open file
	}

	 // Dump information about file onto standard error
	cout<<"-----------------------------------------\n";
    av_dump_format(pFormatCtx, 0, filename, 0);//最后一个参数输入1会报错
	cout<<"-----------------------------------------\n";

    //=================================== 获取视频流信息 ===================================//
	// Retrieve stream information检索流信息
    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		cout<<"无法找到流信息!\n";return ; // Couldn't find stream information
	}
    
 	//循环查找视频中包含的流信息，直到找到视频类型的流
    //便将其记录下来 保存到videoStream变量中
    videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;break;
        }
    }
    if (videoStream == -1)//如果videoStream为-1 说明没有找到视频流
    {
        cout << "没有找到视频流！\n" ;return ;// Didn't find a video stream
    }


	// Get a pointer to the codec context for the video stream
    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

	// Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);//查找解码器
    if(pCodec==NULL){cout<<"找不到解码器!\n";return;}// Codec not found
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)// Open codec
	{cout<<"打不开解码器!\n";return ;} // Could not open codec

	// Hack to correct wrong frame rates that seem to be generated by some codecs
    if(pCodecCtx->time_base.num>1000 && pCodecCtx->time_base.den==1)
        pCodecCtx->time_base.den=1000;

	
    pFrame=av_frame_alloc();// Allocate video frame
	// Allocate an AVFrame structure
    pFrameRGB=av_frame_alloc();//创建存储帧的内存
    if(pFrameRGB==NULL){cout<<"分配失败！\n";return ;}

	// Determine required buffer size and allocate buffer获取一帧图像数据大小
    numBytes=avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width,pCodecCtx->height);
	//buffer=malloc(numBytes);创设字符串（用来存储一帧图片）
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
    avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_YUV420P,
    pCodecCtx->width, pCodecCtx->height);
	//会将pFrameRGB的数据按RGB格式自动"关联"到buffer  即pFrameRGB中的数据改变了 buffer中的数据也会相应的改变

	// Read frames and save first five frames to disk
        i=0;
        while(av_read_frame(pFormatCtx, &packet)>=0)//读取的是一帧视频  数据存入一个AVPacket的结构中
        {
            // Is this a packet from the video stream?该包信息流是否是视频流？
            if(packet.stream_index==videoStream)
            {
                // Decode video frame解码
                avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

                // Did we get a video frame?
                if(frameFinished)
                {
                    static struct SwsContext *img_convert_ctx;//创建一副空图

#if 0
                    // Older removed code
                    // Convert the image from its native format to RGB swscale
                    img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24, 
                        (AVPicture*)pFrame, pCodecCtx->pix_fmt, pCodecCtx->width, 
                        pCodecCtx->height);

                    // function template, for reference
                    int sws_scale(struct SwsContext *context, uint8_t* src[], int srcStride[], int srcSliceY,
                        int srcSliceH, uint8_t* dst[], int dstStride[]);
#endif
                    // Convert the image into YUV format that SDL uses
                    if(img_convert_ctx == NULL) {
                        int w = pCodecCtx->width;
                        int h = pCodecCtx->height;

                        img_convert_ctx = sws_getContext(w, h, 
                            pCodecCtx->pix_fmt, 
                            w, h, AV_PIX_FMT_YUV420P, SWS_BICUBIC,
                            NULL, NULL, NULL);
                        if(img_convert_ctx == NULL) {//如果依然为空
							cout<<"Cannot initialize the conversion context!\n";
                            fprintf(stderr, "Cannot initialize the conversion context!\n");
                            exit(1);
                        }
                    }

					//转换图片规格，例如，将YUV420P转换成YUYV422
                    int ret = sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, 
                        pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
#if 0 
                    // this use to be true, as of 1/2009, but apparently it is no longer true in 3/2009
                    if(ret) {
                        fprintf(stderr, "SWS_Scale failed [%d]!\n", ret);
                        exit(-1);
                    }
#endif
                    // Save the frame to disk,保存前500帧
					i++;
                    if(i<=50)
					{
                        //SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);//ppmg格式
						SaveAsJPEG(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);//jpg格式
					}
                }
            }
           
            av_free_packet(&packet); // Free the packet that was allocated by av_read_frame
        }

		// Free the RGB image
        //free(buffer);
        av_free(buffer);
        av_free(pFrameRGB);

        // Free the YUV frame
        av_free(pFrame);

        // Close the codec
        avcodec_close(pCodecCtx);

        // Close the video file
        avformat_close_input(&pFormatCtx);
}

void movie_F(client_member* c1)
{
	special_signal(c1,show_video);//视频功能
	FILE *stream;
	char buf[65535]={0};  
    //打开文件
        if ((stream = fopen("F:\\JY\\www.NewXing.com\\moive\\Wildlife.wmv", "rb")) == NULL) {
            printf("The file doctor_strange.flv not opend!");
            exit(1);
        } else
            printf("open file and transmit\n");
        /* 开始处理每个新连接上的数据收发 */
        memset(buf, 0,sizeof(buf));

        int lengsize = 0;
        while ((lengsize = fread(buf, 1, sizeof(buf)-1, stream)) > 0) {
			Sleep(20);
            if (send(g_ClientSocket[c1->member_No-1], buf, lengsize, 0) < 0) {
                printf("send file failed!");
                break;
            }
            memset(buf, 0,sizeof(buf));
        }
		cout<<"传输结束!\n";

        if (fclose(stream)) {
            printf("File data not closed\n");
            exit(1);
        }
}
#endif