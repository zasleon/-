
#include "special_transmit.h"
#include "client_movie.h"



void C_Receive_Message()// ��������
{
	char szBuf[MAX_PATH] = {0};//�������ݣ���������
	char Buf[MAX_PATH] = {0};//�������ݣ�����ǿ��ת��һЩ���ַ���Ϊ�ַ���
	int possible_message;
	stop_send_CGM=false;
	ini_catalog();//��ʼ������Ŀ¼

	while(true)
	{
		memset(szBuf,0,sizeof(szBuf));memset(Buf,0,sizeof(Buf));
		if (0>=recv(Client, szBuf, MAX_PATH, 0))//���Խ������ݣ�������ؽ���ı�־��Ϊ-1�������Ͽ�����
		{
			printf("\n����������Ͽ�!\n");
			connect_situation=false;//����״̬��Ϊ����ʧ��
			server_overload=true;//�������Ƿ��ر�Ϊδ֪
			server_overload=true;//��ֹ���������أ���Ϊtrue��˼�Ƿ�����������������
			create_thread=false;//��δ�����߳�
			claim_overload=false;//��δ��������������
			break;
		}
		Sleep(50);
		

		//SuspendThread(hthread);//��ֹ�û�������������
		//if(vc6)fflush(stdin);else cin.ignore(1, EOF);
		strcpy(Buf,szBuf);

		if(strlen(Buf)!=5)//��鷢�����ַ����Ƿ��������ź�
			possible_message=0;
		else
		{
			Buf[4]='\0';
			possible_message=atoi(Buf);
		}
		switch(possible_message)
		{
			case 0:
				cout<<szBuf;break;//���ַ����޷�ת��Ϊ����,�������

			case start_link://��start_link��7070��ǿ��ת��Ϊ�ַ���
			//���γɹ����ӷ��������ᷢ��start_link��7070��,���szBuf=Buf���򷵻���
				ini_character(c1,162359);ini_character(c2,162359);//��ʼ�������ɫ//��ʼ�������ɫ
				memset(szBuf,0,sizeof(szBuf));
				reply_i_get();

				sprintf(szBuf,"%d",win_console);
				send(Client, szBuf, (1 + ::strlen(szBuf)), 0);//�����Լ���windows����̨��

				*state=no_info;//������Ϣ��no_infoΪû�н�����Ϸ
				server_overload=false;//���������Ƿ��ظ�Ϊ��
				cout<<"���ѵ�¼!\n\n�봴�������û���:";
				break;

			case start_turn_settle://��ʼ���лغϽ���

				cout<<"����غϽ���!����\n";
				//SuspendThread(hthread);//��ֹ�û�����
				break;

			case start_another_turn://��ʼ�µ�һ�غ�
				cout<<"����Կ�ʼ��һ�غϵ��ж���!\n";

				//ResumeThread(hthread);//����û���������
				break;
			
			case event_end://����Լ������ж�
			
				//cin.ignore(1,EOF);
				//cout<<"\n�Է��ж��¼�����!\n";
				Sleep(50);
				reply_i_get();
				Sleep(50);
				
				
				memset(Buf,0,sizeof(Buf));
				
				break;
			
			case event_happen://�����Լ��ж�
			
				cout<<"\n�ȴ��Է�������һ���ж�����\n";
				Sleep(30);
				reply_i_get();
				Sleep(30);
				
				
				memset(Buf,0,sizeof(Buf));
				break;
			
			case event_happen_N://�����Լ��ж�����������ȴ��Է�������һ���ж�����Ϊ�ڲ�������Ϣ���̣������Ϻܿ�͹�ȥ���û�ע�ⲻ���ù���		
				Sleep(30);
				reply_i_get();
				Sleep(30);
				memset(Buf,0,sizeof(Buf));
				break;

			case please_cls://��ս��棬ˢ���б�
		
				system("cls");
				break;
			case whether_launch_hideeffect:
				cout<<"�Ƿ񷢶���Ƭ��Ч������1������Ч����0����Ч����\n��ѡ��";
				break;
			
			case choice_complete	:			
				cout<<"\n�����˽�ɫѡȡ���!\n";
				break;

			case  whether_fight_back:		
				cout<<"�Ƿ񷴻������Ƿ��ж�����0:���ж���1:�����ֿ���2:��������Ч����3:��������Ч����4.�Ӹ�Ч����\n��ѡ��";
				break;

			case someone_offline:
				cout<<"\n���˶���!\n";
				break;

			case card_launch:
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));
				if (0>=recv(Client, szBuf, MAX_PATH, 0))//���շ���������
					return;
				cout<<szBuf<<"������Ƭ:";
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));
				if (0>=recv(Client, szBuf, MAX_PATH, 0))//���շ�����Ƭ��
					return;
				cout<<search_card(p_name,atoi(szBuf))<<endl;
				cout<<"��ƬЧ��:"<<search_card(p_details,atoi(szBuf))<<endl;
				reply_i_get();
				stop_send_CGM=true;
				break;

			case someone_leave:		
				Sleep(50);
				reply_i_get();
				memset(szBuf,0,sizeof(szBuf));
				if (0>=recv(Client, szBuf, MAX_PATH, 0))break;//����������you_are_host�ź�
				system("cls");
				cout<<"\n-------------------------------\n (";
				
				if(atoi(szBuf)==you_are_host)
				{
					memset(szBuf,0,sizeof(szBuf));
					reply_i_get();
					if (0>=recv(Client, szBuf, MAX_PATH, 0))break;
					szBuf[strlen(szBuf)-1]='\0';
					cout<<szBuf<<"�뿪�˷���!���Ϊ��������!)\n-------------------------------\n";
				}
				else
				{
					szBuf[strlen(szBuf)-1]='\0';
					cout<<szBuf<<"�뿪�˷���!)\n-------------------------------\n";
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
				cout<<"��������ķ���!)\n-------------------------------\n";

				break;

			case  no_room_in_use:
				cout<<"��ǰû��ʹ������ʹ�õķ���!������ѡ��!\n";
				break;

			case  you_not_in_room:
				cout<<"�㵱ǰû�ڷ�����!������ѡ��!\n";
				break;

			case enter_room_success://���뷿��ɹ�
				cout<<"����ɹ�!\n";
				break;

			case leave_room:
				cout<<"�˳��ɹ�!\n";
				break;
			case get_in_room:
				cout<<"�����ɹ�!\n";
				break;

			case please_choose_room:
				cout<<"\nĿǰ�ķ���������£�\n";
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
				cout<<"\n������Ҫ����ķ���ţ�����0ȡ�����뷿�䣩\n�����룺";
				stop_send_CGM=true;
				break;

			case now_is_your_turn:
				cout<<"��������Ļغ�!\n";
				break;

			case you_win:
				cout<<"======================\t ��Ӯ��!!! \t======================\n";
				break;
			case you_lose:
				cout<<"======================\t ������!!! \t======================\n";
				break;
			case game_standoff:
				cout<<"======================\t ƽ��!!! \t======================\n";
				break;
			
			case show_choice://��ʾ�������
			
				*state=no_info;
				cout<<"\n\n�������������ѡ��\n";
				cout<<"|---------------------------------------------------------------------------|\n";
				cout<<"|  ѡ1�������Լ��ķ���         ѡ2��������˵ķ���           ѡ3���˳�����  |\n";
				cout<<"|                                                                           |\n";
				cout<<"|  ѡ4��ˢ������б�           ѡ5����ʼս��                 ѡ6����ӻ�����|\n";
				cout<<"|                                                                           |\n";
				cout<<"|  ѡ7���Ƴ�������                                                          |\n";
				cout<<"|---------------------------------------------------------------------------|\n";
				cout<<"��ѡ��:";
				
				break;
			
			case show_user://��show_user  7071ǿ��ת��Ϊ�ַ���		
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				cout<<"\n��ǰ���������û���Ϣ���£�";
							cout<<"\n-----------------------------------------------\n";
				cout<<" ���к�\t\t״̬\t\t�û���\n\n";
				Sleep(30);
				reply_i_get();
				while(recv(Client, szBuf, MAX_PATH, 0)>0)
				{
					memset(Buf,0,sizeof(Buf));
					strcpy(Buf,szBuf);

					if(strlen(Buf)!=5)//��鷢�����ַ����Ƿ��������ź�
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
			
				cout<<"�����ѡ�������ж���\n";
				cout<<"---------------------------------------------------------------------------|\n";
				cout<<"1.�����չ�            2.������ɫ����Ч��      3.ʹ���ֿ�                   |\n";
				cout<<"                                                                           |\n";
				cout<<"4.������װ��������    5.������װ��������Ч��  6.װ������     7.�����غ�    |\n";
				cout<<"---------------------------------------------------------------------------|\n";
				cout<<"����ж�����ѡ��:";
				break;
			
			case character_choice:
			
				system("cls");
				cout<<"Ŀǰ���ŵĿ�ʹ�ý�ɫ:\n";
				cout<<"|-----------------------------------------------------------------|\n";
				cout<<"| 1.սʿ                                                          |\n";
				cout<<"|���Լ�ÿ�������չ�ʱ���˴��չ�����ǰ�Լ���+1                     |\n";
				cout<<"| 2.��ʿ                                                          |\n";
				cout<<"|���Լ��غ��Լ�ÿ�γɹ������ֿ�ʱ���Լ���ָ��1��-1Ѫ              |\n";
				cout<<"|                                                                 |\n";
				cout<<"| (��ʹ��������ɫ����ϵ������Ա����봽���!)                     |\n";
				cout<<"|-----------------------------------------------------------------|\n";
				cout<<"������ѡ��:";
				stop_send_CGM=true;
				break;

			case refresh_state_blood_M://���ܷ������������Լ�Ѫ��

				reply_i_get();
				recv_server_transmit_B(true);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;
			
			case refresh_state_blood_H://���ܷ����������Ķ���Ѫ��

				reply_i_get();
				recv_server_transmit_B(false);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;

			case refresh_state_CM://���ܷ����������Ķ��������ֿ�����
				
				reply_i_get();
				recv_server_transmit_C(true);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;
			case refresh_state_CH://���ܷ����������Ķ��������ֿ�����
				
				reply_i_get();
				recv_server_transmit_C(false);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;
			case refresh_state_attack_M://���ܷ������������Լ�����������
				
				reply_i_get();
				recv_server_transmit_A(true);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;

			case refresh_state_attack_H://���ܷ����������Ķ��ֹ���������
				
				reply_i_get();
				recv_server_transmit_A(false);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;

			case refresh_state_weapon_M://���ܷ����������Ķ�����������
				
				reply_i_get();
				recv_server_transmit_W(true);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;

			case refresh_state_weapon_H://���ܷ����������Ķ�����������
				
				reply_i_get();
				recv_server_transmit_W(false);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;

			case refresh_state_buff_H://����buff��Ϣ

				reply_i_get();
				recv_server_transmit_buff(false);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;

			case refresh_state_buff_M://�Լ�buff��Ϣ

				reply_i_get();
				recv_server_transmit_buff(true);//��ʽ������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;


			
			case refresh_first_state:
				
				reply_i_get();
				recv_server_transmit_F();//������Ϣ����
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				*state=have_been_changed;//������Ϣ���޸���
				break;
			case show_video:
				reply_i_get();
				get_movie();
				stop_send_CGM=true;//����ظ����ˣ������ٻظ���
				break;
		
			
		}
		
		if(!stop_send_CGM)//�Ƿ���Ҫ�ظ���
		{
			Sleep(50);
			reply_i_get();
		}
		else
			stop_send_CGM=false;//��Ϊ��Ҫ�ظ�
		//ResumeThread(hthread);//��������յ�����Ϣ������û���������
		
		
	}
}


void C_Send_Message()//�û�������������
{
	char szBuf[MAX_PATH] = {0};//�������ݣ���������
	// ��������	
	while (true)// ��ʼ������Ϣ
	{	
		if(connect_situation==false)
			break;
		memset(szBuf,0,sizeof(szBuf));
		
		
			cin>>szBuf;//��ȡ����
		
			send(Client, szBuf, (1 + ::strlen(szBuf)), 0);

			Sleep(300);//��ֹ�û�������������
	}
}


//���ӷ�����
bool Connection(char *lpszServerIp, int iServerPort)//lpszServerIp=������ip��iServerPort=ʹ�ö˿ں�
{
	// ��ʼ�� Winsock ��
	WSADATA wsaData = { 0 };
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
	// ������ʽ�׽���
	Client=::socket(AF_INET, SOCK_STREAM, 0);
	if(Client==INVALID_SOCKET){cout<<"����ʧ��!\n";return false;}////��������׽��ַ���INVALID_SOCKET��������ʧ��

	// ���÷���˵�ַ�Ͷ˿���Ϣ
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(iServerPort);//ָ�����Ӷ˿�
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszServerIp);//���ӵ�IP

	// connect �������ӵ����������ɹ��򷵻�0, ʧ�ܷ���-1
	if(!claim_overload)//������������ڣ�connect���ӱض��ɹ������������Ƿ��ؾ��޷��ڴ˺������ж�����Ҫ�������ж�����
		if (0 != ::connect(Client, (sockaddr *)(&addr), sizeof(addr))){cout<<"���ӷ�����ʧ��!\n";return false;}//
	
	return true;//���ش������ӳɹ�
}

