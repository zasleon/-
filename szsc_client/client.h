
#include "special_transmit.h"
#include "client_movie.h"



void C_Receive_Message()// 接受数据
{
	char szBuf[MAX_PATH] = {0};//缓冲数据，用来接收
	char Buf[MAX_PATH] = {0};//缓冲数据，用来强制转换一些非字符串为字符串
	int possible_message;
	stop_send_CGM=false;
	ini_catalog();//初始化卡牌目录

	while(true)
	{
		memset(szBuf,0,sizeof(szBuf));memset(Buf,0,sizeof(Buf));
		if (0>=recv(Client, szBuf, MAX_PATH, 0))//尝试接受数据，如果返回结果的标志符为-1，表明断开连接
		{
			printf("\n已与服务器断开!\n");
			connect_situation=false;//连接状态改为连接失败
			server_overload=true;//服务器是否超载变为未知
			server_overload=true;//防止服务器超载，变为true意思是服务器告诉你他超载
			create_thread=false;//尚未创建线程
			claim_overload=false;//尚未声明服务器超载
			break;
		}
		Sleep(50);
		

		//SuspendThread(hthread);//防止用户输入过快而出错
		//if(vc6)fflush(stdin);else cin.ignore(1, EOF);
		strcpy(Buf,szBuf);

		if(strlen(Buf)!=5)//检查发来的字符串是否是特殊信号
			possible_message=0;
		else
		{
			Buf[4]='\0';
			possible_message=atoi(Buf);
		}
		switch(possible_message)
		{
			case 0:
				cout<<szBuf;break;//该字符串无法转换为数字,输出内容

			case start_link://将start_link（7070）强制转换为字符串
			//初次成功连接服务器，会发来start_link（7070）,如果szBuf=Buf，则返回零
				ini_character(c1,162359);ini_character(c2,162359);//初始化人物角色//初始化人物角色
				memset(szBuf,0,sizeof(szBuf));
				reply_i_get();

				sprintf(szBuf,"%d",win_console);
				send(Client, szBuf, (1 + ::strlen(szBuf)), 0);//发送自己是windows控制台端

				*state=no_info;//人物信息，no_info为没有进入游戏
				server_overload=false;//将服务器是否超载改为否
				cout<<"您已登录!\n\n请创建您的用户名:";
				break;

			case start_turn_settle://开始进行回合结算

				cout<<"进入回合结算!……\n";
				//SuspendThread(hthread);//禁止用户输入
				break;

			case start_another_turn://开始新的一回合
				cout<<"你可以开始新一回合的行动了!\n";

				//ResumeThread(hthread);//解放用户输入自由
				break;
			
			case event_end://解除自己自由行动
			
				//cin.ignore(1,EOF);
				//cout<<"\n对方行动事件结束!\n";
				Sleep(50);
				reply_i_get();
				Sleep(50);
				
				
				memset(Buf,0,sizeof(Buf));
				
				break;
			
			case event_happen://锁死自己行动
			
				cout<<"\n等待对方做出下一步行动……\n";
				Sleep(30);
				reply_i_get();
				Sleep(30);
				
				
				memset(Buf,0,sizeof(Buf));
				break;
			
			case event_happen_N://锁死自己行动，不输出“等待对方做出下一步行动”，为内部处理信息过程，理论上很快就过去，用户注意不到该过程		
				Sleep(30);
				reply_i_get();
				Sleep(30);
				memset(Buf,0,sizeof(Buf));
				break;

			case please_cls://清空界面，刷新列表
		
				system("cls");
				break;
			case whether_launch_hideeffect:
				cout<<"是否发动卡片隐效果？（1：正常效果，0：隐效果）\n你选择：";
				break;
			
			case choice_complete	:			
				cout<<"\n所有人角色选取完毕!\n";
				break;

			case  whether_fight_back:		
				cout<<"是否反击？你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.加附效果）\n请选择：";
				break;

			case someone_offline:
				cout<<"\n有人断线!\n";
				break;

			case card_launch:
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));
				if (0>=recv(Client, szBuf, MAX_PATH, 0))//接收发动者名字
					return;
				cout<<szBuf<<"发动卡片:";
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));
				if (0>=recv(Client, szBuf, MAX_PATH, 0))//接收发动卡片号
					return;
				cout<<search_card(p_name,atoi(szBuf))<<endl;
				cout<<"卡片效果:"<<search_card(p_details,atoi(szBuf))<<endl;
				reply_i_get();
				stop_send_CGM=true;
				break;

			case someone_leave:		
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));
				if (0>=recv(Client, szBuf, MAX_PATH, 0))break;//接受姓名或you_are_host信号
				system("cls");
				cout<<"\n-------------------------------\n (";
				
				if(atoi(szBuf)==you_are_host)
				{
					memset(szBuf,0,sizeof(szBuf));
					reply_i_get();
					if (0>=recv(Client, szBuf, MAX_PATH, 0))break;
					szBuf[strlen(szBuf)-1]='\0';
					cout<<szBuf<<"离开了房间!你成为房间主人!)\n-------------------------------\n";
				}
				else
				{
					szBuf[strlen(szBuf)-1]='\0';
					cout<<szBuf<<"离开了房间!)\n-------------------------------\n";
				}

				break;

			case someone_get_in:
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));				
				if (0>=recv(Client, szBuf, MAX_PATH, 0))break;
				system("cls");
				cout<<"\n-------------------------------\n (";
				szBuf[strlen(szBuf)-1]='\0';
				cout<<szBuf;
				cout<<"加入了你的房间!)\n-------------------------------\n";

				break;

			case  no_room_in_use:
				cout<<"当前没有使用正在使用的房间!请重新选择!\n";
				break;

			case  you_not_in_room:
				cout<<"你当前没在房间内!请重新选择!\n";
				break;

			case enter_room_success://加入房间成功
				cout<<"加入成功!\n";
				break;

			case leave_room:
				cout<<"退出成功!\n";
				break;
			case get_in_room:
				cout<<"创建成功!\n";
				break;

			case please_choose_room:
				cout<<"\n目前的房间情况如下：\n";
				reply_i_get();
				while(recv(Client, szBuf, MAX_PATH, 0)>=0)
				{
					reply_i_get();
					if(strlen(szBuf)==5)
					{
						szBuf[4]='\0';
						if(atoi(szBuf)==stop_show_room)
							break;
					}
					else
						cout<<szBuf;
				}
				cout<<"\n请输入要加入的房间号（输入0取消加入房间）\n请输入：";
				stop_send_CGM=true;
				break;

			case now_is_your_turn:
				cout<<"现在是你的回合!\n";
				break;

			case you_win:
				cout<<"======================\t 你赢啦!!! \t======================\n";
				break;
			case you_lose:
				cout<<"======================\t 你输啦!!! \t======================\n";
				break;
			case game_standoff:
				cout<<"======================\t 平局!!! \t======================\n";
				break;
			
			case show_choice://显示房间界面
			
				*state=no_info;
				cout<<"\n\n你可以做出以下选择：\n";
				cout<<"|---------------------------------------------------------------------------|\n";
				cout<<"|  选1：创建自己的房间         选2：加入别人的房间           选3：退出房间  |\n";
				cout<<"|                                                                           |\n";
				cout<<"|  选4：刷新玩家列表           选5：开始战斗                 选6：添加机器人|\n";
				cout<<"|                                                                           |\n";
				cout<<"|  选7：移除机器人                                                          |\n";
				cout<<"|---------------------------------------------------------------------------|\n";
				cout<<"请选择:";
				
				break;
			
			case show_user://将show_user  7071强制转换为字符串		
				stop_send_CGM=true;//上面回复过了，不用再回复了
				cout<<"\n当前所有在线用户信息如下：";
							cout<<"\n-----------------------------------------------\n";
				cout<<" 序列号\t\t状态\t\t用户名\n\n";
				Sleep(30);
				reply_i_get();
				while(recv(Client, szBuf, MAX_PATH, 0)>0)
				{
					memset(Buf,0,sizeof(Buf));
					strcpy(Buf,szBuf);

					if(strlen(Buf)!=5)//检查发来的字符串是否是特殊信号
					{
						cout<<szBuf;
						Sleep(30);
						reply_i_get();
						memset(szBuf,0,sizeof(szBuf));
					}
					else
					{
						Buf[4]='\0';
						if(atoi(Buf)==stop_show_user)
						{
							cout<<"\n-----------------------------------------------\n";
							Sleep(30);
							reply_i_get();
							break;
						}
					}
				}
				break;
			
			case battle_choice:
			
				cout<<"你可以选择以下行动：\n";
				cout<<"---------------------------------------------------------------------------|\n";
				cout<<"1.进行普攻            2.发动角色自身效果      3.使用手卡                   |\n";
				cout<<"                                                                           |\n";
				cout<<"4.丢弃已装备的武器    5.发动已装备武器的效果  6.装备武器     7.结束回合    |\n";
				cout<<"---------------------------------------------------------------------------|\n";
				cout<<"如何行动？你选择:";
				break;
			
			case character_choice:
			
				system("cls");
				cout<<"目前开放的可使用角色:\n";
				cout<<"|-----------------------------------------------------------------|\n";
				cout<<"| 1.战士                                                          |\n";
				cout<<"|【自己每次主动普攻时】此次普攻结束前自己攻+1                     |\n";
				cout<<"| 2.法士                                                          |\n";
				cout<<"|【自己回合自己每次成功发动手卡时】自己可指定1人-1血              |\n";
				cout<<"|                                                                 |\n";
				cout<<"| (想使用其他角色请联系开发人员进行氪金定制!)                     |\n";
				cout<<"|-----------------------------------------------------------------|\n";
				cout<<"请输入选择:";
				stop_send_CGM=true;
				break;

			case refresh_state_blood_M://接受服务器发来的自己血量

				reply_i_get();
				recv_server_transmit_B(true);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;
			
			case refresh_state_blood_H://接受服务器发来的对手血量

				reply_i_get();
				recv_server_transmit_B(false);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;

			case refresh_state_CM://接受服务器发来的对手人物手卡数据
				
				reply_i_get();
				recv_server_transmit_C(true);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;
			case refresh_state_CH://接受服务器发来的对手人物手卡数据
				
				reply_i_get();
				recv_server_transmit_C(false);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;
			case refresh_state_attack_M://接受服务器发来的自己攻击力数据
				
				reply_i_get();
				recv_server_transmit_A(true);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;

			case refresh_state_attack_H://接受服务器发来的对手攻击力数据
				
				reply_i_get();
				recv_server_transmit_A(false);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;

			case refresh_state_weapon_M://接受服务器发来的对手武器数据
				
				reply_i_get();
				recv_server_transmit_W(true);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;

			case refresh_state_weapon_H://接受服务器发来的对手武器数据
				
				reply_i_get();
				recv_server_transmit_W(false);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;

			case refresh_state_buff_H://对手buff信息

				reply_i_get();
				recv_server_transmit_buff(false);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;

			case refresh_state_buff_M://自己buff信息

				reply_i_get();
				recv_server_transmit_buff(true);//正式接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;


			
			case refresh_first_state:
				
				reply_i_get();
				recv_server_transmit_F();//接受信息传输
				stop_send_CGM=true;//上面回复过了，不用再回复了
				*state=have_been_changed;//人物信息被修改了
				break;
			case show_video:
				reply_i_get();
				get_movie();
				stop_send_CGM=true;//上面回复过了，不用再回复了
				break;
		
			
		}
		
		if(!stop_send_CGM)//是否需要回复？
		{
			Sleep(50);
			reply_i_get();
		}
		else
			stop_send_CGM=false;//置为需要回复
		//ResumeThread(hthread);//处理完接收到的信息，解放用户输入自由
		
		
	}
}


void C_Send_Message()//用户主动发送数据
{
	char szBuf[MAX_PATH] = {0};//缓冲数据，用来发出
	// 发送数据	
	while (true)// 开始传输信息
	{	
		if(connect_situation==false)
			break;
		memset(szBuf,0,sizeof(szBuf));
		
		
			cin>>szBuf;//获取数据
		
			send(Client, szBuf, (1 + ::strlen(szBuf)), 0);

			Sleep(300);//防止用户连续过快输入
	}
}


//连接服务器
bool Connection(char *lpszServerIp, int iServerPort)//lpszServerIp=服务器ip，iServerPort=使用端口号
{
	// 初始化 Winsock 库
	WSADATA wsaData = { 0 };
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
	// 创建流式套接字
	Client=::socket(AF_INET, SOCK_STREAM, 0);
	if(Client==INVALID_SOCKET){cout<<"创建失败!\n";return false;}////如果创建套接字返回INVALID_SOCKET表明创建失败

	// 设置服务端地址和端口信息
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(iServerPort);//指定连接端口
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszServerIp);//连接的IP

	// connect 尝试连接到服务器，成功则返回0, 失败返回-1
	if(!claim_overload)//如果服务器存在，connect连接必定成功，但服务器是否超载就无法在此函数内判定，需要另建超载判定策略
		if (0 != ::connect(Client, (sockaddr *)(&addr), sizeof(addr))){cout<<"连接服务器失败!\n";return false;}//
	
	return true;//返回创建连接成功
}

