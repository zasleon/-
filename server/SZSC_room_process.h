#ifndef INCLUDE_SZSC_room_process_H
#define INCLUDE_SZSC_room_process_H

#include "server_general_function.h"

void room_broadcast(fight_room* this_room,char* message1)//�����ڹ㲥��Ϣ
{
	int count;
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL&&this_room->c[count]!=NULL)
		{
			confirm_send_success(this_room->c[count],message1);//ͳһ�㲥������仰
		}
		else
			return;
}


bool check_sb_offline(client_member* p,fight_room* this_room)//���ս��ʱ�Ƿ������˳�,����true���������˳�
{
	int count;
	for(count=0;count<playernumber;count++)
	{
		if(this_room->c[count]==NULL||this_room->c[count]->state==in_room)
		{
			this_room->ready_number=0;
			special_signal(p,someone_offline);//���˶���!
			return true;
		}
	}
	return false;
}


void exit_the_room(client_member* member)//�û��˳�����
{
	char StrBuf[MAX_PATH] = {0};

	player_room[member->room_No].ready_number=0;
	bool host=false;//�Ƿ��Ƿ�������
	client_member* c2;
	fight_room* this_room=&player_room[member->room_No];//ȷ���˳������ĸ�����

	if(!member->guest)//ȷ��Ϊ��������
		host=true;

	member->state=in_online;//״̬ȫ��Ϊ��ʼ̬
	member->guest=false;
	if(host)//���������������˳�
	{
		if(this_room->c[1]==NULL)//����λ���û��
		{
			this_room->room_in_use=false;//������ռ��״̬
			return;//����û��˳���������
		}
		if(this_room->c[1]->member_No==robot_symbol)//���˵�������л�����
		{
			this_room->c[1]=NULL;
			this_room->room_in_use=false;
			return;//����û��˳���������
		}
		else
			this_room->c[0]=this_room->c[1];//�������б��ˣ����Ǹ�����Ϊ��������
	}

	this_room->c[1]=NULL;//���������Ϊ��
	c2=this_room->c[0];//ȷ������������һ����
	c2->guest=false;//�ض���Ϊ��������
	if(c2==NULL){this_room->room_in_use=false;return;}//�������һ����ҲͻȻ���ߣ������¼�����

	SuspendThread(hthread[c2->member_No-1]);//������һ����ԭ�����еĶ���
	Sleep(200);
	special_signal(c2,someone_leave);//�����������˴ӷ����뿪��
	if(host)special_signal(c2,you_are_host);//����뿪���Ƿ�������,���߱�������Ϊ�˷�������
		
	addtext(StrBuf,member->client_name);addtext(StrBuf,"\n");
	confirm_send_success(c2,StrBuf);//�����뿪������

	if(c2->device==win_console&&c2->state==in_room)
		special_signal(c2,show_choice);//��ʾѡ��˵�
	//c2->state=in_room;//�����ڲ�����Ϸ״̬�����ص�����״̬
	ResumeThread(hthread[c2->member_No-1]);//�����һ���˽��еĶ���
	
}

bool check_empty_room()//�����û�пշ��䣬�еĻ�����true��û���򷵻�false
{
	int count;
	for(count=0;count<roomlimit;count++)
	{
		if(!player_room[count].room_in_use)
		{
			empty_room_pointer=count;
			return true;
		}
	}
	return false;
}
bool check_in_use_room()//�����û�������÷���
{
	int count;
	for(count=0;count<roomlimit;count++)
	{
		if(player_room[count].room_in_use)
		{
			return true;//�����б�ʹ�õķ���
		}
	}
	return false;//ȫΪ��
}
void show_all_room(client_member* member)//��ʾ��ʹ�õķ���
{
	char StrBuf[MAX_PATH] = {0};
	char Buf[MAX_PATH] = {0};

	int count;

	for(count=0;count<roomlimit;count++)
		if(player_room[count].room_in_use)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,count+1);
			addtext(StrBuf,"�ŷ���:\t");
			addtext(StrBuf,player_room[count].c[0]->client_name);
			if(player_room[count].c[1]!=NULL)
			{
				addtext(StrBuf,"\t");
				addtext(StrBuf,player_room[count].c[1]->client_name);
				//strcat(StrBuf,"(�÷�������!)");
			}
			addtext(StrBuf,"\n");
			confirm_send_success(member,StrBuf);
		}
		
		special_signal(member,stop_show_room);
}
#endif