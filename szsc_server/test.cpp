#include "server.h"
#undef  main;
void main()
{

	//char my_ip[50]="222.222.221.215";//���Լ�����ip����socket//
	char my_ip[50]="10.125.39.201";//���Լ�����ip����socket//
	int my_port=8001;
	
	
	//�������������
	WSADATA  Data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &Data);
	bool wrong_set=false;//Ĭ�ϴ���socket�ɹ�

	while(TCP_InitSocket(my_ip, my_port)==false)//�����������
	{
		if(!wrong_set)
		{
			printf("��ʼ�������˿�ʧ��!\n�뱣֤�������ε����������ڡ����á��롰���߱��γ���״̬!\n��������������ip��");cin>>my_ip;
			//cout<<"������ip:";cin>>my_ip;
			
			wrong_set=true;
			continue;
		}
			
	}
	printf("�����˿ڳ�ʼ���ɹ�!�ȴ��ͻ�������!\n");
	//cout<<"sizeof(fight_room)="<<sizeof(fight_room)<<endl;
	cout<<"sizeof(player_room)��������ռ���ڴ�="<<sizeof(player_room)<<"bit\n";
	cout<<"sizeof(Membership)��������ռ���ڴ�="<<sizeof(Membership)<<"bit\n";

	init_server_set();//��ʼ����������������
	accept_client();//��ʼ���ӿͻ���

	//�رմ���socket
	if (g_ServerSocket != INVALID_SOCKET)
	{
		cout<<"���ڽ��йر�socket����\n";
		closesocket(g_ServerSocket);
		g_ServerSocket = INVALID_SOCKET;
	}
	
	cout<<"����ж������⡭��\n";
	WSACleanup();//ж�������
	return;
}