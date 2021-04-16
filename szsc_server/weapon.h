#ifndef INCLUDE_weapon_H
#define INCLUDE_weapon_H


#include "judge_situation.h"


//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓对武器做处理↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

void refresh_weapon(client_member* c1,fight_room* this_room)//玩家武器变化才会用到该函数，用于刷新武器槽
{
	
	int count,count2;
	for(count=0;count<weaponlimit-1;count++)
	if(c1->p1->w[count]==false&&c1->p1->w[count+1]==true)
	{
		//如果某武器槽为空，而后一武器槽有装备，则将后一武器槽的装备数据放入前一个武器槽，后一武器槽置空，该武器槽重新置为非空
		c1->p1->weapon[count]=c1->p1->weapon[count+1];//武器号赋予
		c1->p1->w[count]=true;
		c1->p1->exattack_weapon[count]=c1->p1->exattack_weapon[count+1];//武器攻击力赋予
	    c1->p1->exattack_weapon_turn[count]=c1->p1->exattack_weapon_turn[count+1];//武器该回合临时攻击力赋予
		c1->p1->exattack_weapon_time[count]=c1->p1->exattack_weapon_time[count+1];//武器该次搏斗临时攻击力赋予
		


		c1->p1->weaponeffect001[count]=c1->p1->weaponeffect001[count+1];//承皇剑剩余效果1次数赋予
		c1->p1->weaponeffect002[count]=c1->p1->weaponeffect002[count+1];//承皇剑剩余效果2次数赋予
		c1->p1->weaponeffect051[count]=c1->p1->weaponeffect051[count+1];//5号食腐再生装置1效果剩余次数
		c1->p1->weaponeffect052[count]=c1->p1->weaponeffect052[count+1];//5号食腐再生装置2效果剩余次数
		c1->p1->weaponeffect06[count]=c1->p1->weaponeffect06[count+1];//6号恩空法棒魔力指示物个数


		for(count2=0;count2<weaponeffectlimit;count2++)//效果赋予
		{
			c1->p1->w_e[count][count2]=c1->p1->w_e[count+1][count2];
			c1->p1->weapon_effect[count][count2]=c1->p1->weapon_effect[count+1][count2];
			c1->p1->w_e[count+1][count2]=false;
		}
		c1->p1->w[count+1]=false;//后一槽置空
		c1->p1->weapon[count+1]=-1;

	}
	weapon_change(c1,this_room);
}


void delete_weapon(client_member* c1,int whichweapon,fight_room* this_room)//删除某个槽内的一把武器,该槽号为whichweapon
{
	int count;
	if(whichweapon>weaponlimit||whichweapon<0){cout<<"异常，输入超出上下限\n";return;}
	if(c1->p1->w[whichweapon]==false){cout<<"异常，该卡槽没有卡";return;}

	c1->p1->w[whichweapon]=false;
	c1->p1->weapon[whichweapon]=-1;
	c1->p1->exattack_weapon[whichweapon]=0;//所有装备武器增加的攻击力置0
	c1->p1->exattack_weapon_turn[whichweapon]=0;//所有装备武器该回合增加的攻击力置0
	c1->p1->exattack_weapon_time[whichweapon]=0;//所有装备武器该次搏斗增加的攻击力置0

	c1->p1->weaponeffect001[whichweapon]=0;//承皇剑效果1次数置0
	c1->p1->weaponeffect002[whichweapon]=0;//承皇剑效果2次数置0
	c1->p1->weaponeffect051[whichweapon]=0;//5号食腐再生装置1效果剩余次数
	c1->p1->weaponeffect052[whichweapon]=0;//5号食腐再生装置2效果剩余次数
	c1->p1->weaponeffect06[whichweapon]=0;//6号恩空法棒魔力指示物个数


	for(count=0;count<weaponeffectlimit;count++)//效果清空
	{
		c1->p1->w_e[whichweapon][count]=false;
		c1->p1->weapon_effect[whichweapon][count]=-1;
	}
	
	refresh_weapon(c1,this_room);//刷新武器栏			
}


void delete_weapon_all(client_member* c1,fight_room* this_room)//删除该玩家全部武器
{
	int count,count2;

	for(count=0;count<weaponlimit;count++)
	{
		c1->p1->w[count]=false;//武器全部清空
		c1->p1->weapon[count]=false;
		c1->p1->w_valid[count]=true;//有效状态
		c1->p1->exattack_weapon[count]=0;//所有装备武器增加的攻击力置0
		c1->p1->exattack_weapon_turn[count]=0;//所有装备武器该回合增加的攻击力置0
		c1->p1->exattack_weapon_time[count]=0;//所有装备武器该次搏斗增加的攻击力置0
		for(count2=0;count2<weaponeffectlimit;count2++)
		{
			c1->p1->w_e[count][count2]=false;//所有装备效果清空
			c1->p1->weapon_effect[count][count2]=-1;
		}
		c1->p1->weaponeffect001[count]=0;//承皇剑效果1次数置0
		c1->p1->weaponeffect002[count]=0;//承皇剑效果2次数置0
		c1->p1->weaponeffect051[count]=0;//5号食腐再生装置1效果剩余次数
		c1->p1->weaponeffect052[count]=0;//5号食腐再生装置2效果剩余次数
		c1->p1->weaponeffect06[count]=0;//6号恩空法棒魔力指示物个数
	}
	weapon_change(c1,this_room);
}



bool weapon_arm(client_member* c1,int choice,fight_room* this_room)//将所给的武器号choice进行装备
{

	int count;
	int locktime=0;
	bool arm_success=false;


	for(count=0;count<weaponlimit;count++)
	{if(c1->p1->w[count]==false)break;}//如果检测到该装备槽为空，则选择该空槽装备上
	
	c1->p1->w[count]=true;//先表明已不为空
	c1->p1->weapon[count]=choice;//该武器槽设为该武器的卡号

	switch(choice)//根据武器卡号效果增加额外攻击力
	{
	case 0:c1->p1->exattack_weapon[count]=1;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1002;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1003;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1004;
		   c1->p1->weaponeffect001[count]=1;c1->p1->weaponeffect002[count]=4;
	   	   arm_success=true;break;//承皇剑,+1,
	case 1:c1->p1->exattack_weapon[count]=1;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1012;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1013;
		   arm_success=true;break;//疾风刃,+1
	case 2:c1->p1->exattack_weapon[count]=2;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1022;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1023;
		   arm_success=true;break;//长者之镰+2
	case 3:c1->p1->exattack_weapon[count]=1;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1032;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1033;
	   	   arm_success=true;break;//噬剑之剑+1
	case 4:c1->p1->exattack_weapon[count]=2;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1043;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1044;
		   arm_success=true;break;//黑曜剑,血-2，攻+2,
	case 5:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1051;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1052;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1053;
			c1->p1->weaponeffect051[count]=1;//5号食腐再生装置1效果剩余次数
			c1->p1->weaponeffect052[count]=1;//5号食腐再生装置2效果剩余次数
		   arm_success=true;break;//食腐再生装置
	case 6:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1061;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1062;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1063;
		   c1->p1->weaponeffect06[count]=2;//魔力指示物初始为2
		   arm_success=true;break;//恩空法棒
	case 7:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1071;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1072;
		   c1->p1->w_e[count][2]=true;c1->p1->weapon_effect[count][2]=1073;
		   arm_success=true;break;//骷髅诡面
	case 8:c1->p1->exattack_weapon[count]=0;
		   c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1081;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1082;
		   arm_success=true;break;//破败披风
	case 9:c1->p1->exattack_weapon[count]=2;
           c1->p1->w_e[count][0]=true;c1->p1->weapon_effect[count][0]=1092;
		   c1->p1->w_e[count][1]=true;c1->p1->weapon_effect[count][1]=1093;
		   arm_success=true;break;//冠阳剜月斧+2
	case 10:c1->p1->exattack_weapon[count]=2;arm_success=true;break;//幻影剑+2
		
	}

	if(arm_success)weapon_change(c1,this_room);
		
	if(choice==4)//黑曜剑血-3
		if(auto_judge(c1,i_will_be_effect_A,this_room)!=i_immune_effect)//如果没防御自主伤血的能力，自主伤血效果发动
		{
			hit_damage(3,c1,NULL,this_room,&locktime,"黑曜剑汲取了装备者的生命力!\n",i_will_be_effect_A);
		}
	//如果装备上刚好死了
	if(c1->p1->blood<=0&&c1->p1->overdeath==0)auto_judge(c1,i_will_die,this_room);

	if(arm_success)return true;

	
cout<<"错误的编号!武器装备失败!choice="<<choice<<endl;
return false;
}

void show_him_his_buff_effect(client_member* c1)
{
	char StrBuf[MAX_PATH] = { 0 };
	int result=still_fight;

	if(c1->device!=win_console){special_signal(c1,show_buff_list);return;}

			addtext(StrBuf,"你当前的加附效果有:\n");
			if(c1->p1->hideshield>0)
			{addtext(StrBuf,"1.隐天盾（剩余");addtext(StrBuf,c1->p1->hideshield);addtext(StrBuf,"轮回）\n");}

			addtext(StrBuf,"请问你想发动哪个？（选0取消发动）\n你选择：");
			confirm_send_success(c1,StrBuf);
}





void show_him_his_weapon_effect(client_member* c1)
{
	if(c1->device!=win_console){special_signal(c1,show_weapon_list);return;}//UI客户端显示武器总览
	int count,count2;
	char StrBuf[MAX_PATH] = { 0 };
	confirm_send_success(c1,"你的武器有:\n");
		for(count=0;count<weaponlimit;count++)//检查装备卡槽是否为空
			if(c1->p1->w[count]==true)//若不为空，输出该武器号
			{
				addtext(StrBuf,"武器");
				addtext(StrBuf,count+1);
				addtext(StrBuf,":");
				addtext(StrBuf,search_card(p_name,c1->p1->weapon[count]));
				addtext(StrBuf,"\t");
				confirm_send_success(c1,StrBuf);
			}
		confirm_send_success(c1,"\n\n");
		for(count=0;count<weaponlimit;count++)
		{
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"武器");
			addtext(StrBuf,count+1);
			addtext(StrBuf,":\t");
			for(count2=0;count2<weaponeffectlimit;count2++)
			{	
				if(c1->p1->w_e[count][count2]==true)//进行角色装备武器的效果显示，如果有效果则显示
				{
					addtext(StrBuf,"效果");
					addtext(StrBuf,count2+1);
					addtext(StrBuf,":");
					addtext(StrBuf,search_skill(c1->p1->weapon_effect[count][count2]));
					addtext(StrBuf,"\n");
					confirm_send_success(c1,StrBuf);
					
					memset(StrBuf,0,sizeof(StrBuf));
					
				}
				else 
					break;
				addtext(StrBuf,"\t");//对齐后续效果
			}

		}
	confirm_send_success(c1,"\n\n请输入想发动第几个武器(输入0表示‘取消该操作’):");

}

#endif