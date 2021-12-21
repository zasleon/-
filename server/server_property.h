#ifndef INCLUDE_server_property_H
#define INCLUDE_server_property_H

#include "iostream"
#include "winsock2.h"
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>

#include "UNO_property.h"//UNO游戏数据
#include "card_effect.h"//SZSC游戏数据
using namespace std;
#pragma comment(lib,"Ws2_32.lib")


#define memberlimit					20//服务器承载人数上限
#define sleeptime					50//服务器稍微延迟发送数据时间



#define start_link					6666//告诉客户端成功通讯连接了（当客户端收不到这个数据时会告诉自己“服务器可能超载了”）
#define please_cls					6667//让客户端刷新屏幕system("cls");
#define show_choice					6668//让客户端显示选择界面
#define show_user					6669//让客户端显示所有用户状态
#define	stop_show_user				6700//停止显示所有用户状态
#define client_get_message			6701//客户端发来自己接收到信息
#define show_video					6702//视频功能

#define	in_game						6800//处于游戏状态
#define in_room						6801//处于在房间状态
#define	in_online					6802//处于在线状态


#define force_offline				7000//客户端下线了
#define you_are_in_the_lobby		7000//用户输入0时，让其回到用户界面

#define apply_refresh_online_member	-4//请求刷新页面



#define android_phone				9999//安卓手机端
#define win_console					9998//win32控制台版

int client_number;//总连接人员





struct UNO_player;
struct client_member//客户端
{
	int member_No;
	bool member_in_use;//客户端该位置是否被占用？false为空，true为占用
	char client_name[30];//用户名

	int device;//手机或者电脑
	int state;//客户状态：房间/在线/战斗中
	int room_No;//自己所在房间号
	bool guest;//在房间中是否是宾客？true为宾客，false可能为主人或不在房间内

	bool F_offline;//是否掉线

	player* p1;
	player body;

	UNO_player* uno_player;

	bool be_locked; //是否被锁（不听该客户端发来的内容）

	char message[MAX_PATH];
	bool not_be_read;//客户端发来的message是否还没被系统读（分析），true为没被读，false为读了
	bool listen;//是否监听中
	HANDLE listen_this;//指向“监听客户端发来内容”的线程，在accept客户端后创建该线程并附给这个指针
	char p_message[MAX_PATH];//系统将客户端发来的信息先存储在message，要处理时放入p_message处理

};

client_member Robot[roomlimit];//房间机器人，SZSC专用


client_member Membership[memberlimit+1];
SOCKET g_ServerSocket, g_ClientSocket[memberlimit+1]; //创建服务端 客户端套接字
HANDLE hthread[memberlimit+1];
















void addtext(char* StrBuf,int number);
void addtext(char* StrBuf,char* Buf);
void addtext(char* StrBuf,float number);


#endif