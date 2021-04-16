#ifndef INCLUDE_player_general_function_H
#define INCLUDE_player_general_function_H

#include "No_instruction.h"

using namespace std;
//һЩͨ�ú�����ͨ���չ���launch_sth_effect.h�ڣ�
int ask_whether_do(client_member* c1,int situation)//ѯ���Ƿ�Ҫ������
{
	char StrBuf[MAX_PATH]={0};
	char message_ask[MAX_PATH]={0};
	char message_do[MAX_PATH]={0};
	char message_not[MAX_PATH]={0};
	switch(situation)
	{
		case do_attack:
			if(c1->device==win_console)
				addtext(message_ask,"�Ƿ���Ҫ���䷢���չ�����0����������1��������\n��ѡ��");	
			else
				addtext(message_ask,"�Ƿ���Ҫ���䷢���չ���\n");
			addtext(message_not,"��ѡ�񲻷����չ�!\n");
			addtext(message_do,"��ѡ�񷢶��չ�!\n");
			break;
		case make_shield:
			if(c1->device==win_console)
				addtext(message_ask,"�Ƿ����ڷ�������ܣ���0����������1��������\n��ѡ��:");	
			else
				addtext(message_ask,"�Ƿ����ڷ�������ܣ�\n");
			addtext(message_not,"��ѡ�񲻷��������!\n");
			addtext(message_do,"��ѡ�񷢶������!\n");
			break;
		

		default:cout<<"???��ֵ�ѡ����Ŀ��situation="<<situation<<endl;
	}

	if(c1->device!=win_console)special_signal(c1,chooseYN);

	confirm_send_success(c1,message_ask);
		while(true)
		{
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,message_not);return cancel_choice;}//ȡ��ѡ�������Ч���Լ���Ѫ
				if(StrBuf[0]=='1'&&strlen(StrBuf)==1){confirm_send_success(c1,message_do);return do_choice;}//ȷ��ѡ��

				confirm_send_success(c1,"������Ч!���������룺\n");
		}
}
int choose_enemy(client_member* c1,char* message,fight_room* this_room)//ѡ��ָ���Ķ���(�ú���Ϊͨ�ú���u),�������Լ�
{
	int count;
	int choice;
	char StrBuf[MAX_PATH]={0};
	
	if(c1->device==win_console)
	{
		confirm_send_success(c1,"\n����ѡ��Ķ���:\n");
		for(count=0;count<playernumber;count++)
			if(this_room->c[count]->p1->player_No!=c1->p1->player_No)//����ѡ���Լ�
				if(this_room->c[count]->p1->alive==true)
				{
					addtext(StrBuf,count+1);
					addtext(StrBuf,". ");
					addtext(StrBuf,search_character(this_room->c[count]->p1->character_number));
					addtext(StrBuf,"\n");
				}
		confirm_send_success(c1,StrBuf);
	}
	

	while(true)
	{
		if(c1->device!=win_console)
		{
			special_signal(c1,show_enemy_list);
			confirm_send_success(c1,message);
		}
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ȡ����ѡ��!\n");return i_cancel_effect;}//���ȡ��ѡ�����ֱ�ӷ���

		choice=atoi(StrBuf);//���ַ���Ϊ����//����player_No
		if(choice==0){confirm_send_success(c1,"������Ч�ַ���������ѡ��:");continue;}

		//���ѡ������ȷ�Ķ�����ʾ�ɹ�����������ѭ��	
		//if(choice<0||choice>playernumber){confirm_send_success(c1,"���볬����\\���ޣ�������ѡ��:");continue;}
		for(count=0;count<playernumber;count++)
		{
			if(this_room->c[count]->p1->player_No==choice)
			{
				if(this_room->c[count]->p1->player_No!=c1->p1->player_No)
					return count;
			}
				
		}

		//���ûѡ����ȷ�Ķ��������������ѡ��
		if(c1->device==win_console)
			confirm_send_success(c1,"ѡ���˴���Ķ���!������ѡ��:");
		else
			confirm_send_success(c1,"ѡ���˴���Ķ���!������ѡ��!\n");
		
	}
}
int choose_someone(client_member* c1,char* message,fight_room* this_room)//ѡ��ָ���Ķ���(�ú���Ϊͨ�ú���u)�������Լ�
{
	int count;
	int choice;
	char StrBuf[MAX_PATH]={0};

	if(c1->device==win_console)
	{
		confirm_send_success(c1,"\n����ѡ��Ķ���:\n");

		for(count=0;count<playernumber;count++)//��ѡ���Լ�
			if(this_room->c[count]->p1->alive==true)
			{
				addtext(StrBuf,count+1);
				addtext(StrBuf,". ");
				addtext(StrBuf,search_skill(this_room->c[count]->p1->character_number));
				addtext(StrBuf,"\n");
			}
		confirm_send_success(c1,StrBuf);
		confirm_send_success(c1,"\n��ѡ�����(ѡ0��ʾȡ���ò���):");
	}

	while(true)
	{
		if(c1->device!=win_console)
		{
			special_signal(c1,show_alive_list);
			confirm_send_success(c1,message);
		}

		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"��ȡ����ѡ��!\n");return i_cancel_effect;}//���ȡ��ѡ�����ֱ�ӷ���

		choice=atoi(StrBuf);//���ַ���Ϊ����
		if(choice==0){confirm_send_success(c1,"������Ч�ַ���������ѡ��:");continue;}
		//���ѡ������ȷ�Ķ�����ʾ�ɹ�����������ѭ��
		//if(choice<0||choice>playernumber){confirm_send_success(c1,"���볬����\\���ޣ�������ѡ��:");continue;}
		for(count=0;count<playernumber;count++)
		{
			if(this_room->c[count]->p1->player_No==choice)
				return count;
		}
		//���ûѡ����ȷ�Ķ��������������ѡ��
		if(c1->device==win_console)
			confirm_send_success(c1,"ѡ���˴���Ķ���!������ѡ��:");
		else
			confirm_send_success(c1,"ѡ���˴���Ķ���!������ѡ��!\n");
	}
}

bool whether_it_happen(int percent)//�������ʣ������Ƿ���
{
	srand((unsigned)time(NULL)*rand()); //���ȡֵ0-100
	int result=rand()%(100-0);
	
	if(percent>result)
		return true;
	else
		return false;
}
bool check_robot(client_member* c1)//��λ����Ƿ���ϵͳ�����ˣ�
{
	if(c1->member_No==robot_symbol)return true;
	return false;
}
//void AI_think(fight_room* this_room){}

void addtext(char* StrBuf,int number)//׷������
{
	char Buf[MAX_PATH] = { 0 };
	sprintf_s(Buf,"%d",number);
	strcat(StrBuf,Buf);
}
void addtext(char* StrBuf,char* Buf)//׷���ַ���
{strcat(StrBuf,Buf);}
void addtext(char* StrBuf,float number)//׷��С��
{
	char Buf[MAX_PATH] = { 0 };
	int d,f;
	d=int(floor(number));
	f=int((number-floor(number))*100);
	addtext(StrBuf,d);
	addtext(StrBuf,".");
	addtext(StrBuf,f);
}

//���������������������������������������������������ԻغϵĴ��������������������������������������������������
	

void turnsettle(client_member* c1,fight_room* this_room)														//�غϽ���ʱ�������״̬����
{
	int count;
	bool whether_weapon_change=false;//�����Ƿ��б仯

	c1->p1->fight_chance=0;//���չ��Ĵ�����0
	if(c1->p1->un_move_able>0)c1->p1->un_move_able--;//�����ж��Ļغ���-1

	if(c1->p1->selfeffect_invalid>0)
		c1->p1->selfeffect_invalid--;//����Ч������Ч���Ļغ�-1
	else
		c1->p1->selfeffect_invalid=0;

		c1->p1->hurtmultiple_turn=1;//�ûغ��Լ���ɵ��չ���Ѫ������1
		c1->p1->hurtmultiple_time=1;//�ô��ж��Լ���ɵ��չ���Ѫ������1

		c1->p1->attacktime_turn=0;//�˻غ��Լ��չ��ɹ�������0
		attack_change(-c1->p1->exattack_turn,&c1->p1->exattack_turn,c1,this_room);//�ûغ��������ӵĶ��⹥������0

	for(count=0;count<weaponlimit;count++)
		if(c1->p1->w[count]==true)
		{
			if(c1->p1->weapon[count]==0){c1->p1->weaponeffect001[count]=1;}//�лʽ��������鿨������Ϊ1
			if(c1->p1->weapon[count]==5){c1->p1->weaponeffect051[count]=1;c1->p1->weaponeffect052[count]=1;}//ʳ������װ�û���Ϊ1
			if(c1->p1->weapon[count]==10){delete_weapon(c1,count,this_room);count--;}//��Ӱ����ʧ,ɾ���󿴿��Ƿ��л�Ӱ��

			if(c1->p1->exattack_weapon_turn[count]!=0){c1->p1->exattack_weapon_turn[count]=0;whether_weapon_change=true;}//����װ�������ûغ����ӵĹ�������0
			if(c1->p1->exattack_weapon_time[count]!=0){c1->p1->exattack_weapon_time[count]=0;whether_weapon_change=true;}//����װ�������ôβ������ӵĹ�������0
		}

	if(whether_weapon_change)
		weapon_change(c1,this_room);
		

}



void natural_set_playerturn(fight_room* this_room)							//��Ȼ�����һغ�
{
	int count;
	bool whether_buff_change=false;

	//һ��غϽ��㣬����������״̬����
	for(count=0;count<playernumber;count++)
		turnsettle(this_room->c[count],this_room);//ÿ���˻غϽ��㣬��Ч�������
	
	for(count=0;count<playernumber;count++)
	{
		if(this_room->c[count]->p1->whether_myturn==true)//ȷ�ϵ���˭�����غ�
		{
			this_room->c[count]->p1->whether_myturn=false;//�������������غ�
			if(count!=playernumber-1)//����������һ���˵Ļغ�
			{
				this_room->c[count+1]->p1->whether_myturn=true;//��һ������Ϊ���غ�ӵ����
				return;
			}
			if(count==playernumber-1)//��������һ���˵Ļغϣ��ֻ�������
			{
				this_room->c[0]->p1->whether_myturn=true;//����Ǹ��˱�Ϊ���غ�
				this_room->passturns++;
				for(count=0;count<playernumber;count++)//�ֻ����н��㣬ȫ�����ݸ��£������꼴�ֻ����н������
				{
					if(this_room->c[count]->p1->overdeath>0){whether_buff_change=true;this_room->c[count]->p1->overdeath--;}//����״̬
					if(this_room->c[count]->p1->hideshield>0){whether_buff_change=true;this_room->c[count]->p1->hideshield--;}//�����
					if(this_room->c[count]->p1->godlaser>0){whether_buff_change=true;this_room->c[count]->p1->godlaser--;}//���м���
					if(whether_buff_change)
					{
						buff_change(this_room->c[count],this_room);
					}
				}
				return;
			}
		}

	}



}

//�����������������������������������������������������ԻغϵĴ������������������������������������������������


//��������������������������������������������������������Ҹ�����Ϣ������������������������������������������������

void ini_character(player* p1,int playerNo)								//�Դ�������p�ĸ���״̬���г�ʼ��
{
	p1->player_No=playerNo;//�ڼ������
	p1->alive=true;//��Ϊ���״̬
	p1->fight_chance=0;//�չ�������0
	p1->whether_myturn=false;//��ǰ������Ļغ�
	p1->un_move_able=0;//�����ж��Ļغ�
	p1->unmoveable_time=false;//��ǰ�ж��Ƿ񲻿��ж���0
	p1->selfeffect_invalid=0;//����Ч������Ч���Ļغ���0
	p1->exattack_turn=0;//�ûغ��������ӵĶ��⹥������0
	p1->exattack_time=0;//�ôβ����������ӵĶ��⹥������0
	p1->whether_use_assist=-1;//��ǰû��ʹ��������

	p1->overdeath=0;//����״̬�ɳ����ֻ�����
	p1->whether_in_attack=false;//�����ڲ���״̬
	p1->whether_attack_T=false;//�Լ������ڴ���״̬
	p1->attacktime_turn=0;//�˻غ��Լ��չ��ɹ�������0
	p1->hideshield=0;////�����ʣ��غ�����0
	p1->godlaser=0;//���м���ʣ���ֻ���
	p1->godlaserdamage=0;//���м�������˺���
	p1->hurtmultiple_turn=1;//�ûغ��˺�����Ϊ1
	p1->hurtmultiple_time=1;//�ô��˺�����Ϊ1
	p1->cannot_ED=false;//�˴ι����޷���ܸ��÷�
	p1->i_soon_die=false;//�Լ�����������־���÷�

	int i,k;
	for(i=0;i<cardlimit;i++)
	{
		p1->c[i]=false;//�ֿ�ȫ�����
		p1->card[i]=-1;
		p1->h[i]=false;//��Ч�����
		p1->hideeffect[i]=-1;
	}

	for(i=0;i<abilitylimit;i++)
	{
		p1->a[i]=false;//����Ч��ȫ�����
		p1->ability[i]=-1;
	}
	
	for(i=0;i<weaponlimit;i++)
	{
		p1->w_valid[i]=true;//��������ȫ����Ϊ��Ч
		p1->w[i]=false;//����ȫ�����
		p1->weapon[i]=-1;
		p1->exattack_weapon[i]=0;//����װ���������ӵĹ�������0
		p1->exattack_weapon_turn[i]=0;//����װ�������ûغ����ӵĹ�������0
		p1->exattack_weapon_time[i]=0;//����װ�������ôβ������ӵĹ�������0
		for(k=0;k<weaponeffectlimit;k++)
		{
			p1->w_e[i][k]=false;//����װ��Ч�����
			p1->weapon_effect[i][k]=-1;
		}
		p1->weaponeffect001[i]=0;//�лʽ��������鿨������Ϊ0
		p1->weaponeffect002[i]=0;//�лʽ��������鿨������Ϊ0
		p1->weaponeffect051[i]=0;//ʳ������װ��1����Ϊ0
		p1->weaponeffect052[i]=0;//ʳ������װ��2����Ϊ0
		p1->weaponeffect06[i]=0;//6�Ŷ��շ���ħ��ָʾ�����
	}

}

void clean_buff(player* p1)
{
	p1->un_move_able=0;//�����ж��Ļغ�
	p1->unmoveable_time=false;//��ǰ�ж��Ƿ񲻿��ж���0
	p1->selfeffect_invalid=0;//����Ч������Ч���Ļغ���0
	p1->exattack_turn=0;//�ûغ��������ӵĶ��⹥������0
	p1->exattack_time=0;//�ôβ����������ӵĶ��⹥������0
	p1->whether_use_assist=-1;//��ǰû��ʹ��������
	p1->overdeath=0;//����״̬�ɳ����ֻ�����

	p1->hideshield=0;////�����ʣ��غ�����0
	p1->godlaser=0;//���м���ʣ���ֻ���
	p1->godlaserdamage=0;//���м�������˺���

	p1->cannot_ED=false;//�˴ι����޷���ܸ��÷�

}


void set_buff(client_member* c1,int situation,int number,fight_room* this_room)//����c1״̬��situationΪ״̬��numberΪ�����غ�
{
	bool whether_change=false;
	switch(situation)
	{
		case buff_clean_all://����buffȫ���
			clean_buff(c1->p1);
			whether_change=true;break;//
		case silence_turn://����Ч������Ĭ�غ�
			if(c1->p1->selfeffect_invalid>number)break;
			c1->p1->selfeffect_invalid=number;
			whether_change=true;break;//
		case hideshield_turn:
			if(c1->p1->hideshield>=number)break;
			c1->p1->hideshield=number;//����ܳ���ʱ��        
			whether_change=true;break;
		case unmove_time://�ô��ж����޷��ж�
			if(auto_judge(c1,i_will_be_unmoveable,this_room)==i_immune_effect)break;//����������޷��ж���Ч��
			c1->p1->unmoveable_time=true;
			whether_change=true;break;
		case unmove_turn://�غ����޷��ж�
			if(auto_judge(c1,i_will_be_unmoveable,this_room)==i_immune_effect)break;//����������޷��ж���Ч��
			if(c1->p1->un_move_able<number)
				c1->p1->un_move_able=number;
			whether_change=true;break;
		case overdeath_turn://����״̬
			if(c1->p1->overdeath>=number)break;
			c1->p1->overdeath=number;
			whether_change=true;break;
	}
	if(whether_change)
		buff_change(c1,this_room);
	return;
}


void fight_data_clean(player* p1,player* p2,bool clean,fight_room* this_room)//�ô��չ���������Ч��ֵ������
{
	int count;

	//p1->exattack_time=0;p2->exattack_time=0;
	attack_change(-this_room->c[0]->p1->exattack_time,&this_room->c[0]->p1->exattack_time,this_room->c[0],this_room);//�ôι�����ʱ�������
	attack_change(-this_room->c[1]->p1->exattack_time,&this_room->c[1]->p1->exattack_time,this_room->c[1],this_room);//�ôι�����ʱ�������
	
	if(clean)//�Ƿ�����ȫ����
	{p1->unmoveable_time=false;p2->unmoveable_time=false;}//�ô��ж��ڡ������ж���״̬�ָ�
	p1->whether_attack_T=false;p2->whether_attack_T=false;
	p1->hurtmultiple_time=1;p2->hurtmultiple_time=1;//�ô��չ����ӵ��˺���������Ϊ1
	p1->cannot_ED=false;p2->cannot_ED=false;//�޷��񵲺Ͷ���÷�
	for(count=0;count<weaponlimit;count++)//�����ôι�������ʱ�����������
	{
		p1->exattack_weapon_time[count]=0;p2->exattack_weapon_time[count]=0;
		p1->w_valid[count]=true;p2->w_valid[count]=true;//������Ч��
	}
	p1->whether_use_assist=-1;p2->whether_use_assist=-1;//��������˵�������
}


void show_character(player* p1,bool i)															//��ʾ����״̬
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//����������ɫ��ֻ����ʾ����ʱ�ı䣩
	cout<<"-------------------------------------------------------\n";
	cout<<"----------------------";
	if(i)
		cout<<"����";
	else 
	{
		SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | FOREGROUND_RED );//�����������ʾ
		cout<<"����";//���ݴ����bool i�б����Լ����Ƕ���
	}

	cout<<"��״̬:";
	SetConsoleTextAttribute(handle,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);////�ָ�Ĭ�ϰ�
	cout<<"----------------------\n";
	cout<<"-------------------------------------------------------\n";
    cout<<" "<<search_character(p1->character_number)<<endl;
	cout<<" Ѫ����"<<p1->blood;

	int count,count2;
	float p_attack_all=	(p1->attack)	+	(p1->exattack_turn)	+	(p1->exattack_time);
	for(count=0;count<weaponlimit;count++)//���װ�����Ƿ��мӹ�����
		if(p1->w[count]==true)
			p_attack_all=	p_attack_all	+	(p1->exattack_weapon[count])+p1->exattack_weapon_turn[count];
	cout<<"    �ܹ���"<<p_attack_all<<"	";

	if(p1->overdeath>1)cout<<p1->overdeath<<"�ֻز���֮��!";
	if(p1->overdeath==1)cout<<"�����ֻز���֮��!";

	cout<<endl<<endl;

	for(count=0;count<abilitylimit;count++)
		if(p1->a[count]==true)//���н�ɫ����Ч����ʾ�������Ч������ʾ
			cout<<"��ɫ����Ч��"<<count+1<<"��"<<search_skill(p1->ability[count])<<endl;
			cout<<endl;

	bool whether_enter=false;
	for(count=0;count<weaponlimit;count++)
		if(p1->w[count]==true)
			{cout<<"����"<<count+1<<": "<<search_card(p_name,p1->weapon[count]);
		     if(p1->weapon[count]==6)
				 cout<<"(����+"<<p1->weaponeffect06[count]<<")"<<"  ";
			 else
				 cout<<"(��+"<<p1->exattack_weapon[count]+p1->exattack_weapon_turn[count]<<")"<<"  ";
			 whether_enter=true;}
	if(whether_enter)cout<<endl;
	for(count=0;count<weaponlimit;count++)
		{
		if(p1->w[count]==true)
		{
			cout<<"����"<<count+1;
			if(p1->weapon[count]==10){cout<<"\t ������Ч��\n";continue;}
			for(count2=0;count2<weaponeffectlimit;count2++)
				{
				if(p1->w_e[count][count2]==true)//���н�ɫװ��������Ч����ʾ�������Ч������ʾ
					{cout<<"\t"<<" Ч��"<<count2+1<<"��"<<search_skill(p1->weapon_effect[count][count2])<<endl;}
				else 
					break;
				}
		}
		}

	if(!i)//ͳ�ƶ����ж����ֿ�
	{
		for(count=0;count<cardlimit;count++)
			if(p1->c[count]!=true)
				break;
		cout<<"��Ŀǰ��"<<count<<"���ֿ�\n";
	}

}




//����������������������������������������������������������Ҹ�����Ϣ����������������������������������������������
#endif