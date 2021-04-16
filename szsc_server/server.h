#ifndef INCLUDE_server_H
#define INCLUDE_server_H

#include "movie_function.h"


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
	Sleep(100);
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
	cout<<"用户名:"<<c1->client_name<<endl;
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
			if(c1->state==in_game)//此判定通过，必然是房间宾客进入了游戏
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

			case 1://创建房间
				if(c1->device!=android_phone)confirm_send_success(c1,"您选择了创建房间!\n");
				if(c1->state==in_room)
				{confirm_send_success(c1,"你已在房间内!无法继续创建!\n");break;}
				if(!check_empty_room())//检查是否有空房间
				{confirm_send_success(c1,"当前房间已满，无法继续创建!\n");break;}
				
				player_room[empty_room_pointer].room_in_use=true;
				player_room[empty_room_pointer].c[0]=c1;
				c1->state=in_room;
				c1->room_No=empty_room_pointer;

				special_signal(c1,get_in_room);//创建成功!
					
				show_room_state();
				break;
			case 2:
				if(c1->device!=android_phone)confirm_send_success(c1,"您选择了加入房间!\n");
				if(c1->state==in_room||c1->state==in_game)
				{confirm_send_success(c1,"你已在房间内!无法继续加入!\n");continue;}
				if(!check_in_use_room())
				{special_signal(c1,no_room_in_use);continue;}
				//{confirm_send_success(c1,"当前没有使用正在使用的房间!请重新选择!\n");continue;}

				special_signal(c1,please_choose_room);//表明可以选择房间了

				show_all_room(c1);
				while(true)//选择房间死循环开始
				{				

					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//玩家做出选择，如果此时断开通讯

					cout<<"用户输入："<<StrBuf<<endl;
					if(strlen(StrBuf)==1&&StrBuf[0]=='0')
					{	if(c1->device!=android_phone)
							confirm_send_success(c1,"你选择取消加入房间!即将返回原页面……\n");
						else
							special_signal(c1,cancel_enter_room);
						break;
					}
						
					choice=atoi(StrBuf);//转换数据为数字
						
					if(choice==0){confirm_send_success(c1,"输入了无效值!\n");continue;}
					if(choice>=roomlimit||choice<0){confirm_send_success(c1,"输入超出房间数量上限!\n");continue;}

					choice--;

					if(player_room[choice].room_in_use==false){confirm_send_success(c1,"该房间内没人!\n");continue;}
					if(player_room[choice].c[playernumber-1]!=NULL){confirm_send_success(c1,"该房间内人满!\n");continue;}
							
					c1->state=in_room;//执行到这一步必然加入成功
					c1->guest=true;//加入房间的人是房间客人

					player_room[choice].c[1]=c1;
					c1->room_No=player_room[choice].c[0]->room_No;

					special_signal(c1,enter_room_success);//加入成功!

					SuspendThread(hthread[player_room[choice].c[0]->member_No-1]);//锁住房间主人监听线程
					special_signal(player_room[choice].c[0],someone_get_in);//告诉房间主人有人进来了
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//告诉房间主人进来者姓名
					confirm_send_success(player_room[choice].c[0],StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					if(player_room[choice].c[0]->device==win_console)special_signal(player_room[choice].c[0],show_choice);//如果是win_console客户端，显示选项
					ResumeThread(hthread[player_room[choice].c[0]->member_No-1]);//解放房间主人监听线程
					show_room_state();//服务器自己显示现在房间状态
					break;
			
				}//选择房间死循环结束
				break;
			case 3:
				if(c1->device==win_console)confirm_send_success(c1,"您选择了退出房间!\n");
				if(c1->state==in_game){confirm_send_success(c1,"正在游戏中!无法退出房间!\n");continue;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);continue;}//你当前没在房间内!请重新选择

				exit_the_room(c1);//执行退出
				special_signal(c1,leave_room);//告诉他退出成功!

				show_room_state();//服务器自己显示现在房间状态
				break;
			case 4://刷新玩家列表
				if(c1->device==win_console)special_signal(c1,please_cls);
				show_all_client(c1);//刷新列表
				continue;
			case 5:
				if(c1->device==win_console)confirm_send_success(c1,"您选择了开始战斗!\n");

				if(c1->state==in_game){confirm_send_success(c1,"你已在游戏中!无法开始新的!\n");continue;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);continue;}//你不在房间内!无法开始战斗!
				if(c1->guest){confirm_send_success(c1,"你不是房间主人!无法开始战斗!\n");continue;}//如果不是房间主人
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"房间内人数不足!无法开始战斗!\n");continue;}//如果房间没有宾客
				
				//执行到这里，已经确定在房间中当主人，而且有宾客
				for(count=0;count<playernumber;count++)
					if(player_room[c1->room_No].c[count]!=NULL)
					{
						player_room[c1->room_No].c[count]->state=in_game;
						if(count!=0)special_signal(player_room[c1->room_No].c[count],event_happen_N);//由于他人还卡在server服务端，用event_happen_N激活一下进入战斗函数
					}
					else
					{
						for(count=0;count<playernumber;count++)
							if(player_room[c1->room_No].c[count]!=NULL)
								player_room[c1->room_No].c[count]->state=in_room;
						confirm_send_success(c1,"房间内人数不足!无法开始战斗!\n");break;
					}
				if(count!=playernumber)break;

				start_fight(c1,&player_room[c1->room_No]);//执行完表示游戏结束
				c1->state=in_room;//游戏结束，回到房间状态
				break;
			case 6://添加机器人
				if(c1->state!=in_room)//确定不是在游戏中，如果不在房间内
				{special_signal(c1,you_not_in_room);continue;}//你不在房间内!
				if(player_room[c1->room_No].c[0]->member_No!=c1->member_No){confirm_send_success(c1,"只有房间主人才能添加机器人!\n");continue;}	
				if(player_room[c1->room_No].c[1]!=NULL){confirm_send_success(c1,"房间内已有别人!无法添加!\n");continue;}
				player_room[c1->room_No].c[1]=&Robot[c1->room_No];
				confirm_send_success(c1,"添加成功!!!\n");
				break;
			case 7://移除机器人
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);continue;}//如果不在房间内
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"房间内没有机器人!无法移除!\n");continue;}
				if(player_room[c1->room_No].c[1]->member_No==c1->member_No){confirm_send_success(c1,"你是房间客人，只有房间主人有权限修改机器人!\n");continue;}
				if(player_room[c1->room_No].c[1]->member_No!=robot_symbol){confirm_send_success(c1,"房间内有活人!无法移除机器人!\n");continue;}
				player_room[c1->room_No].c[1]=NULL;//进行移除
				confirm_send_success(c1,"移除成功!\n");
				continue;

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



bool TCP_InitSocket(char *Addr, int Port)
{
	/*第一步 调用socket函数创建socket*/
	g_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);  
	//如果创建失败则返回
	if(g_ServerSocket == INVALID_SOCKET){cout<<"创建ip失败!\n";return false;}

	/*第二步 bind函数进行ip绑定*/
	sockaddr_in ServerAddr;

	//inet_addr函数将字符串转换为IN_ADDR结构的正确地址
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(Addr);//设置绑定的IP
	ServerAddr.sin_family = AF_INET;//指定地址族为IPv4
	ServerAddr.sin_port = htons(Port);//设置监听端口

	//如果绑定失败则返回
	if(bind(g_ServerSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) != 0)
		{return false;}//cout<<"绑定ip失败!\n";

	/*第三步 监听端口*/
	if(listen(g_ServerSocket, SOMAXCONN) != 0)return false;//如果监听失败返回

	return true;//返回创建监听端口成功
}



#endif