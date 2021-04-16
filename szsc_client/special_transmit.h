#ifndef INCLUDE_special_transmit_H
#define INCLUDE_special_transmit_H

#include "player_general_function.h"

void recv_server_transmit_buff(bool myself)
{
	if(myself)
	{
		set_property(&c1->hideshield);
		set_property(&c1->overdeath);
		//set_property(&c1->godlaser);
		//set_property(&c1->un_move_able);
	}
	else
	{
		set_property(&c2->hideshield);
		set_property(&c2->overdeath);
	}
}












void recv_server_transmit_W(bool myself)
{
	int count,count2;

	if(myself)
	{
		for(count=0;count<weaponlimit;count++)
		{
			set_property(&c1->w_valid[count]);//������ʱ����Ч��Ч
			set_property(&c1->w[count]);   					
			set_property(&c1->weapon[count]);//װ�����ڷֱ���ʲô�������������ű���
							
			set_property(&c1->exattack_weapon[count]);//�ڼ������������ӵĹ�����

			for(count2=0;count2<weaponeffectlimit;count2++)
			{
				//set_property(&c1->w_e[count][count2]);
				set_property(&c1->weapon_effect[count][count2]);//װ��������ӵ�е�Ч��
			} 

			set_property(&c1->weaponeffect06[count]);//6�Ŷ��շ���ħ��ָʾ�����
		}
	}
	else
	{
		for(count=0;count<weaponlimit;count++)
		{
			set_property(&c2->w_valid[count]);//������ʱ����Ч��Ч
			set_property(&c2->w[count]);   					
			set_property(&c2->weapon[count]);//װ�����ڷֱ���ʲô�������������ű���
											
			set_property(&c2->exattack_weapon[count]);//�ڼ������������ӵĹ�����
			for(count2=0;count2<weaponeffectlimit;count2++)
			{
				//set_property(&c1->w_e[count][count2]);
				set_property(&c2->weapon_effect[count][count2]);//װ��������ӵ�е�Ч��
			} 

			set_property(&c2->weaponeffect06[count]);//6�Ŷ��շ���ħ��ָʾ�����
		}
	}

}

void recv_server_transmit_A(bool myself)//���¹�������Ϣ
{
	if(myself)
		set_property(&c1->attack);
	else
		set_property(&c2->attack);

}
void recv_server_transmit_B(bool myself)//����Ѫ����Ϣ
{
	if(myself)
		set_property(&c1->blood);
	else
		set_property(&c2->blood);

}
void recv_server_transmit_C(bool myself)//ר�Ž����ֿ�����
{
	int count,cardnumber;

	memset(getmessage,0,sizeof(getmessage));
	if(0>=recv(Client, getmessage, MAX_PATH, 0))return;
	cardnumber=atoi(getmessage);//��֪�ж������ֿ�
	
	if(!myself)
	{
		for(count=0;count<cardlimit;count++)
			c2->c[count]=false;
		for(count=0;count<cardnumber;count++)
			c2->c[count]=true;
		
		Sleep(50);
		send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
		return;
	}
	if(myself)//������Լ�������������Լ��ֿ�������Ϣ
	{
		for(count=0;count<cardlimit;count++)
			c1->c[count]=false;
		Sleep(50);
		send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
		for(count=0;count<cardnumber;count++)
		{
			c1->c[count]=true;
			set_property(&c1->card[count]);//ӵ���ֿ� 
		}
	}

}


void recv_server_transmit_F()//�״ν�����������
{
	int count;
	set_property(&c1->character_number);
	set_property(&c1->player_No);
	set_property(&c1->blood);//��ǰѪ��
	set_property(&c1->attack);
	for(count=0;count<abilitylimit;count++)
	{
		set_property(&c1->a[count]);
		set_property(&c1->ability[count]);//������������ĸ�
	}
	//���ն�����Ϣ(�������ֿ���Ϣ)
	set_property(&c2->character_number);
	set_property(&c2->player_No);
	set_property(&c2->blood);//��ǰѪ��
	set_property(&c2->attack);
	for(count=0;count<abilitylimit;count++)
	{
		set_property(&c2->a[count]);					
		set_property(&c2->ability[count]);//������������ĸ�
	}

}




#endif