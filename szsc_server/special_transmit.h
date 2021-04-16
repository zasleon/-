
#ifndef INCLUDE_special_transmit_H
#define INCLUDE_special_transmit_H

#include "player_general_function.h"






//���ݴ���ר�Ż�����ͬ��������ϵ�ͻ��ˣ����²�ͬ������
void blood_change(float number,client_member* c1,fight_room* this_room)//Ѫ�������仯,����Ѫ������,c1->p1->blood+number
{
	if(c1->p1->blood+number<c1->p1->bloodlimit	&&	c1->p1->blood+number>0)//���Ѫ���䶯û������ֵ
		c1->p1->blood	=	c1->p1->blood	+	number	;
	else
	{
		if(c1->p1->blood+number>=c1->p1->bloodlimit)	//������ֵ�����ж��ǳ������޻�������
			c1->p1->blood	=	c1->p1->bloodlimit	;	//����ΪѪ������
		else	
			c1->p1->blood	=	0	;//����Ϊ0-
	}
	
	char StrBuf[MAX_PATH] = { 0 };
		
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_blood_M);//���߿ͻ����Լ���ʼ��������������
	special_signal(c2,refresh_state_blood_H);//���߿ͻ����Լ���ʼ���Ͷ�������������
	transmit_blood(c1,c1->p1);//�����Լ�Ѫ����Ϣ
	transmit_blood(c2,c1->p1);//�����������֪�����ж���Ѫ��
}
void transmit_blood(client_member* c1,player* p1)
{
	sp_transmit(c1,p1->blood);
}






void buff_change(client_member* c1,fight_room* this_room)//Ŀǰ��:�����\���м���\����״̬
{
	char StrBuf[MAX_PATH] = { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_buff_M);//���߿ͻ����Լ���ʼ����buff�����
	special_signal(c2,refresh_state_buff_H);//���߿ͻ����Լ���ʼ���Ͷ���buff�����
	transmit_buff(c1,c1->p1);//�����Լ�buff���
	transmit_buff(c2,c1->p1);//�����������֪����buff���
}
void transmit_buff(client_member* c1,player* p1)
{
	sp_transmit(c1,p1->hideshield);//�����ʣ��غ���
	sp_transmit(c1,p1->overdeath);//��������״̬���ֻ���(���׽�)
	//sp_transmit(c1,p1->godlaser);//���м���ʣ���ֻ���
	//sp_transmit(c1,p1->un_move_able);//�����ж��Ļغ�
}








void weapon_change(client_member* c1,fight_room* this_room)//����ҵ�������Ϣ���˱仯
{
	char StrBuf[MAX_PATH] = { 0 };
		
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_weapon_M);//���߿ͻ����Լ���ʼ�������������
	special_signal(c2,refresh_state_weapon_H);//���߿ͻ����Լ���ʼ���Ͷ������������
	transmit_weapon(c1,c1->p1);//�����Լ����������
	transmit_weapon(c2,c1->p1);//�����������֪�����������
}
void transmit_weapon(client_member* c1,player* p1)
{
	int count,count2;
	for(count=0;count<weaponlimit;count++)
	{
		sp_transmit(c1,p1->w_valid[count]);
		sp_transmit(c1,p1->w[count]);
		sp_transmit(c1,p1->weapon[count]);  
		sp_transmit(c1,p1->exattack_weapon[count]+p1->exattack_weapon_turn[count]+p1->exattack_weapon_time[count]);//+�ڼ������������ӵ��ܹ�����

		for(count2=0;count2<weaponeffectlimit;count2++)
		{
			//sp_transmit(c1,p1->w_e[count][count2]);
			sp_transmit(c1,p1->weapon_effect[count][count2]);			
		}
		sp_transmit(c1,p1->weaponeffect06[count]);//6�Ŷ��շ���ħ��ָʾ�����
	}

}




void attack_change(float number,float* ex_attack,client_member* c1,fight_room* this_room)//�������仯����c1��ex_attack��ַ�޸�����numberֵ
{
	char StrBuf[MAX_PATH] = { 0 };
	
	*ex_attack	=	*ex_attack	+	number;//ex_attackΪ��������ַ�������Ǵ˴��չ��������ӵĹ����������������ù������������Ǵ˻غ����ӵĹ�����

	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_attack_M);//���߿ͻ����Լ���ʼ��������������
	special_signal(c2,refresh_state_attack_H);//���߿ͻ����Լ���ʼ���Ͷ�������������
	transmit_attack(c1,c1->p1);//�����Լ�������
	transmit_attack(c2,c1->p1);//�����������֪�����ж��ٹ���
}

void transmit_attack(client_member* c1,player* p1)
{
	sp_transmit(c1,	p1->attack	+	p1->exattack_time	+	p1->exattack_turn);//ֻ��ʾ�ܹ��ˣ����浽�ͻ��˵�p1->attack��
}










void card_change(client_member* c1,fight_room* this_room)//���������ֿ�,c1�ֿ��䶯�����Բ���c2�ģ�������c2�Լ��ֿ�������Ϣ
{
	refresh_hideeffect(c1);
	/*int count;
	system("cls");
	show_character(this_room->c1->p1,true);
	for(count=0;count<10&&this_room->c1->p1->c[count];count++)
		cout<<search_card_name(this_room->c1->p1->card[count])<<endl;
	show_character(this_room->c2->p1,false);
	for(count=0;count<10&&this_room->c2->p1->c[count];count++)
		cout<<search_card_name(this_room->c2->p1->card[count])<<endl;
*/
	char StrBuf[MAX_PATH] = { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_CM);//���߿ͻ����Լ���ʼ��������������
	special_signal(c2,refresh_state_CH);//���߿ͻ����Լ���ʼ���Ͷ�������������
	transmit_card(c1,c1->p1,true);//������Լ����Լ��ֿ�ȫ����Ϣ
	transmit_card(c2,c1->p1,false);//�����������֪�����ж����ֿ�
}

void transmit_card(client_member* c1,player* p1,bool himself)//����p1�����ɫ����c1����������Լ�himself�����ֿ�������Ϣ
{
	int count,count2;


	for(count=0;count<cardlimit;count++)
		if(!p1->c[count])break;
	sp_transmit(c1,count);//�������ж����ſ�
		
	if(himself)//����Ǹ����Լ��ֿ���Ϣ�����;����ֿ���Ϣ
	{
		for(count2=0;count2<count;count2++)
			sp_transmit(c1,p1->card[count2]);//ӵ���ֿ� 
	}
	return;

	
}





void refresh_character_state_F(client_member* c1,fight_room* this_room)//�״μ�¼����״̬
{
	char StrBuf[MAX_PATH] = { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_first_state);//���߿ͻ����Լ���ʼ��������������
	special_signal(c2,refresh_first_state);//���߿ͻ����Լ���ʼ��������������
	transmit_this_character_F(c1,c1->p1);//������Լ����Լ���Ϣ
	transmit_this_character_F(c2,c2->p1);//��������֣������Լ���Ϣ
	transmit_this_character_F(c1,c2->p1);//������Լ���������Ϣ
	transmit_this_character_F(c2,c1->p1);//��������֣��Լ�����Ϣ
}
void transmit_this_character_F(client_member* c1,player* p1)//�״δ������,����������ţ�����Ч��
{
	char StrBuf[MAX_PATH] = { 0 };
	int count;
	int result=still_fight;
	sp_transmit(c1,p1->character_number);//��ɫ���
	sp_transmit(c1,p1->player_No);//�ڼ������
	sp_transmit(c1,p1->blood);//��ɫѪ��
	sp_transmit(c1,p1->attack);//������
	for(count=0;count<abilitylimit;count++)
	{
		sp_transmit(c1,p1->a[count]);
		sp_transmit(c1,p1->ability[count]);//������������ĸ�
	}
	
}


void sp_transmit(client_member* c1,float message)//�ػ������������
{
	if(c1->member_No==robot_symbol)return;//���ø������˴�������
	Sleep(10);
	char StrBuf[MAX_PATH] = { 0 };

	if(c1->device==android_phone)//��׿������float���ʹ��䣬���Ϊ�������ֺͷ������ִ���
	{
		int d,f;
		d=int(floor(message));
		f=int((message-floor(message))*100);
		addtext(StrBuf,d);
		addtext(StrBuf,"\n");
		send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//�����������
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,fast_get_client_message(c1));//recv(g_ClientSocket[c1->member_No-1],StrBuf, MAX_PATH, 0);
		
		memset(StrBuf,0,sizeof(StrBuf));
		Sleep(sleeptime);//��ֹ�������
		addtext(StrBuf,f);
		addtext(StrBuf,"\n");
		send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//���С������
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,fast_get_client_message(c1));//recv(g_ClientSocket[c1->member_No-1],StrBuf, MAX_PATH, 0);
		return;
		//unsigned char *buf = (unsigned char*)&message;//floatתbyte
		//strcat(StrBuf,static_cast<const char*>( static_cast<void*>( const_cast<unsigned char*>(buf))));
	}
	addtext(StrBuf,message);
	send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);
	memset(StrBuf,0,sizeof(StrBuf));
	addtext(StrBuf,fast_get_client_message(c1));
	//recv(g_ClientSocket[c1->member_No-1],StrBuf, MAX_PATH, 0);
	return ;
}
void sp_transmit(client_member* c1,bool message)//�ػ������������
{
	if(c1->member_No==robot_symbol)return;//���ø������˴�������
	Sleep(10);
	char StrBuf[MAX_PATH] = { 0 };
	if(c1->device==android_phone)//��׿�豸ò�ƴ���boolֵ�����ѣ���Ϊ�����͵�1��0
	{
		if(message)
			addtext(StrBuf,1);
		else
			addtext(StrBuf,0);
		addtext(StrBuf,"\n");
		send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,fast_get_client_message(c1));
		return;
	}
	addtext(StrBuf,message);
	
	send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);
	memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,fast_get_client_message(c1));
	return ;
}
void sp_transmit(client_member* c1,int message)//�ػ������������
{
	if(c1->member_No==robot_symbol)return;//���ø������˴�������
	Sleep(10);
	char StrBuf[MAX_PATH] = { 0 };
	addtext(StrBuf,message);
	if(c1->device==android_phone)addtext(StrBuf,"\n");
	send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);
	memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,fast_get_client_message(c1));
	return ;
}




#endif