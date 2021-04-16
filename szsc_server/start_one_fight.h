
#ifndef INCLUDE_start_one_fight_H
#define INCLUDE_start_one_fight_H

#include "server_general_function.h"
#include "player_choose.h"



void start_fight(client_member* p,fight_room* this_room)//开启战斗
{
	char StrBuf[MAX_PATH] = { 0 };//定义变量存储接收到的数据
	char Buf[MAX_PATH] = { 0 };
	bool smooth_link=false;//为了宾客guest的客户端无缝连接
	bool be_shown=false;//检测某值，如果提示了就不用再提示，一开始设为没提示
	int choice;
	int result;
	int count;//计数器
	bool main_turn=false;//是否是你的主回合
	bool i_am_wait=false;//是否在等待
	bool whether_choice_right=false;//玩家选择是否正确
	
	client_member* c2;
	if(p->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	
	Sleep(100);
	//让客户端选取角色(房间主人才发动以下内容)
	if(!p->guest)//如果该客户端为房间主人，群发到房间其他人，让他们选择角色，目前房间里就两个人，p1,p2
	{
		addtext(StrBuf,character_choice);addtext(StrBuf,"\n");
		::send(g_ClientSocket[p->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);// 发送数据到已连接的客户端上
		if(c2!=NULL&&c2->member_No!=robot_symbol)//如果不为空  且  不是机器人
			::send(g_ClientSocket[c2->member_No-1],StrBuf, (1 + ::strlen(StrBuf)), 0);//同时往宾客那里也发一下
	}

	while(true)//开始选择角色死循环
	{
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,get_client_message(p));if(p->F_offline){return;}//接收客户端发来消息，如果此时断开通讯结束沟通

		if(check_sb_offline(p,this_room))return;//如果有人中途退出,确认没断开通讯，确认房间内是否有人退出
		choice=0;
		switch(atoi(StrBuf))//确认玩家选择
		{
			case 1:choice=1;addtext(Buf,"您选择了战士!\n");break;
			case 2:choice=2;addtext(Buf,"您选择了法士!\n");break;
			default:
				if(p->device==win_console)
					confirm_send_success(p,"输入无效，请重新输入：");
				else
					special_signal(p,reopen_choice);
		}
		if(choice==1||choice==2)break;
	}//结束选择角色死循环

	if(p->device==win_console)confirm_send_success(p,Buf);//告诉他选择了什么

	ini_character(p->p1,this_room->ready_number+1);//（角色，第几号玩家）初始化其他内容
	
	memset(p->p1->name,0,sizeof(p->p1->name));
	addtext(p->p1->name,this_room->ready_number+1);//通用名字：“x号玩家：xx”
	addtext(p->p1->name,"号玩家:");
	addtext(p->p1->name,search_character(choice));
	addtext(p->p1->name," ");
	set_character(choice,p->p1);//对血量攻击属性初始化
	this_room->ready_number++;//创建完成者+1

	if(check_robot(c2))
	{
		this_room->ready_number++;//机器人不用选择角色创建
		ini_character(c2->p1,this_room->ready_number+1);//初始化机器人人物数据
		memset(c2->p1->name,0,sizeof(c2->p1->name));
		addtext(c2->p1->name,"2号玩家:木桩 ");
		set_character(0,c2->p1);//对血量攻击属性初始化
		ini_deck(this_room->deck);//卡组初始化洗牌
		this_room->passturns=1;//轮回数置1
		this_room->turn_force_end=false;//清空“强制结束该回合”标志
		p->p1->whether_myturn=true;//先选者第一回合
		be_shown=true;
		i_am_wait=true;
	}

	while(this_room->ready_number<playernumber)//自己创建角色完成后，首先对游戏各种配置初始化，再监听对面是否完成
	{
		if(!be_shown)//显示一遍"等待对面选完角色……\n"，同时初始化游戏各种配置，之后等待对面选择，这些事情不再重复做，只做一次
		{
			ini_deck(this_room->deck);//卡组初始化洗牌
			this_room->passturns=1;//轮回数置1
			this_room->turn_force_end=false;//清空“强制结束该回合”标志
			p->p1->whether_myturn=true;//先选者第一回合

			if(p->device==win_console)confirm_send_success(p,"等待对面选完角色……\n");
			be_shown=true;
			i_am_wait=true;
		}

		if(check_sb_offline(p,this_room))return;//如果有人中途退出
		Sleep(50);
	}
	
	special_signal(p,choice_complete);//选择完成了，进入战斗房间状态

	if(check_sb_offline(p,this_room))return;//如果有人中途退出

	if(i_am_wait)
	{
		if(!check_robot(c2))SuspendThread(hthread[c2->member_No-1]);//挂起其他人线程

		this_room->ready_number=0;
		refresh_character_state_F(p,this_room);
		i_am_wait=false;

		get_specific_card(63,p->p1);
		//get_specific_card(0,p->p1);
		//get_specific_card(1,p->p1);
		//get_specific_card(2,p->p1);
		get_card(4,p,this_room);//玩家初始获取4张手卡
		get_card(4,c2,this_room);//其他玩家初始获取4张手卡

		if(!check_robot(c2))ResumeThread(hthread[c2->member_No-1]);//启动其他人线程
	}
	else
	{
		//get_specific_card(0,p->p1);
		Sleep(1000);
	}

	be_shown=false;

	while(true)//一回合死循环开始
	{
		main_turn=false;//不确定是否是你的主回合

		if(p->p1->whether_myturn==true)//如果是某人的主回合，进行回合宣告
		{
			Sleep(300);	
			if(!check_robot(c2))SuspendThread(hthread[c2->member_No-1]);//挂起其他人线程

			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"目前为第 ");addtext(StrBuf,this_room->passturns);addtext(StrBuf," 个轮回\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//广播目前为第几个轮回
			addtext(StrBuf,"现在是");addtext(StrBuf,p->p1->name);addtext(StrBuf,"的回合!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//广播谁的回合			
			special_signal(p,now_is_your_turn);//"现在是你的回合!\n"
			main_turn=true;	

			rule_get_card(p,this_room);//主回合者进行规则性抽卡
			p->p1->fight_chance=1;//获得一次规则提供的普攻机会
			this_room->ready_number=0;//宣布回合结束者数量置为0

			for(count=0;count<playernumber;count++)
			{
				if(check_robot(this_room->c[count]))this_room->ready_number++;//如果是机器人，选择回合结束者+1
				special_signal(this_room->c[count],start_another_turn);//告诉他们开始下一个回合自由行动了！
			}
			if(!check_robot(c2))
			ResumeThread(hthread[c2->member_No-1]);//启动其他人线程
		}
		else
		{
			for(count=0;count<playernumber;count++)
				if(this_room->c[count]->p1->whether_myturn)//谁是主回合者？
				{
					if(this_room->c[count]->member_No==robot_symbol)//主回合者是不是机器人？
					{
						if(this_room->c[0]->member_No==p->member_No)//如果（多人模式下）有机器人，临时让房主当回合结算者，否则进行等待
						{
							addtext(StrBuf,"目前为第 ");addtext(StrBuf,this_room->passturns);addtext(StrBuf," 个轮回\n");
							room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
							addtext(StrBuf,"现在是");addtext(StrBuf,this_room->c[count]->p1->name);addtext(StrBuf,"的回合!\n");
							room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
							
							rule_get_card(this_room->c[count],this_room);//主回合者(机器人)进行规则性抽卡
							
							for(count=0;count<playernumber;count++)
							{
								if(check_robot(this_room->c[count]))this_room->ready_number++;//如果是机器人，选择回合结束者+1
								special_signal(this_room->c[count],start_another_turn);//告诉他们开始下一个回合自由行动！
							}
						}
					}
					else
						Sleep(1000);//非主回合玩家且非房主者等待主回合玩家宣告完毕
					break;
				}
		}

		while(true)//角色选择行动死循环
		{
			if(p->p1->un_move_able>0)//不能行动则强制结束回合
			{
				memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,p->p1->name);addtext(StrBuf," 由于无法行动而被迫强制结束回合!\n");
				room_broadcast(this_room,StrBuf);
				break;
			}
			
			if(p->device==win_console&&atoi(StrBuf)!=client_get_message)special_signal(p,battle_choice);//显示选择行动界面
			if(check_sb_offline(p,this_room))return;//如果有人中途退出
				
				memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,get_client_message(p));if(p->F_offline){return;}//如果此时断开通讯结束沟通

				if(check_sb_offline(p,this_room))return;//如果中途有其他人断线
				if(!p->p1->alive||!c2->p1->alive){Sleep(500);return;}//有人死了，退出
				if(!p->p1->alive){Sleep(500);return;}//玩家死亡，失败，结束游戏
				if(p->p1->un_move_able>0)continue;//不能行动则强制结束回合
				if(this_room->turn_force_end==true)break;//强制结束该回合

				whether_choice_right=false;//默认没有选择正确

				switch(atoi(StrBuf))//确认是否输入正确,正确则进入行动函数
				{
					case 1:choice=1;whether_choice_right=true;confirm_send_success(p,"你选择了进行普攻!\n");result = player_attack(p,this_room);break;
					case 2:choice=2;whether_choice_right=true;confirm_send_success(p,"你选择了发动角色自身效果!\n");result = player_effect(p,this_room);break;
					case 3:choice=3;whether_choice_right=true;confirm_send_success(p,"你选择了使用手卡!\n");result = player_use_card(p,this_room);break;
					case 4:choice=4;whether_choice_right=true;confirm_send_success(p,"你选择了丢弃已装备的武器!\n");player_delete_weapon(p,this_room);break;
					case 5:choice=5;whether_choice_right=true;confirm_send_success(p,"你选择了发动已装备武器的效果!\n");result = player_weapon_effect(p,this_room);break;
					case 7:choice=7;whether_choice_right=true;confirm_send_success(p,"你选择了结束回合!\n");player_end_turn(p,this_room);break;
					case 8:choice=8;whether_choice_right=true;confirm_send_success(p,"你选择了发动加附效果!\n");result=player_buff_effect(p,this_room);break;
					default:if(p->device==win_console)confirm_send_success(p,"输入了无效字符!请重新输入:");
				}
			
				if(!whether_choice_right)
				{cout<<p->client_name<<"客户端输入的无效字符："<<StrBuf<<" 客户端输入的字符串长度："<<strlen(StrBuf)<<endl;continue;}
	
			if(result==force_end_turn){this_room->turn_force_end=true;room_broadcast(this_room,"此回合被强制结束!\n");break;}
			if(this_room->turn_force_end==true)break;//强制结束该回合
			if(choice==7)break;//自主选择回合结束

			if(result==one_die)
			{
				if(p->p1->alive==c2->p1->alive){special_signal(p,game_standoff);special_signal(c2,game_standoff);}//平局
				if(p->p1->alive){special_signal(p,you_win);special_signal(c2,you_lose);}//你赢了
				else{special_signal(p,you_lose);special_signal(c2,you_win);}//你输了
				p->state=in_room;
				return;
			}

		}//角色选择行动死循环结束

		this_room->ready_number++;//选择回合结束者+1
		while(this_room->ready_number<playernumber)//自己结束回合后，监听别人是否结束
		{
			if(check_sb_offline(p,this_room))return;//如果有人中途退出
			Sleep(50);
			i_am_wait=true;
		}
		if(i_am_wait){Sleep(50);this_room->ready_number=0;i_am_wait=false;}//宣布回合结束者置0

		
		//回合结算,主回合者才进行结算函数，其他不用发动
		if(main_turn)
		{
			if(!check_robot(c2))SuspendThread(hthread[c2->member_No-1]);//结算事件发生,挂起其他人线程

			special_signal(p,start_turn_settle);special_signal(c2,start_turn_settle);//进入回合结算!……

			this_room->turn_force_end=false;//清空“强制结束该回合”标志
			natural_set_playerturn(this_room);//进行回合切换、结算，切换到下一个人的回合

			for(count=0;count<playernumber;count++)
			{
				if(judge_one_die(this_room->c[count],this_room)==one_die)//查看是否有人以0血状态失去不死效果，有则判定胜利结果（仅限1v1使用，多人制不适用）
					result=one_die;
				if(this_room->c[count]->member_No==robot_symbol)this_room->ready_number2++;//回合结算完成进行同步化完成者+1
			}

			if(!check_robot(c2))ResumeThread(hthread[c2->member_No-1]);///结算事件结束，启动其他人线程
		}
		else
		{
			for(count=0;count<playernumber;count++)
				if(this_room->c[count]->p1->whether_myturn)//谁是主回合者？
				{
					if(this_room->c[count]->member_No==robot_symbol)//主回合者是不是机器人？
					{
						if(this_room->c[0]->member_No==p->member_No)//如果（多人模式下）有机器人，临时让房主当回合结算者，否则进行等待
						{
							lock_other_N(p,this_room);//结算事件发生,锁死他人

							special_signal(p,start_turn_settle);//进入回合结算!……
							special_signal(c2,start_turn_settle);

							this_room->turn_force_end=false;//清空“强制结束该回合”标志
							natural_set_playerturn(this_room);//进行回合切换、结算，切换到下一个人的回合
							
							for(count=0;count<playernumber;count++)
							{
								if(judge_one_die(this_room->c[count],this_room)==one_die)//查看是否有人以0血状态失去不死效果，有则判定胜利结果（仅限1v1使用，多人制不适用）
									result=one_die;
								
								if(this_room->c[count]->member_No==robot_symbol)this_room->ready_number2++;//回合结算完成进行同步化完成者+1
							}

							release_other_N(p,this_room);//结算事件结束，解锁他人
						}
					}
					else
						Sleep(2000);//非主回合玩家且非房主者等待主回合玩家宣告完毕
					break;
				}
		}

		if(result==one_die)//有人死了，回合结束
		{
			confirm_send_success(p,"即将退回房间状态…………\n");
			if(p->p1->alive==c2->p1->alive){special_signal(p,game_standoff);special_signal(c2,game_standoff);}//平局
			if(p->p1->alive)	special_signal(p,you_win);//你赢了
			else				special_signal(p,you_lose);//你输了
			return;
		}

		this_room->ready_number2++;//回合结算完成进行同步化

		while(this_room->ready_number2<playernumber)//自己结束回合后，监听别人是否同步化完成
		{
			if(check_sb_offline(p,this_room))return;//如果有人中途退出
			i_am_wait=true;
		}
		if(i_am_wait){this_room->ready_number2=0;i_am_wait=false;}//同步化完成者置0
	}//一回合死循环结束

	
}

#endif