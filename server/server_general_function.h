#ifndef INCLUDE_server_general_function_H
#define INCLUDE_server_general_function_H

#include "server_property.h"



//通用函数

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

void send_this_message(client_member* c1,char* StrBuf)
{
	//专门用于转换非字符形式数值为字符,Buf用于匹配客户端是否回复了“client_get_message”
	if(sizeof(StrBuf)<MAX_PATH)
	{
		::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//发送内容
		cout<<"发送:"<<StrBuf<<"wait reply\n";
	}
	else
	{
		::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 +sizeof(StrBuf)), 0);//发送内容
		cout<<"发送了数据！\n"<<"wait reply\n";
	}

	int deadline=0;
	while(!check_client_reply(c1))
	{
		deadline++;
		if(deadline>10)
		{
			cout<<c1->client_name<<"强制下线\n";
			c1->F_offline=true;return;
		}
		if(c1->F_offline)return;

		Sleep(50);
		if(sizeof(StrBuf)<MAX_PATH)
		{
			::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//发送内容
			cout<<"再次发送:"<<StrBuf<<"wait reply\n";
		}
		else
		{
			::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + sizeof(StrBuf)), 0);//发送内容
			cout<<"再次发送了数据！\n"<<"wait reply\n";
		}
	}
	return;

}
char* get_client_message(client_member* c1)//获取客户端发送的信息
{
	memset(c1->p_message,0,sizeof(c1->p_message));
	Sleep(10);
	cout<<"等待用户输入\n";
	while(strlen(c1->message)==0)
	{
		if(c1->F_offline)return "";
		Sleep(100);
	}//进行等待
	addtext(c1->p_message,c1->message);
	c1->not_be_read=false;
	Sleep(5);
	cout<<"get message:"<<c1->p_message<<endl;
	return c1->p_message;
}
char* fast_get_client_message(client_member* c1)//快速确认客户端回复内容
{
	cout<<"f\n";
	while(strlen(c1->message)==0)
	{
		if(c1->F_offline)return "";
	}//进行等待
	c1->not_be_read=false;
	Sleep(5);
	return "";
}
bool check_client_reply(client_member* c1)//确认客户端回复内容
{
	int overtime=0;
	Sleep(50);
	char Buf[MAX_PATH]={0 };
	while(strlen(Buf)==0)
	{
		if(c1->F_offline)return false;
		overtime++;
		Sleep(50);
		if(overtime>60){cout<<"超时!\n";return false;}
		addtext(Buf,c1->message);
	}

	if(atoi(Buf)==client_get_message){cout<<"reply right\n";c1->not_be_read=false;return true;}
	cout<<"错误回复:"<<Buf<<endl;
	c1->not_be_read=false;return false;
}



void listen_client_message(client_member* c1)
{	
	c1->listen=true;
	while(true)
	{
		c1->not_be_read=true;
		if(c1->F_offline){cout<<"对"<<c1->client_name<<"监听结束!\n";return;}
		if(recv(g_ClientSocket[c1->member_No-1], c1->message, MAX_PATH, 0) <= 0){c1->F_offline=true;cout<<"对"<<c1->client_name<<"监听结束!\n";c1->listen=false;return;}
		while(c1->not_be_read)Sleep(1);
		memset(c1->message,0,sizeof(c1->message));
	}
}

void confirm_send_success(client_member* c1,char* StrBuf)//检测对方消息是否收到报文StrBuf
{
	if(c1->member_No==robot_symbol)return;//如果是机器人，不用向它发送消息

	if(c1==NULL||!c1->member_in_use||c1->F_offline)return;
	send_this_message(c1,StrBuf);
	Sleep(50);	//创建线程，造成不稳定现象：
				//不加sleep延迟用户会直接秒退
				//加太多又容易重复发送数据,反正这sleep就稍微加这么多了
	return;
}

void special_signal(client_member* c1,int signal)//发送特殊信号给他
{
	char StrBuf[MAX_PATH] = { 0 };
	addtext(StrBuf,signal);addtext(StrBuf,"\n");
	confirm_send_success(c1,StrBuf);
}





void check_emptyspace()//计算当前有无服务器空位
{
	int count;
	for(count=0;count<memberlimit;count++)
		if(Membership[count].member_in_use==false)
			{break;}

	empty_player_pointer=count;//将检测后的count值赋予empty_pointer，当前空位指针
}

void show_all_client(client_member* c1)//向客户端显示所有在线人员列表
{
	int count;
	char StrBuf[MAX_PATH] = {0};

	special_signal(c1,show_user);//向客户端发送“自己即将显示用户列表”

	for(count=0;count<memberlimit;count++)
	{
		if(Membership[count].member_in_use)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf," ");
			addtext(StrBuf,Membership[count].member_No);
			addtext(StrBuf,"号");
			addtext(StrBuf,"\t\t");
			
			switch(Membership[count].state)
			{
				case in_game:addtext(StrBuf,"战斗中\t\t");break;
				case in_online:addtext(StrBuf,"在线\t\t");break;
				case in_room:addtext(StrBuf,"房间中\t\t");break;
			}
			
		addtext(StrBuf,Membership[count].client_name);//输出用户名
		if(Membership[count].member_No==c1->member_No)
				addtext(StrBuf,"(你)");

		addtext(StrBuf,"\n");
		confirm_send_success(c1,StrBuf);
		memset(StrBuf,0,sizeof(StrBuf));
		
		//Sleep(50);//传输速度比较快，需要停几秒，不然传输3个以上用户容易出现丢包现象（只显示2个用户，其他用户的发包都丢了）

		}
	}
	if(c1->device==win_console)
	{
		if(c1->state==in_room)
		{
			fight_room this_room=player_room[c1->room_No];
			addtext(StrBuf,"\n你在");
			addtext(StrBuf,c1->room_No+1);
			addtext(StrBuf,"号房间内!\t");
			if(this_room.c[0]!=NULL&&this_room.c[0]->member_No==c1->member_No)
			{
				addtext(StrBuf,"你是房间主人!");
				if(this_room.c[1]!=NULL)
				{
					cout<<"\n";
					addtext(StrBuf,"房间里还有: ");
					addtext(StrBuf,this_room.c[1]->client_name);
				}
			}
			else
			{
				addtext(StrBuf,"你是房间客人!房间内还有: ");
				addtext(StrBuf,this_room.c[0]->client_name);
			}
			addtext(StrBuf,"\n");
			confirm_send_success(c1,StrBuf);
		}
	}
	special_signal(c1,stop_show_user);//向客户端发送“自己结束显示用户列表”
}


void UNO_exit_room(client_member* member);
void client_offline(client_member* member)//客户端下线结算，在高级服务函数内不能使用这个，检测到离线后全都以return直接全部返回server.h的基础服务主函数，再由基础服务内调用即可。若对同一用户多次调用此函数系统则会出现bug
{
	if(!member->member_in_use)return;
	cout<<"第"<<member->member_No<<"号客户端退出本次聊天!\n";
	if(member->state==in_room||member->state==in_game)
		exit_the_room(member);
	if(member->state==UNO_in_room||member->state==UNO_in_game)
		UNO_exit_room(member);
	if(member->listen)
	{
		cout<<"强制关闭监听线程\n";
		TerminateThread(member->listen_this,0);
	}

	member->state=in_online;
	memset(member->client_name,0,sizeof(member->client_name));
	addtext(member->client_name,"未命名");
	client_number--;
	member->member_in_use=false;
	member->F_offline=false;
	check_emptyspace();
	cout<<"（目前还有"<<client_number<<"个客户端已连接）\n";
}

void UNO_ini_room();
void init_server_set()//初始化服务器配置
{
	ini_catalog();//初始化卡片目录
	UNO_ini_room();
	client_number=0;//总连接人员初始化为0
	empty_player_pointer=0;//当前服务器空位指针初始化为0
	int count,count2;//服务器空位全部初始化
	for(count=0;count<=memberlimit;count++)//初始化接受客户端
	{
		Membership[count].member_No=count+1;
		Membership[count].member_in_use=false;
		Membership[count].p1=&Membership[count].body;
		addtext(Membership[count].client_name,"未命名");
		Membership[count].state=in_online;
		Membership[count].guest=false;
		Membership[count].be_locked=false;
		Membership[count].F_offline=false;
		Membership[count].listen=false;
		memset(Membership[count].message,0,sizeof(Membership[count].message));
		memset(Membership[count].p_message,0,sizeof(Membership[count].p_message));

	}
	for(count=0;count<roomlimit;count++)//初始化战斗房间
	{
		for(count2=0;count2<playernumber;count2++)
			player_room[count].c[count2]=NULL;
		player_room[count].room_in_use=false;
		player_room[count].ready_number=0;

		Robot[count].member_No=robot_symbol;
		Robot[count].p1=&Robot[count].body;
		addtext(Robot[count].client_name,"系统机器人");
	}

}


void show_room_state()//服务器上显示被使用的房间
{
/*
	int count;
	cout<<"房间情况：\n";
	cout<<"----------------------\n";
	for(count=0;count<roomlimit;count++)
		if(player_room[count].room_in_use)
		{
			cout<<count<<"号房间\t"<<player_room[count].c1->client_name<<"\t";
			if(player_room[count].c2_in_use==true)
				cout<<player_room[count].c2->client_name;
			cout<<endl;
		}
		else 
			break;
	cout<<"----------------------\n";
	*/
}
void server_monitor()//服务端监视数据
{
	Sleep(1000);
	char Buf[MAX_PATH] = {0};
	int choice,count;
	fight_room* this_room;
	while(true)
	{
		cout<<"请问你想做什么？1.刷新房间情况	2.查看某一房间数据	3.刷新在线人员\n你选择：";
		cin>>(Buf);
		choice=atoi(Buf);
		if(choice==0)continue;
		switch(choice)
		{
			case 1:show_room_state();break;
			case 2:
				cout<<"请输入房间号：";
				cin>>(Buf);
				choice=atoi(Buf);
				if(choice>500||choice<=0)break;
				choice--;
				this_room=&player_room[choice];
				if(!this_room->room_in_use)break;
				if(this_room->c[0]->state!=in_game){cout<<"没在战斗中！\n";break;}
				if(this_room->c[1]==NULL)break;
				system("cls");
				show_character(this_room->c[0]->p1,true);
				for(count=0;count<10&&this_room->c[0]->p1->c[count];count++)
					cout<<search_card(p_name,this_room->c[0]->p1->card[count])<<endl;
				show_character(this_room->c[1]->p1,false);
				for(count=0;count<10&&this_room->c[1]->p1->c[count];count++)
					cout<<search_card(p_name,this_room->c[1]->p1->card[count])<<endl;
				break;
			case 3:
				system("cls");
				cout<<"\n当前所有在线用户信息如下：";
				cout<<"\n-----------------------------------------------\n";
				cout<<" 序列号\t\t状态\t\t用户名\n\n";
				for(count=0;count<memberlimit;count++)
				{
					if(Membership[count].member_in_use)
					{
						
						cout<<" "<<Membership[count].member_No<<"号\t\t";
		
						switch(Membership[count].state)
						{
							case in_game:cout<<"战斗中\t\t";break;
							case in_online:cout<<"在线\t\t";break;
							case in_room:cout<<"房间中\t\t";break;
						}

						cout<<Membership[count].client_name<<"\n";//输出用户名

					}
					else
						break;
				}
				cout<<"\n-----------------------------------------------\n";
				break;
		}//switch结束
	}
}




#endif