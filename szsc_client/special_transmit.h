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
			set_property(&c1->w_valid[count]);//武器暂时性有效无效
			set_property(&c1->w[count]);   					
			set_property(&c1->weapon[count]);//装备槽内分别是什么武器，以武器号表明
							
			set_property(&c1->exattack_weapon[count]);//第几格武器槽增加的攻击力

			for(count2=0;count2<weaponeffectlimit;count2++)
			{
				//set_property(&c1->w_e[count][count2]);
				set_property(&c1->weapon_effect[count][count2]);//装备武器后拥有的效果
			} 

			set_property(&c1->weaponeffect06[count]);//6号恩空法棒魔力指示物个数
		}
	}
	else
	{
		for(count=0;count<weaponlimit;count++)
		{
			set_property(&c2->w_valid[count]);//武器暂时性有效无效
			set_property(&c2->w[count]);   					
			set_property(&c2->weapon[count]);//装备槽内分别是什么武器，以武器号表明
											
			set_property(&c2->exattack_weapon[count]);//第几格武器槽增加的攻击力
			for(count2=0;count2<weaponeffectlimit;count2++)
			{
				//set_property(&c1->w_e[count][count2]);
				set_property(&c2->weapon_effect[count][count2]);//装备武器后拥有的效果
			} 

			set_property(&c2->weaponeffect06[count]);//6号恩空法棒魔力指示物个数
		}
	}

}

void recv_server_transmit_A(bool myself)//更新攻击力信息
{
	if(myself)
		set_property(&c1->attack);
	else
		set_property(&c2->attack);

}
void recv_server_transmit_B(bool myself)//更新血量信息
{
	if(myself)
		set_property(&c1->blood);
	else
		set_property(&c2->blood);

}
void recv_server_transmit_C(bool myself)//专门接收手卡数据
{
	int count,cardnumber;

	memset(getmessage,0,sizeof(getmessage));
	if(0>=recv(Client, getmessage, MAX_PATH, 0))return;
	cardnumber=atoi(getmessage);//得知有多少张手卡
	
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
	if(myself)//如果是自己则接下来接收自己手卡具体信息
	{
		for(count=0;count<cardlimit;count++)
			c1->c[count]=false;
		Sleep(50);
		send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
		for(count=0;count<cardnumber;count++)
		{
			c1->c[count]=true;
			set_property(&c1->card[count]);//拥有手卡 
		}
	}

}


void recv_server_transmit_F()//首次接收人物数据
{
	int count;
	set_property(&c1->character_number);
	set_property(&c1->player_No);
	set_property(&c1->blood);//当前血量
	set_property(&c1->attack);
	for(count=0;count<abilitylimit;count++)
	{
		set_property(&c1->a[count]);
		set_property(&c1->ability[count]);//自身能力最多四个
	}
	//接收对手信息(不接收手卡信息)
	set_property(&c2->character_number);
	set_property(&c2->player_No);
	set_property(&c2->blood);//当前血量
	set_property(&c2->attack);
	for(count=0;count<abilitylimit;count++)
	{
		set_property(&c2->a[count]);					
		set_property(&c2->ability[count]);//自身能力最多四个
	}

}




#endif