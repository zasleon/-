#ifndef INCLUDE_server_H
#define INCLUDE_server_H

#include "movie_function.h"


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
	Sleep(100);
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
	cout<<"�û���:"<<c1->client_name<<endl;
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
			if(c1->state==in_game)//���ж�ͨ������Ȼ�Ƿ�����ͽ�������Ϸ
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

			case 1://��������
				if(c1->device!=android_phone)confirm_send_success(c1,"��ѡ���˴�������!\n");
				if(c1->state==in_room)
				{confirm_send_success(c1,"�����ڷ�����!�޷���������!\n");break;}
				if(!check_empty_room())//����Ƿ��пշ���
				{confirm_send_success(c1,"��ǰ�����������޷���������!\n");break;}
				
				player_room[empty_room_pointer].room_in_use=true;
				player_room[empty_room_pointer].c[0]=c1;
				c1->state=in_room;
				c1->room_No=empty_room_pointer;

				special_signal(c1,get_in_room);//�����ɹ�!
					
				show_room_state();
				break;
			case 2:
				if(c1->device!=android_phone)confirm_send_success(c1,"��ѡ���˼��뷿��!\n");
				if(c1->state==in_room||c1->state==in_game)
				{confirm_send_success(c1,"�����ڷ�����!�޷���������!\n");continue;}
				if(!check_in_use_room())
				{special_signal(c1,no_room_in_use);continue;}
				//{confirm_send_success(c1,"��ǰû��ʹ������ʹ�õķ���!������ѡ��!\n");continue;}

				special_signal(c1,please_choose_room);//��������ѡ�񷿼���

				show_all_room(c1);
				while(true)//ѡ�񷿼���ѭ����ʼ
				{				

					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){client_offline(c1);return;}//�������ѡ�������ʱ�Ͽ�ͨѶ

					cout<<"�û����룺"<<StrBuf<<endl;
					if(strlen(StrBuf)==1&&StrBuf[0]=='0')
					{	if(c1->device!=android_phone)
							confirm_send_success(c1,"��ѡ��ȡ�����뷿��!��������ԭҳ�桭��\n");
						else
							special_signal(c1,cancel_enter_room);
						break;
					}
						
					choice=atoi(StrBuf);//ת������Ϊ����
						
					if(choice==0){confirm_send_success(c1,"��������Чֵ!\n");continue;}
					if(choice>=roomlimit||choice<0){confirm_send_success(c1,"���볬��������������!\n");continue;}

					choice--;

					if(player_room[choice].room_in_use==false){confirm_send_success(c1,"�÷�����û��!\n");continue;}
					if(player_room[choice].c[playernumber-1]!=NULL){confirm_send_success(c1,"�÷���������!\n");continue;}
							
					c1->state=in_room;//ִ�е���һ����Ȼ����ɹ�
					c1->guest=true;//���뷿������Ƿ������

					player_room[choice].c[1]=c1;
					c1->room_No=player_room[choice].c[0]->room_No;

					special_signal(c1,enter_room_success);//����ɹ�!

					SuspendThread(hthread[player_room[choice].c[0]->member_No-1]);//��ס�������˼����߳�
					special_signal(player_room[choice].c[0],someone_get_in);//���߷����������˽�����
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//���߷������˽���������
					confirm_send_success(player_room[choice].c[0],StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					if(player_room[choice].c[0]->device==win_console)special_signal(player_room[choice].c[0],show_choice);//�����win_console�ͻ��ˣ���ʾѡ��
					ResumeThread(hthread[player_room[choice].c[0]->member_No-1]);//��ŷ������˼����߳�
					show_room_state();//�������Լ���ʾ���ڷ���״̬
					break;
			
				}//ѡ�񷿼���ѭ������
				break;
			case 3:
				if(c1->device==win_console)confirm_send_success(c1,"��ѡ�����˳�����!\n");
				if(c1->state==in_game){confirm_send_success(c1,"������Ϸ��!�޷��˳�����!\n");continue;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);continue;}//�㵱ǰû�ڷ�����!������ѡ��

				exit_the_room(c1);//ִ���˳�
				special_signal(c1,leave_room);//�������˳��ɹ�!

				show_room_state();//�������Լ���ʾ���ڷ���״̬
				break;
			case 4://ˢ������б�
				if(c1->device==win_console)special_signal(c1,please_cls);
				show_all_client(c1);//ˢ���б�
				continue;
			case 5:
				if(c1->device==win_console)confirm_send_success(c1,"��ѡ���˿�ʼս��!\n");

				if(c1->state==in_game){confirm_send_success(c1,"��������Ϸ��!�޷���ʼ�µ�!\n");continue;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);continue;}//�㲻�ڷ�����!�޷���ʼս��!
				if(c1->guest){confirm_send_success(c1,"�㲻�Ƿ�������!�޷���ʼս��!\n");continue;}//������Ƿ�������
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"��������������!�޷���ʼս��!\n");continue;}//�������û�б���
				
				//ִ�е�����Ѿ�ȷ���ڷ����е����ˣ������б���
				for(count=0;count<playernumber;count++)
					if(player_room[c1->room_No].c[count]!=NULL)
					{
						player_room[c1->room_No].c[count]->state=in_game;
						if(count!=0)special_signal(player_room[c1->room_No].c[count],event_happen_N);//�������˻�����server����ˣ���event_happen_N����һ�½���ս������
					}
					else
					{
						for(count=0;count<playernumber;count++)
							if(player_room[c1->room_No].c[count]!=NULL)
								player_room[c1->room_No].c[count]->state=in_room;
						confirm_send_success(c1,"��������������!�޷���ʼս��!\n");break;
					}
				if(count!=playernumber)break;

				start_fight(c1,&player_room[c1->room_No]);//ִ�����ʾ��Ϸ����
				c1->state=in_room;//��Ϸ�������ص�����״̬
				break;
			case 6://��ӻ�����
				if(c1->state!=in_room)//ȷ����������Ϸ�У�������ڷ�����
				{special_signal(c1,you_not_in_room);continue;}//�㲻�ڷ�����!
				if(player_room[c1->room_No].c[0]->member_No!=c1->member_No){confirm_send_success(c1,"ֻ�з������˲�����ӻ�����!\n");continue;}	
				if(player_room[c1->room_No].c[1]!=NULL){confirm_send_success(c1,"���������б���!�޷����!\n");continue;}
				player_room[c1->room_No].c[1]=&Robot[c1->room_No];
				confirm_send_success(c1,"��ӳɹ�!!!\n");
				break;
			case 7://�Ƴ�������
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);continue;}//������ڷ�����
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"������û�л�����!�޷��Ƴ�!\n");continue;}
				if(player_room[c1->room_No].c[1]->member_No==c1->member_No){confirm_send_success(c1,"���Ƿ�����ˣ�ֻ�з���������Ȩ���޸Ļ�����!\n");continue;}
				if(player_room[c1->room_No].c[1]->member_No!=robot_symbol){confirm_send_success(c1,"�������л���!�޷��Ƴ�������!\n");continue;}
				player_room[c1->room_No].c[1]=NULL;//�����Ƴ�
				confirm_send_success(c1,"�Ƴ��ɹ�!\n");
				continue;

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



bool TCP_InitSocket(char *Addr, int Port)
{
	/*��һ�� ����socket��������socket*/
	g_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);  
	//�������ʧ���򷵻�
	if(g_ServerSocket == INVALID_SOCKET){cout<<"����ipʧ��!\n";return false;}

	/*�ڶ��� bind��������ip��*/
	sockaddr_in ServerAddr;

	//inet_addr�������ַ���ת��ΪIN_ADDR�ṹ����ȷ��ַ
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(Addr);//���ð󶨵�IP
	ServerAddr.sin_family = AF_INET;//ָ����ַ��ΪIPv4
	ServerAddr.sin_port = htons(Port);//���ü����˿�

	//�����ʧ���򷵻�
	if(bind(g_ServerSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) != 0)
		{return false;}//cout<<"��ipʧ��!\n";

	/*������ �����˿�*/
	if(listen(g_ServerSocket, SOMAXCONN) != 0)return false;//�������ʧ�ܷ���

	return true;//���ش��������˿ڳɹ�
}



#endif