
#ifndef INCLUDE_special_transmit_H
#define INCLUDE_special_transmit_H

#include "player_general_function.h"






//数据传输专门化，不同操作后，联系客户端，更新不同的数据
void blood_change(float number,client_member* c1,fight_room* this_room)//血量发生变化,进行血量更新,c1->p1->blood+number
{
	if(c1->p1->blood+number<c1->p1->bloodlimit	&&	c1->p1->blood+number>0)//如果血量变动没超过阈值
		c1->p1->blood	=	c1->p1->blood	+	number	;
	else
	{
		if(c1->p1->blood+number>=c1->p1->bloodlimit)	//超过阈值，则判定是超过上限还是下限
			c1->p1->blood	=	c1->p1->bloodlimit	;	//上限为血量上限
		else	
			c1->p1->blood	=	0	;//下限为0-
	}
	
	char StrBuf[MAX_PATH] = { 0 };
		
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_blood_M);//告诉客户端自己开始发送人物数据了
	special_signal(c2,refresh_state_blood_H);//告诉客户端自己开始发送对手人物数据了
	transmit_blood(c1,c1->p1);//传输自己血量信息
	transmit_blood(c2,c1->p1);//传输对手让他知道我有多少血量
}
void transmit_blood(client_member* c1,player* p1)
{
	sp_transmit(c1,p1->blood);
}






void buff_change(client_member* c1,fight_room* this_room)//目前有:隐天盾\神尚激光\不死状态
{
	char StrBuf[MAX_PATH] = { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_buff_M);//告诉客户端自己开始发送buff情况了
	special_signal(c2,refresh_state_buff_H);//告诉客户端自己开始发送对手buff情况了
	transmit_buff(c1,c1->p1);//传输自己buff情况
	transmit_buff(c2,c1->p1);//传输对手让他知道我buff情况
}
void transmit_buff(client_member* c1,player* p1)
{
	sp_transmit(c1,p1->hideshield);//隐天盾剩余回合数
	sp_transmit(c1,p1->overdeath);//持续不死状态的轮回数(黑曜剑)
	//sp_transmit(c1,p1->godlaser);//神尚激光剩余轮回数
	//sp_transmit(c1,p1->un_move_able);//不可行动的回合
}








void weapon_change(client_member* c1,fight_room* this_room)//该玩家的武器信息有了变化
{
	char StrBuf[MAX_PATH] = { 0 };
		
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_weapon_M);//告诉客户端自己开始发送武器情况了
	special_signal(c2,refresh_state_weapon_H);//告诉客户端自己开始发送对手武器情况了
	transmit_weapon(c1,c1->p1);//传输自己武器情况！
	transmit_weapon(c2,c1->p1);//传输对手让他知道我武器情况
}
void transmit_weapon(client_member* c1,player* p1)
{
	int count,count2;
	for(count=0;count<weaponlimit;count++)
	{
		sp_transmit(c1,p1->w_valid[count]);
		sp_transmit(c1,p1->w[count]);
		sp_transmit(c1,p1->weapon[count]);  
		sp_transmit(c1,p1->exattack_weapon[count]+p1->exattack_weapon_turn[count]+p1->exattack_weapon_time[count]);//+第几格武器槽增加的总攻击力

		for(count2=0;count2<weaponeffectlimit;count2++)
		{
			//sp_transmit(c1,p1->w_e[count][count2]);
			sp_transmit(c1,p1->weapon_effect[count][count2]);			
		}
		sp_transmit(c1,p1->weaponeffect06[count]);//6号恩空法棒魔力指示物个数
	}

}




void attack_change(float number,float* ex_attack,client_member* c1,fight_room* this_room)//攻击力变化，给c1在ex_attack地址修改增加number值
{
	char StrBuf[MAX_PATH] = { 0 };
	
	*ex_attack	=	*ex_attack	+	number;//ex_attack为攻击力地址，可能是此次普攻额外增加的攻击力，可能是永久攻击力，可能是此回合增加的攻击力

	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_state_attack_M);//告诉客户端自己开始发送人物数据了
	special_signal(c2,refresh_state_attack_H);//告诉客户端自己开始发送对手人物数据了
	transmit_attack(c1,c1->p1);//传输自己攻击力
	transmit_attack(c2,c1->p1);//传输对手让他知道我有多少攻击
}

void transmit_attack(client_member* c1,player* p1)
{
	sp_transmit(c1,	p1->attack	+	p1->exattack_time	+	p1->exattack_turn);//只显示总攻了，保存到客户端的p1->attack上
}










void card_change(client_member* c1,fight_room* this_room)//更新人物手卡,c1手卡变动，所以不读c2的，但告诉c2自己手卡数量信息
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

	special_signal(c1,refresh_state_CM);//告诉客户端自己开始发送人物数据了
	special_signal(c2,refresh_state_CH);//告诉客户端自己开始发送对手人物数据了
	transmit_card(c1,c1->p1,true);//传输给自己，自己手卡全部信息
	transmit_card(c2,c1->p1,false);//传输对手让他知道我有多少手卡
}

void transmit_card(client_member* c1,player* p1,bool himself)//传输p1这个角色传给c1，如果是他自己himself则传输手卡具体信息
{
	int count,count2;


	for(count=0;count<cardlimit;count++)
		if(!p1->c[count])break;
	sp_transmit(c1,count);//告诉他有多少张卡
		
	if(himself)//如果是更新自己手卡信息，则发送具体手卡信息
	{
		for(count2=0;count2<count;count2++)
			sp_transmit(c1,p1->card[count2]);//拥有手卡 
	}
	return;

	
}





void refresh_character_state_F(client_member* c1,fight_room* this_room)//首次记录人物状态
{
	char StrBuf[MAX_PATH] = { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	special_signal(c1,refresh_first_state);//告诉客户端自己开始发送人物数据了
	special_signal(c2,refresh_first_state);//告诉客户端自己开始发送人物数据了
	transmit_this_character_F(c1,c1->p1);//传输给自己，自己信息
	transmit_this_character_F(c2,c2->p1);//传输给对手，对手自己信息
	transmit_this_character_F(c1,c2->p1);//传输给自己，对手信息
	transmit_this_character_F(c2,c1->p1);//传输给对手，自己的信息
}
void transmit_this_character_F(client_member* c1,player* p1)//首次传输给他,传送人物序号，个人效果
{
	char StrBuf[MAX_PATH] = { 0 };
	int count;
	int result=still_fight;
	sp_transmit(c1,p1->character_number);//角色序号
	sp_transmit(c1,p1->player_No);//第几个玩家
	sp_transmit(c1,p1->blood);//角色血量
	sp_transmit(c1,p1->attack);//攻击力
	for(count=0;count<abilitylimit;count++)
	{
		sp_transmit(c1,p1->a[count]);
		sp_transmit(c1,p1->ability[count]);//自身能力最多四个
	}
	
}


void sp_transmit(client_member* c1,float message)//特化输出人物数据
{
	if(c1->member_No==robot_symbol)return;//不用给机器人传输数据
	Sleep(10);
	char StrBuf[MAX_PATH] = { 0 };

	if(c1->device==android_phone)//安卓不兼容float类型传输，拆分为整数部分和分数部分传输
	{
		int d,f;
		d=int(floor(message));
		f=int((message-floor(message))*100);
		addtext(StrBuf,d);
		addtext(StrBuf,"\n");
		send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//输出整数部分
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,fast_get_client_message(c1));//recv(g_ClientSocket[c1->member_No-1],StrBuf, MAX_PATH, 0);
		
		memset(StrBuf,0,sizeof(StrBuf));
		Sleep(sleeptime);//防止输出过快
		addtext(StrBuf,f);
		addtext(StrBuf,"\n");
		send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//输出小数部分
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,fast_get_client_message(c1));//recv(g_ClientSocket[c1->member_No-1],StrBuf, MAX_PATH, 0);
		return;
		//unsigned char *buf = (unsigned char*)&message;//float转byte
		//strcat(StrBuf,static_cast<const char*>( static_cast<void*>( const_cast<unsigned char*>(buf))));
	}
	addtext(StrBuf,message);
	send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);
	memset(StrBuf,0,sizeof(StrBuf));
	addtext(StrBuf,fast_get_client_message(c1));
	//recv(g_ClientSocket[c1->member_No-1],StrBuf, MAX_PATH, 0);
	return ;
}
void sp_transmit(client_member* c1,bool message)//特化输出人物数据
{
	if(c1->member_No==robot_symbol)return;//不用给机器人传输数据
	Sleep(10);
	char StrBuf[MAX_PATH] = { 0 };
	if(c1->device==android_phone)//安卓设备貌似处理bool值有困难，改为传整型的1或0
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
void sp_transmit(client_member* c1,int message)//特化输出人物数据
{
	if(c1->member_No==robot_symbol)return;//不用给机器人传输数据
	Sleep(10);
	char StrBuf[MAX_PATH] = { 0 };
	addtext(StrBuf,message);
	if(c1->device==android_phone)addtext(StrBuf,"\n");
	send(g_ClientSocket[c1->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);
	memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,fast_get_client_message(c1));
	return ;
}




#endif