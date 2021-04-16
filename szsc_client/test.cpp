#include "client.h"



#undef main;
void main()
{
	//47.102.218.193
	//10.125.39.201 本地	8001
	//103.46.128.21 花生壳	34024
	//182.92.237.216

	//char my_ip[50]="103.46.128.21";// 设置目标服务器IP
	//int portnumber=34024;//设置目标服务器端口号
	char my_ip[50]="10.125.39.201";// 设置目标服务器IP
	int portnumber=8001;//设置目标服务器端口号
	//char my_ip[50]="222.222.221.215";// 设置目标服务器IP
	//int portnumber=8001;//设置目标服务器端口号

	connect_situation=false;//连接状态，成功或失败，初始设置为失败
	bool connect_failed_claim=false;//是否已显示连接失败（为了只显示一次）
	bool connect_success_claim=false;//是否已显示连接成功

	server_overload=true;//防止服务器超载，变为true意思是服务器告诉你他超载
	create_thread=false;//尚未创建线程
	claim_overload=false;//尚未声明服务器超载
	//cout<<"请输入服务器ip:";cin>>my_ip;

	player player_all[playernumber];//创建游戏人物列表，p[0]永远是自己，目前最多两个人
	c1=&player_all[0];
	c2=&player_all[1];

	int data_state=no_info;state=&data_state;//人物信息，no_info为没有进入游戏
	
	int process_state;state_p=&process_state;//程序信息，为第几个程序？第一或第二
	if(!control_number())return;//防止程序多开
	
	char reply[260] = { 0 };sprintf(reply,"%d",client_get_message);replymessage=reply;//replymessage指向"回复收到"专用字符串
	
	char StrBuf[260] = { 0 };getmessage=StrBuf;//getmessage指向专门用来接收服务器发来数据的缓冲字符串，专门接收人物数据信息
	
	int count;
	for(count=0;count<playernumber;count++)
	ini_character(&player_all[count],162359);




	while(1)//调试连接开始
	{

		if(connect_situation==false)//如果连接状态为失败
		{
			
			if(Connection(my_ip, portnumber)==true)//开始尝试连接，如果返回结果成功
			{
				connect_situation=true;connect_failed_claim=false;//连接状态改为成功	
			}
			else 
			{
				if(!connect_failed_claim)
					{cout<<"\n目前连接不到服务器!\n";connect_failed_claim=true;connect_success_claim=false;}//如果没显示过失败，进行显示。如果显示过了，跳过此步骤
				Sleep(1000);
			}
			
			//else printf("连接出现问题!\n可能是服务器没开启!也可能是本地局域网地址配置错误!\n请重新输入本地局域网ip:");cin>>my_ip;	
		}

		if(connect_situation)//如果连接成功，开始传输数据
		{
			if(!connect_success_claim)//是否声明过“连接成功”？，如果没有，执行下面命令
			{			
				
				if(!create_thread)//是否创建过接收、发送数据的多线程？如果没有，进行创建
				{
					hthread_R	=	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Receive_Message, NULL, NULL, NULL);//接收信息
					hthread		=	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Send_Message, NULL, NULL, NULL);//发送信息
					create_thread=true;
				}
				
				Sleep(5000);//稍微等待几秒，接受服务器数据，探测服务器是否超载,不进行等待容易造成数据处理混乱，显示乱码
				//没超载时会发回start_link，表明服务器没超载，可以开始传输

				if(server_overload&&!claim_overload)//如果探测到服务器超载且没有声明过，进行申明
				{
					cout<<"\n检测到服务器!\n";
					cout<<"连接超时!可能服务器运行已达负荷上限!\n";
					claim_overload=true;
					//return;
				}
				if(!server_overload)//如果没探测到服务器超载,连接成功!
				{
					claim_overload=false;connect_success_claim=true;//声明过了
				}

			}
		}//连接成功，传输数据结束
		
	}//调试连接结束（理论上只要exe开着，客户端运行就不会结束）

}
