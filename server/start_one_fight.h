
#ifndef INCLUDE_start_one_fight_H
#define INCLUDE_start_one_fight_H

#include "server_general_function.h"
#include "player_choose.h"



void start_fight(client_member* p,fight_room* this_room)//����ս��
{
	char StrBuf[MAX_PATH] = { 0 };//��������洢���յ�������
	char Buf[MAX_PATH] = { 0 };
	bool smooth_link=false;//Ϊ�˱���guest�Ŀͻ����޷�����
	bool be_shown=false;//���ĳֵ�������ʾ�˾Ͳ�������ʾ��һ��ʼ��Ϊû��ʾ
	int choice;
	int result;
	int count;//������
	bool main_turn=false;//�Ƿ���������غ�
	bool i_am_wait=false;//�Ƿ��ڵȴ�
	bool whether_choice_right=false;//���ѡ���Ƿ���ȷ
	
	client_member* c2;
	if(p->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	
	Sleep(100);
	//�ÿͻ���ѡȡ��ɫ(�������˲ŷ�����������)
	if(!p->guest)//����ÿͻ���Ϊ�������ˣ�Ⱥ�������������ˣ�������ѡ���ɫ��Ŀǰ������������ˣ�p1,p2
	{
		addtext(StrBuf,character_choice);addtext(StrBuf,"\n");
		::send(g_ClientSocket[p->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);// �������ݵ������ӵĿͻ�����
		if(c2!=NULL&&c2->member_No!=robot_symbol)//�����Ϊ��  ��  ���ǻ�����
			::send(g_ClientSocket[c2->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//ͬʱ����������Ҳ��һ��
	}

	while(true)//��ʼѡ���ɫ��ѭ��
	{
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,get_client_message(p));if(p->F_offline){return;}//���տͻ��˷�����Ϣ�������ʱ�Ͽ�ͨѶ������ͨ

		if(check_sb_offline(p,this_room))return;//���������;�˳�,ȷ��û�Ͽ�ͨѶ��ȷ�Ϸ������Ƿ������˳�
		choice=0;
		switch(atoi(StrBuf))//ȷ�����ѡ��
		{
			case 1:choice=1;addtext(Buf,"��ѡ����սʿ!\n");break;
			case 2:choice=2;addtext(Buf,"��ѡ���˷�ʿ!\n");break;
			default:
				if(p->device==win_console)
					confirm_send_success(p,"������Ч�����������룺");
				else
					special_signal(p,reopen_choice);
		}
		if(choice==1||choice==2)break;
	}//����ѡ���ɫ��ѭ��

	if(p->device==win_console)confirm_send_success(p,Buf);//������ѡ����ʲô

	ini_character(p->p1,this_room->ready_number+1);//����ɫ���ڼ�����ң���ʼ����������
	
	memset(p->p1->name,0,sizeof(p->p1->name));
	addtext(p->p1->name,this_room->ready_number+1);//ͨ�����֣���x����ң�xx��
	addtext(p->p1->name,"�����:");
	addtext(p->p1->name,search_character(choice));
	addtext(p->p1->name," ");
	set_character(choice,p->p1);//��Ѫ���������Գ�ʼ��
	this_room->ready_number++;//���������+1

	if(check_robot(c2))
	{
		this_room->ready_number++;//�����˲���ѡ���ɫ����
		ini_character(c2->p1,this_room->ready_number+1);//��ʼ����������������
		memset(c2->p1->name,0,sizeof(c2->p1->name));
		addtext(c2->p1->name,"2�����:ľ׮ ");
		set_character(0,c2->p1);//��Ѫ���������Գ�ʼ��
		ini_deck(this_room->deck);//�����ʼ��ϴ��
		this_room->passturns=1;//�ֻ�����1
		this_room->turn_force_end=false;//��ա�ǿ�ƽ����ûغϡ���־
		p->p1->whether_myturn=true;//��ѡ�ߵ�һ�غ�
		be_shown=true;
		i_am_wait=true;
	}

	while(this_room->ready_number<playernumber)//�Լ�������ɫ��ɺ����ȶ���Ϸ�������ó�ʼ�����ټ��������Ƿ����
	{
		if(!be_shown)//��ʾһ��"�ȴ�����ѡ���ɫ����\n"��ͬʱ��ʼ����Ϸ�������ã�֮��ȴ�����ѡ����Щ���鲻���ظ�����ֻ��һ��
		{
			ini_deck(this_room->deck);//�����ʼ��ϴ��
			this_room->passturns=1;//�ֻ�����1
			this_room->turn_force_end=false;//��ա�ǿ�ƽ����ûغϡ���־
			p->p1->whether_myturn=true;//��ѡ�ߵ�һ�غ�

			if(p->device==win_console)confirm_send_success(p,"�ȴ�����ѡ���ɫ����\n");
			be_shown=true;
			i_am_wait=true;
		}

		if(check_sb_offline(p,this_room))return;//���������;�˳�
		Sleep(50);
	}
	
	special_signal(p,choice_complete);//ѡ������ˣ�����ս������״̬

	if(check_sb_offline(p,this_room))return;//���������;�˳�

	if(i_am_wait)
	{
		if(!check_robot(c2))SuspendThread(hthread[c2->member_No-1]);//�����������߳�

		this_room->ready_number=0;
		refresh_character_state_F(p,this_room);
		i_am_wait=false;

		get_specific_card(63,p->p1);
		get_specific_card(70,p->p1);
		//get_specific_card(1,p->p1);
		//get_specific_card(2,p->p1);
		get_card(4,p,this_room);//��ҳ�ʼ��ȡ4���ֿ�
		get_card(4,c2,this_room);//������ҳ�ʼ��ȡ4���ֿ�

		if(!check_robot(c2))ResumeThread(hthread[c2->member_No-1]);//�����������߳�
	}
	else
	{
		//get_specific_card(0,p->p1);
		Sleep(1000);
	}

	be_shown=false;

	while(true)//һ�غ���ѭ����ʼ
	{
		main_turn=false;//��ȷ���Ƿ���������غ�

		if(p->p1->whether_myturn==true)//�����ĳ�˵����غϣ����лغ�����
		{
			Sleep(300);	
			if(!check_robot(c2))SuspendThread(hthread[c2->member_No-1]);//�����������߳�

			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"ĿǰΪ�� ");addtext(StrBuf,this_room->passturns);addtext(StrBuf," ���ֻ�\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//�㲥ĿǰΪ�ڼ����ֻ�
			addtext(StrBuf,"������");addtext(StrBuf,p->p1->name);addtext(StrBuf,"�Ļغ�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//�㲥˭�Ļغ�			
			special_signal(p,now_is_your_turn);//"��������Ļغ�!\n"
			main_turn=true;	

			rule_get_card(p,this_room);//���غ��߽��й����Գ鿨
			p->p1->fight_chance=1;//���һ�ι����ṩ���չ�����
			this_room->ready_number=0;//�����غϽ�����������Ϊ0

			for(count=0;count<playernumber;count++)
			{
				if(check_robot(this_room->c[count]))this_room->ready_number++;//����ǻ����ˣ�ѡ��غϽ�����+1
				special_signal(this_room->c[count],start_another_turn);//�������ǿ�ʼ��һ���غ������ж��ˣ�
			}
			if(!check_robot(c2))
			ResumeThread(hthread[c2->member_No-1]);//�����������߳�
		}
		else
		{
			for(count=0;count<playernumber;count++)
				if(this_room->c[count]->p1->whether_myturn)//˭�����غ��ߣ�
				{
					if(this_room->c[count]->member_No==robot_symbol)//���غ����ǲ��ǻ����ˣ�
					{
						if(this_room->c[0]->member_No==p->member_No)//���������ģʽ�£��л����ˣ���ʱ�÷������غϽ����ߣ�������еȴ�
						{
							addtext(StrBuf,"ĿǰΪ�� ");addtext(StrBuf,this_room->passturns);addtext(StrBuf," ���ֻ�\n");
							room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
							addtext(StrBuf,"������");addtext(StrBuf,this_room->c[count]->p1->name);addtext(StrBuf,"�Ļغ�!\n");
							room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
							
							rule_get_card(this_room->c[count],this_room);//���غ���(������)���й����Գ鿨
							
							for(count=0;count<playernumber;count++)
							{
								if(check_robot(this_room->c[count]))this_room->ready_number++;//����ǻ����ˣ�ѡ��غϽ�����+1
								special_signal(this_room->c[count],start_another_turn);//�������ǿ�ʼ��һ���غ������ж���
							}
						}
					}
					else
						Sleep(1000);//�����غ�����ҷǷ����ߵȴ����غ�����������
					break;
				}
		}

		while(true)//��ɫѡ���ж���ѭ��
		{
			if(p->p1->un_move_able>0)//�����ж���ǿ�ƽ����غ�
			{
				memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,p->p1->name);addtext(StrBuf," �����޷��ж�������ǿ�ƽ����غ�!\n");
				room_broadcast(this_room,StrBuf);
				break;
			}
			
			if(p->device==win_console&&atoi(StrBuf)!=client_get_message)special_signal(p,battle_choice);//��ʾѡ���ж�����
			if(check_sb_offline(p,this_room))return;//���������;�˳�
				
				memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,get_client_message(p));if(p->F_offline){return;}//�����ʱ�Ͽ�ͨѶ������ͨ

				if(check_sb_offline(p,this_room))return;//�����;�������˶���
				if(!p->p1->alive||!c2->p1->alive){Sleep(500);return;}//�������ˣ��˳�
				if(!p->p1->alive){Sleep(500);return;}//���������ʧ�ܣ�������Ϸ
				if(p->p1->un_move_able>0)continue;//�����ж���ǿ�ƽ����غ�
				if(this_room->turn_force_end==true)break;//ǿ�ƽ����ûغ�

				whether_choice_right=false;//Ĭ��û��ѡ����ȷ

				switch(atoi(StrBuf))//ȷ���Ƿ�������ȷ,��ȷ������ж�����
				{
					case 1:choice=1;whether_choice_right=true;confirm_send_success(p,"��ѡ���˽����չ�!\n");result = player_attack(p,this_room);break;
					case 2:choice=2;whether_choice_right=true;confirm_send_success(p,"��ѡ���˷�����ɫ����Ч��!\n");result = player_effect(p,this_room);break;
					case 3:choice=3;whether_choice_right=true;confirm_send_success(p,"��ѡ����ʹ���ֿ�!\n");result = player_use_card(p,this_room);break;
					case 4:choice=4;whether_choice_right=true;confirm_send_success(p,"��ѡ���˶�����װ��������!\n");player_delete_weapon(p,this_room);break;
					case 5:choice=5;whether_choice_right=true;confirm_send_success(p,"��ѡ���˷�����װ��������Ч��!\n");result = player_weapon_effect(p,this_room);break;
					case 7:choice=7;whether_choice_right=true;confirm_send_success(p,"��ѡ���˽����غ�!\n");player_end_turn(p,this_room);break;
					case 8:choice=8;whether_choice_right=true;confirm_send_success(p,"��ѡ���˷����Ӹ�Ч��!\n");result=player_buff_effect(p,this_room);break;
					default:if(p->device==win_console)confirm_send_success(p,"��������Ч�ַ�!����������:");
				}
			
				if(!whether_choice_right)
				{cout<<p->client_name<<"�ͻ����������Ч�ַ���"<<StrBuf<<" �ͻ���������ַ������ȣ�"<<strlen(StrBuf)<<endl;continue;}
	
			if(result==force_end_turn){this_room->turn_force_end=true;room_broadcast(this_room,"�˻غϱ�ǿ�ƽ���!\n");break;}
			if(this_room->turn_force_end==true)break;//ǿ�ƽ����ûغ�
			if(choice==7)break;//����ѡ��غϽ���

			if(result==one_die)
			{
				if(p->p1->alive==c2->p1->alive){special_signal(p,game_standoff);special_signal(c2,game_standoff);}//ƽ��
				if(p->p1->alive){special_signal(p,you_win);special_signal(c2,you_lose);}//��Ӯ��
				else{special_signal(p,you_lose);special_signal(c2,you_win);}//������
				p->state=in_room;
				return;
			}

		}//��ɫѡ���ж���ѭ������

		this_room->ready_number++;//ѡ��غϽ�����+1
		while(this_room->ready_number<playernumber)//�Լ������غϺ󣬼��������Ƿ����
		{
			if(check_sb_offline(p,this_room))return;//���������;�˳�
			Sleep(50);
			i_am_wait=true;
		}
		if(i_am_wait){Sleep(50);this_room->ready_number=0;i_am_wait=false;}//�����غϽ�������0

		
		//�غϽ���,���غ��߲Ž��н��㺯�����������÷���
		if(main_turn)
		{
			if(!check_robot(c2))SuspendThread(hthread[c2->member_No-1]);//�����¼�����,�����������߳�

			special_signal(p,start_turn_settle);special_signal(c2,start_turn_settle);//����غϽ���!����

			this_room->turn_force_end=false;//��ա�ǿ�ƽ����ûغϡ���־
			natural_set_playerturn(this_room);//���лغ��л������㣬�л�����һ���˵Ļغ�

			for(count=0;count<playernumber;count++)
			{
				if(judge_one_die(this_room->c[count],this_room)==one_die)//�鿴�Ƿ�������0Ѫ״̬ʧȥ����Ч���������ж�ʤ�����������1v1ʹ�ã������Ʋ����ã�
					result=one_die;
				if(this_room->c[count]->member_No==robot_symbol)this_room->ready_number2++;//�غϽ�����ɽ���ͬ���������+1
			}

			if(!check_robot(c2))ResumeThread(hthread[c2->member_No-1]);///�����¼������������������߳�
		}
		else
		{
			for(count=0;count<playernumber;count++)
				if(this_room->c[count]->p1->whether_myturn)//˭�����غ��ߣ�
				{
					if(this_room->c[count]->member_No==robot_symbol)//���غ����ǲ��ǻ����ˣ�
					{
						if(this_room->c[0]->member_No==p->member_No)//���������ģʽ�£��л����ˣ���ʱ�÷������غϽ����ߣ�������еȴ�
						{
							lock_other_N(p,this_room);//�����¼�����,��������

							special_signal(p,start_turn_settle);//����غϽ���!����
							special_signal(c2,start_turn_settle);

							this_room->turn_force_end=false;//��ա�ǿ�ƽ����ûغϡ���־
							natural_set_playerturn(this_room);//���лغ��л������㣬�л�����һ���˵Ļغ�
							
							for(count=0;count<playernumber;count++)
							{
								if(judge_one_die(this_room->c[count],this_room)==one_die)//�鿴�Ƿ�������0Ѫ״̬ʧȥ����Ч���������ж�ʤ�����������1v1ʹ�ã������Ʋ����ã�
									result=one_die;
								
								if(this_room->c[count]->member_No==robot_symbol)this_room->ready_number2++;//�غϽ�����ɽ���ͬ���������+1
							}

							release_other_N(p,this_room);//�����¼���������������
						}
					}
					else
						Sleep(2000);//�����غ�����ҷǷ����ߵȴ����غ�����������
					break;
				}
		}

		if(result==one_die)//�������ˣ��غϽ���
		{
			confirm_send_success(p,"�����˻ط���״̬��������\n");
			if(p->p1->alive==c2->p1->alive){special_signal(p,game_standoff);special_signal(c2,game_standoff);}//ƽ��
			if(p->p1->alive)	special_signal(p,you_win);//��Ӯ��
			else				special_signal(p,you_lose);//������
			return;
		}

		this_room->ready_number2++;//�غϽ�����ɽ���ͬ����

		while(this_room->ready_number2<playernumber)//�Լ������غϺ󣬼��������Ƿ�ͬ�������
		{
			if(check_sb_offline(p,this_room))return;//���������;�˳�
			i_am_wait=true;
		}
		if(i_am_wait){this_room->ready_number2=0;i_am_wait=false;}//ͬ�����������0
	}//һ�غ���ѭ������

	
}

#endif