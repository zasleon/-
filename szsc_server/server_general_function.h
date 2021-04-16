#ifndef INCLUDE_server_general_function_H
#define INCLUDE_server_general_function_H

#include "server_property.h"



//ͨ�ú���

void send_this_message(client_member* c1,char* StrBuf)
{
	//ר������ת�����ַ���ʽ��ֵΪ�ַ�,Buf����ƥ��ͻ����Ƿ�ظ��ˡ�client_get_message��
	if(sizeof(StrBuf)<MAX_PATH)
	{
		::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//��������
		cout<<"����:"<<StrBuf<<"wait reply\n";
	}
	else
	{
		::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 +sizeof(StrBuf)), 0);//��������
		cout<<"���������ݣ�\n"<<"wait reply\n";
	}

	int deadline=0;
	while(!check_client_reply(c1))
	{
		deadline++;
		if(deadline>10)
		{
			cout<<c1->client_name<<"ǿ������\n";
			c1->F_offline=true;return;
		}
		if(c1->F_offline)return;

		Sleep(50);
		if(sizeof(StrBuf)<MAX_PATH)
		{
			::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//��������
			cout<<"�ٴη���:"<<StrBuf<<"wait reply\n";
		}
		else
		{
			::send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + sizeof(StrBuf)), 0);//��������
			cout<<"�ٴη��������ݣ�\n"<<"wait reply\n";
		}
	}
	return;

}
char* get_client_message(client_member* c1)//��ȡ�ͻ��˷��͵���Ϣ
{
	memset(c1->p_message,0,sizeof(c1->p_message));
	Sleep(10);
	cout<<"�ȴ��û�����\n";
	while(strlen(c1->message)==0)
	{
		if(c1->F_offline)return "";
		Sleep(100);
	}//���еȴ�
	addtext(c1->p_message,c1->message);
	c1->not_be_read=false;
	Sleep(5);
	cout<<"get message:"<<c1->p_message<<endl;
	return c1->p_message;
}
char* fast_get_client_message(client_member* c1)//����ȷ�Ͽͻ��˻ظ�����
{
	cout<<"f\n";
	while(strlen(c1->message)==0)
	{
		if(c1->F_offline)return "";
	}//���еȴ�
	c1->not_be_read=false;
	Sleep(5);
	return "";
}
bool check_client_reply(client_member* c1)//ȷ�Ͽͻ��˻ظ�����
{
	int overtime=0;
	Sleep(50);
	char Buf[MAX_PATH]={0 };
	while(strlen(Buf)==0)
	{
		if(c1->F_offline)return false;
		overtime++;
		Sleep(50);
		if(overtime>60){cout<<"��ʱ!\n";return false;}
		addtext(Buf,c1->message);
	}

	if(atoi(Buf)==client_get_message){cout<<"reply right\n";c1->not_be_read=false;return true;}
	cout<<"����ظ�:"<<Buf<<endl;
	c1->not_be_read=false;return false;
}



void listen_client_message(client_member* c1)
{	
	c1->listen=true;
	while(true)
	{
		c1->not_be_read=true;
		if(c1->F_offline){cout<<"��"<<c1->client_name<<"��������!\n";return;}
		if(recv(g_ClientSocket[c1->member_No-1], c1->message, MAX_PATH, 0) <= 0){c1->F_offline=true;cout<<"��"<<c1->client_name<<"��������!\n";c1->listen=false;return;}
		while(c1->not_be_read)Sleep(1);
		memset(c1->message,0,sizeof(c1->message));
	}
}

void confirm_send_success(client_member* c1,char* StrBuf)//���Է���Ϣ�Ƿ��յ�����StrBuf
{
	if(c1->member_No==robot_symbol)return;//����ǻ����ˣ���������������Ϣ

	if(c1==NULL||!c1->member_in_use||c1->F_offline)return;
	send_this_message(c1,StrBuf);
	Sleep(50);	//�����̣߳���ɲ��ȶ�����
				//����sleep�ӳ��û���ֱ������
				//��̫���������ظ���������,������sleep����΢����ô����
	return;
}

void special_signal(client_member* c1,int signal)//���������źŸ���
{
	char StrBuf[MAX_PATH] = { 0 };
	addtext(StrBuf,signal);addtext(StrBuf,"\n");
	confirm_send_success(c1,StrBuf);
}
void room_broadcast(fight_room* this_room,char* message1)
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

void check_emptyspace()//���㵱ǰ���޷�������λ
{
	int count;
	for(count=0;count<memberlimit;count++)
		if(Membership[count].member_in_use==false)
			{break;}

	empty_player_pointer=count;//�������countֵ����empty_pointer����ǰ��λָ��
}

void show_all_client(client_member* c1)//��ͻ�����ʾ����������Ա�б�
{
	int count;
	char StrBuf[MAX_PATH] = {0};

	special_signal(c1,show_user);//��ͻ��˷��͡��Լ�������ʾ�û��б�

	for(count=0;count<memberlimit;count++)
	{
		if(Membership[count].member_in_use)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf," ");
			addtext(StrBuf,Membership[count].member_No);
			addtext(StrBuf,"��");
			addtext(StrBuf,"\t\t");
			
			switch(Membership[count].state)
			{
				case in_game:addtext(StrBuf,"ս����\t\t");break;
				case in_online:addtext(StrBuf,"����\t\t");break;
				case in_room:addtext(StrBuf,"������\t\t");break;
			}
			
		addtext(StrBuf,Membership[count].client_name);//����û���
		if(Membership[count].member_No==c1->member_No)
				addtext(StrBuf,"(��)");

		addtext(StrBuf,"\n");
		confirm_send_success(c1,StrBuf);
		memset(StrBuf,0,sizeof(StrBuf));
		
		//Sleep(50);//�����ٶȱȽϿ죬��Ҫͣ���룬��Ȼ����3�������û����׳��ֶ�������ֻ��ʾ2���û��������û��ķ��������ˣ�

		}
	}
	if(c1->device==win_console)
	{
		if(c1->state==in_room)
		{
			fight_room this_room=player_room[c1->room_No];
			addtext(StrBuf,"\n����");
			addtext(StrBuf,c1->room_No+1);
			addtext(StrBuf,"�ŷ�����!\t");
			if(this_room.c[0]!=NULL&&this_room.c[0]->member_No==c1->member_No)
			{
				addtext(StrBuf,"���Ƿ�������!");
				if(this_room.c[1]!=NULL)
				{
					cout<<"\n";
					addtext(StrBuf,"�����ﻹ��: ");
					addtext(StrBuf,this_room.c[1]->client_name);
				}
			}
			else
			{
				addtext(StrBuf,"���Ƿ������!�����ڻ���: ");
				addtext(StrBuf,this_room.c[0]->client_name);
			}
			addtext(StrBuf,"\n");
			confirm_send_success(c1,StrBuf);
		}
	}
	special_signal(c1,stop_show_user);//��ͻ��˷��͡��Լ�������ʾ�û��б�
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

void client_offline(client_member* member)//�ͻ������߽���
{
	if(!member->member_in_use)return;
	cout<<"��"<<member->member_No<<"�ſͻ����˳���������!\n";
	if(member->state==in_room||member->state==in_game)
		exit_the_room(member);
	if(member->listen)
	{
		cout<<"ǿ�ƹرռ����߳�\n";
		TerminateThread(member->listen_this,0);
	}

	member->state=in_online;
	memset(member->client_name,0,sizeof(member->client_name));
	addtext(member->client_name,"δ����");
	client_number--;
	member->member_in_use=false;
	member->F_offline=false;
	check_emptyspace();
	cout<<"��Ŀǰ����"<<client_number<<"���ͻ��������ӣ�\n";
}

void init_server_set()//��ʼ������������
{
	ini_catalog();//��ʼ����ƬĿ¼
	client_number=0;//��������Ա��ʼ��Ϊ0
	empty_player_pointer=0;//��ǰ��������λָ���ʼ��Ϊ0
	int count,count2;//��������λȫ����ʼ��
	for(count=0;count<=memberlimit;count++)//��ʼ�����ܿͻ���
	{
		Membership[count].member_No=count+1;
		Membership[count].member_in_use=false;
		Membership[count].p1=&Membership[count].body;
		addtext(Membership[count].client_name,"δ����");
		Membership[count].state=in_online;
		Membership[count].guest=false;
		Membership[count].be_locked=false;
		Membership[count].F_offline=false;
		Membership[count].listen=false;
		memset(Membership[count].message,0,sizeof(Membership[count].message));
		memset(Membership[count].p_message,0,sizeof(Membership[count].p_message));

	}
	for(count=0;count<roomlimit;count++)//��ʼ��ս������
	{
		for(count2=0;count2<playernumber;count2++)
			player_room[count].c[count2]=NULL;
		player_room[count].room_in_use=false;
		player_room[count].ready_number=0;

		Robot[count].member_No=robot_symbol;
		Robot[count].p1=&Robot[count].body;
		addtext(Robot[count].client_name,"ϵͳ������");
	}

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

void show_room_state()//����������ʾ��ʹ�õķ���
{
/*
	int count;
	cout<<"���������\n";
	cout<<"----------------------\n";
	for(count=0;count<roomlimit;count++)
		if(player_room[count].room_in_use)
		{
			cout<<count<<"�ŷ���\t"<<player_room[count].c1->client_name<<"\t";
			if(player_room[count].c2_in_use==true)
				cout<<player_room[count].c2->client_name;
			cout<<endl;
		}
		else 
			break;
	cout<<"----------------------\n";
	*/
}
void server_monitor()
{
	Sleep(1000);
	char Buf[MAX_PATH] = {0};
	int choice,count;
	fight_room* this_room;
	while(true)
	{
		cout<<"����������ʲô��1.ˢ�·������	2.�鿴ĳһ��������	3.ˢ��������Ա\n��ѡ��";
		cin>>(Buf);
		choice=atoi(Buf);
		if(choice==0)continue;
		switch(choice)
		{
			case 1:show_room_state();break;
			case 2:
				cout<<"�����뷿��ţ�";
				cin>>(Buf);
				choice=atoi(Buf);
				if(choice>500||choice<=0)break;
				choice--;
				this_room=&player_room[choice];
				if(!this_room->room_in_use)break;
				if(this_room->c[0]->state!=in_game){cout<<"û��ս���У�\n";break;}
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
				cout<<"\n��ǰ���������û���Ϣ���£�";
				cout<<"\n-----------------------------------------------\n";
				cout<<" ���к�\t\t״̬\t\t�û���\n\n";
				for(count=0;count<memberlimit;count++)
				{
					if(Membership[count].member_in_use)
					{
						
						cout<<" "<<Membership[count].member_No<<"��\t\t";
		
						switch(Membership[count].state)
						{
							case in_game:cout<<"ս����\t\t";break;
							case in_online:cout<<"����\t\t";break;
							case in_room:cout<<"������\t\t";break;
						}

						cout<<Membership[count].client_name<<"\n";//����û���

					}
					else
						break;
				}
				cout<<"\n-----------------------------------------------\n";
				break;
		}//switch����
	}
}




#endif