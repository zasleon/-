#ifndef INCLUDE_attack_fight_back_H
#define INCLUDE_attack_fight_back_H

#include "launch_sth_effect.h"
int fight_back_UI(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime)//ͼ�ν���ר��
{
	
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int choice,choice2;
	int effect_number;//��¼������ƬЧ���Ŀ���
	int result=still_fight;
	bool whether_hide_effect;
	
	if(c1->p1->un_move_able!=0||c1->p1->unmoveable_time!=false)
	{
		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ǰ�����޷��ж�״̬!\n");
		room_broadcast(this_room,StrBuf);return still_fight;
	}

	confirm_send_success(c2,"�ȴ�����˼������\n");
	addtext(StrBuf,"���¼�:");
	switch(situation)
	{
		case i_will_attack:			addtext(StrBuf,"�㷢�����չ�");break;
		case i_will_be_attacked:	addtext(StrBuf,"�㼴�����չ�");break;
		case i_attack:				addtext(StrBuf,"���չ��ɹ�");break;
		case i_be_attacked:			addtext(StrBuf,"�㱻�չ���");break;
		case i_failed_attack_E:		addtext(StrBuf,"���չ��������");break;
		case i_failed_attack_D:		addtext(StrBuf,"���չ�������");break;
		case i_failed_attack_W:		addtext(StrBuf,"���չ�����������");break;
		case i_be_effect_A:			addtext(StrBuf,"�Լ��Ѿ��ܵ�������ѪЧ��");break;
		case i_be_effect_B:			addtext(StrBuf,"�Լ��ܵ���ѪЧ��");break;
		case i_will_be_hit_die:		addtext(StrBuf,"�Լ������ܵ�������Ѫ");break;
		case i_will_be_effect_B:	addtext(StrBuf,"�Լ������ܵ���ѪЧ��");break;
		case sb_card_used:			addtext(StrBuf,"������һ�ſ�");break;
		default:cout<<"----------------------------����situation="<<situation<<endl;
	}
	addtext(StrBuf,"��\n");
	if(situation==sb_card_used)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,"���Ƿ�Ҫ��ֹ�䷢������\n");
		confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
	}
	else
	{
		confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,"���Ƿ��ж���\n");confirm_send_success(c1,StrBuf);
	}
	
	while(true)//��ѡ����ѭ��
	{
		special_signal(c1,state_fight_back);//�û�����ĳɷ�������
		
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ
		
		special_signal(c1,state_return_normal);

		//if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ���ж�!\n");confirm_send_success(c2,"����ѡ���ж�!\n");return still_fight;}
		//if(strlen(StrBuf)>1||atoi(StrBuf)>4||atoi(StrBuf)<1){confirm_send_success(c1,"��������Ч�ַ�!������ѡ��!\n");continue;}
		
		switch(atoi(StrBuf))
		{
		case 1://�����ֿ���ʼ
			{
				special_signal(c1,get_c_number);//��ȡ�����ڼ����ֿ�

				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ��ȡ�������ֿ�!\n");continue;}
				choice=atoi(StrBuf);//���ַ���Ϊ����
				if(choice==0){confirm_send_success(c1,"������Ч�ַ�!������ѡ���ֿ�:\n");continue;}
				choice--;
				if(choice>=cardlimit||choice<0){confirm_send_success(c1,"���볬���ֿ���/���ޣ�������ѡ���ֿ�:\n");continue;}
				if(c1->p1->c[choice]==false){confirm_send_success(c1,"���ֿ���Ϊ�գ�������ѡ���ֿ�:\n");continue;}

				special_signal(c1,whether_launch_hideeffect);//ѯ���Ƿ񷢶���Ч��

				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ
				
				switch(atoi(StrBuf))
				{
					case  1:effect_number=c1->p1->card[choice];whether_hide_effect=false;break;
					case  0:effect_number=c1->p1->hideeffect[choice];whether_hide_effect=true;break;//ȷ�Ϸ������ĸ�����
				}

				if(judge_card(c1,effect_number,situation,locktime))//����ÿ��ţ��ж��Ƿ���Ϸ�������
				{
					
					result=use_card(c1,choice,whether_hide_effect,locktime,this_room);//��������һ���۵Ŀ�
					if(result==i_failed_effect)return still_fight;
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
					if(judge_assist(effect_number))//����������Ч��
					{
						c1->p1->whether_use_assist=effect_number;//�����ʹ�������������潫������������Ч(��Ϊ�����ǻ�Ӱ��������ܵ�Ч����)
						result=launch_assist_effect(c1,c2,effect_number,fore_effect,locktime,this_room);
						return result;
					}
					else//������ͨЧ����
					{
						result=launch_card_effect(c1,c2,effect_number,situation,locktime,this_room);//������ƬЧ��������һ�����
						if(result==i_cancel_effect&&effect_number!=75&&effect_number!=402){confirm_send_success(c1,"Ч������!�����޷�ȡ��!\n");return still_fight;}
						return result;
					}
				}//��ƬЧ����������
				confirm_send_success(c1,"�ÿ�Ƭ�����Ϸ�������!\n");continue;

			}//�����ֿ�����
			break;
		case 2://������ɫЧ����ʼ
			{
				if(c1->p1->a[1]==false)//���˵ֻ��һ������Ч����ֱ���ж���Ч���ܲ��ܷ������ܷ�����ֱ�ӷ���
				{
					if(judge_character_effect(c1,c1->p1->ability[0],situation,locktime))//�ж��ܷ񷢶�
						result=character_effect(c1,c1->p1->ability[0],this_room);//����Ч��
					else
					{
						confirm_send_success(c1,"������ѡ���ж�!\n");
						continue;
					}
				}
				//չʾ���и���Ч�����ж�ѡ�е�Ч���ܷ񷢶�
				room_broadcast(this_room,"�����˸���Ч��!�Ƿ�������ֹ��\nû����ֹ����\n");
				confirm_send_success(c1,"�����ɹ�!\n");
				return result;
			}//������ɫЧ������
			break;
		case 3://��������Ч����ʼ
			{
				if(c1->p1->w[0]==false){confirm_send_success(c1,"�㵱ǰû��װ������!\n");continue;}

				show_him_his_weapon_effect(c1);//��ʾ�Լ�������������Ч��
//���շ����ڼ�������
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ����������Ч��!\n");continue;}
					
				choice=atoi(StrBuf);//���ַ���Ϊ����
				if(choice==0){confirm_send_success(c1,"��������!\n");continue;}
				choice--;
				if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"���볬��������!\n");continue;}

				if(c1->p1->w[choice]==false){confirm_send_success(c1,"��������Ϊ��!\n");continue;}
					
				special_signal(c1,get_we_number);//��ȡ�����ڼ�������

				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ��������������Ч��!\n");continue;}
					
				choice2=atoi(StrBuf);//���ַ���Ϊ����
				if(choice2==0){confirm_send_success(c1,"��������!\n");continue;}
				choice2--;
				if(choice2<0||choice2>=weaponeffectlimit||c1->p1->w_e[choice][choice2]==false){confirm_send_success(c1,"���볬�������� �� ������Ч����Ϊ��!\n");continue;}
						
				if(!judge_weapon_effect(c1, c1->p1->weapon_effect[choice][choice2],situation,locktime,choice,this_room))//�ж��Ƿ�ɷ���
					{confirm_send_success(c1,"��������������!\n");continue;}
				result=launch_weapon_effect(c1,c1->p1->weapon_effect[choice][choice2],choice,situation,locktime,this_room);
				if(result==i_cancel_effect){continue;}
				return result;//choice��ʾ�ڼ�������
					//c1Ϊ�����ߣ�p1->weapon_effect[choice][choice2]Ϊ������Ч����choiceΪ�ڼ�������������
					//situationΪ��������(���Լ��غ��Լ�����������)��this_roomΪ��������б�
				
			}//��������Ч������
			break;
		case 4://�����Ӹ�Ч����ʼ
			{
				if(c1->p1->hideshield==0){confirm_send_success(c1,"�㵱ǰû�мӸ�Ч��!������ѡ���ж�!\n");continue;}
				show_him_his_buff_effect(c1);
				while(true)
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ�������Ӹ�Ч��!\n");break;}
					
					choice=atoi(StrBuf);//���ַ���Ϊ����
					if(choice==0){confirm_send_success(c1,"��������!\n");continue;}
					choice--;
					if(choice==1&&c1->p1->hideshield>0)
						result=launch_buff_effect(c1,75,situation,locktime,this_room);
					if(result==i_cancel_effect){confirm_send_success(c1,"��ѡ����ȡ�������Ӹ�Ч��!\n");continue;}
					
					return result;
				}
			}//�����Ӹ�Ч������
		case 0:
			{confirm_send_success(c1,"��ѡ���ж�!\n");confirm_send_success(c2,"����ѡ���ж�!\n");return still_fight;}
		default:
			{cout<<atoi(StrBuf)<<endl;continue;}
		}

	}	//��������ѡ����ѭ������
return still_fight;

}


int fight_back(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime)
{
	if(c1->member_No==robot_symbol)return still_fight;//�����˲���������
	int result=still_fight;
	if(c1->device!=win_console){result=fight_back_UI(c1,c2,situation,this_room,locktime);return result;}

	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int choice,choice2;
	int effect_number;//��¼������ƬЧ���Ŀ���
	bool whether_hide_effect;
	
	if(c1->p1->un_move_able!=0||c1->p1->unmoveable_time!=false)
	{
		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ǰ�����޷��ж�״̬!\n");
		room_broadcast(this_room,StrBuf);return still_fight;
	}

	confirm_send_success(c2,"�ȴ�����˼������\n");
	addtext(StrBuf,"���¼�:");
	switch(situation)
	{
		case i_will_be_attacked:	addtext(StrBuf,"�㼴�����չ�");break;
		case i_attack:				addtext(StrBuf,"���չ��ɹ�");break;
		case i_be_attacked:			addtext(StrBuf,"�㱻�չ���");break;
		case i_failed_attack_E:		addtext(StrBuf,"���չ��������");break;
		case i_failed_attack_D:		addtext(StrBuf,"���չ�������");break;
		case i_failed_attack_W:		addtext(StrBuf,"���չ�����������");break;
		case i_be_effect_A:			addtext(StrBuf,"�Լ��Ѿ��ܵ�������ѪЧ��");break;
		case i_be_effect_B:			addtext(StrBuf,"�Լ��ܵ���ѪЧ��");break;
		case i_will_be_hit_die:		addtext(StrBuf,"�Լ������ܵ�������Ѫ");break;
		case i_will_be_effect_B:	addtext(StrBuf,"�Լ������ܵ���ѪЧ��");break;
		default:cout<<"����situation="<<situation<<endl;
	}
	addtext(StrBuf,"!��\n");
	if(situation==sb_card_used)
	{memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,"���Ƿ�Ҫ��ֹ�䷢������\n");}
	else
	{
		confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,"���Ƿ��ж���\n");confirm_send_success(c1,StrBuf);
	}

	special_signal(c1,whether_fight_back);//"���Ƿ��ж�����0:���ж���1:�����ֿ���2:��������Ч����3:��������Ч����4.�Ӹ�Ч����\n��ѡ��");


	while(true)//��ѡ����ѭ��
	{
		
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ���ж�!\n");confirm_send_success(c2,"����ѡ���ж�!\n");return still_fight;}
		
		if(strlen(StrBuf)>1||atoi(StrBuf)>4||atoi(StrBuf)<1){confirm_send_success(c1,"��������Ч�ַ�!������ѡ��");continue;}
		choice=atoi(StrBuf);
		switch(choice)
		{
		case 1://�����ֿ���ʼ
			{
				confirm_send_success(c1,"��ѡ�񷢶��ֿ�!\n��ѡ�񷢶�����:");
				
				while(true)//ѡ�񷢶��ֿ���ѭ����ʼ
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ��ȡ�������ֿ�!����������һ���桭��\n���Ƿ��ж�����0:���ж���1:�����ֿ���2:��������Ч����3:��������Ч����4.�Ӹ�Ч����\n��ѡ��");break;}
					choice=atoi(StrBuf);//���ַ���Ϊ����
					if(choice==0){confirm_send_success(c1,"������Ч�ַ�!������ѡ���ֿ�:");continue;}
					choice--;
					if(choice>=cardlimit||choice<0){confirm_send_success(c1,"���볬���ֿ���/���ޣ�������ѡ���ֿ�:");continue;}
					if(c1->p1->c[choice]==false){confirm_send_success(c1,"���ֿ���Ϊ�գ�������ѡ���ֿ�:");continue;}

					special_signal(c1,whether_launch_hideeffect);//ѯ���Ƿ񷢶���Ч��

					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ
					
					switch(atoi(StrBuf))
					{
						case  1:effect_number=c1->p1->card[choice];whether_hide_effect=false;break;
						case  0:effect_number=c1->p1->hideeffect[choice];whether_hide_effect=true;break;//ȷ�Ϸ������ĸ�����
					}

					if(judge_card(c1,effect_number,situation,locktime))//����ÿ��ţ��ж��Ƿ���Ϸ�������
					{	//��ʼ������ƬЧ��
						result=use_card(c1,choice,whether_hide_effect,locktime,this_room);//��������һ���۵Ŀ�
						if(result==i_failed_effect)return still_fight;
						if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
						
						result=launch_card_effect(c1,c2,effect_number,situation,locktime,this_room);//������ƬЧ��������һ�����
						if(result==i_cancel_effect&&effect_number!=75){confirm_send_success(c1,"Ч������!�����޷�ȡ��!\n");return still_fight;}

						return result;
					}//��ƬЧ����������
					confirm_send_success(c1,"�ÿ�Ƭ�����Ϸ���������������ѡ���ֿ�:");continue;
				}//ѡ�񷢶��ֿ���ѭ������

				
			}//�����ֿ�����
			break;
		case 2://������ɫЧ����ʼ
			{
				confirm_send_success(c1,"��ѡ�񷢶���ɫЧ��!\n");
				if(c1->p1->a[1]==false)//���˵ֻ��һ������Ч����ֱ���ж���Ч���ܲ��ܷ������ܷ�����ֱ�ӷ���
				{
					confirm_send_success(c1,"��Ľ�ɫֻ��һ��Ч��!\n");
					if(judge_character_effect(c1,c1->p1->ability[0],situation,locktime))//�ж��ܷ񷢶�
						result=character_effect(c1,c1->p1->ability[0],this_room);//����Ч��
					else
					{
						confirm_send_success(c1,"����ʧ��!����������һҳ�桭��\n���Ƿ��ж�����0:���ж���1:�����ֿ���2:��������Ч����3:��������Ч����4.�����Ӹ�Ч����\n��ѡ��");
						continue;
					}
				}
				room_broadcast(this_room,"�����˸���Ч��!�Ƿ�������ֹ��\nû����ֹ����\n");
				confirm_send_success(c1,"�����ɹ�!\n");
				return result;
			}//������ɫЧ������
			break;
		case 3://��������Ч����ʼ
			{
				confirm_send_success(c1,"��ѡ�񷢶�����Ч��!\n");
				if(c1->p1->w[0]==false){confirm_send_success(c1,"�㵱ǰû��װ������!������ѡ��:");continue;}

				show_him_his_weapon_effect(c1);//��ʾ�Լ�������������Ч��

				confirm_send_success(c1,"\n\n�������뷢���ڼ�������(����0��ʾ��ȡ���ò�����):");
				while(true)
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ����������Ч��!\n����������һѡ��ҳ�桭��\n���Ƿ��ж�����0:���ж���1:�����ֿ���2:��������Ч����3:��������Ч����4.�Ӹ�Ч����\n");break;}
					
					choice=atoi(StrBuf);//���ַ���Ϊ����
					if(choice==0){confirm_send_success(c1,"��������!����������:");continue;}
					choice--;
					if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"���볬�������ޣ�����������:");continue;}

					if(c1->p1->w[choice]==false){confirm_send_success(c1,"��������Ϊ�գ�����������:");continue;}
					confirm_send_success(c1,"���뷢����������Ч����?(����0��ʾ��ȡ���ò�����):");
					
					while(1)
					{
						memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

						if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ��������������Ч��!\n����������һѡ��ҳ�桭��\n�������뷢���ڼ�������(����0��ʾ��ȡ���ò�����):");break;}
					
						choice2=atoi(StrBuf);//���ַ���Ϊ����
						if(choice2==0){confirm_send_success(c1,"��������!����������:");continue;}
						choice2--;
						if(choice2<0||choice2>=weaponeffectlimit||c1->p1->w_e[choice][choice2]==false){confirm_send_success(c1,"���볬�������� �� ������Ч����Ϊ�գ�����������:");continue;}
						
						if(!judge_weapon_effect(c1, c1->p1->weapon_effect[choice][choice2],situation,locktime,choice,this_room))//�ж��Ƿ�ɷ���
							{confirm_send_success(c1,"��������������!������ѡ��:");continue;}
						result=launch_weapon_effect(c1,c1->p1->weapon_effect[choice][choice2],choice,situation,locktime,this_room);
						if(result==i_cancel_effect){confirm_send_success(c1,"���뷢����������Ч����?(����0��ʾ��ȡ���ò�����):");break;}
						return result;//choice��ʾ�ڼ�������
					//c1Ϊ�����ߣ�p1->weapon_effect[choice][choice2]Ϊ������Ч����choiceΪ�ڼ�������������
					//situationΪ��������(���Լ��غ��Լ�����������)��this_roomΪ��������б�

					}
				}
			}//��������Ч������
			break;
		case 4://�����Ӹ�Ч����ʼ
			{
				if(c1->p1->hideshield==0){confirm_send_success(c1,"�㵱ǰû�мӸ�Ч��!������ѡ��");continue;}
				show_him_his_buff_effect(c1);

				while(true)
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ѡ����ȡ�������Ӹ�Ч��!\n����������һѡ��ҳ�桭��\n���Ƿ��ж�����0:���ж���1:�����ֿ���2:������ɫЧ����3:��������Ч����\n��ѡ��):");break;}
					
					choice=atoi(StrBuf);//���ַ���Ϊ����
					if(choice==0){confirm_send_success(c1,"��������!����������:");continue;}
					choice--;
					if(choice==1&&c1->p1->hideshield>0)
						result=launch_buff_effect(c1,75,situation,locktime,this_room);
					if(result==i_cancel_effect){confirm_send_success(c1,"��ѡ����ȡ�������Ӹ�Ч��!\n����������һѡ��ҳ�桭��\n���Ƿ��ж�����0:���ж���1:�����ֿ���2:������ɫЧ����3:��������Ч����\n��ѡ��):");break;}
					
					return result;
				}
			}//�����Ӹ�Ч������
		}
		
	}	//��������ѡ����ѭ������
return still_fight;

}



//ͨ�ù���������p1����p2��situtation�ж��Ƿ���p1�����������������������Ĺ����չ�����������������޹�
int general_attack(client_member* c1,client_member* c2,int situation,int* locktime,fight_room* this_room)
{
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int count;
	int result=i_attack;//��ʼĬ��Ϊ�չ��ɹ�
	float damage=0;//���˺�����

	bool whether_use_assist=false;//�Ƿ�ʹ������������Ĭ��Ϊ��
	
	c1->p1->whether_in_attack++;c2->p1->whether_in_attack++;//���߽��벫��״̬

	if(situation==myturn_attack)c1->p1->fight_chance--;//�����ĳ�˵Ļغϣ������������չ�,����һ���չ�����

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��");addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�������չ�!\n");
	room_broadcast(this_room,StrBuf);//�㲥�չ��¼�
	confirm_send_success(c2,"�ȴ������ж�����\n");

	result=fight_back(c1,c2,i_will_attack,this_room,locktime);//�¼�������Ҫ�չ��ˡ�
	if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)//������չ��¼��жϻ�����
	{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}
	
	//�����չ���������
	auto_judge(c1,i_will_attack,this_room);//����Ч����������������Ҫ�չ��ˡ�

	//ͳ�ƹ������ܹ�����������p_attack_all�����������ܹ�����������p_defend_all
	float p_attack_all=	(c1->p1->attack)	+	(c1->p1->exattack_turn)	+	(c1->p1->exattack_time);
	for(count=0;count<2;count++)//���װ�����Ƿ��мӹ�����
		if(c1->p1->w[count]==true&&c1->p1->w_valid[count]==true)
			p_attack_all=	p_attack_all	+   c1->p1->exattack_weapon[count] + c1->p1->exattack_weapon_turn[count] + c1->p1->exattack_weapon_time[count];
    float p_defend_all=	c2->p1->attack +	c2->p1->exattack_turn	+	c2->p1->exattack_time;
	for(count=0;count<2;count++)
		if(c2->p1->w[count]==true&&c2->p1->w_valid[count]==true)
			p_defend_all=	p_defend_all	+	c2->p1->exattack_weapon[count] + c2->p1->exattack_weapon_turn[count] + c2->p1->exattack_weapon_time[count];

		memset(StrBuf,0,sizeof(StrBuf));
	if(!c1->p1->whether_attack_T)//����˴��չ����Ǵ���
	{
		//�����չ�Ѫ������
		damage	=( p_attack_all - p_defend_all )*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
		if(p_attack_all>p_defend_all)
		{
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"�ɹ����� ");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"!");
			addtext(StrBuf,"����������� ");addtext(StrBuf,damage);addtext(StrBuf,"���˺�!\n");
		}
		else
		if(p_attack_all<p_defend_all)//����
		{	
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"�Ĺ�����С�ڱ������� ");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�Ĺ�����!���Լ�����˷���!");
			addtext(StrBuf,"�����Լ������");addtext(StrBuf,-damage);addtext(StrBuf,"���˺�!\n");
		}
		else
		if(p_attack_all==p_defend_all)
		{
			addtext(StrBuf,"���߹�������ͬ!���β�������˫���κ�������˺�!\n");
		}
	}//�չ�Ѫ���������
	else//������ֱ�����Ӷ����ܹ���������������˺�
	{
		damage	=	p_attack_all*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
		
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," �� ");
		addtext(StrBuf,c2->p1->name);addtext(StrBuf," ����˴�͸�Թ���!!!");
		addtext(StrBuf,"�����������");addtext(StrBuf,damage);addtext(StrBuf,"���˺�!\n");
	}

	result=hit_damage(damage,c2,c1,this_room,locktime,StrBuf,i_will_be_attacked);//���չ��߽��� ��� �� ����

		if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)//������չ��¼��жϻ�����
		{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}
		if(result==i_failed_attack_E||result==i_failed_attack_D||result==i_failed_attack_W)
		{
			if(result==i_failed_attack_W)delete_weapon_all(c1,this_room);//���е����ˣ�����ȫ��
			fight_data_clean(c1->p1,c2->p1,true,this_room);//�ô��չ���������Ч��ֵ������

			result=fight_back(c1,c2,result,this_room,locktime);//�չ��� ��� ���Լ��չ�ʧ�ܡ��¼� ���з���
			c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;//�����˳�����״̬
			return result;
		}

	result=i_attack;//ִ�е��˲����ж�Ϊ�����ɹ�

	if(judge_one_die(c1,this_room)==one_die||judge_one_die(c2,this_room)==one_die)
	{
		c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;//�����˳�����״̬
		return one_die;
	}
	auto_judge(c1,c2,has_fought,this_room,locktime);//���˽�����һ�β���������һЩ���������� �ɽ�֮����

	//���ʹ��������������������������Ч������(ֻ��֮ǰЧ�����չ�֮���ȫ���ɹ�������²Ž���)
	if(c1->p1->whether_use_assist!=-1)
		result=launch_assist_effect(c1,c2,c1->p1->whether_use_assist,after_effect,locktime,this_room);
	if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)
	{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}//������Ч�������᲻����������

	c1->p1->attacktime_turn++;//�ô��չ����������������չ��ɹ�����

	memset(StrBuf,0,sizeof(StrBuf));
	addtext(StrBuf,"�˻غ��Լ��չ��ɹ�����:");addtext(StrBuf,c1->p1->attacktime_turn);addtext(StrBuf,"\n");
	confirm_send_success(c1,StrBuf);
	
	fight_data_clean(c1->p1,c2->p1,false,this_room);//�ô��չ���������Ч��ֵ������,�޷��ж�״̬������fight_data_clean����false����ʾ���޷��ж�״̬��������

	if(c2->p1->un_move_able==0&&c2->p1->unmoveable_time==false)
		result=fight_back(c2,c1,i_be_attacked,this_room,locktime);//���չ��߷���
	if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)//�¼��Ƿ����ڣ�
	{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}
	if(c1->p1->un_move_able==0&&c1->p1->unmoveable_time==false)//�¼�û���ڣ��չ��߷�������
		result=fight_back(c1,c2,i_attack,this_room,locktime);//�չ�������
	fight_data_clean(c1->p1,c2->p1,true,this_room);//�ô��չ���������Ч��ֵ������������޷��ж�״̬
	c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;//�����˳�����״̬
	return result;
}



//damage �ܵ������˺� c1���˺��� c2ʩ���˺��� this_room������� locktime�������� situationʲô�˺����ͣ�������Ѫ��Ч����Ѫ���չ���Ѫ��
int hit_damage(float damage,client_member* c1,client_member* c2,fight_room* this_room,int* locktime,char* StrBuf,int situation)//c2��c1���damage��Ѫ
{
	int result=still_fight;

	if(c1->p1->blood-damage<0)//�����������Ѫ�ͷ���
	{
		if(c2!=NULL&&situation!=i_will_be_effect_A)//����Լ��ܹ��ж��Ҳ���������ѪЧ��
		{
			c1->p1->i_soon_die=true;//�Լ���������
			result=fight_back(c1,c2,situation,this_room,locktime);//�Լ�����
			c1->p1->i_soon_die=false;//����������־
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
			if(situation==i_will_be_attacked&&result==i_failed_attack_E||result==i_failed_attack_D||result==i_failed_attack_W)return result;//���չ���ȡ�˸�/����/��������Ϊ
			if(situation==i_will_be_effect_B&&result==i_failed_effect_B)return i_failed_effect_B;//��Ч������Ч����Ѫ
			if(result==i_failed_damage)//�����Ѫ����Ч�����ж�����Ѫ��ԴΪʲô��Ŀǰֻ�й�ն��ɱ������ܻ᷵��i_failed_damage��
			{
				if(situation==i_will_be_attacked)return i_failed_attack_E;//������չ������ܸô��չ�
				if(situation==i_will_be_effect_B)return i_failed_effect_B;//�������ѪЧ������Ч���ô���ѪЧ��		
			}
		}
	}//������Ѫ�ж����
	else//��������ͨ��Ѫ����
	{
		if(situation!=i_will_be_effect_A&&c1->p1->un_move_able==0&&c1->p1->unmoveable_time==false&&(c2!=NULL))//����Լ��ܹ��ж��Ҳ���������ѪЧ��
		{
			result=fight_back(c1,c2,situation,this_room,locktime);//�Լ������ܵ���������Ѫ�¼������з���
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

			if(situation==i_will_be_attacked&&result==i_failed_attack_E||result==i_failed_attack_D||result==i_failed_attack_W)return result;//���չ���ȡ�˸�/����/��������Ϊ
			if(situation==i_will_be_effect_B&&result==i_failed_effect_B)return i_failed_effect_B;//��Ч������Ч����Ѫ

			if(result==i_failed_damage)//�����Ѫ����Ч�����ж�����Ѫ��ԴΪʲô��Ŀǰֻ�й�ն��ɱ������ܻ᷵��i_failed_damage��
			{
				if(situation==i_will_be_attacked)return i_failed_attack_E;//������չ������ܸô��չ�	
				if(situation==i_will_be_effect_B)return i_failed_effect_B;//�������ѪЧ������Ч���ô���ѪЧ��
			}
		}
	}

	if(StrBuf==NULL)
		cout<<"StrBuf==NULL\n";//�ַ���ָ��Ϊ�գ�
	else
		if(strlen(StrBuf)<=1)cout<<"strlen(StrBuf)<=1\n";//�ַ�����СС��1��
		else
			room_broadcast(this_room,StrBuf);//�˺��ɹ�����Ŀ�꣬�㲥װ��Ч����
	if(damage>0)//�ɹ�����
	{
		blood_change(-damage,c1,this_room);//��Ѫ
		if(c2!=NULL&&situation!=i_will_be_effect_A)//�������������Ѫ
			auto_judge(c2,i_hit_someone,&damage,locktime,this_room);//c2��ĳ�˳ɹ�������˺�����������֮��������Ч���ı���		
	}
	else
		blood_change(damage,c2,this_room);//����

	if(judge_one_die(c1,this_room)==one_die)return one_die;
	if(c2!=NULL&&judge_one_die(c2,this_room)==one_die)return one_die;//�ܵ���Ѫ���ж���û�������˾��޷����з���

	switch(situation)//���¼�����ת��
	{
		case i_will_be_effect_B:situation=i_be_effect_B;break;//�Լ��Ѿ��ܵ���ѪЧ��
		case i_will_be_attacked:situation=i_be_attacked;break;//�Լ��Ѿ��ܵ��չ���Ѫ
		case i_will_be_effect_A:situation=i_be_effect_A;break;//�Լ��Ѿ��ܵ�������ѪЧ��
	}

	if(c2==NULL||situation==i_be_effect_A)//�����������Ѫ�¼�
		if(c1->member_No==this_room->c[0]->member_No)
			c2=this_room->c[1];
		else
			c2=this_room->c[0];
	if(c2==NULL)return force_offline;

	if(situation==i_be_attacked)
		return i_attack;//�չ��߱��չ���Ѫ�ˣ��չ��������չ�������ʵ�֣���Ϊ�չ���Ѫ����ܻ�Ҫ��������������Ч�����������ﲻ�����չ�����

	result=fight_back(c1,c2,situation,this_room,locktime);//���з���

	if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
	return still_fight;
}



int lightspeed_attack(client_member* c1,client_member* c2,float damage,int* locktime,fight_room* this_room)//�����չ�,c1��c2,�ݶ�Ϊֱ���޸�Ѫ��
{

	if(damage==0)
	{
		int count;
		float p_attack_all=	(c1->p1->attack)	+	(c1->p1->exattack_turn)	+	(c1->p1->exattack_time);
	for(count=0;count<2;count++)//���װ�����Ƿ��мӹ�����
		if(c1->p1->w[count]==true&&c1->p1->w_valid[count]==true)
			p_attack_all=	p_attack_all	+   c1->p1->exattack_weapon[count] + c1->p1->exattack_weapon_turn[count] + c1->p1->exattack_weapon_time[count];
    float p_defend_all=	c2->p1->attack +	c2->p1->exattack_turn	+	c2->p1->exattack_time;
	for(count=0;count<2;count++)
		if(c2->p1->w[count]==true&&c2->p1->w_valid[count]==true)
			p_defend_all=	p_defend_all	+	c2->p1->exattack_weapon[count] + c2->p1->exattack_weapon_turn[count] + c2->p1->exattack_weapon_time[count];

		if(p_attack_all==p_defend_all)
			return still_fight;
		if(p_attack_all>p_defend_all)
		{
			damage	=( p_attack_all - p_defend_all )*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
			blood_change(-damage,c2,this_room);
		}
		else//����
		{
			damage	=( p_defend_all - p_attack_all )*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
			blood_change(-damage,c1,this_room);
		}
		
	}
	else//һ�㶼������ֵ�ĳ����չ�
	{
		blood_change(-damage,c2,this_room);
	}
	c1->p1->attacktime_turn++;//�˻غ��չ��ɹ�����+1
	auto_judge(c1,i_hit_someone,&damage,locktime,this_room);//c2��ĳ�˳ɹ�������˺�����������֮��������Ч���ı���	
	auto_judge(c1,c2,has_fought,this_room,locktime);//���˽�����һ�β���������һЩ���������� �ɽ�֮����
	if(judge_one_die(c1,this_room)==one_die||judge_one_die(c2,this_room)==one_die)return one_die;
	return still_fight;

}


void recover_blood(float number,client_member* c1,fight_room* this_room)//c1����Ѫ��
{
	if(number<0)
	{
		cout<<"�ָ���ֵ�쳣!Ϊ��!\n";
		number=-number;
	}
	blood_change(number,c1,this_room);
}

#endif