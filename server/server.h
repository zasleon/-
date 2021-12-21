#ifndef INCLUDE_server_H
#define INCLUDE_server_H

#include "SZSC_service.h"
#include "movie_function.h"
#include "UNO_service.h"
#include <WS2tcpip.h>

//#pragma comment(lib,"F:\\VC6.0PROJECT\\szsc_server\\Debug\\fight_room.lib")
//_declspec(dllexport) void start_fight(client_member* p,fight_room* this_room);

//����ͨѶ
void communicate_with_this_client_O(client_member* c1)//��������"ϵͳ������\n"
{
	send(g_ClientSocket[c1->member_No-1],"ϵͳ������\n", (1 + ::strlen("ϵͳ������\n")), 0);
}

void communicate_with_this_client(client_member* c1)//��������ͻ��˽��й�ͨ
{
	char StrBuf[MAX_PATH] = { 0 };//��������洢���յ�������
	char Buf[MAX_PATH] = { 0 };//ר������ת�����ַ���ʽ��ֵΪ�ַ�
	int choice,count;

	c1->listen_this=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)listen_client_message,c1, NULL, NULL);//��ʼ�����߳�

	special_signal(c1,start_link);//������ǿ��ת��Ϊ�ַ�,���μ���
	Sleep(200);
	memset(StrBuf,0,sizeof(StrBuf));
	addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//��ȡ�û�������Ϣ���������������ͨ
	choice=atoi(StrBuf);
	switch(choice)
	{
		case android_phone:c1->device=android_phone;cout<<"��׿���豸!\n";break;
		case win_console:c1->device=win_console;cout<<"win_console���豸!\n";break;
		default:cout<<"δ��ʶ����豸!ǿ�ƶϿ�����!\n";client_offline(c1);return;
	}
	//memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,"���ǵ�");addtext(StrBuf,client_number);addtext(StrBuf,"��������\n");confirm_send_success(c1,StrBuf);

	memset(c1->client_name,0,sizeof(c1->client_name));
	while(true)//�û������û�����ѭ����ֱ��������ȷ���û���Ϊֹ
	{
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//��ȡ�û�������Ϣ���������������ͨ
		//�Ǽ��û�����ʼ
		if(strlen(StrBuf)<=0){confirm_send_success(c1,"�û�������Ϊ��!\n����������:");continue;}
		if(strlen(StrBuf)<sizeof(c1->client_name))//sizeof()���������������С��strlen()��ͳ�Ƶ�ǰ�ַ������ȵ�\0Ϊֹ
		{addtext(c1->client_name,StrBuf);break;}//�Ǽǳɹ����˳��û������û�����ѭ��
		confirm_send_success(c1,"�û����볤�ȳ�������!\n");
	}
	cout<<"�û���:"<<c1->client_name<<"  "<<"�̺߳�"<<c1->member_No<<endl;
	//�û������û�����ѭ������

	while(true)//ѡ��ʼ��ѭ��
	{
			
			//"\n�������������ѡ��\n";
			//"|-------------------------------------------------------------------------|\n";
			//"|  ѡ1�������Լ��ķ���         ѡ2��������˵ķ���           ѡ3���˳�����|\n";
			//"|                                                                         |\n";
			//"|  ѡ3���˳�����               ѡ4��ˢ������б�           ѡ5����ʼս��|\n";
			//"|-------------------------------------------------------------------------|\n";
			//"��ѡ��:";
		if(c1->device!=android_phone)special_signal(c1,show_choice);

		cout<<"��ʼ�����û�ѡ��\n";

			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//��ȡ�û�������Ϣ���������������ͨ
			cout<<"�ͻ��˷�����Ϣ��"<<StrBuf<<endl;
			if(c1->state==in_game)//���ж�ͨ������Ȼ�Ƿ�����ͽ�����SZSC��Ϸ
			{
				start_fight(c1,&player_room[c1->room_No]);
				c1->state=in_room;//��Ϸ�������ص�����״̬

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
				SZSC_provide_service(c1,atoi(StrBuf));//�ṩSZSC��Ϸ����
				break;
			case UNO_apply_create_room:
			case UNO_apply_enter_room:
			case UNO_apply_exit_room:
				UNO_provide_service(c1,atoi(StrBuf));//�ṩSZSC��Ϸ����
				break;

			
			case apply_refresh_online_member://ˢ������б�
				if(c1->device==win_console)special_signal(c1,please_cls);
				show_all_client(c1);//ˢ���б�
				break;
			case 0:
				special_signal(c1,you_are_in_the_lobby);




			default:
				if(c1->device==win_console)confirm_send_success(c1,"��������Чֵ!\n");
				
		}//switch����
					
	}//ѡ��ʼ��ѭ������
}

void accept_client()//���м��������ܿͻ��˷�����Ϣ������
{
	char StrBuf[MAX_PATH] = { 0 };//��������洢���յ�������
	char Buf[MAX_PATH] = { 0 };//ר������ת�����ַ���ʽ��ֵΪ�ַ�
	
	/*���Ĳ� ���ܿͻ���������*/
	sockaddr_in ClientAddr[memberlimit+1]= { 0 };//�����ṹ�� ���ڻ�ȡ�ͻ��� socketaddr �ṹ��
	int len=sizeof(ClientAddr);//��ȡ�ṹ���С
	bool claim_limit=false;//�Ƿ���ʾ�����������������ﵽ����

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)server_monitor,NULL, NULL, NULL);//����ԱȨ�޲鿴����
	//Sleep(500);
	while(true)//��ʼ������ѭ��
	{	
		check_emptyspace();

		if(empty_player_pointer>=memberlimit)//�������������Ƿ����أ��������
		{
			if(!claim_limit)
			{
				cout<<"���������������ﵽ���!�޷��ټ�������!\n";claim_limit=true;//��Ϊ����ʾ���������
			}
			//���Ͻ��ܹ������������������ظ����������½�ʱ�������ĵ�һ���ͻ��˽��ᱻ����
			g_ClientSocket[memberlimit]= accept(g_ServerSocket, (sockaddr*)&ClientAddr[memberlimit], &len);
			hthread[memberlimit]=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communicate_with_this_client_O,&Membership[memberlimit], NULL, NULL);//���ú�����ÿͻ��˽��й�ͨ
		}
		else//������û���أ����м��������ܿͻ��˷�����Ϣ������
		{
			if(claim_limit)//���֮ǰ�����������������ˣ����ڼ�鵽��λ����ʾ���������Լ������ܿͻ���������
			{
				CloseHandle(hthread[memberlimit]);
				claim_limit=false;
				cout<<"���������������½�!���Լ������տͻ�����!\n";
			}

			cout<<"���������С�����Ŀǰ��"<<client_number<<"���ͻ��������ӣ�����Ϊ"<<memberlimit<<"����\n";
			g_ClientSocket[empty_player_pointer]= accept(g_ServerSocket, (sockaddr*)&ClientAddr[empty_player_pointer], &len);//���ܿͻ��˵���������
			
			SOCKADDR_IN   sockAddr; 
			int   iLen=sizeof(sockAddr); 
			getpeername(g_ClientSocket[empty_player_pointer] ,(struct   sockaddr   *)&sockAddr,&iLen);//��ȡ�ͻ��˵�ip
			cout<<"\n���û������ӣ��ͻ���IP: "<<::inet_ntoa(sockAddr.sin_addr)<<"�˿ں�: "<<ntohs(sockAddr.sin_port)<<endl;


			Membership[empty_player_pointer].member_in_use=true;//����������λ�ñ�Ϊ����ռ�á�
			client_number++;
			cout<<"�ѽ��ܿͻ����������󡭡�\n";
			
		/*���岽 ��������*/
			//�̣߳���ȫ���ԣ���ʼջ��С�����������������Σ��߳�״̬��־���߳�id�����ַ��
			//�߳�״̬��־��CREATE_SUSPENDED����/0��������
			hthread[empty_player_pointer]=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communicate_with_this_client,&Membership[empty_player_pointer], NULL, NULL);//���ú�����ÿͻ��˽��й�ͨ

		}//�������¿ͻ��˵Ĺ�ͨ���ӽ��������¼��������Ƿ����أ�������ֹͣ������û��������������Ƿ�����һ���ͻ��˵�����

	}//������ѭ�����������Ƿ������رգ�������ѭ����һֱ������ȥ��

}







#endif