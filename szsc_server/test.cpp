#include "server.h"
#undef  main;
void main()
{

	//char my_ip[50]="222.222.221.215";//用自己电脑ip创建socket//
	char my_ip[50]="10.125.39.201";//用自己电脑ip创建socket//
	int my_port=8001;
	
	
	//加载网络网络库
	WSADATA  Data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &Data);
	bool wrong_set=false;//默认创建socket成功

	while(TCP_InitSocket(my_ip, my_port)==false)//创建网络监听
	{
		if(!wrong_set)
		{
			printf("初始化监听端口失败!\n请保证设置网段的网卡不处于“禁用”与“网线被拔出”状态!\n或者请重新设置ip：");cin>>my_ip;
			//cout<<"请配置ip:";cin>>my_ip;
			
			wrong_set=true;
			continue;
		}
			
	}
	printf("监听端口初始化成功!等待客户端连接!\n");
	//cout<<"sizeof(fight_room)="<<sizeof(fight_room)<<endl;
	cout<<"sizeof(player_room)创建房间占用内存="<<sizeof(player_room)<<"bit\n";
	cout<<"sizeof(Membership)创建人物占用内存="<<sizeof(Membership)<<"bit\n";

	init_server_set();//初始化服务器各种配置
	accept_client();//开始连接客户端

	//关闭创建socket
	if (g_ServerSocket != INVALID_SOCKET)
	{
		cout<<"正在进行关闭socket……\n";
		closesocket(g_ServerSocket);
		g_ServerSocket = INVALID_SOCKET;
	}
	
	cout<<"正在卸载网络库……\n";
	WSACleanup();//卸载网络库
	return;
}
