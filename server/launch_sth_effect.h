#ifndef INCLUDE_launch_sth_effect_H
#define INCLUDE_launch_sth_effect_H

#include "weapon.h"
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓对各种效果的处理↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

int launch_weapon_effect(client_member* c1,int card_No,int whichweapon,int situation,int* locktime,fight_room* this_room)//发动武器效果
{
	int result,count,choice;

	char StrBuf[MAX_PATH]= { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(c2==NULL)return force_offline;

	switch(card_No)
	{
	case 1002://1回合限1次，自己可丢1手卡，抽1卡（承皇剑1）

				result=discard_choose(1,c1,this_room);//选择丢1卡
				if(result!=card_discard)
					return result;

				addtext(StrBuf,c1->p1->name);addtext(StrBuf," 发动承皇剑的1号效果:");addtext(StrBuf,search_skill(card_No));addtext(StrBuf,"\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

				get_card(1,c1,this_room);//抽一张卡
				auto_judge(c1,i_get_card,this_room);
				c1->p1->weaponeffect001[whichweapon]--;//发动成功，武器效果1回合内可使用次数-1

				return still_fight;

	case 1003://自己可挡4次普攻，抵挡第4次普攻后此卡被破坏（承皇剑2）
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"用承皇剑格挡了普攻!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			if(c1->p1->weaponeffect002[whichweapon]>1)
				c1->p1->weaponeffect002[whichweapon]--;
			else
				{room_broadcast(this_room,"此效果次数用完，承皇剑被破坏!\n");delete_weapon(c1,whichweapon,this_room);}
			return i_failed_attack_D;
 
	case 1004://【自己回合可发动】自己主动将此卡破坏，指定1人伤1血（承皇剑3）
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"已取消该操作!\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
				//c2=this_room->c2;
			//else
				//c2=this_room->c1;
            delete_weapon(c1,whichweapon,this_room);//删除一把承皇剑
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 飞出了手中的承皇剑!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			addtext(StrBuf,"承皇剑成功击中了");addtext(StrBuf,c2->p1->name);addtext(StrBuf," 并造成了伤血!\n");//如果命中，显示这句话
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血

			return result;

	case 1012:  //"1回合限2次，自己可丢1手卡，自己此回合攻+1";疾风刃1
			
			result=discard_choose(1,c1,this_room);//选择丢1卡
				if(result!=card_discard)
					return result;

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 屏息凝神……\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"大地上的气流正逐渐往其手中的 疾风刃 上聚合!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"匕刃上的气劲更加猛烈了!\n");
			room_broadcast(this_room,StrBuf);

			c1->p1->exattack_weapon_turn[whichweapon]++;
			weapon_change(c1,this_room);

			return still_fight;

	case 1013:  //"【自己回合可发动】自己主动将此卡破坏，指定1人伤1血";疾风刃2
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"已取消该操作!\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
				//c2=this_room->c2;
			//else
				//c2=this_room->c1;

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 飞出了手中的疾风刃!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//删除一把疾风刃
			
			addtext(StrBuf,"疾风刃成功击中了");addtext(StrBuf,c2->p1->name);addtext(StrBuf," 并造成了伤血!\n");//如果命中，显示这句话
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血

			return result;
		
	case 1022: cout<<"1长者之镰1\n"; //"自己每使1人伤1血，自己+1血";长者之镰1
	case 1023:  //【自己回合可发动】自己主动将此卡破坏，指定1人+3血";长者之镰2

			choice=choose_someone(c1,"请指定一人+3血\n",this_room);
			if(choice==i_cancel_effect||choice==force_offline){return choice;}
			c2=this_room->c[choice];

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 献祭了手中的长者之镰!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//删除一把长者之镰

			addtext(StrBuf,"长者之镰中的生命精华涌入了");addtext(StrBuf,c2->p1->name);addtext(StrBuf," 的体内!他的生命值回复了!\n");
			room_broadcast(this_room,StrBuf);
			recover_blood(3,c2,this_room);//血量增加3

			return still_fight;
	case 1032: cout<<"1007噬剑之剑1\n"; //"自己每次与人搏斗过后限1次，随机破坏 与自己搏斗者 装备的1张装备卡";//噬剑之剑1
	case 1033:  ;//"【自己回合可发动】自己主动将此卡破坏，指定1人伤1血";//噬剑之剑2	
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"已取消该操作!\n");return still_fight;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
				//c2=this_room->c2;
			//else
				//c2=this_room->c1;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 飞出了手中的噬剑之剑!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//删除一把噬剑之剑

			addtext(StrBuf,"噬剑之剑成功击中了");addtext(StrBuf,c2->p1->name);addtext(StrBuf," 并造成了伤血!\n");//如果命中，显示这句话
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血

			return result;

	case 1043:  //"此次游戏限1次可发动，发动回合过后，自己持续不死2轮回";//黑曜剑1
			set_buff(c1,overdeath_turn,3,this_room);//不死状态更新为3

			room_broadcast(this_room,"黑曜剑发出了前所未有的光芒!\n");//该效果可主动触发，当自己血量归0时自动触发
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"  感到自己体内充满了无穷的力量!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"黑曜剑失去了以往的光泽…………\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			c1->p1->w_e[whichweapon][1]=false;c1->p1->weapon_effect[whichweapon][0]=1010;//原有效果失效，仅剩丢剑伤害效果
			c1->p1->weapon_effect[whichweapon][1]=-1;
			weapon_change(c1,this_room);
			
			return still_fight;

	case 1044: ;//"【自己回合可发动】自己主动将此卡破坏，指定1人50%伤2血";//黑曜剑2
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"已取消该操作!\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
					//c2=this_room->c2;
				//else
				//	c2=this_room->c1;

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 飞出了手中的黑曜剑!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//删除一把黑曜剑
			
			if(whether_it_happen(50))
			{
				addtext(StrBuf,"黑曜剑成功击中了");	addtext(StrBuf,c2->p1->name);addtext(StrBuf," 并造成了伤血!\n");//如果命中，显示这句话
				result=hit_damage(2,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血

				return result;			
			}
			else
				room_broadcast(this_room,"可惜没击中…………\n");
			return still_fight;

	case 1051: cout<<"1011食腐再生装置1\n";//"自己每抽1卡，自己血+1";//食腐再生装置1
	case 1052: //1回合限1次，自己可丢1手卡，血+1;//食腐再生装置2
			if(c1->p1->blood==c1->p1->bloodlimit){confirm_send_success(c1,"系统提示：血量为满!建议取消发动该效果!\n");}
			
			result=discard_choose(1,c1,this_room);//选择丢1卡
				if(result!=card_discard)
					return result;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 选择发动食腐再生装置的2号效果!(1回合限1次，自己可丢1手卡，血+1)\n");
			room_broadcast(this_room,StrBuf);
			if(c1->p1->blood!=c1->p1->bloodlimit)
			{
				room_broadcast(this_room,"发动成功!他的生命值恢复了!\n");
				recover_blood(1,c1,this_room);//血量增加1
			}
			else
				room_broadcast(this_room,"发动成功!但他生命值没有增加!\n");

			c1->p1->weaponeffect051[whichweapon]--;//发动成功，武器效果1回合内可使用次数-1
			return still_fight;

	case 1053: //"自己回合限1次，自己可额外抽1卡";//食腐再生装置3
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 选择发动食腐再生装置的3号效果!(自己回合限1次，自己可额外抽1卡)\n");
			room_broadcast(this_room,StrBuf);

			get_card(1,c1,this_room);//抽一张卡
			auto_judge(c1,i_get_card,this_room);

			room_broadcast(this_room,"发动成功!\n");
			
			c1->p1->weaponeffect052[whichweapon]--;

			return still_fight;

	case 1061: cout<<"1014恩空法棒1\n";//"此卡被装备上时给此卡放置4魔力指示物";//恩空法棒1

	case 1062: cout<<"1015恩空法棒2\n";//"己方每发动1手卡，给此卡放置1魔力指示物";//恩空法棒2

	case 1063: ;//"【自己回合可发动】自己消耗2魔力指示物使1人伤1血";//恩空法棒3
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"已取消该操作!\n即将返回主页面…………\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
			//	c2=this_room->c2;
			//else
			//	c2=this_room->c1;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 举起法棒释放了法术冲击波!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"祈法.恩空之力!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			c1->p1->weaponeffect06[whichweapon]=c1->p1->weaponeffect06[whichweapon]-2;//消耗2魔力指示物
			weapon_change(c1,this_room);

			addtext(StrBuf,"法术能量成功击中了");addtext(StrBuf,c2->p1->name);addtext(StrBuf," 并造成了伤血!\n");//如果命中，显示这句话
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血

			return result;

	case 1071: cout<<"1017骷髅诡面1\n";//"自己不受任何丢卡和手卡公开的效果影响";//骷髅诡面1
	case 1072: cout<<"1018骷髅诡面2\n";//"【场上除己外1人死亡时】自己血量回满";//骷髅诡面2
	case 1073: cout<<"1019骷髅诡面3\n";//"自己不受任何自主伤血效果影响";//骷髅诡面3
	case 1081: cout<<"1020破败披风1\n";//"自己可装备无限数量装备卡"; //破败披风1
	case 1082: //"【自己受到普攻时可发动】自己主动将此卡破坏，自己抵挡此次普攻";//破败披风2
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 用破败披风格挡了普攻!破败披风被破坏了!\n");
			room_broadcast(this_room,StrBuf);
			delete_weapon(c1,whichweapon,this_room);
			return i_failed_attack_D;
	case 1092: cout<<"1022冠阳剜月斧1\n";//"【自己每普攻1次】所有人丢3手卡，抽3卡，卡堆最上方3张卡送入废卡区";//冠阳剜月斧1
	case 1093: //"【自己回合可发动】自己主动将此卡破坏，指定1人伤2血,场上所有人丢5手卡，抽5卡";//冠阳剜月斧2
			addtext(StrBuf,c1->p1->name);
			addtext(StrBuf," 折断了手中的稀世珍宝:冠阳剜月!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"周围环境开始风云骤变!仿佛在一瞬间内过了千秋万代!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"所有人随机各丢 5 张手卡!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			delete_weapon(c1,whichweapon,this_room);//删除一把冠阳剜月
			
			for(count=0;count<playernumber;count++)
			{
				discard_random(5,this_room->c[count],this_room);
				confirm_send_success(this_room->c[count],"周遭环境的变化又让所有人出现了新的契机!每个人各抽5张手卡!\n");
				get_card(5,this_room->c[count],this_room);auto_judge(this_room->c[count],i_get_card,this_room);
			}

			return still_fight;
	}
	cout<<"武器效果没发动成功!card_No="<<card_No;
	return still_fight;
}



int character_effect(client_member* c1,int card_No,fight_room* this_room)			//自己主动发动各种人物效果
{
	switch(card_No)
	{
	case 10000:cout<<"发动了不该发动的效果10000!\n";return still_fight;
	case 10001:cout<<"发动了不该发动的效果10001!\n";return still_fight;
	case 10002:cout<<"发动了不该发动的效果10002!\n";return still_fight;
	}
	cout<<"自身效果发动失败!card_No="<<card_No;
	return card_No;
}


//发动助攻卡效果,assist为助攻卡号，stage为前阶效果或后续效果
//stage:  after_effect后续效果    fore_effect 前阶效果
int launch_assist_effect(client_member* c1,client_member* c2,int card_No,int stage,int* locktime,fight_room* this_room)
{
	int count,count2;
	int result;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };

	switch(card_No)
	{
	case 30://使 被攻者 伤1血（被攻者可丢一卡躲避此次伤血），若命中，则 被攻者 在此次普攻结束前不能行动
		if(stage==fore_effect)
		{
			
				confirm_send_success(c1,"对方思考中……\n");
				confirm_send_success(c2,"是否丢一手卡躲避“1血伤害和此次普攻结束前不能行动”效果？\n");
				result=discard_choose(1,c2,this_room);
				if(result!=card_discard)//三种情况：手卡不足，不丢卡，断线
				{
					if(result==force_offline)return force_offline;

					result=hit_damage(1,c2,c1,this_room,locktime,"受到对面地动天摇的攻势影响，在此次普攻结束前不能行动!\n",i_will_be_effect_B);//扣血
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;
					if(judge_one_die(c2,this_room)==one_die)return one_die;//受到伤血效果，可能会死
					set_buff(c2,unmove_time,0,this_room);
				}		
			return i_attack;
		}
		return i_attack;
	case 31://在此次普攻结束前自己攻+1，自己查看 被攻者 的所有手卡，并选其中1张加入自己手卡，若选中装备卡，则可给自己装备上
		if(stage!=fore_effect)return still_fight;
	
		attack_change(1,&c1->p1->exattack_time,c1,this_room);//攻+1

		plunder_ones_card(c1,c2,1,this_room);//夺取手卡1张
		return i_attack;

	case 32:;//"此次普攻内自己装备的所有  卡无效化，普攻结束后  卡重新有效化且 被攻者 受到自己当前装备  卡数量×2的血伤
		if(stage==fore_effect)
		{	
			count2=0;
			for(count=0;count<cardlimit;count++)
				if(c1->p1->w[count]==true){c1->p1->w_valid[count]=false;count2++;}

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 飞出了手中的所有武器!\n");
			room_broadcast(this_room,StrBuf);

			if(count2==0)	
				room_broadcast(this_room,"但他并没有装备任何武器……\n");
			else
				weapon_change(c1,this_room);
			return i_attack;
		}
		if(stage==after_effect)
		{
			count2=0;//统计飞出去几把武器
			for(count=0;count<weaponlimit;count++)//装备复原
				if(c1->p1->w[count]==true)//如果该槽有装备
				{
					c1->p1->w_valid[count]=true;//装备效果复原
					count2++;
				}
			if(count2==0)return i_attack;//自己其实没装备武器，直接返回，不计算伤害

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 之前飞出去的武器又飞了回来!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			weapon_change(c1,this_room);

			addtext(StrBuf,"击穿了");addtext(StrBuf,c2->p1->name);addtext(StrBuf," 的身体,造成了");addtext(StrBuf,count2*2);addtext(StrBuf,"点伤害!\n");
			result=hit_damage(float(2*count2),c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;

			return i_attack;
		}

	case 33://【自己装备武器时可发动】自己抽1卡，使 被攻者 伤1血（被攻者可丢一卡躲避此次伤血）
		if(stage==fore_effect)
		{
			get_card(1,c1,this_room);
			auto_judge(c1,i_get_card,this_room);

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 挥出一道气刃!击向");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf," !\n");
			room_broadcast(this_room,StrBuf);


				confirm_send_success(c1,"对方思考中……\n");
				confirm_send_success(c2,"是否丢一手卡躲避“1血伤害”效果？\n");
				result=discard_choose(1,c2,this_room);
				if(result!=card_discard)
				{
					if(result==force_offline)return force_offline;
					result=hit_damage(1,c2,c1,this_room,locktime,"受到气刃伤害!血-1!\n",i_will_be_effect_B);//扣血
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;
				}
				else
					room_broadcast(this_room,"他躲避了气刃伤害!\n");
			
			return i_attack;
		}
		if(stage==after_effect)
		{
			return i_attack;
		}
	case 34://本次攻击自己攻+1,普攻成功后30%使被普攻者接下来的此回合和下回合不能行动
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//攻+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(30))
			{
				addtext(StrBuf,"极寒加护特殊效果发动成功!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," 被凝冰封锁了行动!接下来的此回合和下回合不能行动!\n");
				room_broadcast(this_room,StrBuf);

				set_buff(c2,unmove_turn,2,this_room);
			}
			return i_attack;	
		}
		
	case 35://return "本次攻击自己攻+2,普攻成功后50%使被普攻者随机丢1卡\n\n";
		if(stage==fore_effect)
		{
			attack_change(2,&c1->p1->exattack_time,c1,this_room);//攻+2
			return i_attack;
		}
		if(stage==after_effect)
		{

			if(whether_it_happen(50))
			{
				addtext(StrBuf,"雷霆加护特殊效果发动成功!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," 被雷电的余威所震慑!随机丢弃1张手卡!\n");
				room_broadcast(this_room,StrBuf);
				discard_random(1,c2,this_room);
			}
			return i_attack;
		}
		
	case 36://return "本次攻击自己攻+1,普攻成功后40%使被普攻者伤1血\n\n";
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//攻+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(40))
			{
				addtext(StrBuf,"炽焰加护特殊效果发动成功!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," 被火焰的余焰所烧伤!\n");//命中时广播这句话
				result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
				if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;
			}
			return i_attack;
		}
	case 37://return "本次攻击自己攻+1,普攻成功后使被普攻者随机丢2卡，抽2卡\n\n";
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//攻+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(100))
			{
				addtext(StrBuf,"流水加护特殊效果发动成功!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," 被强烈的水流冲击打乱了阵势!被迫随机丢弃2张手卡!并获得新的契机!抽2卡!\n");
				room_broadcast(this_room,StrBuf);

				discard_random(2,c2,this_room);//随机丢弃2张手卡
				get_card(2,c2,this_room);
				auto_judge(c2,i_get_card,this_room);
			}	
			return i_attack;
		}
	case 38://return "本次攻击自己攻+1,普攻成功后自己抽1卡\n\n";
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//攻+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 收到圣光的祝福!获得了新契机!抽1卡!\n");
			room_broadcast(this_room,StrBuf);

			get_card(1,c1,this_room);
			auto_judge(c1,i_get_card,this_room);

			return i_attack;
		}		
	case 39://return "本次攻击自己攻+2,普攻成功后30%使被普攻者装备的武器全部破坏\n\n";
		if(stage==fore_effect)
		{
			attack_change(2,&c1->p1->exattack_time,c1,this_room);//攻+1
			return still_fight;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(30))
			{
				addtext(StrBuf,"冥幽加护特殊效果发动成功!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," 被冥渊的诅咒所侵染!所持武器全部被破坏!\n");
				room_broadcast(this_room,StrBuf);

				delete_weapon_all(c2,this_room);			
			}
			return i_attack;
		}
		
	case 40://神空斩  此次普攻无法被躲避、格挡，自己总攻击力减半且变为穿攻
		if(stage!=fore_effect)return still_fight;
		
		c1->p1->whether_attack_T=true;c2->p1->cannot_ED=true;
		c1->p1->hurtmultiple_time=float(c1->p1->hurtmultiple_time*0.5);
		room_broadcast(this_room,"他在普攻中化为了光……\n");
		return i_attack;
		
	}
cout<<"助攻卡序号错误异常!card_No="<<card_No;
return still_fight;
}




int launch_card_effect(client_member* c1,client_member* c2,int card_No,int situation,int* locktime,fight_room* this_room)//发动手卡
{

	//c1为发动者，c2为被发动者，卡号，发动时的条件，连锁次数，这个房间
	if(card_No>=0&&card_No<=29){confirm_send_success(c1,"该卡为装备卡，在自己回合选择'3.装备武器'才可发动!\n");return false;}
	if(card_No>=30&&card_No<=59){confirm_send_success(c1,"该卡为助攻卡，在自己普攻别人时才可发动!\n");return false;}
	
	char StrBuf[MAX_PATH] = { 0 };
	int result=still_fight;
	int count;
	int card_number;
	bool whether_get_hurt=false;//双臂剑专用
	float p_attack_all=0;//光斩绝杀专用
	int choice;
	
	if(c1->member_No==this_room->c[0]->member_No)//多人模式下需要修改！
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(c2==NULL)return force_offline;


	switch(card_No)
	{
	case 60://神起·焰射\n【自己普攻被躲避时可发动】

			discard_all(c1,this_room);//丢光手卡
			room_broadcast(this_room,"他释放出自己的全部能量!熔浆如流星般溅射到大地上!\n");
			room_broadcast(this_room,"其他人的装备全都被熔化损毁了!\n");
			delete_weapon_all(c2,this_room);//删除其他人武器，，多人模式下需要修改！
			room_broadcast(this_room,"受到滚烫熔浆的烧伤，其他所有人血-1!\n");

			result=hit_damage(1,c2,c1,this_room,locktime,NULL,i_will_be_effect_B);//扣血
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

			return force_end_event;
	case 61://近击·搏杀\n【自己参与的1次搏斗内双方发动过大于4次连锁时 可发动】
		room_broadcast(this_room,"正当两人在连锁中怒斗时，他来了一记瞬拳搏杀!\n");
		
		result=lightspeed_attack(c1,c2,3,locktime,this_room);
		room_broadcast(this_room,"此回合被强制结束!!!\n");
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_turn;
	case 62://"[效果卡]阴势·突进\n【自己回合时可发动】自己指定1人-1血且此回合不能行动（被攻者可丢1手卡躲避此次伤血），接下来的此回合内自己只能对被指定者或自己发动效果卡、搏斗\n\n";
			/*while(true)
			{
				choice=choose_enemy(c1,this_room);
				if(choice==force_offline)return force_offline;
				if(choice==i_cancel_effect){confirm_send_success(c1,"已发动!无法取消!请重新选择：\n");continue;}
				break;
			}
			if(this_room->player_all[choice]->player_No==this_room->c1->p1->player_No)
				c2=this_room->c1;
			else
				c2=this_room->c2;*/
			room_broadcast(this_room,"阴郁气息环绕在了两人间!\n");

			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"需要丢1手卡才能摆脱深渊之力的束缚!\n");
			room_broadcast(this_room,StrBuf);
			memset(StrBuf,0,sizeof(StrBuf));

			confirm_send_success(c2,"请选择1手卡丢弃来摆脱深渊之力的束缚!\n");
				confirm_send_success(c1,"对方思考中……\n");
				result=discard_choose(1,c2,this_room);
				if(result!=card_discard)//如果没丢卡成功
				{
					if(result==force_offline)return force_offline;
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"受到了深渊之力的压制(本回合无法行动且血-1)!\n");
					result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
					
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
					if(result!=i_failed_effect_B)//如果没抵御伤害，行动受限制
						set_buff(c2,unmove_turn,1,this_room);//增加不能行动buff
				}
				else
					room_broadcast(this_room,"他摆脱了深渊之力的压制!\n");
			
			return still_fight;
		
	case 63:// "[效果卡]神空·闪降\n【自己回合时可发动】自己抽1卡，将抽到的该卡公开，自己这回合的规则性普攻次数被重置为1\n\n";
			c1->p1->fight_chance=1;//自己这回合的规则性普攻次数被重置为1

			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"抽取一卡!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			get_card(1,c1,this_room);
			auto_judge(c1,i_get_card,this_room);

			if(auto_judge(c1,i_will_public_card,this_room)!=i_immune_effect)//公开手卡效果是否受影响
			{
				for(count=cardlimit-1;count>=0;count--)
				{
					if(c1->p1->c[count]==true)
					{
						addtext(StrBuf,"将该卡公开!该卡为:\n");
						room_broadcast(this_room,StrBuf);
						memset(StrBuf,0,sizeof(StrBuf));
						addtext(StrBuf,"【");
						addtext(StrBuf,search_card(p_name,c1->p1->card[count]));
						addtext(StrBuf,"】!!!\n");
						room_broadcast(this_room,StrBuf);
						break;
					}
				}
			}

			return still_fight;

	case 64:// "[效果卡]空遁·闪回\n【自己受到普攻时可发动】自己抽1卡且躲避此次搏斗，自己可对 发动该次普攻者 普攻1次\n\n";
		room_broadcast(this_room,"他飞向空中，躲避了普攻!\n");
		get_card(1,c1,this_room);
		auto_judge(c1,i_get_card,this_room);
		confirm_send_success(c2,"对方思考中……\n");
		result=ask_whether_do(c1,do_attack);
		if(result==force_offline)return result;

		room_broadcast(this_room,"又曲折迂返，进行回击!\n");
		result=general_attack(c1,c2,nothing_else,locktime,this_room);
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_event;


	case 65:// "[效果卡]光斩·绝杀\n【自己受到致命伤血时可发动】此次伤血无效化且此回合自己基础攻击力×2，自己可对 使自己受到致命伤血者 普攻1次\n\n";
		room_broadcast(this_room,"希望之光划破天际\n");
		confirm_send_success(c2,"对方思考中……\n");
		result=ask_whether_do(c1,do_attack);
		if(result==force_offline)return result;
		
		p_attack_all=	(c1->p1->attack)	+	(c1->p1->exattack_turn)	+	(c1->p1->exattack_time);//攻击力翻倍
		
		for(count=0;count<2;count++)//检查装备卡是否有加攻击力
			if(c1->p1->w[count]==true&&c1->p1->w_valid[count]==true)
				p_attack_all=	p_attack_all	+   c1->p1->exattack_weapon[count] + c1->p1->exattack_weapon_turn[count] + c1->p1->exattack_weapon_time[count];
		
		attack_change(p_attack_all,&c1->p1->exattack_turn,c1,this_room);

		if(result==cancel_choice)return i_failed_damage;//仅仅无效化伤害，不做普攻
		room_broadcast(this_room,"背水之阵，绝地回击!\n");

		result=general_attack(c1,c2,nothing_else,locktime,this_room);
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_event;
	case 66:// "[效果卡]天华绝伦\n【自己一回合内成功普攻超过2次时可发动】自己指定1人，自己此回合可 超速普攻 其4次，每次超速普攻对其造成的普攻伤血强制变为1且不会造成反伤\n\n";
		/*while(1)
		{
			confirm_send_success(c1,"请选择普攻的对象:\n");
			count=choose_enemy(c1,this_room);
			if(count==force_offline)return force_offline;
			if(count==i_cancel_effect)
				{confirm_send_success(c1,"卡片效果已发动!无法取消!\n");continue;}

			if(this_room->player_all[count]->player_No==this_room->c1->p1->player_No)
				c2=this_room->c1;
			else
				c2=this_room->c2;
			break;
		}*/
		room_broadcast(this_room,"刷刷刷快速四刀……\n");
		result=lightspeed_attack(c1,c2,4,locktime,this_room);
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_event;
	case 67:// "[效果卡]神尚激光\n【己方回合时可发动】自己丢光手卡，此回合之后的3轮回内自己若受到致命伤血，使自己受到致命伤血者 将受到之前自己丢光手卡数值×2的伤血\n\n";
			if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//玩家是否受丢卡效果影响？
			count=discard_all(c1,this_room);//如果受，丢卡，计算丢卡数额

			c1->p1->godlaserdamage=c1->p1->godlaserdamage + count*2;
			c1->p1->godlaser=3;//回合数不会叠加，只会重置到3
			return still_fight;
			
	case 68:// "[效果卡]轰炸星球\n【自己回合时可发动】破坏场上所有场景卡和结界卡，每破坏1张，使 放置该结界或场景者 伤1血\n\n";
		room_broadcast(this_room,"场上检测不到场景或结界卡……\n");return still_fight;
	case 69:cout<<"69最后的指望\n";// "[效果卡]最后的指望\n【自己处于濒死状态时可发动】自己指定1人，将自己全部手卡、自己装备的武器全部加入他手卡\n\n";
	case 70:// "[效果卡]双臂剑\n自己血-3，将卡堆最上方的2张武器卡装备上\n\n";
		
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"腾空跃起，张开双臂!\n剑来!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
		
			if(c1->p1->w[weaponlimit-1]==true){room_broadcast(this_room,"武器栏已满!没有武器能够从远方飞来!此手卡白发动!");return true;}//如果装备栏本来就是满的

			if(auto_judge(c1,i_will_be_effect_A,this_room)!=i_immune_effect)//如果没防御自主伤血的能力
			{
				whether_get_hurt=true;
			}

			for(count=0;count<weaponlimit;count++)//将武器装备
			{
				if(c1->p1->w[count]==false)
				{
					srand((unsigned)time(NULL)*rand()); //随机取装备
					card_number=rand()%(10-0);
				
					addtext(StrBuf,"从远处飞来了 ");addtext(StrBuf,search_card(p_name,card_number));addtext(StrBuf," !并装备到了他身上!\n");

					weapon_arm(c1,card_number,this_room);
					room_broadcast(this_room,StrBuf);
					memset(StrBuf,0,sizeof(StrBuf));
				}
			}

			if(whether_get_hurt)//如果没防御自主伤血的能力
			{

				result=hit_damage(3,c1,NULL,this_room,locktime,"由于飞来的武器速度过快，他受到了3血伤害!\n",i_will_be_effect_A);//扣血
				if(result==one_die){room_broadcast(this_room,"却最终因失血过多而亡!\n");return one_die;}
			}
			return still_fight;
	
	case 71:// "[效果卡]离心续\n【自己普攻成功时可发动】自己再进行1次普攻，此次普攻造成的普攻伤血翻倍\n\n";
			c1->p1->hurtmultiple_time=c1->p1->hurtmultiple_time*2;//伤害翻倍
			result=general_attack(c1,c2,nothing_else,locktime,this_room);
			if(result==force_offline||result==one_die||result==force_end_turn)return result;
			return force_end_event;
	case 72:;// "[效果卡]虚混·太清\n【自己受到普攻时可发动】自己躲避本次普攻，自己丢光手卡，发动该次普攻者 的血量减半且武器全部破坏\n\n";
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"运用极意之力扭曲碎化了周围的空间!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"受不稳定空间波动的影响，血量减半!且武器全部被破坏!\n");
			room_broadcast(this_room,StrBuf);
			discard_all(c1,this_room);//丢光手卡
			blood_change(-(c2->p1->blood/2),c2,this_room);//血量减半
			delete_weapon_all(c2,this_room);//武器全部破坏
			return i_failed_attack_E;//被躲避
	case 73:// "[效果卡]幻影剑\n将此卡作为 攻+2 的武器给自己装备上，在装备上此卡的回合结束时，此卡自动被破坏\n\n";
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"抽出一把幻影剑!\n");
			room_broadcast(this_room,StrBuf);
		
			weapon_arm(c1,10,this_room);
			return still_fight;	
			
	case 74:// "[效果卡]冷现·巨剑\n【自己受到效果伤血时可发动】自己丢1手卡中的装备卡，指定1人伤3血\n\n";
		
		/*confirm_send_success(c1,"请指定被攻击对象!\n");
		while(1)
		{
			count=choose_enemy(c1,this_room);
			if(count==force_offline)return force_offline;
			if(count==i_cancel_effect)
				{confirm_send_success(c1,"卡片效果已发动!无法取消!\n");continue;} 
			break;
		}
		if(c1->p1->player_No==this_room->c1->p1->player_No)
			c2=this_room->c2;
		else
			c2=this_room->c1;
		*/
		card_number=discard_choose_weapon(c1,this_room);
		if(card_number==force_offline)return force_offline;

		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"召唤出冰霜附魔的武器 ");
		addtext(StrBuf,search_card(p_name,card_number));addtext(StrBuf," !并飞了出去!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"被击中!受到了2血伤害!\n");
		result=hit_damage(2,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
		if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

		return still_fight;//不会主动造成事件逾期

	case 75:;// "[效果卡]隐天盾\n此卡发动的回合以及发动后的2轮回内，自己可丢1手卡使 1次对己的效果伤血 无效化或抵挡1次普攻\n\n";
			set_buff(c1,hideshield_turn,3,this_room);
			result=ask_whether_do(c1,make_shield);
			if(result==force_offline)return force_offline;
			if(result==cancel_choice)return still_fight;

			result=discard_choose(1,c1,this_room);
			if(result!=card_discard){return result;}
			
			room_broadcast(this_room,"他动用契机临时制出了一块透明无暇又坚硬无比的防御盾!\n");
			
			if(situation==i_will_be_attacked)
			{room_broadcast(this_room,"他成功格挡了普攻!\n");return i_failed_attack_D;}	//格挡普攻
			if(situation==i_will_be_effect_B)
			{room_broadcast(this_room,"他成功抵御了伤血效果!\n");return i_failed_effect_B;}//无效化伤血效果	
			confirm_send_success(c1,"在不必要的场合下发动该效果，没啥用\n");return still_fight;
			
	
	case 76:// "[效果卡]破局·神击\n【自己普攻失败时可发动】自己普攻 使自己普攻失败者 1次，且此次普攻内 被攻者 无法行动。自己可丢任意数量的手卡，使 被攻者 受到丢卡数量伤血\n\n";		
			if(c1->member_No==this_room->c[0]->member_No)
				c2=this_room->c[1];
			else
				c2=this_room->c[0];
			if(c2==NULL)return force_offline;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"打破了混乱的局势!对");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"发动了最终的猛烈一击!\n");
			room_broadcast(this_room,StrBuf);

			set_buff(c2,unmove_time,0,this_room);//被攻者无法行动

			confirm_send_success(c1,"请丢任意数量的手卡!\n");
			count=discard_free_choose(c1,false,this_room);
			if(count>0)result=hit_damage(float(count),c2,c1,this_room,locktime,"他受到了破格级的猛烈伤害！\n",i_will_be_effect_B);//扣血
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

			result=general_attack(c1,c2,nothing_else,locktime,this_room);
			if(result==one_die||result==force_offline||result==force_end_turn)return result;
			return force_end_event;
	case 77://"[效果卡]旋地·回击\n【自己受到普攻时可发动】自己抽1卡，自己可普攻 发动普攻者 1次，本次攻击自己攻+1";
			
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," 尽可能减少普攻的冲击对自己的影响，重新划地为圈 稳住阵势，嗅探新的契机\n");
			room_broadcast(this_room,StrBuf);
			get_card(1,c1,this_room);
			result=ask_whether_do(c1,do_attack);
			if(result==force_offline)return result;

			room_broadcast(this_room,"调整阵势后，他进行了回击！");
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//攻击力+1
			result=general_attack(c1,c2,nothing_else,locktime,this_room);
			if(result==one_die||result==force_offline||result==force_end_turn)return result;
			return force_end_event;	
	case 400://连攻
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," 发动了连续攻击!\n");
		room_broadcast(this_room,StrBuf);
		result=general_attack(c1,c2,nothing_else,locktime,this_room);
		return result;

	case 401://闪
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," 闪避了攻击!\n");
		room_broadcast(this_room,StrBuf);
		return i_failed_attack_E;
	case 402://+4血
			
			choice=choose_someone(c1,"请指定一人+4血\n",this_room);
			if(choice==i_cancel_effect)return still_fight;
			if(choice==force_offline){return choice;}
			c2=this_room->c[choice];

			addtext(StrBuf,c2->p1->name);addtext(StrBuf," 回复4点血量!\n");
			room_broadcast(this_room,StrBuf);
			recover_blood(4,c2,this_room);//血量增加4
			return still_fight;
	case 403://403 暗枪

		addtext(StrBuf,c1->p1->name);addtext(StrBuf," 出其不意甩出了暗能法球!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"受到了法球的伤害!\n");
		result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
		return result;
	case 404://404 稻草人
		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"攻击到的其实是幻象稻草人!自身装备被全部破坏\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		delete_weapon_all(c2,this_room);

		return i_failed_attack_E;
	case 405://405 憎恨
		if(situation==sb_card_used)
			return i_failed_effect;
		else
		{
			special_signal(c1,which_to_hatred);
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}
			switch(atoi(StrBuf))
			{
				case 1://对面人物效果
					memset(StrBuf,0,sizeof(StrBuf));
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的人物效果被无效化1回合!\n");
					room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					set_buff(c2,silence_turn,1,this_room);break;
				case 2://对手人物buff
					memset(StrBuf,0,sizeof(StrBuf));
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的人物加附效果被清空了!\n");
					room_broadcast(this_room,StrBuf);
					set_buff(c2,buff_clean_all,0,this_room);break;
				case 3://自身人物buff
					memset(StrBuf,0,sizeof(StrBuf));
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的人物加附效果被清空了!\n");
					room_broadcast(this_room,StrBuf);
					set_buff(c1,buff_clean_all,0,this_room);break;
				case 4://破坏对面一把武器
					special_signal(c1,get_w_number);
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}
					choice=atoi(StrBuf);
					choice--;
					if(choice>=0&&choice<weaponlimit&&c2->p1->w[choice]&&c2->p1->w_valid[choice])
					{
						memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的武器");addtext(StrBuf,search_card(p_name,c2->p1->weapon[choice]));addtext(StrBuf,"被破坏了!\n");
						room_broadcast(this_room,StrBuf);
						delete_weapon(c2,choice,this_room);
					}
					else
					{
						cout<<"错误的回复："<<choice<<endl;
					}
					
					break;
				default:cout<<"错误的选择："<<atoi(StrBuf)<<endl;break;
			}
		}
		if(judge_one_die(c1,this_room)==one_die||judge_one_die(c2,this_room)==one_die)return one_die;
		return still_fight;
		break;
	case 406://406 极天束
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," 乘着攻势射出一道激光!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"受到了极天束的伤害!\n");
		result=hit_damage(2,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
		return result;
	case 407://407 蓝爆冰晶
		int hit_time=0;//命中次数
		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"向");addtext(StrBuf,c2->p1->name);addtext(StrBuf,"投掷出爆破冰晶!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		for(count=0;count<3;count++)
			if(whether_it_happen(50))
			{
				addtext(StrBuf,c2->p1->name);addtext(StrBuf,"受到了冰晶炸裂带来的溅射伤害!\n");
				result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
				if(result==one_die||result==force_offline||result==force_end_turn)return result;
				if(result!=i_failed_effect_B)hit_time++;
			}
		if(hit_time>=3)//如果三次全部命中则装备全部被破坏
		{
			addtext(StrBuf,"冰晶3次全中!");addtext(StrBuf,c1->p1->name);addtext(StrBuf,"的武器全部被破坏!\n");
			room_broadcast(this_room,StrBuf);
			delete_weapon_all(c2,this_room);
		}

		return still_fight;
	}
	cout<<"出现莫名其妙卡号故障!!卡号为:"<<card_No<<endl;
	return false;
}


int launch_buff_effect(client_member* c1,int choice,int situation,int* locktime,fight_room* this_room)//发动附加效果
{
	int result=still_fight;
	switch(choice)
	{
		case 75://隐天盾		此卡发动的回合以及发动后的2轮回内，自己可丢1手卡使 1次对己的效果伤血 无效化或抵挡1次普攻\n\n";
			if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"自己无法躲避和格挡!\n");return i_cancel_effect;}
			if(auto_judge(c1,i_will_discard_card_A,this_room)!=i_immune_effect)
			{
				result=discard_choose(1,c1,this_room);
				if(result==few_card){confirm_send_success(c1,"手卡不足!无法发动!\n");return i_cancel_effect;}
				if(result==i_cancel_effect){return i_cancel_effect;}
			}
			room_broadcast(this_room,"他动用契机临时制出了一块透明无暇又坚硬无比的防御盾格挡了伤害!\n");
			return i_failed_damage;	//无效化伤害
			
			
	}
	cout<<"加附效果发动错误！choice="<<choice<<endl;
	return still_fight;
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑对各种效果的处理↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

#endif