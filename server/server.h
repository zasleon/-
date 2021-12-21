#ifndef INCLUDE_server_H
#define INCLUDE_server_H

#include "SZSC_service.h"
#include "movie_function.h"
#include "UNO_service.h"
#include <WS2tcpip.h>

//#pragma comment(lib,"F:\\VC6.0PROJECT\\szsc_server\\Debug\\fight_room.lib")
//_declspec(dllexport) void start_fight(client_member* p,fight_room* this_room);

//进行通讯
void communicate_with_this_client_O(client_member* c1)//发送内容"系统超载啦\n"
{
	send(g_ClientSocket[c1->member_No-1],"系统超载啦\n", (1 + ::strlen("系统超载啦\n")), 0);
}

void communicate_with_this_client(client_member* c1)//服务器与客户端进行沟通
{
	char StrBuf[MAX_PATH] = { 0 };//定义变量存储接收到的数据
	char Buf[MAX_PATH] = { 0 };//专门用于转换非字符形式数值为字符
	int choice,count;

	c1->listen_this=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)listen_client_message,c1, NULL, NULL);//开始监听线程

	special_signal(c1,start_link);//将数字强制转换为字符,初次激活
	Sleep(200);
	memset(StrBuf,0,sizeof(StrBuf));
	addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//获取用户输入信息，若断线则结束沟通
	choice=atoi(StrBuf);
	switch(choice)
	{
		case android_phone:c1->device=android_phone;cout<<"安卓端设备!\n";break;
		case win_console:c1->device=win_console;cout<<"win_console端设备!\n";break;
		default:cout<<"未能识别该设备!强制断开连接!\n";client_offline(c1);return;
	}
	//memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,"你是第");addtext(StrBuf,client_number);addtext(StrBuf,"个！！！\n");confirm_send_success(c1,StrBuf);

	memset(c1->client_name,0,sizeof(c1->client_name));
	while(true)//用户输入用户名死循环，直到输入正确的用户名为止
	{
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//获取用户输入信息，若断线则结束沟通
		//登记用户名开始
		if(strlen(StrBuf)<=0){confirm_send_success(c1,"用户名不能为空!\n请重新输入:");continue;}
		if(strlen(StrBuf)<sizeof(c1->client_name))//sizeof()是整个最大容量大小，strlen()是统计当前字符串长度到\0为止
		{addtext(c1->client_name,StrBuf);break;}//登记成功，退出用户输入用户名死循环
		confirm_send_success(c1,"用户输入长度超出上限!\n");
	}
	cout<<"用户名:"<<c1->client_name<<"  "<<"线程号"<<c1->member_No<<endl;
	//用户输入用户名死循环结束

	while(true)//选择开始死循环
	{
			
			//"\n你可以做出以下选择：\n";
			//"|-------------------------------------------------------------------------|\n";
			//"|  选1：创建自己的房间         选2：加入别人的房间           选3：退出房间|\n";
			//"|                                                                         |\n";
			//"|  选3：退出房间               选4：刷新玩家列表）           选5：开始战斗|\n";
			//"|-------------------------------------------------------------------------|\n";
			//"请选择:";
		if(c1->device!=android_phone)special_signal(c1,show_choice);

		cout<<"开始监听用户选择\n";

			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//获取用户输入信息，若断线则结束沟通
			cout<<"客户端发来消息："<<StrBuf<<endl;
			if(c1->state==in_game)//此判定通过，必然是房间宾客进入了SZSC游戏
			{
				start_fight(c1,&player_room[c1->room_No]);
				c1->state=in_room;//游戏结束，回到房间状态

				if(c1->device!=android_phone)special_signal(c1,show_choice);
				continue;
			}

		switch(atoi(StrBuf))
		{
			
			case 8:
				//movie_F(c1);

				break;
			case SZSC_apply_create_room:
			case SZSC_apply_enter_room:
			case SZSC_apply_exit_room:
			case SZSC_apply_start_game:
			case SZSC_apply_add_robot:
			case SZSC_apply_remove_robot:
				SZSC_provide_service(c1,atoi(StrBuf));//提供SZSC游戏服务
				break;
			case UNO_apply_create_room:
			case UNO_apply_enter_room:
			case UNO_apply_exit_room:
				UNO_provide_service(c1,atoi(StrBuf));//提供SZSC游戏服务
				break;

			
			case apply_refresh_online_member://刷新玩家列表
				if(c1->device==win_console)special_signal(c1,please_cls);
				show_all_client(c1);//刷新列表
				break;
			case 0:
				special_signal(c1,you_are_in_the_lobby);




			default:
				if(c1->device==win_console)confirm_send_success(c1,"输入了无效值!\n");
				
		}//switch结束
					
	}//选择开始死循环结束
}

void accept_client()//进行监听，接受客户端发送消息的请求
{
	char StrBuf[MAX_PATH] = { 0 };//定义变量存储接收到的数据
	char Buf[MAX_PATH] = { 0 };//专门用于转换非字符形式数值为字符
	
	/*第四步 接受客户连接请求*/
	sockaddr_in ClientAddr[memberlimit+1]= { 0 };//创建结构体 用于获取客户端 socketaddr 结构体
	int len=sizeof(ClientAddr);//获取结构体大小
	bool claim_limit=false;//是否显示过服务器承载人数达到上限

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)server_monitor,NULL, NULL, NULL);//管理员权限查看所有
	//Sleep(500);
	while(true)//开始监听死循环
	{	
		check_emptyspace();

		if(empty_player_pointer>=memberlimit)//检测服务器负荷是否满载？如果满载
		{
			if(!claim_limit)
			{
				cout<<"服务器承载人数达到最大!无法再继续接收!\n";claim_limit=true;//设为已显示过负荷最大
			}
			//不断接受过量的请求包，并不予回复，当负荷下降时，后接入的第一个客户端将会被忽视
			g_ClientSocket[memberlimit]= accept(g_ServerSocket, (sockaddr*)&ClientAddr[memberlimit], &len);
			hthread[memberlimit]=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communicate_with_this_client_O,&Membership[memberlimit], NULL, NULL);//调用函数与该客户端进行沟通
		}
		else//服务器没满载，进行监听，接受客户端发送消息的请求
		{
			if(claim_limit)//如果之前声明过服务器满载了，由于检查到空位，显示服务器可以继续接受客户端请求了
			{
				CloseHandle(hthread[memberlimit]);
				claim_limit=false;
				cout<<"服务器承载数量下降!可以继续接收客户端了!\n";
			}

			cout<<"继续监听中……（目前有"<<client_number<<"个客户端已连接，上限为"<<memberlimit<<"个）\n";
			g_ClientSocket[empty_player_pointer]= accept(g_ServerSocket, (sockaddr*)&ClientAddr[empty_player_pointer], &len);//接受客户端的连接请求
			
			SOCKADDR_IN   sockAddr; 
			int   iLen=sizeof(sockAddr); 
			getpeername(g_ClientSocket[empty_player_pointer] ,(struct   sockaddr   *)&sockAddr,&iLen);//获取客户端的ip
			cout<<"\n新用户已连接！客户端IP: "<<::inet_ntoa(sockAddr.sin_addr)<<"端口号: "<<ntohs(sockAddr.sin_port)<<endl;


			Membership[empty_player_pointer].member_in_use=true;//将服务器该位置标为“已占用”
			client_number++;
			cout<<"已接受客户端连接请求……\n";
			
		/*第五步 接受数据*/
			//线程（安全属性，初始栈大小，函数名，函数传参，线程状态标志，线程id保存地址）
			//线程状态标志：CREATE_SUSPENDED挂起/0立即激活
			hthread[empty_player_pointer]=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communicate_with_this_client,&Membership[empty_player_pointer], NULL, NULL);//调用函数与该客户端进行沟通

		}//创建和新客户端的沟通连接结束，重新检测服务器是否满载？满载则停止监听，没满载则继续监听是否有下一个客户端的请求

	}//监听死循环结束（除非服务器关闭，否则死循环将一直进行下去）

}







#endif