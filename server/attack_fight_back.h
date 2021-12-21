#ifndef INCLUDE_attack_fight_back_H
#define INCLUDE_attack_fight_back_H

#include "launch_sth_effect.h"
int fight_back_UI(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime)//图形界面专用
{
	
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int choice,choice2;
	int effect_number;//记录发动卡片效果的卡号
	int result=still_fight;
	bool whether_hide_effect;
	
	if(c1->p1->un_move_able!=0||c1->p1->unmoveable_time!=false)
	{
		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"当前处于无法行动状态!\n");
		room_broadcast(this_room,StrBuf);return still_fight;
	}

	confirm_send_success(c2,"等待对面思考……\n");
	addtext(StrBuf,"【事件:");
	switch(situation)
	{
		case i_will_attack:			addtext(StrBuf,"你发动了普攻");break;
		case i_will_be_attacked:	addtext(StrBuf,"你即将被普攻");break;
		case i_attack:				addtext(StrBuf,"你普攻成功");break;
		case i_be_attacked:			addtext(StrBuf,"你被普攻了");break;
		case i_failed_attack_E:		addtext(StrBuf,"你普攻被躲避了");break;
		case i_failed_attack_D:		addtext(StrBuf,"你普攻被格挡了");break;
		case i_failed_attack_W:		addtext(StrBuf,"你普攻到稻草人了");break;
		case i_be_effect_A:			addtext(StrBuf,"自己已经受到自主伤血效果");break;
		case i_be_effect_B:			addtext(StrBuf,"自己受到伤血效果");break;
		case i_will_be_hit_die:		addtext(StrBuf,"自己即将受到致命伤血");break;
		case i_will_be_effect_B:	addtext(StrBuf,"自己即将受到伤血效果");break;
		case sb_card_used:			addtext(StrBuf,"发动了一张卡");break;
		default:cout<<"----------------------------意外situation="<<situation<<endl;
	}
	addtext(StrBuf,"】\n");
	if(situation==sb_card_used)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,"【是否要阻止其发动？】\n");
		confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
	}
	else
	{
		confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,"你是否行动？\n");confirm_send_success(c1,StrBuf);
	}
	
	while(true)//做选择死循环
	{
		special_signal(c1,state_fight_back);//用户界面改成反击界面
		
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯
		
		special_signal(c1,state_return_normal);

		//if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择不行动!\n");confirm_send_success(c2,"对面选择不行动!\n");return still_fight;}
		//if(strlen(StrBuf)>1||atoi(StrBuf)>4||atoi(StrBuf)<1){confirm_send_success(c1,"输入了无效字符!请重新选择!\n");continue;}
		
		switch(atoi(StrBuf))
		{
		case 1://发动手卡开始
			{
				special_signal(c1,get_c_number);//获取发动第几张手卡

				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择取消发动手卡!\n");continue;}
				choice=atoi(StrBuf);//化字符串为数字
				if(choice==0){confirm_send_success(c1,"输入无效字符!请重新选择手卡:\n");continue;}
				choice--;
				if(choice>=cardlimit||choice<0){confirm_send_success(c1,"输入超出手卡上/下限！请重新选择手卡:\n");continue;}
				if(c1->p1->c[choice]==false){confirm_send_success(c1,"该手卡槽为空！请重新选择手卡:\n");continue;}

				special_signal(c1,whether_launch_hideeffect);//询问是否发动隐效果

				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯
				
				switch(atoi(StrBuf))
				{
					case  1:effect_number=c1->p1->card[choice];whether_hide_effect=false;break;
					case  0:effect_number=c1->p1->hideeffect[choice];whether_hide_effect=true;break;//确认发动了哪个卡号
				}

				if(judge_card(c1,effect_number,situation,locktime))//输入该卡号，判断是否符合发动条件
				{
					
					result=use_card(c1,choice,whether_hide_effect,locktime,this_room);//消耗了这一卡槽的卡
					if(result==i_failed_effect)return still_fight;
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
					if(judge_assist(effect_number))//发动助攻卡效果
					{
						c1->p1->whether_use_assist=effect_number;//如果是使用助攻卡，后面将发动助攻卡特效(因为可能是幻影剑、隐天盾等效果卡)
						result=launch_assist_effect(c1,c2,effect_number,fore_effect,locktime,this_room);
						return result;
					}
					else//发动普通效果卡
					{
						result=launch_card_effect(c1,c2,effect_number,situation,locktime,this_room);//发动卡片效果，返回一个结果
						if(result==i_cancel_effect&&effect_number!=75&&effect_number!=402){confirm_send_success(c1,"效果错误!本该无法取消!\n");return still_fight;}
						return result;
					}
				}//卡片效果发动结束
				confirm_send_success(c1,"该卡片不符合发动条件!\n");continue;

			}//发动手卡结束
			break;
		case 2://发动角色效果开始
			{
				if(c1->p1->a[1]==false)//如果说只有一个个人效果则直接判定该效果能不能发动，能发动则直接发动
				{
					if(judge_character_effect(c1,c1->p1->ability[0],situation,locktime))//判定能否发动
						result=character_effect(c1,c1->p1->ability[0],this_room);//发动效果
					else
					{
						confirm_send_success(c1,"请重新选择行动!\n");
						continue;
					}
				}
				//展示所有个人效果，判定选中的效果能否发动
				room_broadcast(this_room,"发动了个人效果!是否有人阻止？\n没人阻止……\n");
				confirm_send_success(c1,"发动成功!\n");
				return result;
			}//发动角色效果结束
			break;
		case 3://发动武器效果开始
			{
				if(c1->p1->w[0]==false){confirm_send_success(c1,"你当前没有装备武器!\n");continue;}

				show_him_his_weapon_effect(c1);//显示自己所有武器及其效果
//接收发动第几个武器
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动武器效果!\n");continue;}
					
				choice=atoi(StrBuf);//化字符串为数字
				if(choice==0){confirm_send_success(c1,"输入有误!\n");continue;}
				choice--;
				if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"输入超出上下限!\n");continue;}

				if(c1->p1->w[choice]==false){confirm_send_success(c1,"该武器槽为空!\n");continue;}
					
				special_signal(c1,get_we_number);//获取发动第几个武器

				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动该武器的效果!\n");continue;}
					
				choice2=atoi(StrBuf);//化字符串为数字
				if(choice2==0){confirm_send_success(c1,"输入有误!\n");continue;}
				choice2--;
				if(choice2<0||choice2>=weaponeffectlimit||c1->p1->w_e[choice][choice2]==false){confirm_send_success(c1,"输入超出上下限 或 武器该效果槽为空!\n");continue;}
						
				if(!judge_weapon_effect(c1, c1->p1->weapon_effect[choice][choice2],situation,locktime,choice,this_room))//判定是否可发动
					{confirm_send_success(c1,"发动条件不满足!\n");continue;}
				result=launch_weapon_effect(c1,c1->p1->weapon_effect[choice][choice2],choice,situation,locktime,this_room);
				if(result==i_cancel_effect){continue;}
				return result;//choice表示第几个武器
					//c1为发动者，p1->weapon_effect[choice][choice2]为发动的效果，choice为第几个发动的武器
					//situation为发动条件(“自己回合自己主动发动”)，this_room为传入玩家列表
				
			}//发动武器效果结束
			break;
		case 4://发动加附效果开始
			{
				if(c1->p1->hideshield==0){confirm_send_success(c1,"你当前没有加附效果!请重新选择行动!\n");continue;}
				show_him_his_buff_effect(c1);
				while(true)
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动加附效果!\n");break;}
					
					choice=atoi(StrBuf);//化字符串为数字
					if(choice==0){confirm_send_success(c1,"输入有误!\n");continue;}
					choice--;
					if(choice==1&&c1->p1->hideshield>0)
						result=launch_buff_effect(c1,75,situation,locktime,this_room);
					if(result==i_cancel_effect){confirm_send_success(c1,"你选择了取消发动加附效果!\n");continue;}
					
					return result;
				}
			}//发动加附效果结束
		case 0:
			{confirm_send_success(c1,"你选择不行动!\n");confirm_send_success(c2,"对面选择不行动!\n");return still_fight;}
		default:
			{cout<<atoi(StrBuf)<<endl;continue;}
		}

	}	//被攻者做选择死循环结束
return still_fight;

}


int fight_back(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime)
{
	if(c1->member_No==robot_symbol)return still_fight;//机器人不做出反击
	int result=still_fight;
	if(c1->device!=win_console){result=fight_back_UI(c1,c2,situation,this_room,locktime);return result;}

	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int choice,choice2;
	int effect_number;//记录发动卡片效果的卡号
	bool whether_hide_effect;
	
	if(c1->p1->un_move_able!=0||c1->p1->unmoveable_time!=false)
	{
		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"当前处于无法行动状态!\n");
		room_broadcast(this_room,StrBuf);return still_fight;
	}

	confirm_send_success(c2,"等待对面思考……\n");
	addtext(StrBuf,"（事件:");
	switch(situation)
	{
		case i_will_be_attacked:	addtext(StrBuf,"你即将被普攻");break;
		case i_attack:				addtext(StrBuf,"你普攻成功");break;
		case i_be_attacked:			addtext(StrBuf,"你被普攻了");break;
		case i_failed_attack_E:		addtext(StrBuf,"你普攻被躲避了");break;
		case i_failed_attack_D:		addtext(StrBuf,"你普攻被格挡了");break;
		case i_failed_attack_W:		addtext(StrBuf,"你普攻到稻草人了");break;
		case i_be_effect_A:			addtext(StrBuf,"自己已经受到自主伤血效果");break;
		case i_be_effect_B:			addtext(StrBuf,"自己受到伤血效果");break;
		case i_will_be_hit_die:		addtext(StrBuf,"自己即将受到致命伤血");break;
		case i_will_be_effect_B:	addtext(StrBuf,"自己即将受到伤血效果");break;
		default:cout<<"意外situation="<<situation<<endl;
	}
	addtext(StrBuf,"!）\n");
	if(situation==sb_card_used)
	{memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,"【是否要阻止其发动？】\n");}
	else
	{
		confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,"你是否行动？\n");confirm_send_success(c1,StrBuf);
	}

	special_signal(c1,whether_fight_back);//"你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.加附效果）\n请选择：");


	while(true)//做选择死循环
	{
		
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择不行动!\n");confirm_send_success(c2,"对面选择不行动!\n");return still_fight;}
		
		if(strlen(StrBuf)>1||atoi(StrBuf)>4||atoi(StrBuf)<1){confirm_send_success(c1,"输入了无效字符!请重新选择：");continue;}
		choice=atoi(StrBuf);
		switch(choice)
		{
		case 1://发动手卡开始
			{
				confirm_send_success(c1,"你选择发动手卡!\n请选择发动哪张:");
				
				while(true)//选择发动手卡死循环开始
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择取消发动手卡!即将返回上一界面……\n你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.加附效果）\n请选择：");break;}
					choice=atoi(StrBuf);//化字符串为数字
					if(choice==0){confirm_send_success(c1,"输入无效字符!请重新选择手卡:");continue;}
					choice--;
					if(choice>=cardlimit||choice<0){confirm_send_success(c1,"输入超出手卡上/下限！请重新选择手卡:");continue;}
					if(c1->p1->c[choice]==false){confirm_send_success(c1,"该手卡槽为空！请重新选择手卡:");continue;}

					special_signal(c1,whether_launch_hideeffect);//询问是否发动隐效果

					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯
					
					switch(atoi(StrBuf))
					{
						case  1:effect_number=c1->p1->card[choice];whether_hide_effect=false;break;
						case  0:effect_number=c1->p1->hideeffect[choice];whether_hide_effect=true;break;//确认发动了哪个卡号
					}

					if(judge_card(c1,effect_number,situation,locktime))//输入该卡号，判断是否符合发动条件
					{	//开始发动卡片效果
						result=use_card(c1,choice,whether_hide_effect,locktime,this_room);//消耗了这一卡槽的卡
						if(result==i_failed_effect)return still_fight;
						if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
						
						result=launch_card_effect(c1,c2,effect_number,situation,locktime,this_room);//发动卡片效果，返回一个结果
						if(result==i_cancel_effect&&effect_number!=75){confirm_send_success(c1,"效果错误!本该无法取消!\n");return still_fight;}

						return result;
					}//卡片效果发动结束
					confirm_send_success(c1,"该卡片不符合发动条件！请重新选择手卡:");continue;
				}//选择发动手卡死循环结束

				
			}//发动手卡结束
			break;
		case 2://发动角色效果开始
			{
				confirm_send_success(c1,"你选择发动角色效果!\n");
				if(c1->p1->a[1]==false)//如果说只有一个个人效果则直接判定该效果能不能发动，能发动则直接发动
				{
					confirm_send_success(c1,"你的角色只有一个效果!\n");
					if(judge_character_effect(c1,c1->p1->ability[0],situation,locktime))//判定能否发动
						result=character_effect(c1,c1->p1->ability[0],this_room);//发动效果
					else
					{
						confirm_send_success(c1,"发动失败!即将返回上一页面……\n你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.发动加附效果）\n请选择：");
						continue;
					}
				}
				room_broadcast(this_room,"发动了个人效果!是否有人阻止？\n没人阻止……\n");
				confirm_send_success(c1,"发动成功!\n");
				return result;
			}//发动角色效果结束
			break;
		case 3://发动武器效果开始
			{
				confirm_send_success(c1,"你选择发动武器效果!\n");
				if(c1->p1->w[0]==false){confirm_send_success(c1,"你当前没有装备武器!请重新选择:");continue;}

				show_him_his_weapon_effect(c1);//显示自己所有武器及其效果

				confirm_send_success(c1,"\n\n请输入想发动第几个武器(输入0表示‘取消该操作’):");
				while(true)
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动武器效果!\n即将返回上一选项页面……\n你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.加附效果）\n");break;}
					
					choice=atoi(StrBuf);//化字符串为数字
					if(choice==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
					choice--;
					if(choice<0||choice>=weaponlimit){confirm_send_success(c1,"输入超出上下限！请重新输入:");continue;}

					if(c1->p1->w[choice]==false){confirm_send_success(c1,"该武器槽为空！请重新输入:");continue;}
					confirm_send_success(c1,"你想发动该武器的效果几?(输入0表示‘取消该操作’):");
					
					while(1)
					{
						memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

						if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动该武器的效果!\n即将返回上一选项页面……\n请输入想发动第几个武器(输入0表示‘取消该操作’):");break;}
					
						choice2=atoi(StrBuf);//化字符串为数字
						if(choice2==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
						choice2--;
						if(choice2<0||choice2>=weaponeffectlimit||c1->p1->w_e[choice][choice2]==false){confirm_send_success(c1,"输入超出上下限 或 武器该效果槽为空！请重新输入:");continue;}
						
						if(!judge_weapon_effect(c1, c1->p1->weapon_effect[choice][choice2],situation,locktime,choice,this_room))//判定是否可发动
							{confirm_send_success(c1,"发动条件不满足!请重新选择:");continue;}
						result=launch_weapon_effect(c1,c1->p1->weapon_effect[choice][choice2],choice,situation,locktime,this_room);
						if(result==i_cancel_effect){confirm_send_success(c1,"你想发动该武器的效果几?(输入0表示‘取消该操作’):");break;}
						return result;//choice表示第几个武器
					//c1为发动者，p1->weapon_effect[choice][choice2]为发动的效果，choice为第几个发动的武器
					//situation为发动条件(“自己回合自己主动发动”)，this_room为传入玩家列表

					}
				}
			}//发动武器效果结束
			break;
		case 4://发动加附效果开始
			{
				if(c1->p1->hideshield==0){confirm_send_success(c1,"你当前没有加附效果!请重新选择：");continue;}
				show_him_his_buff_effect(c1);

				while(true)
				{
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

					if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你选择了取消发动加附效果!\n即将返回上一选项页面……\n你是否行动？（0:不行动；1:发动手卡；2:发动角色效果；3:发动武器效果）\n请选择：):");break;}
					
					choice=atoi(StrBuf);//化字符串为数字
					if(choice==0){confirm_send_success(c1,"输入有误!请重新输入:");continue;}
					choice--;
					if(choice==1&&c1->p1->hideshield>0)
						result=launch_buff_effect(c1,75,situation,locktime,this_room);
					if(result==i_cancel_effect){confirm_send_success(c1,"你选择了取消发动加附效果!\n即将返回上一选项页面……\n你是否行动？（0:不行动；1:发动手卡；2:发动角色效果；3:发动武器效果）\n请选择：):");break;}
					
					return result;
				}
			}//发动加附效果结束
		}
		
	}	//被攻者做选择死循环结束
return still_fight;

}



//通用攻击函数，p1攻击p2，situtation判断是否是p1主场主动攻击，若是则消耗规则普攻次数，不是则此项无关
int general_attack(client_member* c1,client_member* c2,int situation,int* locktime,fight_room* this_room)
{
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int count;
	int result=i_attack;//初始默认为普攻成功
	float damage=0;//总伤害计算

	bool whether_use_assist=false;//是否使用了助攻卡？默认为否
	
	c1->p1->whether_in_attack++;c2->p1->whether_in_attack++;//两者进入搏斗状态

	if(situation==myturn_attack)c1->p1->fight_chance--;//如果是某人的回合，他主动发起普攻,消耗一次普攻机会

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"对");addtext(StrBuf,c2->p1->name);addtext(StrBuf,"发动了普攻!\n");
	room_broadcast(this_room,StrBuf);//广播普攻事件
	confirm_send_success(c2,"等待对面行动……\n");

	result=fight_back(c1,c2,i_will_attack,this_room,locktime);//事件：“我要普攻了”
	if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)//如果有普攻事件中断或逾期
	{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}
	
	//进入普攻结算流程
	auto_judge(c1,i_will_attack,this_room);//部分效果被动触发，“我要普攻了”

	//统计攻击者总攻击力，算入p_attack_all，被攻击者总攻击力，算入p_defend_all
	float p_attack_all=	(c1->p1->attack)	+	(c1->p1->exattack_turn)	+	(c1->p1->exattack_time);
	for(count=0;count<2;count++)//检查装备卡是否有加攻击力
		if(c1->p1->w[count]==true&&c1->p1->w_valid[count]==true)
			p_attack_all=	p_attack_all	+   c1->p1->exattack_weapon[count] + c1->p1->exattack_weapon_turn[count] + c1->p1->exattack_weapon_time[count];
    float p_defend_all=	c2->p1->attack +	c2->p1->exattack_turn	+	c2->p1->exattack_time;
	for(count=0;count<2;count++)
		if(c2->p1->w[count]==true&&c2->p1->w_valid[count]==true)
			p_defend_all=	p_defend_all	+	c2->p1->exattack_weapon[count] + c2->p1->exattack_weapon_turn[count] + c2->p1->exattack_weapon_time[count];

		memset(StrBuf,0,sizeof(StrBuf));
	if(!c1->p1->whether_attack_T)//如果此次普攻不是穿攻
	{
		//进行普攻血量计算
		damage	=( p_attack_all - p_defend_all )*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
		if(p_attack_all>p_defend_all)
		{
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"成功攻击 ");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"!");
			addtext(StrBuf,"并对他造成了 ");addtext(StrBuf,damage);addtext(StrBuf,"点伤害!\n");
		}
		else
		if(p_attack_all<p_defend_all)//反伤
		{	
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"的攻击力小于被攻击者 ");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的攻击力!对自己造成了反伤!");
			addtext(StrBuf,"并对自己造成了");addtext(StrBuf,-damage);addtext(StrBuf,"点伤害!\n");
		}
		else
		if(p_attack_all==p_defend_all)
		{
			addtext(StrBuf,"两者攻击力相同!本次搏斗不对双方任何人造成伤害!\n");
		}
	}//普攻血量计算结束
	else//穿攻，直接无视对面总攻击力，给予对手伤害
	{
		damage	=	p_attack_all*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
		
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," 对 ");
		addtext(StrBuf,c2->p1->name);addtext(StrBuf," 造成了穿透性攻击!!!");
		addtext(StrBuf,"并对他造成了");addtext(StrBuf,damage);addtext(StrBuf,"点伤害!\n");
	}

	result=hit_damage(damage,c2,c1,this_room,locktime,StrBuf,i_will_be_attacked);//被普攻者进行 躲避 格挡 反击

		if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)//如果有普攻事件中断或逾期
		{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}
		if(result==i_failed_attack_E||result==i_failed_attack_D||result==i_failed_attack_W)
		{
			if(result==i_failed_attack_W)delete_weapon_all(c1,this_room);//打中稻草人，武器全清
			fight_data_clean(c1->p1,c2->p1,true,this_room);//该次普攻产生的特效数值被消除

			result=fight_back(c1,c2,result,this_room,locktime);//普攻者 针对 “自己普攻失败”事件 进行反击
			c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;//两者退出搏斗状态
			return result;
		}

	result=i_attack;//执行到此步，判断为攻击成功

	if(judge_one_die(c1,this_room)==one_die||judge_one_die(c2,this_room)==one_die)
	{
		c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;//两者退出搏斗状态
		return one_die;
	}
	auto_judge(c1,c2,has_fought,this_room,locktime);//两人进行了一次搏斗，触发一些被动（例如 噬剑之剑）

	//如果使用了助攻卡，进行助攻卡后续效果结算(只在之前效果、普攻之类的全部成功的情况下才结算)
	if(c1->p1->whether_use_assist!=-1)
		result=launch_assist_effect(c1,c2,c1->p1->whether_use_assist,after_effect,locktime,this_room);
	if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)
	{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}//助攻卡效果结算后会不会有人死了

	c1->p1->attacktime_turn++;//该次普攻基本结束，进行普攻成功连锁

	memset(StrBuf,0,sizeof(StrBuf));
	addtext(StrBuf,"此回合自己普攻成功次数:");addtext(StrBuf,c1->p1->attacktime_turn);addtext(StrBuf,"\n");
	confirm_send_success(c1,StrBuf);
	
	fight_data_clean(c1->p1,c2->p1,false,this_room);//该次普攻产生的特效数值被消除,无法行动状态保留（fight_data_clean最后加false即表示“无法行动状态保留”）

	if(c2->p1->un_move_able==0&&c2->p1->unmoveable_time==false)
		result=fight_back(c2,c1,i_be_attacked,this_room,locktime);//被普攻者反击
	if(result==force_offline||result==force_end_turn||result==force_end_event||result==one_die)//事件是否逾期？
	{fight_data_clean(c1->p1,c2->p1,true,this_room);c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;return result;}
	if(c1->p1->un_move_able==0&&c1->p1->unmoveable_time==false)//事件没逾期，普攻者发动连锁
		result=fight_back(c1,c2,i_attack,this_room,locktime);//普攻者连锁
	fight_data_clean(c1->p1,c2->p1,true,this_room);//该次普攻产生的特效数值被消除，解除无法行动状态
	c1->p1->whether_in_attack--;c2->p1->whether_in_attack--;//两者退出搏斗状态
	return result;
}



//damage 受到多少伤害 c1受伤害者 c2施加伤害者 this_room这个房间 locktime连锁次数 situation什么伤害类型（自主伤血，效果伤血，普攻伤血）
int hit_damage(float damage,client_member* c1,client_member* c2,fight_room* this_room,int* locktime,char* StrBuf,int situation)//c2对c1造成damage伤血
{
	int result=still_fight;

	if(c1->p1->blood-damage<0)//如果是致命伤血型反击
	{
		if(c2!=NULL&&situation!=i_will_be_effect_A)//如果自己能够行动且不是自主伤血效果
		{
			c1->p1->i_soon_die=true;//自己即将死亡
			result=fight_back(c1,c2,situation,this_room,locktime);//自己反击
			c1->p1->i_soon_die=false;//消除死亡标志
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
			if(situation==i_will_be_attacked&&result==i_failed_attack_E||result==i_failed_attack_D||result==i_failed_attack_W)return result;//对普攻采取了格挡/闪避/稻草人行为
			if(situation==i_will_be_effect_B&&result==i_failed_effect_B)return i_failed_effect_B;//有效防御了效果伤血
			if(result==i_failed_damage)//如果伤血被无效化，判定该伤血来源为什么（目前只有光斩绝杀和隐天盾会返回i_failed_damage）
			{
				if(situation==i_will_be_attacked)return i_failed_attack_E;//如果是普攻，则躲避该次普攻
				if(situation==i_will_be_effect_B)return i_failed_effect_B;//如果是伤血效果，无效化该次伤血效果		
			}
		}
	}//致命伤血判定完成
	else//非致命普通伤血反击
	{
		if(situation!=i_will_be_effect_A&&c1->p1->un_move_able==0&&c1->p1->unmoveable_time==false&&(c2!=NULL))//如果自己能够行动且不是自主伤血效果
		{
			result=fight_back(c1,c2,situation,this_room,locktime);//自己即将受到非自主伤血事件，进行反击
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

			if(situation==i_will_be_attacked&&result==i_failed_attack_E||result==i_failed_attack_D||result==i_failed_attack_W)return result;//对普攻采取了格挡/闪避/稻草人行为
			if(situation==i_will_be_effect_B&&result==i_failed_effect_B)return i_failed_effect_B;//有效防御了效果伤血

			if(result==i_failed_damage)//如果伤血被无效化，判定该伤血来源为什么（目前只有光斩绝杀和隐天盾会返回i_failed_damage）
			{
				if(situation==i_will_be_attacked)return i_failed_attack_E;//如果是普攻，则躲避该次普攻	
				if(situation==i_will_be_effect_B)return i_failed_effect_B;//如果是伤血效果，无效化该次伤血效果
			}
		}
	}

	if(StrBuf==NULL)
		cout<<"StrBuf==NULL\n";//字符串指针为空？
	else
		if(strlen(StrBuf)<=1)cout<<"strlen(StrBuf)<=1\n";//字符串大小小于1？
		else
			room_broadcast(this_room,StrBuf);//伤害成功命中目标，广播装逼效果句
	if(damage>0)//成功受伤
	{
		blood_change(-damage,c1,this_room);//扣血
		if(c2!=NULL&&situation!=i_will_be_effect_A)//如果并非自主伤血
			auto_judge(c2,i_hit_someone,&damage,locktime,this_room);//c2对某人成功造成了伤害，触发长者之镰或其他效果的被动		
	}
	else
		blood_change(damage,c2,this_room);//反伤

	if(judge_one_die(c1,this_room)==one_die)return one_die;
	if(c2!=NULL&&judge_one_die(c2,this_room)==one_die)return one_die;//受到伤血后判断死没死，死了就无法进行反击

	switch(situation)//将事件条件转化
	{
		case i_will_be_effect_B:situation=i_be_effect_B;break;//自己已经受到伤血效果
		case i_will_be_attacked:situation=i_be_attacked;break;//自己已经受到普攻伤血
		case i_will_be_effect_A:situation=i_be_effect_A;break;//自己已经受到自主伤血效果
	}

	if(c2==NULL||situation==i_be_effect_A)//如果是自主伤血事件
		if(c1->member_No==this_room->c[0]->member_No)
			c2=this_room->c[1];
		else
			c2=this_room->c[0];
	if(c2==NULL)return force_offline;

	if(situation==i_be_attacked)
		return i_attack;//普攻者被普攻伤血了，普攻反击在普攻函数中实现，因为普攻伤血后可能还要结算助攻卡后续效果，所以这里不进行普攻反击

	result=fight_back(c1,c2,situation,this_room,locktime);//进行反击

	if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
	return still_fight;
}



int lightspeed_attack(client_member* c1,client_member* c2,float damage,int* locktime,fight_room* this_room)//超速普攻,c1打c2,暂定为直接修改血量
{

	if(damage==0)
	{
		int count;
		float p_attack_all=	(c1->p1->attack)	+	(c1->p1->exattack_turn)	+	(c1->p1->exattack_time);
	for(count=0;count<2;count++)//检查装备卡是否有加攻击力
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
		else//反伤
		{
			damage	=( p_defend_all - p_attack_all )*c1->p1->hurtmultiple_time*c1->p1->hurtmultiple_turn*c2->p1->hurtmultiple_time*c2->p1->hurtmultiple_turn;
			blood_change(-damage,c1,this_room);
		}
		
	}
	else//一般都是特殊值的超速普攻
	{
		blood_change(-damage,c2,this_room);
	}
	c1->p1->attacktime_turn++;//此回合普攻成功次数+1
	auto_judge(c1,i_hit_someone,&damage,locktime,this_room);//c2对某人成功造成了伤害，触发长者之镰或其他效果的被动	
	auto_judge(c1,c2,has_fought,this_room,locktime);//两人进行了一次搏斗，触发一些被动（例如 噬剑之剑）
	if(judge_one_die(c1,this_room)==one_die||judge_one_die(c2,this_room)==one_die)return one_die;
	return still_fight;

}


void recover_blood(float number,client_member* c1,fight_room* this_room)//c1增加血量
{
	if(number<0)
	{
		cout<<"恢复数值异常!为负!\n";
		number=-number;
	}
	blood_change(number,c1,this_room);
}

#endif