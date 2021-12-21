#ifndef INCLUDE_player_choose_H
#define INCLUDE_player_choose_H



#include "attack_fight_back.h"

void lock_other(client_member* c1,fight_room* this_room)//停止c1之外所有人的自由行动
{
	int count;
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(!check_robot(c2))//如果不是机器人，上锁
		SuspendThread(hthread[c2->member_No-1]);
	special_signal(c2,event_happen);//事件发生
	special_signal(c1,your_event);

	
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL)this_room->c[count]->be_locked=true;
}
void lock_other_N(client_member* c1,fight_room* this_room)//停止c1之外所有人的自由行动,不显示“对面正在思考……”
{
	int count;
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(!check_robot(c2))//如果不是机器人，上锁
		SuspendThread(hthread[c2->member_No-1]);
	special_signal(c2,event_happen_N);//事件发生
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
	room_broadcast(this_room,"事件结束!你可以继续行动了!\n");
	if(c2->p1->un_move_able==0&&result!=force_end_turn)
	{
		if(c2->device==win_console)
			special_signal(c2,battle_choice);//没被封锁行动，显示行动界面
		
	}
	special_signal(c2,event_end);//事件结束
	if(!check_robot(c2))//如果不是机器人，解放
		ResumeThread(hthread[c2->member_No-1]);//启动线程
	for(count=0;count<playernumber;count++)
		if(this_room->c[count]!=NULL)this_room->c[count]->be_locked=false;

}
void release_other_N(client_member* c1,fight_room* this_room)//事件结束,不显示行动界面
{
	int count;
	client_member* c2;
	if(c1->member_No-1==this_room->c[0]->member_No-1)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	special_signal(c2,event_end);//事件结束
	if(!check_robot(c2))//如果不是机器人，解放
		ResumeThread(hthread[c2->member_No-1]);//启动线程
	
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

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"准备发动手卡!\n ");//玩家姓名
	room_broadcast(this_room,StrBuf);

	if(c1->device==win_console)
		confirm_send_success(c1,"请选择手卡进行发动!(0代表‘取消该操作’)\n你选择:");
	else
		special_signal(c1,get_c_number);//图形化客户端直接获取卡号

	while(true)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯
		
		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			room_broadcast(this_room,"他选择取消发动手卡!\n");
			return still_fight;
		}
		choice=atoi(StrBuf);//化字符串为数字
		if(choice==0){confirm_send_success(c1,"输入无效字符!请重新选择手卡:");continue;}
		choice--;
		if(choice>=cardlimit||choice<0){confirm_send_success(c1,"输入超出手卡上/下限！请重新选择手卡:");continue;}
		if(c1->p1->c[choice]==false){confirm_send_success(c1,"抱歉，该手卡槽为空！请重新选择手卡:");continue;}

		memset(StrBuf,0,sizeof(StrBuf));
		special_signal(c1,whether_launch_hideeffect);//询问是否发动隐效果

		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

		switch(atoi(StrBuf))
		{
			case  1:effect_cardnumber=c1->p1->card[choice];whether_hide_effect=false;break;
			case  0:effect_cardnumber=c1->p1->hideeffect[choice];whether_hide_effect=true;break;
		}
		if(judge_weapon(effect_cardnumber))//输入该卡号，判断该手卡是否为装备卡，是 则装备
		{
			if(!c1->p1->whether_myturn)//检查是否为该玩家回合
			{
				confirm_send_success(c1,"现在不是你的回合!无法装备武器!\n");
				room_broadcast(this_room,"他选择取消发动手卡!\n");
				return still_fight;
			}
			if(c1->p1->w[weaponlimit-1])//检查武器数量，若装备武器已达上限则直接返回
			{
				confirm_send_success(c1,"装备武器已达上限!无法继续装备!\n");
				room_broadcast(this_room,"他选择取消发动手卡!\n");
				return still_fight;
			}

			effect_cardnumber=c1->p1->card[choice];//存储卡号

			result=use_card(c1,choice,false,&locktime,this_room);
			if(result==i_failed_effect)return still_fight;
			
			weapon_arm(c1,effect_cardnumber,this_room);//进行装备

			confirm_send_success(c1,"你装备成功!\n");memset(StrBuf,0,sizeof(StrBuf));
			return result;
		}//输入正常，直接结束函数使用，退出死循环
		
		if(judge_card(c1,effect_cardnumber,my_turn,&locktime))//输入该卡号，判断是否符合发动条件
		{	
			result=use_card(c1,choice,whether_hide_effect,&locktime,this_room);//使用了该槽手卡
			if(result==i_failed_effect)return still_fight;
			if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)return result;
			result=launch_card_effect(c1,NULL,effect_cardnumber,my_turn,&locktime,this_room);//发动卡片效果，返回一个结果
			if(result==i_cancel_effect&&effect_cardnumber!=75){confirm_send_success(c1,"效果错误!本该无法取消!\n");return still_fight;}//检测效果有无发动错误
			return result;
		}
		else//该卡无法发动
			if(c1->device==win_console)
				confirm_send_success(c1,"该手卡发动条件不满足!请重新选择手卡:");
			else
			{
				confirm_send_success(c1,"该卡不满足发动条件!\n");
				room_broadcast(this_room,"他取消发动手卡!\n");
				return still_fight;
			}
	}
}

int player_use_card(client_member* c1,fight_room* this_room)//自己主动使用手卡
{
	char StrBuf[MAX_PATH] = { 0 };
	int result;

	if(c1->p1->c[0]==false){confirm_send_success(c1,"你当前没有手卡!\n");return still_fight;}

	lock_other(c1,this_room);//事件发生

	result=use_card_process(c1,this_room);
	release_other(c1,result,this_room);
	return result;
}

//返回值为still_fight则表示没人死去
int player_attack(client_member* c1,fight_room* this_room)//玩家c行动选择普攻
{
	//int choice;
	int result=still_fight;
	int locktime=0;//连锁次数初始为0
    char StrBuf[MAX_PATH] = { 0 };
	
	
	if(!c1->p1->whether_myturn)//检查是否为该玩家回合
	{confirm_send_success(c1,"现在不是你的回合!你没有普攻机会!\n");return still_fight;}
	if(c1->p1->fight_chance==0)//确认是该玩家回合，检查该玩家在该回合剩余的普攻次数
	{confirm_send_success(c1,"该回合你的普攻次数已消耗完!\n");return still_fight;}
	
	lock_other(c1,this_room);//挂起线程//event_happen=事件发生标志符
/*	
	//strcpy(StrBuf,"\n");
	sprintf(Buf,"%d",c1->p1->player_No);strcat(StrBuf,Buf);strcat(StrBuf,"号玩家:");
	strcat(StrBuf,search_character_name(c1->p1->character_number));strcat(StrBuf,"发动了普攻！\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));memset(Buf,0,sizeof(Buf));



	choice=choose_enemy(c1,this_room);//选择普攻对象
	if(choice==force_offline)
	{		release_other(c2,result,this_room);return force_offline;	}

	if(choice==i_cancel_effect)//如果选择取消普攻，直接返回
	{
		room_broadcast(this_room,"他取消了普攻！\n");
		confirm_send_success(c1,"即将返回主页面…………\n");
		release_other(c2,result,this_room);
		return still_fight;
	}
*/	
		client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	
	result=general_attack(c1,c2,myturn_attack,&locktime,this_room);//普攻宣言发动，进入通用普攻事件
	release_other(c1,result,this_room);//事件结束,解放其他人行动
	return result;
}

void delete_weapon_process(client_member* c1,fight_room* this_room)
{
	int count;
	int choice;
	char StrBuf[MAX_PATH] = { 0 };

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"开始丢弃武器！\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));

	if(c1->device==win_console)
	{
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true)
			{
				addtext(StrBuf,"装备");
				addtext(StrBuf,count+1);
				addtext(StrBuf,": ");
				addtext(StrBuf,search_card(p_name,c1->p1->weapon[count]));
				addtext(StrBuf,"  ");
				confirm_send_success(c1,StrBuf);
				memset(StrBuf,0,sizeof(StrBuf));
			}
			
		confirm_send_success(c1,"\n清除第几个装备？(0代表‘不清除’)(1或2表示‘清除第几个卡槽装备’):");
	}
	else
		special_signal(c1,delete_which_weapon);

	while(true)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return ;}//玩家做出选择，如果此时断开通讯

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			confirm_send_success(c1,"他选择了取消丢弃武器!\n");	
			return ;
		}
		
		choice=atoi(StrBuf);//化字符串为数字
		if(choice==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
		choice--;
		if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
		if(c1->p1->w[choice]==false){confirm_send_success(c1,"该武器槽为空!请重新输入:");continue;}

		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,search_card(p_name,c1->p1->weapon[choice]));
		addtext(StrBuf,"被清除!\n");
		room_broadcast(this_room,StrBuf);
		delete_weapon(c1,choice,this_room);
		return;
	}
}
void player_delete_weapon(client_member* c1,fight_room* this_room) //丢弃已装备的武器
{
	if(c1->p1->w[0]==false){confirm_send_success(c1,"当前没有装备武器!\n");return;}
	if(!c1->p1->whether_myturn)//检查是否为该玩家回合
	{confirm_send_success(c1,"现在不是你的回合!你无法丢弃武器!\n");return ;}

	lock_other(c1,this_room);//事件开始
	delete_weapon_process(c1,this_room);
	release_other(c1,still_fight,this_room);//事件结束,解放其他人行动
	return;
}




int weapon_effect_process(client_member* c1,fight_room* this_room)
{
	int result=still_fight;
	int choice,choice2;
	int locktime=0;
	char StrBuf[MAX_PATH] = { 0 };
	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"开始发动武器效果！\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));

	show_him_his_weapon_effect(c1);//显示武器总览

	while(true)
	{
		//选择发动哪个武器
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			room_broadcast(this_room,"他选择了取消发动武器效果!\n");
			return still_fight;
		}

		choice=atoi(StrBuf);//化字符串为数字
		if(choice==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
		choice--;
		cout<<choice<<endl;
		if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"武器输入超出上下限！请重新输入:");continue;}
		if(c1->p1->w[choice]==false){confirm_send_success(c1,"该武器槽为空！请重新输入:");continue;}

		if(c1->device==win_console)
			confirm_send_success(c1,"你想发动该武器的效果几?(输入0表示‘取消该操作’):");
		else
			special_signal(c1,get_we_number);//获取图像界面客户端武器第几个效果
	    
		while(true)
		{
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

			if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动该武器的效果!\n即将返回上一选项页面……\n请输入想发动第几个武器(输入0表示‘取消该操作’):");break;}
		
			choice2=atoi(StrBuf);//化字符串为数字
			if(choice2==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
			choice2--;
			if(choice2<0||choice2>=weaponeffectlimit||c1->p1->w_e[choice][choice2]==false){confirm_send_success(c1,"输入超出上下限 或 武器该效果槽为空！请重新输入:");continue;}
			//发动武器效果的时候自动判定是否可发动
			//客户，武器号，条件：“我的回合”，连锁次数，哪个武器，这个房间
			if(!judge_weapon_effect(c1, c1->p1->weapon_effect[choice][choice2],my_turn,&locktime,choice,this_room))
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"发动条件不满足!请重新选择:");
				else
				{
					confirm_send_success(c1,"发动条件不满足!\n");
					room_broadcast(this_room,"他选择了取消发动武器效果!\n");
					return result;
				}
				continue;
			}
			result=launch_weapon_effect(c1,c1->p1->weapon_effect[choice][choice2],choice,my_turn,&locktime,this_room);
			if(result==i_cancel_effect)
			{
				room_broadcast(this_room,"他选择了取消发动武器效果!\n");
				return result;
			}
		//choice表示第几个武器
		//c1为发动者，p1->weapon_effect[choice][choice2]为发动的效果，choice为第几个发动的武器
		//my_turn为发动条件(“自己回合自己主动发动”)，this_room为传入玩家列表
			
			return result;
		}//选该武器的效果号结束

	}//发动哪个武器结束

}

//读取人物属性栏weapon_effect中的号码，显示其效果，玩家选择效果发动。进入发动函数，判断是否符合发动条件，不符合的返回无法发动，符合的进行发动
int player_weapon_effect(client_member* c1,fight_room* this_room)
{
	int result=still_fight;
	int locktime=0;

	if(c1->p1->w[0]==false){confirm_send_success(c1,"当前没有装备武器!\n");return still_fight;}

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
	if(effectnumber==0){confirm_send_success(c1,"当前武器没有任何效果可发动!\n");return still_fight;}
	lock_other(c1,this_room);
	result=weapon_effect_process(c1,this_room);
	release_other(c1,result,this_room);//事件结束,解放其他人行动
	return result;
}

int player_effect(client_member* c1,fight_room* this_room)
{
	int locktime=0;
	char StrBuf[MAX_PATH] = { 0 };
	int result=still_fight;
	if(c1->p1->a[0]==false)//该人物没有个人效果
	{
		confirm_send_success(c1,"该人物没有个人效果!\n");
		return still_fight;
	}
	if(c1->p1->a[1]==false)//如果说只有一个个人效果则直接判定该效果能不能发动，能发动则直接发动
	{
		if(judge_character_effect(c1,c1->p1->ability[0],my_turn,&locktime))//判定能否发动
			result=character_effect(c1,c1->p1->ability[0],this_room);//发动效果
		else
		{
			confirm_send_success(c1,"发动失败!\n");
			return still_fight;
		}
	}
	if(c1->p1->a[1]==true)//如果说有多个技能，则进行选择
	{
		confirm_send_success(c1,"请问你想发动哪个自身效果？\n请选择：");
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯
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
	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"开始发动加附效果！\n ");
	room_broadcast(this_room,StrBuf);
	memset(StrBuf,0,sizeof(StrBuf));

	show_him_his_buff_effect(c1);//展示他所有的buff效果
			while(true)
			{
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动加附效果!\n即将返回上一选项页面……\n");break;}
				
				choice=atoi(StrBuf);//化字符串为数字
				if(choice==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
				choice--;
				if(choice==1&&c1->p1->hideshield>0)
					result=launch_buff_effect(c1,75,my_turn,&locktime,this_room);
				if(result==i_cancel_effect){confirm_send_success(c1,"你选择了取消发动加附效果!\n即将返回上一选项页面……\n");break;}
				
				return result;
			}
			return result;
}

void player_end_turn(client_member* c1,fight_room* this_room)//该玩家宣布回合结束
{

	client_member* c2;
	if(this_room->c[0]->member_No==c1->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];

	if(this_room->ready_number==0)
					{
						lock_other_N(c1,this_room);
						confirm_send_success(c1,"等待对面结束回合……\n");
						
						if(c2->device==win_console)
							confirm_send_success(c2,"\n\n对面选择了结束回合!\n\n");
						else
							confirm_send_success(c2,"对面选择了结束回合!\n");
						release_other(c1,still_fight,this_room);
					}
}







int player_buff_effect(client_member* c1,fight_room* this_room)
{
	int result=still_fight;
	if(c1->p1->hideshield==0)
	{
		if(c1->device==win_console)
			confirm_send_success(c1,"你当前没有加附效果!请重新选择：");
		else
			confirm_send_success(c1,"你当前没有加附效果!\n");
		return result;
	}
	lock_other(c1,this_room);
	result=buff_effect_process(c1,this_room);
	release_other(c1,result,this_room);//事件结束,解放其他人行动
	return result;
			
}


#endif