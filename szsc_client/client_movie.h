#ifndef INCLUDE_client_movie_H
#define INCLUDE_client_movie_H
#include <stdio.h>
#include "special_transmit.h"
#include <assert.h>
#include <queue>
#include <cassert>
extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "SDL.h"
#include <SDL_thread.h>
};
#pragma  comment(lib,"avcodec.lib")
#pragma  comment(lib,"avdevice.lib")
#pragma  comment(lib,"avfilter.lib")
#pragma  comment(lib,"avformat.lib")
#pragma  comment(lib,"avutil.lib")
#pragma  comment(lib,"postproc.lib")
#pragma  comment(lib,"swresample.lib")
#pragma  comment(lib,"swscale.lib")

const int buffersize=65535;
const int pcm_buffer_size=4096;
#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000



//void  fill_audio(void *udata,Uint8 *stream,int len);
char* GBKToUTF8(const std::string& strGBK)
 {
     char* strOutUTF8= "";
     WCHAR * str1;
     int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
     str1 = new WCHAR[n];
     MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
     n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
     char * str2 = new char[n];
     WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
     strOutUTF8 = str2;
     delete[]str1;
     str1 = NULL;
     delete[]str2;
     str2 = NULL;
	 return strOutUTF8;
 }
void get_file()//获取文件
{
	
	char buffer[65535*10]={0};
    FILE *stream;
	FILE * fp = fopen("F:\\JY\\www.NewXing.com\\moive\\test\\2.wmv","wb");
    if(NULL == fp )
    {
        printf("Can Not Open To Write\n");
        exit(1);
    }
    int length=0;
    while (length=recv(Client,buffer,sizeof(buffer),0))
    {
        if(length < 0)
        {
            printf("Recieve Data From Server Failed!\n");
            break;
        }
		if(atoi(buffer)==show_choice){cout<<"传输完成!\n";break;}
        int write_length = fwrite(buffer,sizeof(char),length,fp);
        if (write_length<length)
        {
            printf("Write Failed\n");
            break;
        }
        memset(buffer,0,sizeof(buffer));

    }
    printf("Receive file finished\n");
    fclose(fp);
    return ;  
}//获取服务器发来的文件
void show_moive();//读取本地视频
void get_movie()
{
	cout<<"获取电影中！\n";
	Sleep(50);
	reply_i_get();
	show_moive();
}



static uint8_t audio_buf[7644800];//44100*16*2*4
static unsigned int audio_buf_left_index = 0;
static unsigned int audio_buf_right_index = 0;

int wake_up;
void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if(wake_up==0)Sleep(1);
    memcpy(stream, audio_buf + audio_buf_left_index, len);
    audio_buf_left_index += len;
	if(audio_buf_left_index>sizeof(audio_buf)-10000)
	{
		cout<<audio_buf_left_index<<endl;
		audio_buf_left_index=0;
	}
	wake_up=0;
}

void show_moive()//读取本地视频
{
	char buf[260]={0};
	//char rtspUrl[] = "F:\\JY\\www.NewXing.com\\moive\\btest\\Wildlife.wmv";
  char rtspUrl[] = "F:\\JY\\www.NewXing.com\\moive\\英雄联盟 .mp3";

    // SDL相关变量预先定义
    SDL_Window *pSDLWindow = 0;
    SDL_Renderer *pSDLRenderer = 0;
    SDL_Surface *pSDLSurface = 0;
    SDL_Texture *pSDLTexture = 0;
    SDL_Event event;

    //__int64 startTime = 0;                           // 记录播放开始
	SYSTEMTIME startTime,nextTime;//解码前时间//解码完成时间
    double fps = 0;                                 // 帧率
    double interval = 0;                            // 帧间隔

    // ffmpeg相关变量预先定义与分配
    AVFormatContext *pAVFormatContext = 0;          // ffmpeg的全局上下文，所有ffmpeg操作都需要
    AVStream *pAVStream = 0;                        // ffmpeg流信息
    AVCodecContext *vCodecCtx = 0;					// ffmpeg编码上下文
    AVCodec *vCodec = 0;							// ffmpeg编码器
    AVPacket *pAVPacket = 0;                        // ffmpag单帧数据包
    //AVFrame *pAVFrame = 0;                          // ffmpeg单帧缓存
    AVFrame *pAVFrameRGB32 = 0;                     // ffmpeg单帧缓存转换颜色空间后的缓存
    struct SwsContext *pSwsContext = 0;             // ffmpeg编码数据格式转换
    AVDictionary *pAVDictionary = 0;                // ffmpeg数据字典，用于配置一些编码器属性等
    int ret = 0;                                    // 函数执行结果
    int videoIndex = -1;                            // 视频流所在的序号
    int numBytes = 0;                               // 解码后的数据长度
    unsigned char* outBuffer = 0;                   // 解码后的数据存放缓存区

	
    pAVFormatContext = avformat_alloc_context();    // 分配
    pAVPacket = av_packet_alloc();                  // 分配
    AVFrame *pAVFrame;
	pAVFrame = av_frame_alloc();                    // 分配
    pAVFrameRGB32 = av_frame_alloc();               // 分配

    if(!pAVFormatContext || !pAVPacket || !pAVFrame || !pAVFrameRGB32)
    {cout << "Failed to alloc";return;}
    // 步骤一：注册所有容器和编解码器（也可以只注册一类，如注册容器、注册编码器等）
    av_register_all();
    avformat_network_init();
    // 步骤二：打开文件(ffmpeg成功则返回0)
    cout << "打开:" << rtspUrl;

    if(avformat_open_input(&pAVFormatContext,rtspUrl, NULL, NULL))
    {av_strerror(ret, buf, 1024);cout << "\n错误编号："<<ret<<"\nFailed";return;}

    // 步骤三：探测流媒体信息
    if(avformat_find_stream_info(pAVFormatContext, 0)< 0)
    {cout << "Failed to avformat_find_stream_info(pAVFormatContext, 0)";return;}

	
    // 步骤四：提取流信息,提取视频信息
    for(int index = 0; index < pAVFormatContext->nb_streams; index++)
    {
        vCodecCtx = pAVFormatContext->streams[index]->codec;
        pAVStream = pAVFormatContext->streams[index];
        switch (vCodecCtx->codec_type)
        {
			case AVMEDIA_TYPE_UNKNOWN:					cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_UNKNOWN"<<endl;break;
			case AVMEDIA_TYPE_VIDEO:videoIndex = index;	cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_VIDEO"<<endl;break;
			case AVMEDIA_TYPE_AUDIO:					cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_AUDIO"<<endl;break;
			case AVMEDIA_TYPE_DATA:						cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_DATA"<<endl;break;
			case AVMEDIA_TYPE_SUBTITLE:					cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_SUBTITLE"<<endl;break;
			case AVMEDIA_TYPE_ATTACHMENT:				cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_ATTACHMENT"<<endl;break;
			case AVMEDIA_TYPE_NB:						cout << "流序号:" << index << "类型为:" << "AVMEDIA_TYPE_NB"<<endl;break;
			default:break;
        }
        if(videoIndex != -1){break;}// 已经找打视频品流,跳出循环
    }
    if(videoIndex == -1 || !vCodecCtx){cout << "Failed to find video stream";return;}//判断是否找到视频流



    // 步骤五：对找到的视频流寻解码器
    vCodec = avcodec_find_decoder(vCodecCtx->codec_id);
    if(!vCodec){cout<<"Failed to avcodec_find_decoder(vCodecCtx->codec_id):"<< vCodecCtx->codec_id;return; }

    // 步骤六：打开解码器    
    av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);// 设置缓存大小 1024000byte    
    av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);// 设置超时时间 20s    
    av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);// 设置最大延时 3s    
    av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);// 设置打开方式 tcp/udp

    if(avcodec_open2(vCodecCtx, vCodec, &pAVDictionary)){cout << "Failed to avcodec_open2(vCodecCtx, vCodec, pAVDictionary)";return;}
    
    // 显示视频相关的参数信息（编码上下文）
    cout << "比特率:" << vCodecCtx->bit_rate<<endl;
    cout << "宽高:" << vCodecCtx->width << "x" << vCodecCtx->height<<endl;
    cout << "格式:" << vCodecCtx->pix_fmt<<endl;  // AV_PIX_FMT_YUV420P 0
    cout << "帧率分母:" << vCodecCtx->time_base.den<<endl;
    cout << "帧率分子:" << vCodecCtx->time_base.num<<endl;
    cout << "帧率分母:" << pAVStream->avg_frame_rate.den<<endl;
    cout << "帧率分子:" << pAVStream->avg_frame_rate.num<<endl;
    cout << "总时长:" << pAVStream->duration / 10000.0 << "s"<<endl;
    cout << "总帧数:" << pAVStream->nb_frames;
    // 有总时长的时候计算帧率（较为准确）
//    fps = pAVStream->nb_frames / (pAVStream->duration / 10000.0);
//    interval = pAVStream->duration / 10.0 / pAVStream->nb_frames;
    // 没有总时长的时候，使用分子和分母计算
    fps = pAVStream->avg_frame_rate.num * 1.0f / pAVStream->avg_frame_rate.den;
    interval = 1 * 1000 / fps;
    cout << "平均帧率:" << fps<<endl;
    cout << "帧间隔:" << interval << "ms"<<endl;

    // 步骤七：对拿到的原始数据格式进行缩放转换为指定的格式高宽大小
    pSwsContext = sws_getContext(vCodecCtx->width,vCodecCtx->height,vCodecCtx->pix_fmt,vCodecCtx->width,vCodecCtx->height,AV_PIX_FMT_RGBA,SWS_FAST_BILINEAR,0,0,0);

    numBytes = avpicture_get_size(AV_PIX_FMT_RGBA,vCodecCtx->width,vCodecCtx->height);

    outBuffer = (unsigned char *)av_malloc(numBytes);
    // pAVFrame32的data指针指向了outBuffer
    avpicture_fill((AVPicture *)pAVFrameRGB32,outBuffer,AV_PIX_FMT_RGBA,vCodecCtx->width,vCodecCtx->height);

    if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO | SDL_INIT_TIMER))//初始化SDL 
    {cout << "Failed";return;}
    

    pSDLWindow = SDL_CreateWindow("AAA",0,0,
                                  vCodecCtx->width,
                                  vCodecCtx->height,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!pSDLWindow){cout << "Failed";return;}

    pSDLRenderer = SDL_CreateRenderer(pSDLWindow, -1, 0);
    if(!pSDLRenderer){cout << "Failed";return;}

    pSDLTexture = SDL_CreateTexture(pSDLRenderer,
//                                  SDL_PIXELFORMAT_IYUV,
                                    SDL_PIXELFORMAT_YV12,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    vCodecCtx->width,
                                    vCodecCtx->height);
    if(!pSDLTexture){cout << "Failed";return;}

	//----------------------------------------------------------------------------------------------------------
	int audio_stream_index = av_find_best_stream(pAVFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    AVCodecContext *pAudioCodecCtx = pAVFormatContext->streams[audio_stream_index]->codec;
    AVCodec *pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
    assert(avcodec_open2(pAudioCodecCtx, pAudioCodec, nullptr) >= 0);

    SDL_AudioSpec desired_spec;
    SDL_AudioSpec obtained_spec;
    desired_spec.freq = pAudioCodecCtx->sample_rate;
    desired_spec.format = AUDIO_F32SYS;
    desired_spec.channels = pAudioCodecCtx->channels;
    desired_spec.silence = 0;
    desired_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    desired_spec.callback = audio_callback;
    desired_spec.userdata = audio_buf;
    
    assert(SDL_OpenAudio(&desired_spec, &obtained_spec) >= 0);
    SDL_PauseAudio(0);
	//----------------------------------------------------------------------------------------------------------

    // 步骤八：读取一帧数据的数据包
    while(av_read_frame(pAVFormatContext, pAVPacket) >= 0)
    {
		GetLocalTime(&startTime);//获取解码前精确时间
        if(pAVPacket->stream_index == videoIndex)
        {
			
            // 步骤八：对读取的数据包进行解码
			ret=avcodec_send_packet(vCodecCtx, pAVPacket);
            if(ret){cout << "Failed to avcodec_send_packet(vCodecCtx, pAVPacket) ,ret =" << ret;break;}

            while(!avcodec_receive_frame(vCodecCtx, pAVFrame))
            {
                sws_scale(pSwsContext,
                          (const uint8_t * const *)pAVFrame->data,
                          pAVFrame->linesize,
                          0,
                          vCodecCtx->height,
                          pAVFrameRGB32->data,
                          pAVFrameRGB32->linesize);
				
                // 格式为RGBA=8:8:8:8”
                // rmask 应为 0xFF000000  但是颜色不对 改为 0x000000FF 对了
                // gmask     0x00FF0000                  0x0000FF00
                // bmask     0x0000FF00                  0x00FF0000
                // amask     0x000000FF                  0xFF000000
                // 测试了ARGB，也是相反的，而QImage是可以正确加载的
                // 暂时只能说这个地方标记下，可能有什么设置不对什么的
                SDL_UpdateYUVTexture(pSDLTexture,
                                     NULL,
                                     pAVFrame->data[0], pAVFrame->linesize[0],
                                     pAVFrame->data[1], pAVFrame->linesize[1],
                                     pAVFrame->data[2], pAVFrame->linesize[2]);
                
                SDL_RenderClear(pSDLRenderer);
                // Texture复制到Renderer
                SDL_Rect        sdlRect;
                sdlRect.x = 0;
                sdlRect.y = 0;
                sdlRect.w = pAVFrame->width;
                sdlRect.h = pAVFrame->height;
                 SDL_RenderCopy(pSDLRenderer, pSDLTexture, 0, &sdlRect) ;
                // 更新Renderer显示
                SDL_RenderPresent(pSDLRenderer);
                // 事件处理
                SDL_PollEvent(&event);
            }   
        }//视频处理结束
		else 
			if(pAVPacket->stream_index == audio_stream_index)
			{
				avcodec_send_packet(pAudioCodecCtx, pAVPacket);
            
				AVFrame *frame = av_frame_alloc();
				if(avcodec_receive_frame(pAudioCodecCtx, frame) >= 0) {
                
					int uSampleSize = 4;
                
					int data_size = uSampleSize * pAudioCodecCtx->channels * frame->nb_samples;
                
					uint8_t *pBufferIterator = audio_buf + audio_buf_right_index;
					for(int i=0;i<frame->nb_samples;i++){
						for(int j=0;j<pAudioCodecCtx->channels;j++){
							memcpy(pBufferIterator, frame->data[j] + uSampleSize * i, uSampleSize);
							pBufferIterator += uSampleSize;
						}
					}
                
					audio_buf_right_index += data_size;
					wake_up=1;
					if(audio_buf_right_index>sizeof(audio_buf)-10000)
					{
						cout<<"right:"<<audio_buf_right_index<<endl;
						audio_buf_right_index=0;
					}
				}


			}//音频处理结束
			
		GetLocalTime(&nextTime);//获取解码后精确时间
		if(nextTime.wMilliseconds<startTime.wMilliseconds)nextTime.wMilliseconds=nextTime.wMilliseconds+1000;
		int delaytime=int(interval)-int(nextTime.wMilliseconds-startTime.wMilliseconds)-3;
		if(delaytime<0)continue;//解码超时
		if(delaytime<interval)Sleep(delaytime);//正常延迟，防止播放速度过快
		else{Sleep(20);cout<<nextTime.wMilliseconds<<" "<<startTime.wMilliseconds<<delaytime<<endl;}//奇怪的情况，k大于延迟帧率？？？
 
    }
    cout << "释放回收资源"<<endl;

    if(outBuffer)
    {
        av_free(outBuffer);
        outBuffer = 0;
    }
    if(pSwsContext)
    {
        sws_freeContext(pSwsContext);
        pSwsContext = 0;
        cout << "sws_freeContext(pSwsContext)"<<endl;
    }
    if(pAVFrameRGB32)
    {
        av_frame_free(&pAVFrameRGB32);
        pAVFrame = 0;
        cout << "av_frame_free(pAVFrameRGB888)"<<endl;
    }
    if(pAVFrame)
    {
        av_frame_free(&pAVFrame);
        pAVFrame = 0;
        cout << "av_frame_free(pAVFrame)"<<endl;
    }
    if(pAVPacket)
    {
        av_free_packet(pAVPacket);
        pAVPacket = 0;
        cout << "av_free_packet(pAVPacket)"<<endl;
    }
    if(vCodecCtx)
    {
        avcodec_close(vCodecCtx);
        vCodecCtx = 0;
        cout << "avcodec_close(vCodecCtx);";
    }
    if(pAVFormatContext)
    {
        avformat_close_input(&pAVFormatContext);
        avformat_free_context(pAVFormatContext);
        pAVFormatContext = 0;
        cout << "avformat_free_context(pAVFormatContext)"<<endl;
    }

    // 步骤五：销毁渲染器
    SDL_DestroyRenderer(pSDLRenderer);
    // 步骤六：销毁窗口
    SDL_DestroyWindow(pSDLWindow);
    // 步骤七：退出SDL
    SDL_Quit();

}


//#define SDL_AUDIO_BUFFER_SIZE 1024


void show_moive1()
{
	wake_up=0;
	char* filename ="F:\\JY\\www.NewXing.com\\moive\\我们都是超能力者！.mp4";
	static const char *input_filename = "F:\\JY\\www.NewXing.com\\moive\\英雄联盟 .mp3";
	av_register_all();
    avformat_network_init();
    
    AVFormatContext *pFormatContext = avformat_alloc_context();
    avformat_open_input(&pFormatContext, input_filename, nullptr, nullptr);
    avformat_find_stream_info(pFormatContext, nullptr);
    av_dump_format(pFormatContext, 0, input_filename, 0);
    
    int video_stream_index = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    AVCodecContext *pVideoCodecCtx = pFormatContext->streams[video_stream_index]->codec;
    AVCodec *pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
    assert(avcodec_open2(pVideoCodecCtx, pVideoCodec, nullptr) >= 0);

    assert(0==SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER));
    SDL_Window * window = SDL_CreateWindow("ZPlayer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pVideoCodecCtx->width, pVideoCodecCtx->height, SDL_WINDOW_RESIZABLE);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = pVideoCodecCtx->width;
    rect.h = pVideoCodecCtx->height;

    int audio_stream_index = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    AVCodecContext *pAudioCodecCtx = pFormatContext->streams[audio_stream_index]->codec;
    AVCodec *pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
    assert(avcodec_open2(pAudioCodecCtx, pAudioCodec, nullptr) >= 0);

    SDL_AudioSpec desired_spec;
    SDL_AudioSpec obtained_spec;
    desired_spec.freq = pAudioCodecCtx->sample_rate;
    desired_spec.format = AUDIO_F32SYS;
    desired_spec.channels = pAudioCodecCtx->channels;
    desired_spec.silence = 0;
    desired_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    desired_spec.callback = audio_callback;
    desired_spec.userdata = audio_buf;
    
    assert(SDL_OpenAudio(&desired_spec, &obtained_spec) >= 0);
    SDL_PauseAudio(0);
    
    
    AVPacket pkt;
    while(av_read_frame(pFormatContext, &pkt)>=0){
        if(pkt.stream_index == video_stream_index){
            
            avcodec_send_packet(pVideoCodecCtx, &pkt);
            
            AVFrame *picture = av_frame_alloc();
            if(avcodec_receive_frame(pVideoCodecCtx, picture) >= 0){
                SDL_Texture *vid_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, picture->width, picture->height);
                
                SDL_UpdateYUVTexture(vid_texture, NULL,
                                     picture->data[0], picture->linesize[0],
                                     picture->data[1], picture->linesize[1],
                                     picture->data[2], picture->linesize[2]);
                
                SDL_RenderCopyEx(renderer, vid_texture, NULL, &rect, 0, NULL, (SDL_RendererFlip)0);
                SDL_RenderPresent(renderer);
            }
        } 
		else 
			if(pkt.stream_index == audio_stream_index) 
			{
			
				avcodec_send_packet(pAudioCodecCtx, &pkt);
            
				AVFrame *frame = av_frame_alloc();
				if(avcodec_receive_frame(pAudioCodecCtx, frame) >= 0) {
                
					int uSampleSize = 4;
                
					int data_size = uSampleSize * pAudioCodecCtx->channels * frame->nb_samples;
                
					uint8_t *pBufferIterator = audio_buf + audio_buf_right_index;
					for(int i=0;i<frame->nb_samples;i++){
						for(int j=0;j<pAudioCodecCtx->channels;j++){
							memcpy(pBufferIterator, frame->data[j] + uSampleSize * i, uSampleSize);
							pBufferIterator += uSampleSize;
						}
					}
                
					audio_buf_right_index += data_size;
					wake_up=1;
					if(audio_buf_right_index>sizeof(audio_buf)-10000)
					{
						cout<<"right:"<<audio_buf_right_index<<endl;
						audio_buf_right_index=0;
					}
				
				
                
				}
			}
    }
    
    return ;


}




#endif