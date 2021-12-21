#ifndef INCLUDE_SZSC_service_H
#define INCLUDE_SZSC_service_H

#include "start_one_fight.h"

void SZSC_provide_service(client_member* c1,int choice)
{
	char StrBuf[MAX_PATH] = { 0 };//��������洢���յ�������
	char Buf[MAX_PATH] = { 0 };//ר������ת�����ַ���ʽ��ֵΪ�ַ�
	int count;
	switch(choice)
	{
		case SZSC_apply_create_room://��������
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
			case SZSC_apply_enter_room:
				if(c1->device!=android_phone)confirm_send_success(c1,"��ѡ���˼��뷿��!\n");
				if(c1->state!=in_online)
				{confirm_send_success(c1,"��ǰ״̬�޷��������뷿��!\n");break;}
				if(!check_in_use_room())
				{special_signal(c1,no_room_in_use);break;}
				//{confirm_send_success(c1,"��ǰû��ʹ������ʹ�õķ���!������ѡ��!\n");continue;}

				special_signal(c1,please_choose_room);//��������ѡ�񷿼���

				show_all_room(c1);
				while(true)//ѡ�񷿼���ѭ����ʼ
				{				

					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return;}//�������ѡ�������ʱ�Ͽ�ͨѶ

					cout<<"�û����룺"<<StrBuf<<endl;
					if(strlen(StrBuf)==1&&StrBuf[0]=='0')
					{	
						special_signal(c1,cancel_enter_room);//��ѡ��ȡ�����뷿��!��������ԭҳ�桭��
						break;
					}
						
					choice=atoi(StrBuf);//ת������Ϊ����
						
					if(choice==0){confirm_send_success(c1,"��������Чֵ!\n");continue;}
					if(choice>roomlimit||choice<0){confirm_send_success(c1,"���볬��������������!\n");continue;}

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
			case SZSC_apply_exit_room:
				if(c1->device==win_console)confirm_send_success(c1,"��ѡ�����˳�����!\n");
				if(c1->state==in_game){confirm_send_success(c1,"������Ϸ��!�޷��˳�����!\n");break;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);break;}//�㵱ǰû�ڷ�����!������ѡ��

				exit_the_room(c1);//ִ���˳�
				special_signal(c1,leave_room);//�������˳��ɹ�!
				show_room_state();//�������Լ���ʾ���ڷ���״̬
				break;
			case SZSC_apply_start_game:
				if(c1->device==win_console)confirm_send_success(c1,"��ѡ���˿�ʼս��!\n");

				if(c1->state==in_game){confirm_send_success(c1,"��������Ϸ��!�޷���ʼ�µ�!\n");break;}
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);break;}//�㲻�ڷ�����!�޷���ʼս��!
				if(c1->guest){confirm_send_success(c1,"�㲻�Ƿ�������!�޷���ʼս��!\n");break;}//������Ƿ�������
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"��������������!�޷���ʼս��!\n");break;}//�������û�б���
				
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
			case SZSC_apply_add_robot://��ӻ�����
				if(c1->state!=in_room)//ȷ����������Ϸ�У�������ڷ�����
				{special_signal(c1,you_not_in_room);break;}//�㲻�ڷ�����!
				if(player_room[c1->room_No].c[0]->member_No!=c1->member_No){confirm_send_success(c1,"ֻ�з������˲�����ӻ�����!\n");break;}	
				if(player_room[c1->room_No].c[1]!=NULL){confirm_send_success(c1,"���������б���!�޷����!\n");break;}
				player_room[c1->room_No].c[1]=&Robot[c1->room_No];
				confirm_send_success(c1,"��ӳɹ�!!!\n");
				break;
			case SZSC_apply_remove_robot://�Ƴ�������
				if(c1->state!=in_room){special_signal(c1,you_not_in_room);break;}//������ڷ�����
				if(player_room[c1->room_No].c[1]==NULL){confirm_send_success(c1,"������û�л�����!�޷��Ƴ�!\n");break;}
				if(player_room[c1->room_No].c[1]->member_No==c1->member_No){confirm_send_success(c1,"���Ƿ�����ˣ�ֻ�з���������Ȩ���޸Ļ�����!\n");break;}
				if(player_room[c1->room_No].c[1]->member_No!=robot_symbol){confirm_send_success(c1,"�������л���!�޷��Ƴ�������!\n");break;}
				player_room[c1->room_No].c[1]=NULL;//�����Ƴ�
				confirm_send_success(c1,"�Ƴ��ɹ�!\n");
				break;
	}
}



#endif