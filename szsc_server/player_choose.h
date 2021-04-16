#ifndef INCLUDE_player_choose_H
#define INCLUDE_player_choose_H



#include "attack_fight_back.h"

void lock_other(client_member* c1,fight_room* this_room)//ֹͣc1֮�������˵������ж�
{
	int count;
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(!check_robot(c2))//������ǻ����ˣ�����
		SuspendThread(hthread[c2->member_No-1]);
	special_signal(c2,event_happen);//�¼�����
	special_signal(c1,your_event);

	
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL)this_room->c[count]->be_locked=true;
}
void lock_other_N(client_member* c1,fight_room* this_room)//ֹͣc1֮�������˵������ж�,����ʾ����������˼��������
{
	int count;
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(!check_robot(c2))//������ǻ����ˣ�����
		SuspendThread(hthread[c2->member_No-1]);
	special_signal(c2,event_happen_N);//�¼�����
	special_signal(c1,your_event);
	
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL)this_room->c[count]->be_locked=true;
}
void release_other(client_member* c1,int result,fight_room* this_room)
{
	int count;
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	room_broadcast(this_room,"�¼�����!����Լ����ж���!\n");
	if(c2->p1->un_move_able==0&&result!=force_end_turn)
	{
		if(c2->device==win_console)
			special_signal(c2,battle_choice);//û�������ж�����ʾ�ж�����
		
	}
	special_signal(c2,event_end);//�¼�����
	if(!check_robot(c2))//������ǻ����ˣ����
		ResumeThread(hthread[c2->member_No-1]);//�����߳�
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL)this_room->c[count]->be_locked=false;

}
void release_other_N(client_member* c1,fight_room* this_room)//�¼�����,����ʾ�ж�����
{
	int count;
	client_member* c2;
	if(c1->member_No-1==this_room->c[0]->member_No-1)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	special_signal(c2,event_end);//�¼�����
	if(!check_robot(c2))//������ǻ����ˣ����
		ResumeThread(hthread[c2->member_No-1]);//�����߳�
	
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL)this_room->c[count]->be_locked=false;
}


int use_card_process(client_member* c1,fight_room* this_room)
{
	char StrBuf[MAX_PATH] = { 0 };
	int choice;
	int result=still_fight;
	int locktime=0;
	int effect_cardnumber;
	bool whether_hide_effect;

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"׼�������ֿ�!\n ");//�������
	room_broadcast(this_room,StrBuf);

	if(c1->device==win_console)
		confirm_send_success(c1,"��ѡ���ֿ����з���!(0����ȡ���ò�����)\n��ѡ��:");
	else
		special_signal(c1,get_c_number);//ͼ�λ��ͻ���ֱ�ӻ�ȡ����

	while(true)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ
		
		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			room_broadcast(this_room,"��ѡ��ȡ�������ֿ�!\n");
			return still_fight;
		}
		choice=atoi(StrBuf);//���ַ���Ϊ����
		if(choice==0){confirm_send_success(c1,"������Ч�ַ�!������ѡ���ֿ�:");continue;}
		choice--;
		if(choice>=cardlimit||choice<0){confirm_send_success(c1,"���볬���ֿ���/���ޣ�������ѡ���ֿ�:");continue;}
		if(c1->p1->c[choice]==false){confirm_send_success(c1,"��Ǹ�����ֿ���Ϊ�գ�������ѡ���ֿ�:");continue;}

		memset(StrBuf,0,sizeof(StrBuf));
		special_signal(c1,whether_launch_hideeffect);//ѯ���Ƿ񷢶���Ч��

		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		switch(atoi(StrBuf))
		{
			case  1:effect_cardnumber=c1->p1->card[choice];whether_hide_effect=false;break;
			case  0:effect_cardnumber=c1->p1->hideeffect[choice];whether_hide_effect=true;break;
		}
		if(judge_weapon(effect_cardnumber))//����ÿ��ţ��жϸ��ֿ��Ƿ�Ϊװ�������� ��װ��
		{
			if(!c1->p1->whether_myturn)//����Ƿ�Ϊ����һغ�
			{
				confirm_send_success(c1,"���ڲ�����Ļغ�!�޷�װ������!\n");
				room_broadcast(this_room,"��ѡ��ȡ�������ֿ�!\n");
				return still_fight;
			}
			if(c1->p1->w[weaponlimit-1])//���������������װ�������Ѵ�������ֱ�ӷ���
			{
				confirm_send_success(c1,"װ�������Ѵ�����!�޷�����װ��!\n");
				room_broadcast(this_room,"��ѡ��ȡ�������ֿ�!\n");
				return still_fight;
			}

			effect_cardnumber=c1->p1->card[choice];//�洢����

			result=use_card(c1,choice,false,&locktime,this_room);
			if(result==i_failed_effect)return still_fight;
			
			weapon_arm(c1,effect_cardnumber,this_room);//����װ��

			confirm_send_success(c1,"��װ���ɹ�!\n");memset(StrBuf,0,sizeof(StrBuf));
			return result;
		}//����������ֱ�ӽ�������ʹ�ã��˳���ѭ��
		
		if(judge_card(c1,effect_cardnumber,my_turn,&locktime))//����ÿ��ţ��ж��Ƿ���Ϸ�������
		{	
			result=use_card(c1,choice,whether_hide_effect,&locktime,this_room);//ʹ���˸ò��ֿ�
			if(result==i_failed_effect)return still_fight;
			if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)return result;
			result=launch_card_effect(c1,NULL,effect_cardnumber,my_turn,&locktime,this_room);//������ƬЧ��������һ�����
			if(result==i_cancel_effect&&effect_cardnumber!=75){confirm_send_success(c1,"Ч������!�����޷�ȡ��!\n");return still_fight;}//���Ч�����޷�������
			return result;
		}
		else//�ÿ��޷�����
			if(c1->device==win_console)
				confirm_send_success(c1,"���ֿ���������������!������ѡ���ֿ�:");
			else
			{
				confirm_send_success(c1,"�ÿ������㷢������!\n");
				room_broadcast(this_room,"��ȡ�������ֿ�!\n");
				return still_fight;
			}
	}
}

int player_use_card(client_member* c1,fight_room* this_room)//�Լ�����ʹ���ֿ�
{
	char StrBuf[MAX_PATH] = { 0 };
	int result;

	if(c1->p1->c[0]==false){confirm_send_success(c1,"�㵱ǰû���ֿ�!\n");return still_fight;}

	lock_other(c1,this_room);//�¼�����

	result=use_card_process(c1,this_room);
	release_other(c1,result,this_room);
	return result;
}

//����ֵΪstill_fight���ʾû����ȥ
int player_attack(client_member* c1,fight_room* this_room)//���c�ж�ѡ���չ�
{
	//int choice;
	int result=still_fight;
	int locktime=0;//����������ʼΪ0
    char StrBuf[MAX_PATH] = { 0 };
	
	
	if(!c1->p1->whether_myturn)//����Ƿ�Ϊ����һغ�
	{confirm_send_success(c1,"���ڲ�����Ļغ�!��û���չ�����!\n");return still_fight;}
	if(c1->p1->fight_chance==0)//ȷ���Ǹ���һغϣ���������ڸûغ�ʣ����չ�����
	{confirm_send_success(c1,"�ûغ�����չ�������������!\n");return still_fight;}
	
	lock_other(c1,this_room);//�����߳�//event_happen=�¼�������־��
/*	
	//strcpy(StrBuf,"\n");
	sprintf(Buf,"%d",c1->p1->player_No);strcat(StrBuf,Buf);strcat(StrBuf,"�����:");
	strcat(StrBuf,search_character_name(c1->p1->character_number));strcat(StrBuf,"�������չ���\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));memset(Buf,0,sizeof(Buf));



	choice=choose_enemy(c1,this_room);//ѡ���չ�����
	if(choice==force_offline)
	{		release_other(c2,result,this_room);return force_offline;	}

	if(choice==i_cancel_effect)//���ѡ��ȡ���չ���ֱ�ӷ���
	{
		room_broadcast(this_room,"��ȡ�����չ���\n");
		confirm_send_success(c1,"����������ҳ�桭������\n");
		release_other(c2,result,this_room);
		return still_fight;
	}
*/	
		client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	
	result=general_attack(c1,c2,myturn_attack,&locktime,this_room);//�չ����Է���������ͨ���չ��¼�
	release_other(c1,result,this_room);//�¼�����,����������ж�
	return result;
}

void delete_weapon_process(client_member* c1,fight_room* this_room)
{
	int count;
	int choice;
	char StrBuf[MAX_PATH] = { 0 };

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ʼ����������\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));

	if(c1->device==win_console)
	{
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true)
			{
				addtext(StrBuf,"װ��");
				addtext(StrBuf,count+1);
				addtext(StrBuf,": ");
				addtext(StrBuf,search_card(p_name,c1->p1->weapon[count]));
				addtext(StrBuf,"  ");
				confirm_send_success(c1,StrBuf);
				memset(StrBuf,0,sizeof(StrBuf));
			}
			
		confirm_send_success(c1,"\n����ڼ���װ����(0�����������)(1��2��ʾ������ڼ�������װ����):");
	}
	else
		special_signal(c1,delete_which_weapon);

	while(true)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return ;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			confirm_send_success(c1,"��ѡ����ȡ����������!\n");	
			return ;
		}
		
		choice=atoi(StrBuf);//���ַ���Ϊ����
		if(choice==0){confirm_send_success(c1,"��������!����������:");continue;}
		choice--;
		if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"��������!����������:");continue;}
		if(c1->p1->w[choice]==false){confirm_send_success(c1,"��������Ϊ��!����������:");continue;}

		delete_weapon(c1,choice,this_room);

		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,search_card(p_name,c1->p1->weapon[choice]));
		addtext(StrBuf,"�����!\n");
		room_broadcast(this_room,StrBuf);
		return;
	}
}
void player_delete_weapon(client_member* c1,fight_room* this_room) //������װ��������
{
	if(c1->p1->w[0]==false){confirm_send_success(c1,"��ǰû��װ������!\n");return;}
	if(!c1->p1->whether_myturn)//����Ƿ�Ϊ����һغ�
	{confirm_send_success(c1,"���ڲ�����Ļغ�!���޷���������!\n");return ;}

	lock_other(c1,this_room);//�¼���ʼ
	delete_weapon_process(c1,this_room);
	release_other(c1,still_fight,this_room);//�¼�����,����������ж�
	return;
}




int weapon_effect_process(client_member* c1,fight_room* this_room)
{
	int result=still_fight;
	int choice,choice2;
	int locktime=0;
	char StrBuf[MAX_PATH] = { 0 };
	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ʼ��������Ч����\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));

	show_him_his_weapon_effect(c1);//��ʾ��������

	while(true)
	{
		//ѡ�񷢶��ĸ�����
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			room_broadcast(this_room,"��ѡ����ȡ����������Ч��!\n");
			return still_fight;
		}

		choice=atoi(StrBuf);//���ַ���Ϊ����
		if(choice==0){confirm_send_success(c1,"��������!����������:");continue;}
		choice--;
		cout<<choice<<endl;
		if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"�������볬�������ޣ�����������:");continue;}
		if(c1->p1->w[choice]==false){confirm_send_success(c1,"��������Ϊ�գ�����������:");continue;}

		if(c1->device==win_console)
			confirm_send_success(c1,"���뷢����������Ч����?(����0��ʾ��ȡ���ò�����):");
		else
			special_signal(c1,get_we_number);//��ȡͼ�����ͻ��������ڼ���Ч��
	    
		while(true)
		{
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

			if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ��������������Ч��!\n����������һѡ��ҳ�桭��\n�������뷢���ڼ�������(����0��ʾ��ȡ���ò�����):");break;}
		
			choice2=atoi(StrBuf);//���ַ���Ϊ����
			if(choice2==0){confirm_send_success(c1,"��������!����������:");continue;}
			choice2--;
			if(choice2<0||choice2>=weaponeffectlimit||c1->p1->w_e[choice][choice2]==false){confirm_send_success(c1,"���볬�������� �� ������Ч����Ϊ�գ�����������:");continue;}
			//��������Ч����ʱ���Զ��ж��Ƿ�ɷ���
			//�ͻ��������ţ����������ҵĻغϡ��������������ĸ��������������
			if(!judge_weapon_effect(c1, c1->p1->weapon_effect[choice][choice2],my_turn,&locktime,choice,this_room))
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"��������������!������ѡ��:");
				else
				{
					confirm_send_success(c1,"��������������!\n");
					room_broadcast(this_room,"��ѡ����ȡ����������Ч��!\n");
					return result;
				}
				continue;
			}
			result=launch_weapon_effect(c1,c1->p1->weapon_effect[choice][choice2],choice,my_turn,&locktime,this_room);
			if(result==i_cancel_effect)
			{
				room_broadcast(this_room,"��ѡ����ȡ����������Ч��!\n");
				return result;
			}
		//choice��ʾ�ڼ�������
		//c1Ϊ�����ߣ�p1->weapon_effect[choice][choice2]Ϊ������Ч����choiceΪ�ڼ�������������
		//my_turnΪ��������(���Լ��غ��Լ�����������)��this_roomΪ��������б�
			
			return result;
		}//ѡ��������Ч���Ž���

	}//�����ĸ���������

}

//��ȡ����������weapon_effect�еĺ��룬��ʾ��Ч�������ѡ��Ч�����������뷢���������ж��Ƿ���Ϸ��������������ϵķ����޷����������ϵĽ��з���
int player_weapon_effect(client_member* c1,fight_room* this_room)
{
	int result=still_fight;
	int locktime=0;

	if(c1->p1->w[0]==false){confirm_send_success(c1,"��ǰû��װ������!\n");return still_fight;}

	int count,count2,effectnumber;
	effectnumber=0;
	for(count=0;count<weaponlimit;count++)
	{
		if(c1->p1->w[count])
		{
			for(count2=0;count2<weaponeffectlimit;count2++)
				if(c1->p1->w_e[count2])
				{effectnumber++;break;}
			break;
		}
	}
	if(effectnumber==0){confirm_send_success(c1,"��ǰ����û���κ�Ч���ɷ���!\n");return still_fight;}
	lock_other(c1,this_room);
	result=weapon_effect_process(c1,this_room);
	release_other(c1,result,this_room);//�¼�����,����������ж�
	return result;
}

int player_effect(client_member* c1,fight_room* this_room)
{
	int locktime=0;
	char StrBuf[MAX_PATH] = { 0 };
	int result=still_fight;
	if(c1->p1->a[0]==false)//������û�и���Ч��
	{
		confirm_send_success(c1,"������û�и���Ч��!\n");
		return still_fight;
	}
	if(c1->p1->a[1]==false)//���˵ֻ��һ������Ч����ֱ���ж���Ч���ܲ��ܷ������ܷ�����ֱ�ӷ���
	{
		if(judge_character_effect(c1,c1->p1->ability[0],my_turn,&locktime))//�ж��ܷ񷢶�
			result=character_effect(c1,c1->p1->ability[0],this_room);//����Ч��
		else
		{
			confirm_send_success(c1,"����ʧ��!\n");
			return still_fight;
		}
	}
	if(c1->p1->a[1]==true)//���˵�ж�����ܣ������ѡ��
	{
		confirm_send_success(c1,"�������뷢���ĸ�����Ч����\n��ѡ��");
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ
	}
	return result;
}
int buff_effect_process(client_member* c1,fight_room* this_room)
{
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int choice;
	int result=still_fight;
	int locktime=0;
	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ʼ�����Ӹ�Ч����\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));

	show_him_his_buff_effect(c1);//չʾ�����е�buffЧ��
			while(true)
			{
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ�������Ӹ�Ч��!\n����������һѡ��ҳ�桭��\n");break;}
				
				choice=atoi(StrBuf);//���ַ���Ϊ����
				if(choice==0){confirm_send_success(c1,"��������!����������:");continue;}
				choice--;
				if(choice==1&&c1->p1->hideshield>0)
					result=launch_buff_effect(c1,75,my_turn,&locktime,this_room);
				if(result==i_cancel_effect){confirm_send_success(c1,"��ѡ����ȡ�������Ӹ�Ч��!\n����������һѡ��ҳ�桭��\n");break;}
				
				return result;
			}
			return result;
}

void player_end_turn(client_member* c1,fight_room* this_room)//����������غϽ���
{

	client_member* c2;
	if(this_room->c[0]->member_No==c1->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	if(this_room->ready_number==0)
					{
						lock_other_N(c1,this_room);
						confirm_send_success(c1,"�ȴ���������غϡ���\n");
						
						if(c2->device==win_console)
							confirm_send_success(c2,"\n\n����ѡ���˽����غ�!\n\n");
						else
							confirm_send_success(c2,"����ѡ���˽����غ�!\n");
						release_other(c1,still_fight,this_room);
					}
}







int player_buff_effect(client_member* c1,fight_room* this_room)
{
	int result=still_fight;
	if(c1->p1->hideshield==0)
	{
		if(c1->device==win_console)
			confirm_send_success(c1,"�㵱ǰû�мӸ�Ч��!������ѡ��");
		else
			confirm_send_success(c1,"�㵱ǰû�мӸ�Ч��!\n");
		return result;
	}
	lock_other(c1,this_room);
	result=buff_effect_process(c1,this_room);
	release_other(c1,result,this_room);//�¼�����,����������ж�
	return result;
			
}


#endif