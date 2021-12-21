#ifndef INCLUDE_player_general_function_H
#define INCLUDE_player_general_function_H

#include "No_instruction.h"

using namespace std;
//一些通用函数（通用普攻在launch_sth_effect.h内）
int ask_whether_do(client_member* c1,int situation)//询问是否要这样做
{
	char StrBuf[MAX_PATH]={0};
	char message_ask[MAX_PATH]={0};
	char message_do[MAX_PATH]={0};
	char message_not[MAX_PATH]={0};
	switch(situation)
	{
		case do_attack:
			if(c1->device==win_console)
				addtext(message_ask,"是否需要对其发动普攻？（0：不发动；1：发动）\n你选择：");	
			else
				addtext(message_ask,"是否需要对其发动普攻？\n");
			addtext(message_not,"你选择不发动普攻!\n");
			addtext(message_do,"你选择发动普攻!\n");
			break;
		case make_shield:
			if(c1->device==win_console)
				addtext(message_ask,"是否现在发动隐天盾？（0：不发动；1：发动）\n你选择:");	
			else
				addtext(message_ask,"是否现在发动隐天盾？\n");
			addtext(message_not,"你选择不发动隐天盾!\n");
			addtext(message_do,"你选择发动隐天盾!\n");
			break;
		

		default:cout<<"???奇怪的选择项目，situation="<<situation<<endl;
	}

	if(c1->device!=win_console)special_signal(c1,chooseYN);

	confirm_send_success(c1,message_ask);
		while(true)
		{
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

				if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,message_not);return cancel_choice;}//取消选择对象，无效化对己伤血
				if(StrBuf[0]=='1'&&strlen(StrBuf)==1){confirm_send_success(c1,message_do);return do_choice;}//确认选择

				confirm_send_success(c1,"输入无效!请重新输入：\n");
		}
}
int choose_enemy(client_member* c1,char* message,fight_room* this_room)//选择指定的对象(该函数为通用函数u),不包括自己
{
	int count;
	int choice;
	char StrBuf[MAX_PATH]={0};
	
	if(c1->device==win_console)
	{
		confirm_send_success(c1,"\n可以选择的对象:\n");
		for(count=0;count<playernumber;count++)
			if(this_room->c[count]->p1->player_No!=c1->p1->player_No)//不能选择自己
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
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你取消了选择!\n");return i_cancel_effect;}//如果取消选择对象，直接返回

		choice=atoi(StrBuf);//化字符串为数字//传回player_No
		if(choice==0){confirm_send_success(c1,"输入无效字符！请重新选择:");continue;}

		//如果选择了正确的对象，显示成功，并跳出死循环	
		//if(choice<0||choice>playernumber){confirm_send_success(c1,"输入超出上\\下限！请重新选择:");continue;}
		for(count=0;count<playernumber;count++)
		{
			if(this_room->c[count]->p1->player_No==choice)
			{
				if(this_room->c[count]->p1->player_No!=c1->p1->player_No)
					return count;
			}
				
		}

		//如果没选择正确的对象，则玩家重新做选择
		if(c1->device==win_console)
			confirm_send_success(c1,"选择了错误的对象!请重新选择:");
		else
			confirm_send_success(c1,"选择了错误的对象!请重新选择!\n");
		
	}
}
int choose_someone(client_member* c1,char* message,fight_room* this_room)//选择指定的对象(该函数为通用函数u)，包括自己
{
	int count;
	int choice;
	char StrBuf[MAX_PATH]={0};

	if(c1->device==win_console)
	{
		confirm_send_success(c1,"\n可以选择的对象:\n");

		for(count=0;count<playernumber;count++)//能选择自己
			if(this_room->c[count]->p1->alive==true)
			{
				addtext(StrBuf,count+1);
				addtext(StrBuf,". ");
				addtext(StrBuf,search_skill(this_room->c[count]->p1->character_number));
				addtext(StrBuf,"\n");
			}
		confirm_send_success(c1,StrBuf);
		confirm_send_success(c1,"\n请选择对象(选0表示取消该操作):");
	}

	while(true)
	{
		if(c1->device!=win_console)
		{
			special_signal(c1,show_alive_list);
			confirm_send_success(c1,message);
		}

		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

		if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"你取消了选择!\n");return i_cancel_effect;}//如果取消选择对象，直接返回

		choice=atoi(StrBuf);//化字符串为数字
		if(choice==0){confirm_send_success(c1,"输入无效字符！请重新选择:");continue;}
		//如果选择了正确的对象，显示成功，并跳出死循环
		//if(choice<0||choice>playernumber){confirm_send_success(c1,"输入超出上\\下限！请重新选择:");continue;}
		for(count=0;count<playernumber;count++)
		{
			if(this_room->c[count]->p1->player_No==choice)
				return count;
		}
		//如果没选择正确的对象，则玩家重新做选择
		if(c1->device==win_console)
			confirm_send_success(c1,"选择了错误的对象!请重新选择:");
		else
			confirm_send_success(c1,"选择了错误的对象!请重新选择!\n");
	}
}

bool whether_it_happen(int percent)//发生概率，返回是否发生
{
	srand((unsigned)time(NULL)*rand()); //随机取值0-100
	int result=rand()%(100-0);
	
	if(percent>result)
		return true;
	else
		return false;
}
bool check_robot(client_member* c1)//这位玩家是否是系统机器人？
{
	if(c1->member_No==robot_symbol)return true;
	return false;
}
//void AI_think(fight_room* this_room){}

void addtext(char* StrBuf,int number)//追加整数
{
	char Buf[MAX_PATH] = { 0 };
	sprintf_s(Buf,"%d",number);
	strcat(StrBuf,Buf);
}
void addtext(char* StrBuf,char* Buf)//追加字符串
{strcat(StrBuf,Buf);}
void addtext(char* StrBuf,float number)//追加小数
{
	char Buf[MAX_PATH] = { 0 };
	int d,f;
	d=int(floor(number));
	f=int((number-floor(number))*100);
	addtext(StrBuf,d);
	addtext(StrBuf,".");
	addtext(StrBuf,f);
}

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓对回合的处理↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	

void turnsettle(client_member* c1,fight_room* this_room)														//回合结束时所有玩家状态结算
{
	int count;
	bool whether_weapon_change=false;//武器是否有变化

	c1->p1->fight_chance=0;//可普攻的次数置0
	if(c1->p1->un_move_able>0)c1->p1->un_move_able--;//不可行动的回合数-1

	if(c1->p1->selfeffect_invalid>0)
		c1->p1->selfeffect_invalid--;//自身效果被无效化的回合-1
	else
		c1->p1->selfeffect_invalid=0;

		c1->p1->hurtmultiple_turn=1;//该回合自己造成的普攻伤血倍率置1
		c1->p1->hurtmultiple_time=1;//该次行动自己造成的普攻伤血倍率置1

		c1->p1->attacktime_turn=0;//此回合自己普攻成功次数置0
		attack_change(-c1->p1->exattack_turn,&c1->p1->exattack_turn,c1,this_room);//该回合自身增加的额外攻击力置0

	for(count=0;count<weaponlimit;count++)
		if(c1->p1->w[count]==true)
		{
			if(c1->p1->weapon[count]==0){c1->p1->weaponeffect001[count]=1;}//承皇剑“丢、抽卡”机会为1
			if(c1->p1->weapon[count]==5){c1->p1->weaponeffect051[count]=1;c1->p1->weaponeffect052[count]=1;}//食腐再生装置机会为1
			if(c1->p1->weapon[count]==10){delete_weapon(c1,count,this_room);count--;}//幻影剑消失,删除后看看是否还有幻影剑

			if(c1->p1->exattack_weapon_turn[count]!=0){c1->p1->exattack_weapon_turn[count]=0;whether_weapon_change=true;}//所有装备武器该回合增加的攻击力置0
			if(c1->p1->exattack_weapon_time[count]!=0){c1->p1->exattack_weapon_time[count]=0;whether_weapon_change=true;}//所有装备武器该次搏斗增加的攻击力置0
		}

	if(whether_weapon_change)
		weapon_change(c1,this_room);
		

}



void natural_set_playerturn(fight_room* this_room)							//自然变更玩家回合
{
	int count;
	bool whether_buff_change=false;

	//一般回合结算，对所有人物状态结算
	for(count=0;count<playernumber;count++)
		turnsettle(this_room->c[count],this_room);//每个人回合结算，特效清除结算
	
	for(count=0;count<playernumber;count++)
	{
		if(this_room->c[count]->p1->whether_myturn==true)//确认到是谁的主回合
		{
			this_room->c[count]->p1->whether_myturn=false;//不再是他的主回合
			if(count!=playernumber-1)//如果不是最后一个人的回合
			{
				this_room->c[count+1]->p1->whether_myturn=true;//后一个人作为主回合拥有者
				return;
			}
			if(count==playernumber-1)//如果是最后一个人的回合，轮回数增加
			{
				this_room->c[0]->p1->whether_myturn=true;//最初那个人变为主回合
				this_room->passturns++;
				for(count=0;count<playernumber;count++)//轮回特有结算，全部数据更新，更新完即轮回特有结算结束
				{
					if(this_room->c[count]->p1->overdeath>0){whether_buff_change=true;this_room->c[count]->p1->overdeath--;}//不死状态
					if(this_room->c[count]->p1->hideshield>0){whether_buff_change=true;this_room->c[count]->p1->hideshield--;}//隐天盾
					if(this_room->c[count]->p1->godlaser>0){whether_buff_change=true;this_room->c[count]->p1->godlaser--;}//神尚激光
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

//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑对回合的处理↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑


//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓处理玩家各种信息↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

void ini_character(player* p1,int playerNo)								//对传入的玩家p的各种状态进行初始化
{
	p1->player_No=playerNo;//第几个玩家
	p1->alive=true;//置为存活状态
	p1->fight_chance=0;//普攻次数置0
	p1->whether_myturn=false;//当前不是你的回合
	p1->un_move_able=0;//不可行动的回合
	p1->unmoveable_time=false;//当前行动是否不可行动置0
	p1->selfeffect_invalid=0;//自身效果被无效化的回合置0
	p1->exattack_turn=0;//该回合自身增加的额外攻击力置0
	p1->exattack_time=0;//该次搏斗自身增加的额外攻击力置0
	p1->whether_use_assist=-1;//当前没有使用助攻卡

	p1->overdeath=0;//不死状态可持续轮回数量
	p1->whether_in_attack=false;//不处于搏斗状态
	p1->whether_attack_T=false;//自己不处于穿攻状态
	p1->attacktime_turn=0;//此回合自己普攻成功次数置0
	p1->hideshield=0;////隐天盾剩余回合数置0
	p1->godlaser=0;//神尚激光剩余轮回数
	p1->godlaserdamage=0;//神尚激光造成伤害数
	p1->hurtmultiple_turn=1;//该回合伤害倍率为1
	p1->hurtmultiple_time=1;//该次伤害倍率为1
	p1->cannot_ED=false;//此次攻击无法躲避格挡置否
	p1->i_soon_die=false;//自己即将死亡标志符置否

	int i,k;
	for(i=0;i<cardlimit;i++)
	{
		p1->c[i]=false;//手卡全部清空
		p1->card[i]=-1;
		p1->h[i]=false;//隐效果清空
		p1->hideeffect[i]=-1;
	}

	for(i=0;i<abilitylimit;i++)
	{
		p1->a[i]=false;//个人效果全部清空
		p1->ability[i]=-1;
	}
	
	for(i=0;i<weaponlimit;i++)
	{
		p1->w_valid[i]=true;//个人武器全部置为有效
		p1->w[i]=false;//武器全部清空
		p1->weapon[i]=-1;
		p1->exattack_weapon[i]=0;//所有装备武器增加的攻击力置0
		p1->exattack_weapon_turn[i]=0;//所有装备武器该回合增加的攻击力置0
		p1->exattack_weapon_time[i]=0;//所有装备武器该次搏斗增加的攻击力置0
		for(k=0;k<weaponeffectlimit;k++)
		{
			p1->w_e[i][k]=false;//所有装备效果清空
			p1->weapon_effect[i][k]=-1;
		}
		p1->weaponeffect001[i]=0;//承皇剑“丢、抽卡”机会为0
		p1->weaponeffect002[i]=0;//承皇剑“丢、抽卡”机会为0
		p1->weaponeffect051[i]=0;//食腐再生装置1机会为0
		p1->weaponeffect052[i]=0;//食腐再生装置2机会为0
		p1->weaponeffect06[i]=0;//6号恩空法棒魔力指示物个数
	}

}

void clean_buff(player* p1)
{
	p1->un_move_able=0;//不可行动的回合
	p1->unmoveable_time=false;//当前行动是否不可行动置0
	p1->selfeffect_invalid=0;//自身效果被无效化的回合置0
	p1->exattack_turn=0;//该回合自身增加的额外攻击力置0
	p1->exattack_time=0;//该次搏斗自身增加的额外攻击力置0
	p1->whether_use_assist=-1;//当前没有使用助攻卡
	p1->overdeath=0;//不死状态可持续轮回数量

	p1->hideshield=0;////隐天盾剩余回合数置0
	p1->godlaser=0;//神尚激光剩余轮回数
	p1->godlaserdamage=0;//神尚激光造成伤害数

	p1->cannot_ED=false;//此次攻击无法躲避格挡置否

}


void set_buff(client_member* c1,int situation,int number,fight_room* this_room)//设置c1状态，situation为状态，number为持续回合
{
	bool whether_change=false;
	switch(situation)
	{
		case buff_clean_all://个人buff全清空
			clean_buff(c1->p1);
			whether_change=true;break;//
		case silence_turn://自身效果被沉默回合
			if(c1->p1->selfeffect_invalid>number)break;
			c1->p1->selfeffect_invalid=number;
			whether_change=true;break;//
		case hideshield_turn:
			if(c1->p1->hideshield>=number)break;
			c1->p1->hideshield=number;//隐天盾持续时间        
			whether_change=true;break;
		case unmove_time://该次行动内无法行动
			if(auto_judge(c1,i_will_be_unmoveable,this_room)==i_immune_effect)break;//如果有免疫无法行动的效果
			c1->p1->unmoveable_time=true;
			whether_change=true;break;
		case unmove_turn://回合内无法行动
			if(auto_judge(c1,i_will_be_unmoveable,this_room)==i_immune_effect)break;//如果有免疫无法行动的效果
			if(c1->p1->un_move_able<number)
				c1->p1->un_move_able=number;
			whether_change=true;break;
		case overdeath_turn://不死状态
			if(c1->p1->overdeath>=number)break;
			c1->p1->overdeath=number;
			whether_change=true;break;
	}
	if(whether_change)
		buff_change(c1,this_room);
	return;
}


void fight_data_clean(player* p1,player* p2,bool clean,fight_room* this_room)//该次普攻产生的特效数值被消除
{
	int count;

	//p1->exattack_time=0;p2->exattack_time=0;
	attack_change(-this_room->c[0]->p1->exattack_time,&this_room->c[0]->p1->exattack_time,this_room->c[0],this_room);//该次攻击临时威力清空
	attack_change(-this_room->c[1]->p1->exattack_time,&this_room->c[1]->p1->exattack_time,this_room->c[1],this_room);//该次攻击临时威力清空
	
	if(clean)//是否是完全结束
	{p1->unmoveable_time=false;p2->unmoveable_time=false;}//该次行动内“不可行动”状态恢复
	p1->whether_attack_T=false;p2->whether_attack_T=false;
	p1->hurtmultiple_time=1;p2->hurtmultiple_time=1;//该次普攻增加的伤害倍率重置为1
	p1->cannot_ED=false;p2->cannot_ED=false;//无法格挡和躲避置否
	for(count=0;count<weaponlimit;count++)//武器该次攻击的临时增加威力清空
	{
		p1->exattack_weapon_time[count]=0;p2->exattack_weapon_time[count]=0;
		p1->w_valid[count]=true;p2->w_valid[count]=true;//重新有效化
	}
	p1->whether_use_assist=-1;p2->whether_use_assist=-1;//清除发动了的助攻卡
}


void show_character(player* p1,bool i)															//显示人物状态
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//设置字体颜色（只在显示对手时改变）
	cout<<"-------------------------------------------------------\n";
	cout<<"----------------------";
	if(i)
		cout<<"主人";
	else 
	{
		SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | FOREGROUND_RED );//对手用亮红表示
		cout<<"宾客";//根据传入的bool i判别是自己还是对手
	}

	cout<<"的状态:";
	SetConsoleTextAttribute(handle,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);////恢复默认白
	cout<<"----------------------\n";
	cout<<"-------------------------------------------------------\n";
    cout<<" "<<search_character(p1->character_number)<<endl;
	cout<<" 血量："<<p1->blood;

	int count,count2;
	float p_attack_all=	(p1->attack)	+	(p1->exattack_turn)	+	(p1->exattack_time);
	for(count=0;count<weaponlimit;count++)//检查装备卡是否有加攻击力
		if(p1->w[count]==true)
			p_attack_all=	p_attack_all	+	(p1->exattack_weapon[count])+p1->exattack_weapon_turn[count];
	cout<<"    总攻："<<p_attack_all<<"	";

	if(p1->overdeath>1)cout<<p1->overdeath<<"轮回不死之身!";
	if(p1->overdeath==1)cout<<"仅本轮回不死之身!";

	cout<<endl<<endl;

	for(count=0;count<abilitylimit;count++)
		if(p1->a[count]==true)//进行角色自身效果显示，如果有效果则显示
			cout<<"角色自身效果"<<count+1<<"："<<search_skill(p1->ability[count])<<endl;
			cout<<endl;

	bool whether_enter=false;
	for(count=0;count<weaponlimit;count++)
		if(p1->w[count]==true)
			{cout<<"武器"<<count+1<<": "<<search_card(p_name,p1->weapon[count]);
		     if(p1->weapon[count]==6)
				 cout<<"(充能+"<<p1->weaponeffect06[count]<<")"<<"  ";
			 else
				 cout<<"(攻+"<<p1->exattack_weapon[count]+p1->exattack_weapon_turn[count]<<")"<<"  ";
			 whether_enter=true;}
	if(whether_enter)cout<<endl;
	for(count=0;count<weaponlimit;count++)
		{
		if(p1->w[count]==true)
		{
			cout<<"武器"<<count+1;
			if(p1->weapon[count]==10){cout<<"\t 无特殊效果\n";continue;}
			for(count2=0;count2<weaponeffectlimit;count2++)
				{
				if(p1->w_e[count][count2]==true)//进行角色装备武器的效果显示，如果有效果则显示
					{cout<<"\t"<<" 效果"<<count2+1<<"："<<search_skill(p1->weapon_effect[count][count2])<<endl;}
				else 
					break;
				}
		}
		}

	if(!i)//统计对手有多少手卡
	{
		for(count=0;count<cardlimit;count++)
			if(p1->c[count]!=true)
				break;
		cout<<"他目前有"<<count<<"张手卡\n";
	}

}




//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑处理玩家各种信息↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
#endif