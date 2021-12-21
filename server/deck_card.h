#ifndef INCLUDE_deck_card_H
#define INCLUDE_deck_card_H

#include "special_transmit.h"

using namespace std;
//手卡变动：抽卡，丢卡，使用卡
//丢卡、使用卡后refresh
//抽卡、refresh后card_change信息传输
//card_change传输时更新隐效果
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓专门对卡组、抽卡做处理↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

int random_card(int i)//根据送入的随机数赋予卡片序号，部分卡片抽到的权重不同，可以适当调整
{
switch(i)
	{
	case 0:;
    case 1:return 60;;
	case 2:
	case 3:;
	case 4:return 30;
	case 5:;
	case 6:;
	case 7:return 61;;
	case 8:
	case 9:;
	case 10:return 0;
	case 11:;
	case 12:;
	case 13:return 62;
	case 14:;
	case 15:;
	case 16:return 31;
	case 17:;
	case 18:
	case 19:return 64;;
	case 20:
	case 21:;
	case 22:return 1; ;
	case 23:
	case 24:;
	case 25:return 63;
	case 26:;
	case 27:
	case 28:return 32;
	case 29:;
	case 30:return 65;;
	case 31:
	case 32:;
	case 33:return 2;;
	case 34:
	case 35:
	case 36:return 66;;;
	case 37:
	case 38:
	case 39:return 33;;;
	case 40:
	case 41:return 3;;
	case 42:;
	case 43:
	case 44:return 77;;;
	case 45:;
	case 46:
	case 47:return 34;;
	case 48:;
	case 49:;
	case 50:return 4;
	case 51:;
	case 52:;
	case 53:return 70;
	case 54:;
	case 55:;
	case 56:return 35;
	case 57:;
	case 58:;
	case 59:return 5;;
	case 60:
	case 61:;
	case 62:return 71;
	case 63:;
	case 64:return 72;
	case 65:;
	case 66:
	case 67:return 36;;
	case 68:
	case 69:;
	case 70:return 6;
	case 71:;
	case 72:;
	case 73:return 73;
	case 74:;
	case 75:return 37;
	case 76:;
	case 77:;
	case 78:return 75;
	case 79:;
	case 80:
    case 81:return 7;
	case 82:;
	case 83:;
	case 84:return 76;
	case 85:;
	case 86:;
	case 87:return 38;
	case 88:;
	case 89:;
	case 90:return 8;
    case 91:;
	case 92:;
	case 93:return 74;;
	case 94:;
	case 95:return 39;
	case 96:;
	case 97:;
	case 98:return 40;
	case 99:;
	case 100:return 9;
	default:return 0;


	}
}



void refresh_hideeffect(client_member* c1)//更新卡片隐效果
{
	int count;
	for(count=0;count<cardlimit;count++)
		if(c1->p1->c[count])
		{
			c1->p1->h[count]=true;
			c1->p1->hideeffect[count]=get_hideeffect(c1->p1->card[count]);
		}
		else
		{
			c1->p1->h[count]=false;
			c1->p1->hideeffect[count]=-1;
			count++;
			break;
		}
	for(;count<cardlimit;count++)
	{
		c1->p1->h[count]=false;
		c1->p1->hideeffect[count]=-1;
	}
}


int discard_all(client_member* c1,fight_room* this_room)//丢光手卡
{
	int count,count2;
	count2=0;
	if(auto_judge(c1,i_will_discard_card_A,this_room)!=i_immune_effect)//无免疫丢卡效果
	{
		for(count=0;count<cardlimit;count++)
			if(c1->p1->c[count]==true)
			{
				c1->p1->c[count]=false;
				count2++;
			}
			else
				break;
		refresh_card(c1,this_room);
	}
	else//如果免疫了丢光手卡效果
	{
		for(count=0;count<cardlimit;count++)
			if(c1->p1->c[count]==true)
			{
				
				count2++;//查看手卡数量，进行伤害数值计算
			}
			else
				break;
	}
	return count2;//有时候要统计自己丢了几张手卡

}

int supply_card()																			//随机刷新卡，加入deck卡组
{
	int i;
	srand((unsigned)time(NULL)*rand()); 
	i=random_card(rand()%(100-0));//随机取值0-100,查询该数字表示的卡片序列号
	return i;
}

int deck1[decklimit];//卡组，Q：为什么要预设卡组？不能直接随机发放？A：有些角色有“预测未来(即预测即将抽到的卡)”的能力，因此需要预设卡。

int garbage[garbagelimit];//墓地，废卡区，有待设计

void refresh_deck(int* deck)//玩家每抽1张卡后会用到该功能,即get_card函数才会用到该函数，用于刷新卡组最后一张
{
	int i;
	for(i=0;i<decklimit;i++)
		deck[i]=deck[i+1];//之前六张卡全部往前移一格
	deck[decklimit-1]=supply_card();//第七张卡随机生成
}

void ini_deck(int* deck)																					//全部重新洗牌
{
    //Sleep(1000);
	int i;
	for(i=0;i<decklimit;i++)
	deck[i]=supply_card();
	
}

void get_card(int number,client_member* c1,fight_room* this_room)															//人物从deck卡组抽卡
{	
	if(number<1){cout<<"抽卡异常，卡片数为："<<number<<endl;return;}

	char StrBuf[MAX_PATH] = { 0 };

	addtext(StrBuf,c1->p1->name);addtext(StrBuf,"抽");addtext(StrBuf,number);addtext(StrBuf,"卡!\n");
	room_broadcast(this_room,StrBuf);//广播c1抽取x卡
	
	int i=0;
	while(number>0)
	{
		if(c1->p1->c[cardlimit-1])//如果手卡满了
		{		
			if(c1->p1->un_move_able==0&&c1->p1->unmoveable_time==false)//如果可以行动，自主丢卡，如果不能，直接跳过抽卡环节
			{
					
				confirm_send_success(c1,"你手卡已达上限!可以丢弃手卡来继续抽卡!\n");
				i=cardlimit-1-discard_free_choose(c1,true,this_room);
			}
			if(c1->p1->c[cardlimit-1])
			{
				confirm_send_success(c1,"你不丢卡!放弃本次抽卡机会!\n");
				break;
			}//如果手卡还是满的，说明不想丢或不能丢，就停止继续抽卡，跳出死循环
			
		}
		if(!c1->p1->c[i])//如果玩家第i格卡槽为空，进行抽卡，否则检查下一个卡槽
		{
			c1->p1->card[i]=this_room->deck[0];
			c1->p1->c[i]=true;
			refresh_deck(this_room->deck);//玩家每抽1张卡，刷新卡组最后一张
			i++;
			number--;
			//auto_judge(c1,i_get_card);//需要在所有抽卡函数使用后再使用此函数进行被动效果发动
		}
		else
		{
			i++;//看后一卡槽有无卡
		}
			
	}//卡抽好了，或主动选择停止继续抽卡
	card_change(c1,this_room);//刷新手卡数据
}

void rule_get_card(client_member* c1,fight_room* this_room)//规则性抽卡
{
	int count2,count;
	count2=0;
	count=0;
			if(!c1->p1->c[2])//主回合的玩家不满3张手卡的情况下补满到5张，其余情况获取2张手卡
			{
				for(;count2<=4;count2++)
					if(!c1->p1->c[count2])count++;
				get_card(count,c1,this_room);
			}
			else
				get_card(2,c1,this_room);
}

void get_specific_card(int number,player* p1)													//获取指定卡片
{
	int i=0;
	while(i<cardlimit)
	{
		
		if(p1->c[i]==false)//如果玩家第i格卡槽为空，进行抽卡，否则检查下一个卡槽
		{
			p1->card[i]=number;//将指定卡片号为number的卡，给玩家p
			p1->c[i]=true;
			return ;
		}
		else
		{
			i++;
		}
		if(i==10){cout<<"该玩家手卡数量已达上限！无法继续抽卡"<<endl;return;}
	}
}

void show_card_name(player* p1)															//显示人物手卡名称，不显示效果
{
	
	int i;
	for(i=0;i<cardlimit;i++)
	if(p1->c[i]==true)cout<<"第"<<i+1<<"张:"<<search_card(p_name,p1->card[i])<<endl;
	
}
void refresh_card(client_member* c1,fight_room* this_room)                    //如果有卡使用后，需要马上使用这个函数,消耗手卡后重新整理手卡
{
	int i,k;

	k=1;
	for(i=0;i<cardlimit-1;i++)
		if(c1->p1->c[i]==false)
		{	//如果i卡槽空，而后第k个卡槽有卡，则将k卡槽的卡放入i卡槽，k卡槽置空，该卡槽重新置为非空
			while(c1->p1->c[k]==false)
			{
				k++;
				if(k==cardlimit)break;//到头了，强制退出
			}
			if(k==cardlimit)break;//到头了，强制退出

			c1->p1->card[i]=c1->p1->card[k];//k没到底，将k位上的卡移到第i卡槽
			c1->p1->c[i]=true;
			c1->p1->c[k]=false;
			k=i+2;//因为每次循环会i++，所以提前k=i+2
		}
		else 
		{
			k=i+2;//因为每次循环会i++，所以提前k=i+2
			if(k>=cardlimit)break;
		}
	card_change(c1,this_room);
}
void discard_card(client_member*c1,int whichcard,fight_room* this_room)//丢了这一卡槽的卡
{
	if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//免疫丢卡
		return;
	c1->p1->c[whichcard]=false;
	refresh_card(c1,this_room);
}
int use_card(client_member*c1,int whichcard,bool hide_effect,int* locktime,fight_room* this_room)//消耗了这一卡槽的卡
{
	char StrBuf[MAX_PATH]={0};

	int result=still_fight;
	int effect_cardnumber;
	int count;

	if(hide_effect)//如果是发动隐藏效果
		effect_cardnumber=c1->p1->hideeffect[whichcard];//提取卡号
	else
		effect_cardnumber=c1->p1->card[whichcard];//提取卡号

	if(judge_weapon(effect_cardnumber))//如果是武器，显示卡片效果
	{
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," 装备了武器: ");addtext(StrBuf,search_card(p_name,effect_cardnumber));addtext(StrBuf," !\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
	}
	else
	{
		for(count=0;count<playernumber;count++)
		{
			special_signal(this_room->c[count],card_launch);//广播【使用卡片】事件发生
		}
		addtext(StrBuf,c1->p1->name);if(c1->device!=win_console)addtext(StrBuf,"\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//memset(StrBuf,0,sizeof(StrBuf));//广播发送发动卡片者名字
		addtext(StrBuf,effect_cardnumber);addtext(StrBuf,"\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//广播他发动了哪个卡号，客户端自动处理显示卡名和详细效果
		*locktime=*locktime+1;//连锁次数+1
		addtext(StrBuf,"locktime=");addtext(StrBuf,*locktime);addtext(StrBuf,"\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));//广播连锁次数
	}

	c1->p1->c[whichcard]=false;//卡槽置空
	refresh_card(c1,this_room);//刷新手卡
	result=auto_judge(c1,i_card_used,this_room);//使用了手卡

	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	result=fight_back(c2,c1,sb_card_used,this_room,locktime);
	if(result!=i_failed_effect)//判断是否发动出来？如果发动出来
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->p1->name);addtext(StrBuf,"卡片发动成功!\n");
		room_broadcast(this_room,StrBuf);
		result=auto_judge(c1,NULL,i_card_used_S,this_room,locktime);//手卡使用成功
	}
	else
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->p1->name);addtext(StrBuf,"卡片发动失败!\n");
		room_broadcast(this_room,StrBuf);
	}

	//if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;//如果刚宣布手卡使用成功后就死了
	return result;
}

int discard_choose_weapon(client_member* c1,fight_room* this_room)//冷现巨剑专用，选择一张武器卡丢弃,返回丢了哪一把武器
{
	char StrBuf[MAX_PATH] = { 0 };
	int choice,card_number;

	if(c1->device==win_console)
		confirm_send_success(c1,"请丢弃1张装备卡!\n你选择:");
	else
	{
		confirm_send_success(c1,"请丢弃1张装备卡!\n");
		special_signal(c1,state_discard_one_card);//切换到丢一张卡的图形界面
	}
		while(true)
		{
			
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯
			
			if(StrBuf[0]=='0'&&strlen(StrBuf)==1){confirm_send_success(c1,"卡片效果已发动!无法取消!请重新选择：");continue;}//如果取消选择对象，直接返回
			choice=atoi(StrBuf);
			if(choice==0){confirm_send_success(c1,"输入无效字符!请重新选择：");continue;}
			choice--;
			if(choice>=cardlimit||choice<0){confirm_send_success(c1,"输入超出手卡上//下限！请重新选择手卡:");continue;}	
			if(c1->p1->c[choice]==false){confirm_send_success(c1,"抱歉，该手卡槽为空！请重新选择手卡:");continue;}
			if(judge_weapon(c1->p1->card[choice]))//输入该卡号，判断该手卡是否为装备卡
				{
					card_number=c1->p1->card[choice];					
					discard_card(c1,choice,this_room);//丢掉该手卡			
					break;
				}
			else
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"该卡不是装备卡!请重新选择:");
				else
					confirm_send_success(c1,"该卡不是装备卡!请重新选择!\n");
			}
		}
		if(c1->device!=win_console)special_signal(c1,state_return_normal);//图形界面按钮还原
		return card_number;
}




//返回card_discard丢卡成功，返回few_card丢卡失败，返回i_cancel_effect主动取消
int discard_choose(int number,client_member* c1,fight_room* this_room)//丢卡，由手卡持有者选择丢number数量的卡
{
	if(c1->member_No==robot_symbol)return i_cancel_effect;//如果是机器人，默认不会丢卡
	int choice;
    int cardnumber=0;//确认手卡总数
	int count,count2,count3;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int cachecard[cardlimit];
	int locktime=0;

	if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//是否免疫丢卡效果
		return card_discard;//免疫丢卡效果，直接算走完丢卡流程

	while(c1->p1->c[cardnumber]==true&&cardnumber<cardlimit)//确认手卡总数
		cardnumber++;

	if(cardnumber<number)
	{
		confirm_send_success(c1,"手卡数量不足!\n");
		return few_card;//手卡不足，无法丢卡
	}
	
	for(count=0;count<cardlimit;count++)//初始化选择项
		cachecard[count]=-1;

	if(c1->device==win_console)
		confirm_send_success(c1,"请选择需要丢的手卡(选择0表示“清除之前所有的选择”,再选一次0则表示“取消丢卡”):");
	else
	{
		addtext(StrBuf,"请丢");addtext(StrBuf,number);addtext(StrBuf,"张手卡!\n");
		confirm_send_success(c1,StrBuf);

		if(number==1)
			special_signal(c1,state_discard_one_card);//图形界面客户端专用
		else
			special_signal(c1,state_discard_muti_card);
	}
	count=0;
	while(number>0)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

		
		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			if(count==0)
			{
				confirm_send_success(c1,"已选择取消丢卡!\n");
				special_signal(c1,state_return_normal);
				return i_cancel_effect;//取消丢卡操作
			}
			for(;count>=0;count--)//清空已选项
			{cachecard[count]=-1;}
			confirm_send_success(c1,"原有选择已全部清除，请重新输入：");
			count=0;//统计已选手卡数量重新归零
			continue;
		}

		choice=atoi(StrBuf);
		if(choice==0){confirm_send_success(c1,"输入无效字符，请重新输入：");continue;}

		choice--;
		if(choice<0||choice>=cardlimit){confirm_send_success(c1,"输入越出手卡范围!请重新输入:");continue;}//跳过循环重新选卡
		if(!c1->p1->c[choice]){confirm_send_success(c1,"该卡槽为空!请重新输入:");continue;}
		
		count3=0;
		for(count2=0;count2<cardlimit;count2++)
		{
			if(cachecard[count2]==-1)break;//已进入没选择项，跳出循环检查
			if(cachecard[count2]==choice)
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"该卡已被选中!请选择其他卡:");
				else
					confirm_send_success(c1,"该卡已被选中!请选择其他卡!\n");
				count3=1;break;
			}
		}
		if(count3==1)continue;//count3=1表明该卡已被选中!跳过循环重新选卡

		cachecard[count]=choice;//登记已选卡号
		count++;
		if(number>count)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"还需要丢");addtext(StrBuf,number-count);
			if(c1->device==win_console)
				addtext(StrBuf,"张卡，请继续选择:");
			else
				addtext(StrBuf,"张卡，请继续选择!\n");
			confirm_send_success(c1,StrBuf);
		}
		else 
		{
			confirm_send_success(c1,"选择完成！\n");
			break;//选择完成,退出选卡死循环
		}
	}
	count=0;
    while(cachecard[count]!=-1&&count<11)
	{
		c1->p1->c[cachecard[count]]=false;//该卡槽置空，表明已经消耗
		count++;
	}

	special_signal(c1,state_return_normal);
	refresh_card(c1,this_room);
	return card_discard;//表示丢卡流程成功走完
}


int discard_free_choose(client_member* c1,bool full,fight_room* this_room)//丢卡，由手卡持有者选择丢任意数量的卡,full表明是否是因为手卡满了而自由丢弃
{
	if(c1->member_No==robot_symbol)return 0;//如果是机器人，默认不会丢卡

	if(!c1->p1->c[0])return 0;//没有手卡直接退出

	int choice;
    int cardnumber=0;//确认手卡总数
	int count,count2,count3;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int cachecard[cardlimit];

	while(c1->p1->c[cardnumber]==true&&cardnumber<cardlimit)//确认手卡总数
		cardnumber++;

	for(count=0;count<cardlimit;count++)//初始化选择项
		cachecard[count]=-1;

	if(c1->device==win_console)
		confirm_send_success(c1,"(输入y表示“确认完成选择”;输入0表示“清除之前所有的选择”;输入其他数字表示卡号)\n请选择:");
	else
		special_signal(c1,state_discard_free_card);
	count=0;
	while(true)
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}//玩家做出选择，如果此时断开通讯

	
		if(StrBuf[0]=='y'&&strlen(StrBuf)==1)break;//确认完成选择
		if(StrBuf[0]=='0'&&strlen(StrBuf)==1)
		{
			for(;count>=0;count--)//清空已选项
			{cachecard[count]=-1;}
			if(c1->device==win_console)
				confirm_send_success(c1,"原有选择已全部清除，请重新输入：");
			else
				confirm_send_success(c1,"原有选择已全部清除!\n");
			count=0;//统计已选手卡数量重新归零
			continue;
		}

		choice=atoi(StrBuf);
		if(choice==0){confirm_send_success(c1,"输入无效字符，请重新输入：\n");continue;}

		choice--;
		if(choice<0||choice>=cardlimit){confirm_send_success(c1,"输入越出手卡范围!请重新输入:\n");continue;}//跳过循环重新选卡
		if(!c1->p1->c[choice]){confirm_send_success(c1,"该卡槽为空!请重新输入:\n");continue;}
		
		count3=0;
		for(count2=0;count2<cardlimit;count2++)
		{
			if(cachecard[count2]==-1)break;//已进入没选择项，跳出循环检查
			if(cachecard[count2]==choice)
			{
				if(c1->device==win_console)
					confirm_send_success(c1,"该卡已被选中!请选择其他卡:");
				else
					confirm_send_success(c1,"该卡已被选中!请选择其他卡!\n");
				count3=1;break;
			}

		}
		if(count3==1)continue;//count3=1表明该卡已被选中!跳过循环重新选卡
		
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,"你选中了 ");addtext(StrBuf,search_card(p_name,c1->p1->card[choice]));addtext(StrBuf,"!\n请继续选择:");
		confirm_send_success(c1,StrBuf);
		cachecard[count]=choice;//登记已选卡号
		count++;//选中手卡数量+1
	}
	count=0;

	if(full)//如果是因为手卡满了没法抽卡而导致的自由丢手卡,免疫丢卡效果不发动
	{
		while(cachecard[count]!=-1&&count<cardnumber)
		{
			c1->p1->c[cachecard[count]]=false;//该卡槽置空，表明已经消耗
			count++;
		}

		refresh_card(c1,this_room);
	}

	if(auto_judge(c1,i_will_discard_card_A,this_room)!=i_immune_effect)//是否免疫丢卡效果
	{
		while(cachecard[count]!=-1&&count<cardnumber)
		{
			c1->p1->c[cachecard[count]]=false;//该卡槽置空，表明已经消耗
			count++;
		}

		refresh_card(c1,this_room);
	}
	else
	{
		special_signal(c1,state_return_normal);
		return cardnumber;
	}
	special_signal(c1,state_return_normal);
	return count;//丢卡流程成功走完,返回总丢卡数量
}



int random_choose_card(int cardnumber)									//随机刷新卡，加入deck卡组
{
	int i;
	srand((unsigned)time(NULL)*7*cardnumber*rand()); 
	i=rand()%(cardnumber-0);//随机取值0～手卡上限-1,查询该数字表示的卡片序列号
	return i;
}
void discard_random(int number,client_member* c1,fight_room* this_room)//丢卡，随机丢number数量的卡
{
	if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//是否有免疫丢卡效果
		return;//有则直接返回
    int cardnumber=0;//确认手卡总数
	int choice;
	while(c1->p1->c[cardnumber]==true&&cardnumber<cardlimit)//确认手卡总数
		cardnumber++;
	
	if(cardnumber<=number)//手卡不足，直接清空手卡即可
	{
		discard_all(c1,this_room);
		return;
	}

	while(number!=0)
	{
		choice=random_choose_card(cardnumber-1);
		if(!c1->p1->c[choice])continue;//如果该卡槽已经没卡，重新随机
		c1->p1->c[choice]=false;//该卡槽置空，表明已经丢弃
		c1->p1->card[choice]=-1;
		number--;
	}

	refresh_card(c1,this_room);
	
	return;//表示丢卡流程成功走完
}


void show_aims_card(client_member* c1,client_member* c2)//向c1展示c2所有手卡
{
	int count,cardnumber;
	char StrBuf[MAX_PATH]={0};
	if(c1->device==win_console)
	{
		confirm_send_success(c1,"被普攻者有以下手卡：\n\n");
		
		for(count=0;count<cardlimit;count++)
			if(c2->p1->c[count]==true)
			{
				addtext(StrBuf,"第");addtext(StrBuf,count+1);addtext(StrBuf,"张：");				
				addtext(StrBuf,search_card(p_name,c2->p1->card[count]));
				addtext(StrBuf,"\n");
				confirm_send_success(c1,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			}
	}
	else
	{
		cardnumber=0;//统计卡片总数

		special_signal(c1,show_rivals_card_P);
		for(count=0;count<cardlimit;count++)
			if(c2->p1->c[count]==true)
				cardnumber++;
			else
				break;
			cout<<"告诉他有多少张手卡！-----------------------\n";
		sp_transmit(c1,cardnumber);//告诉他对手有多少手卡
		for(count=0;count<cardnumber;count++)
		{
			cout<<"第"<<count<<"张手卡！-----------------------\n";
			sp_transmit(c1,c2->p1->card[count]);//传输具体手卡信息
		}
		cout<<"传输完成！-----------------------\n";
	}

}

void plunder_ones_card(client_member* c1,client_member* c2,int number,fight_room* this_room)//number为要抢他多少张卡，c1抢c2
{
	if(auto_judge(c2,i_will_public_card,this_room)==i_immune_effect)
		{confirm_send_success(c1,"抢夺手卡失败!\n");return ;}//看被普攻者能否防御公开手卡效果，如果能则抢夺失败
	if(c2->p1->c[0]==false){confirm_send_success(c1,"被普攻玩家无手卡，无法抢夺!\n");return ;}

	int count,choice;
	char StrBuf[MAX_PATH]={0};
	
		if(c1->p1->c[cardlimit-1]==true)
		{
			confirm_send_success(c1,"你手卡已满，无法继续抢夺!\n");
			if(discard_free_choose(c1,true,this_room)==0)//如果自己不丢手卡
				return ;//直接返回
		}

		show_aims_card(c1,c2);//向c1展示c2手卡

		if(c1->device==win_console)confirm_send_success(c1,"\n请选择夺取哪张:");

		while(true)//客户端选择夺取哪张
		{
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return;}//玩家做出选择，如果此时断开通讯


			if(strlen(StrBuf)==1&&StrBuf[0]=='0'){confirm_send_success(c1,"你选择取消抢夺手卡!\n");return;}

			choice=atoi(StrBuf);
			if(choice==0){confirm_send_success(c1,"输入无效字符，请重新输入:");continue;}
			choice--;
			if(choice<0||choice>=cardlimit){confirm_send_success(c1,"输入超出阈值，请重新输入:");continue;}
			if(c2->p1->c[choice]==false){confirm_send_success(c1,"该玩家的该卡槽为空，无法抢夺!请重新输入:");continue;}
			if(c2->p1->c[choice]==true)break;
		}
		memset(StrBuf,0,sizeof(StrBuf));
		addtext(StrBuf,c2->p1->name);addtext(StrBuf," 的手卡被抢夺了!\n");
		room_broadcast(this_room,StrBuf);

		if(c1->p1->w[weaponlimit-1]==true)//判断装备栏是否已满
		{	
			confirm_send_success(c1,"武器装备已满!抢夺的装备卡不会直接装备，而是加入手卡!\n");
			for(count=0;count<cardlimit;count++)
				if(c1->p1->c[count]==false)
					break;
			c1->p1->c[count]=true;//手卡交换环节
			c1->p1->card[count]=c2->p1->card[choice];
		}
		else 
			if(!judge_weapon(c2->p1->card[choice]))
			{
				confirm_send_success(c1,"抢夺的不是装备卡!\n");
				for(count=0;count<cardlimit;count++)
					if(c1->p1->c[count]==false)
						break;
				c1->p1->c[count]=true;//手卡交换环节
				c1->p1->card[count]=c2->p1->card[choice];
			}
			else
			{
				weapon_arm(c1,c2->p1->card[choice],this_room);

				memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," 气势汹汹地抢夺了");
				addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的武器: ");
				addtext(StrBuf,search_card(p_name,c2->p1->card[choice]));
				addtext(StrBuf," !并将其装备上进行接下来的普攻!\n");
				room_broadcast(this_room,StrBuf);
			}
		c2->p1->c[choice]=false;
		refresh_card(c2,this_room);refresh_card(c1,this_room);
	return ;
}

//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑对卡组、抽卡的处理↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

#endif