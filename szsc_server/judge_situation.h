#ifndef INCLUDE_judge_situation_H
#define INCLUDE_judge_situation_H


#include "deck_card.h"


//检查choice卡号是否是助攻卡，是则返回true，否则返回false
bool judge_assist(int choice)
{
	if(choice>=30&&choice<60)return true;

	return false;
}
bool judge_weapon(int choice)
{
	if(choice>=0&&choice<30)return true;

	return false;
}
bool judge_weapon_effect(client_member* c1,int card_No,int situation,int* locktime,int whichweapon,fight_room* this_room)
{
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };

	switch(card_No)
	{
	case 1002://1回合限1次，自己可丢1手卡，抽1卡（承皇剑1）
			if(c1->p1->whether_in_attack>0){confirm_send_success(c1,"处于搏斗中!不符合发动契机!发动失败!\n");return false;}
			if(c1->p1->weaponeffect001[whichweapon]!=0)//如果该回合效果还有剩余使用次数
			{
				if(!auto_judge(c1,i_will_discard_card_A,this_room))//如果没装备防自主丢卡效果
				{
					if(c1->p1->c[0]==false){confirm_send_success(c1,"手卡数量过少，无法发动该效果!\n");return false;}
				}
				return true;	
		   }
		   else
				{confirm_send_success(c1,"该回合的此武器效果次数已用完!\n");return false;}
	case 1003://自己可挡4次普攻，抵挡第4次普攻后此卡被破坏（承皇剑2）
			if(situation!=i_will_be_attacked){confirm_send_success(c1,"自己没受到普攻!\n");return false;}
			if(c1->p1->cannot_ED){confirm_send_success(c1,"自己无法躲避和格挡!\n");return false;}
			return true;
 
	case 1004://【自己回合可发动】自己主动将此卡破坏，指定1人伤2血（承皇剑3）
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"不符合发动契机!（不是自己回合或自己处于搏斗状态中）\n");return false;}
			return true;
	case 1012:  //"1回合限2次，自己可丢1手卡，自己此回合攻+1";疾风刃1
			if(c1->p1->exattack_weapon_turn[whichweapon]>=2)//如果说该位置的疾风刃叠满该效果，发动失败
				{confirm_send_success(c1,"该回合的此武器效果次数已用完!");return false;}
			return true;
				
	case 1013:  //"【自己回合可发动】自己主动将此卡破坏，指定1人伤1血";疾风刃2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"不符合发动契机!（不是自己回合或自己处于搏斗状态中）\n");return false;}
			return true;
		
	case 1022:  ;//"自己每使1人伤1血，自己+1血";长者之镰1
			{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}

	case 1023:  //【自己回合可发动】自己主动将此卡破坏，指定1人+3血";长者之镰2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"不符合发动契机!（不是自己回合或自己处于搏斗状态中）\n");return false;}
			return true;
	case 1032:  //"自己每次与人搏斗过后限1次，随机破坏 与自己搏斗者 装备的1张装备卡";//噬剑之剑1
			{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}

	case 1033:  ;//"【自己回合可发动】自己主动将此卡破坏，指定1人伤2血";//噬剑之剑2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"不符合发动契机!（不是自己回合或自己处于搏斗状态中）\n");return false;}
			return true;

	case 1043:  //"此次游戏限1次可发动，发动回合过后，自己持续不死2轮回";//黑曜剑1
			return true;//该效果可主动触发，当自己血量归0时自动触发

	case 1044: ;//"【自己回合可发动】自己主动将此卡破坏，指定1人50%伤3血";//黑曜剑2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"不符合发动契机!（不是自己回合或自己处于搏斗状态中）\n");return false;}	
			return true;

	case 1051: //"自己每抽1卡，自己血+1";//食腐再生装置1
			{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}

	case 1052: //1回合限1次，自己可丢1手卡，血+1;//食腐再生装置2	
			if(c1->p1->weaponeffect051[whichweapon]<=0){confirm_send_success(c1,"该回合的此武器效果次数已用完!\n");return false;}	
			return true;

	case 1053: //"自己回合限1次，自己可额外抽1卡";//食腐再生装置3
			if(c1->p1->whether_myturn!=true){confirm_send_success(c1,"不是自己的回合!不符合发动契机!发动失败!\n");return false;}
			if(c1->p1->weaponeffect052[whichweapon]<=0){confirm_send_success(c1,"次数已用完!发动失败!\n");return false;}
			return true;

	case 1061: ;//"此卡被装备上时给此卡放置4魔力指示物";//恩空法棒1
			{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}

	case 1062: ;//"己方每发动1手卡，给此卡放置1魔力指示物";//恩空法棒2
			{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}

	case 1063: ;//"【自己回合可发动】自己消耗2魔力指示物使1人伤1血";//恩空法棒3
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"（不在自己回合或自己处于搏斗状态中）不符合发动契机!发动失败!\n");return false;}
			if(c1->p1->weaponeffect06[whichweapon]<2){confirm_send_success(c1,"魔力指示物过少!发动失败!\n");return false;}
			return true;

	case 1071: //"自己不受任何丢卡和手卡公开的效果影响";//骷髅诡面1
		{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}
	case 1072: //"【场上除己外1人死亡时】自己血量回满";//骷髅诡面2
		{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}
	case 1073: //"自己不受任何自主伤血效果影响";//骷髅诡面3
		{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}
	case 1081: //"自己可装备无限数量装备卡"; //破败披风1
		{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}
	case 1082: //"【自己受到普攻时可发动】自己主动将此卡破坏，自己抵挡此次普攻";//破败披风2
		if(c1->p1->cannot_ED){confirm_send_success(c1,"自己无法躲避和格挡!不符合发动契机!\n");return false;}
		return true;

	case 1092: //"【自己每普攻1次】所有人丢3手卡，抽3卡，卡堆最上方3张卡送入废卡区";//冠阳剜月斧1
		{confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;}

	case 1093: ;//"【自己回合可发动】自己主动将此卡破坏，指定1人伤2血,场上所有人丢5手卡，抽5卡";//冠阳剜月斧2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"不符合发动契机（不是自己回合或自己处于搏斗状态中）!发动失败!\n");return false;}
			return true;

	}
	cout<<"武器效果没发动成功!card_No="<<card_No;	
	return false;
}

bool judge_card(client_member* c1,int choice,int situation,int* locktime)//situation发动条件场景，lock_time连锁次数,如果符合发动条件直接使用
{
	if(choice>=0&&choice<=29)
	{
		if(c1->device==win_console)
			confirm_send_success(c1,"该卡为武器卡，在自己回合选择'3.装备武器'才可发动!\n");
		else
			confirm_send_success(c1,"该卡为武器卡，在自己回合选择才可装备!\n");
		return false;
	}

	if(situation==i_will_attack&&choice>=30&&choice<60)//如果条件为“我要进攻了”，可以发动助攻卡
	{
		if(choice==33&&c1->p1->w[0]==false)//霸地·挥刃【自己装备武器时才可发动】
		{confirm_send_success(c1,"该助攻卡发动条件不满足(你没装备武器)!\n");return false;}
		return true;//其他助攻卡应该都能发动
	}

	if(choice>=30&&choice<=59){confirm_send_success(c1,"该卡为助攻卡，在自己普攻别人时才可发动!\n");return false;}

	int count;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	
	switch(choice)
	{
//--------------------------------------------------------隐效果----------------------------------------
	case 400://连攻，【自己普攻成功时或自己回合自己未处于搏斗状态可发动】自己发动一次普攻
		if(situation==i_attack||situation==i_failed_attack_E||situation==i_failed_attack_D||situation==i_failed_attack_W)//自己普攻成功
			return true;
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)//自己回合自己未处于搏斗状态
			return true;
		return false;
	case 401://闪
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"自己无法躲避和格挡!\n");return false;}
		if(situation==i_will_be_attacked)return true;
		return false;
	case 402://+4血
		return true;
	case 403://暗枪
		if(c1->p1->whether_in_attack)
			return true;
		return false;
	case 404://稻草人
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"自己无法躲避和格挡!\n");return false;}
		if(situation==i_will_be_attacked)return true;
	case 405://憎恨
		return true;
	case 406://极天束
		if(situation==i_attack)return true;
		return false;
	case 407://蓝爆冰晶
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)
			return true;
		else
			return false;
//--------------------------------------------------------隐效果----------------------------------------
//------------------------------------------------------一般效果卡--------------------------------------
	case 60://神起·焰射\n【自己普攻被躲避时可发动】
		if(situation==i_failed_attack_E||situation==i_failed_attack_W||situation==i_failed_attack_D)
			return true;
		else
			return false;
	case 61://近击·搏杀\n【自己参与的1次搏斗内双方发动过大于3次手卡时 可发动】
		if(c1->p1->whether_in_attack>0&&(*locktime)>=4)
			return true;
		else
			confirm_send_success(c1,"连锁次数不够!\n");
			return false;
	case 62://"[效果卡]阴势·突进\n【自己回合时可发动】自己指定1人-1血且此回合不能行动（被攻者可丢1手卡躲避此次伤血），接下来的此回合内自己只能对被指定者或自己发动效果卡、搏斗\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)
			return true;
		else
			return false;
	case 63:// "[效果卡]神空·闪降\n【自己回合时可发动】自己抽1卡，将抽到的该卡公开，自己这回合的规则性普攻次数被重置为1\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)
			return true;
		else
			return false;
	case 64:// "[效果卡]空遁·闪回\n【自己受到普攻时可发动】自己抽1卡且躲避此次搏斗，自己此回合可对 发动该次普攻者 普攻1次\n\n";
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"自己无法躲避和格挡!\n");return false;}
		if(situation==i_will_be_attacked)return true;
		return false;

	case 65:// "[效果卡]光斩·绝杀\n【自己受到致命伤血时可发动】此次伤血无效化且此回合自己基础攻击力×2，自己可进行1次普攻\n\n";
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"自己无法躲避和格挡!\n");return false;}
		if(c1->p1->i_soon_die)return true;//如果接到这招我血量就要归零了
		return false;

	case 66:// "[效果卡]天华绝伦\n【自己普攻成功且该回合内自己成功普攻超过2次时可发动】自己指定1人，自己此回合可 超速普攻 其4次，每次超速普攻对其造成的普攻伤血强制变为1且不会造成反伤\n\n";
		
		if(situation!=i_attack){confirm_send_success(c1,"自己当前没处于普攻成功状态!\n");return false;}
		if(c1->p1->attacktime_turn<3)
		{addtext(StrBuf,"当前回合自己普攻成功次数为:");addtext(StrBuf,c1->p1->attacktime_turn);addtext(StrBuf,",没超过2次!\n");
		confirm_send_success(c1,StrBuf);return false;}
		return true;

	case 67:// "[效果卡]神尚激光\n【己方回合时可发动】自己丢光手卡，此回合之后的3轮回内自己若受到致命伤血，使自己受到致命伤血者 将受到之前自己丢光手卡数值×2的伤血\n\n";
		if(c1->p1->c[0]==false){confirm_send_success(c1,"无手卡!此卡效果无法发动!\n");return false;}
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)return true;
		return false;

	case 68:// "[效果卡]轰炸星球\n【自己回合时可发动】破坏场上所有场景卡和结界卡，每破坏1张，使 放置该结界或场景者 伤3血\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)return true;
		return false;
	case 69:// "[效果卡]最后的指望\n【自己处于濒死状态时可发动】自己指定1人，将自己全部手卡、自己装备的武器全部加入他手卡\n\n";
		confirm_send_success(c1,"仅限多人对战时使用!\n");return false;
			
	case 70:// "[效果卡]双臂剑\n自己血-3，将卡堆最上方的2张武器卡装备上\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)return true;
		return false;
	
	case 71:// "[效果卡]离心续\n【自己普攻成功时可发动】自己再进行1次普攻，此次普攻造成的普攻伤血翻倍\n\n";
		if(situation==i_attack)return true;
		return false;

	case 72:// "[效果卡]虚混·太清\n【自己受到普攻时可发动】自己躲避本次普攻，自己丢光手卡，发动该次普攻者 的血量减半\n\n";
		if(c1->p1->cannot_ED){confirm_send_success(c1,"自己无法躲避和格挡!\n");return false;}
		if(situation==i_will_be_attacked)return true;
		return false;

	case 73:// "[效果卡]幻影剑\n将此卡作为 攻+2 的武器给自己装备上，在装备上此卡的回合结束时，此卡自动被破坏\n\n";
		if(c1->p1->w[weaponlimit-1]==true){confirm_send_success(c1,"装备栏已满!不能发动!\n");return false;}
		return true;	
	case 74:// "[效果卡]冷现·巨剑\n【自己受到效果伤血时可发动】自己丢1手卡中的装备卡，指定1人伤3血\n\n";
		if(c1->p1->c[0]==false){confirm_send_success(c1,"你手卡为空!此卡无法发动!\n");return false;}
		if(situation!=i_be_effect_B&&situation!=i_be_effect_A)return false;

		for(count=0;count<cardlimit;count++)//检测有无武器类的手卡
			if(c1->p1->c[count]==true&&judge_weapon(c1->p1->card[count]))
				break;
		if(count==cardlimit||c1->p1->c[count]==false)
			{confirm_send_success(c1,"你没有武器类的手卡!\n");return false;}
		return true;

	case 75:// "[效果卡]隐天盾\n此卡发动的回合以及发动后的2轮回内，自己可丢1手卡使 1次对己的效果伤血 无效化或抵挡1次普攻\n\n";
			return true;//可以瞬防
	
	case 76:// "[效果卡]破局·神击\n【自己普攻失败时可发动】自己普攻 使自己普攻失败者 1次，且此次普攻内 被攻者 无法行动。自己可丢1次不超过4张数量的手卡，使 被攻者 受到丢卡数量一半的伤血\n\n";
		if(situation==i_failed_attack_E||situation==i_failed_attack_W||situation==i_failed_attack_D)
			return true;
		else
			return false;
	case 77:;// 旋地·回击\n【自己受到普攻时可发动】自己抽1卡，自己可普攻 发动普攻者 1次，本次攻击自己攻+1";
		if(situation==i_be_attacked)
			return true;
		else
			return false;
	}
	cout<<"出现莫名其妙卡号故障!!卡号为:"<<choice<<endl;
	return false;
}


bool judge_character_effect(client_member* c1,int choice,int situation,int* locktime)
{
	switch(choice)
	{
		case 10000:
			return true;
		case 10001:confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;
			
		case 10002:confirm_send_success(c1,"该效果为被动效果!无法主动发动!\n");return false;
	}
	cout<<"出现了莫名其妙的人物效果编号!编号为:"<<choice<<endl;	
	return false;
}

int auto_judge(client_member* c1,int situation,float* number,int* locktime,fight_room* this_room)//长者镰刀特殊
{
	int count;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int result=still_fight;

	switch(situation)
	{
		case i_hit_someone:
			for(count=0;count<weaponlimit;count++)
				if(c1->p1->blood!=c1->p1->bloodlimit&&c1->p1->w[count]==true&&c1->p1->weapon[count]==2)//长者之镰
				{
					recover_blood(*number,c1,this_room);//血量增加number值
					addtext(StrBuf,c1->p1->name);addtext(StrBuf," 的长者之镰从伤害中汲取了生命精华!\n");
					cout<<"精华量："<<*number<<endl;
					room_broadcast(this_room,StrBuf);
				}
			break;

	}
	return result;
}
int auto_judge(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime)
{
	int result,choice;
	int count,count2;
	int c1_got,c2_got;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	result=still_fight;//默认不发动，触发发动则改为true或削弱情况
	switch(situation)
	{
		case i_card_used_S://自己一张手卡发动成功
		if(c1->p1->whether_myturn==true)
			for(count=0;count<abilitylimit;count++)
				if(c1->p1->a[count]==true&&c1->p1->ability[count]==10002&&c1->p1->selfeffect_invalid==0)//法士效果：【自己回合自己每次成功发动手卡时】自己可指定1人-1血
				{
					addtext(StrBuf,c1->p1->name);addtext(StrBuf,"获取了契机能量!\n");
					room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					//confirm_send_success(c1,"现在你可以使用法力指定1名角色伤1血!\n");
					choice=choose_enemy(c1,"现在你可以指定1名角色-1血!\n",this_room);
					if(choice==force_offline||choice==i_cancel_effect)break;

					c2=this_room->c[choice];
					
					addtext(StrBuf,c1->p1->name);addtext(StrBuf,"投掷出能量法球!\n");
					room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
						
					addtext(StrBuf,"能量法球击中了");addtext(StrBuf,c2->p1->name);addtext(StrBuf,"!\n");

					result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//扣血
					
					break;
				}
		
		break;
		
	case has_fought://自己参与了一次搏斗
		c1_got=0;c2_got=0;

		if(!c2->p1->w[0]||!c1->p1->w[0])break;//如果搏斗有一方没有武器，跳出
		for(count=0;count<weaponlimit;count++)//如果普攻者有噬剑之剑
		{
			if(c1->p1->weapon[count]==3)c1_got++;//c1拥有数加1
				
			if(c2->p1->weapon[count]==3)c2_got++;//c2拥有数加1
		}

		count=0;
		while(c1_got>0)//噬剑之剑被动：随机破坏对面一件武器
		{
			if(!c2->p1->w[0])break;//如果有多把噬剑之剑，而前几把已经将对面武器全部破坏了，结束发动
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"的噬剑之剑吞噬掉了对面的一件武器!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			for(;count<weaponlimit;count++)//噬剑之剑 武器攻+1（count=0在外面设定，防止破坏对面两把武器后攻击力都加在同一把上）
				if(c1->p1->weapon[count]==3)
				{c1->p1->exattack_weapon[count]++;count++;break;}
			while(1)
			{
				srand((unsigned)time(NULL)*rand()); 
				count2=rand()%(weaponlimit-0);
				if(!c2->p1->w[count2])continue;//如果该槽没有武器,重新随机
				addtext(StrBuf,search_card(p_name,c2->p1->weapon[count2]));addtext(StrBuf,"被吞噬了!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				
				delete_weapon(c2,count2,this_room);//破坏该槽武器
				c1_got--;
				break;
			}	
		}
		count=0;
		while(c2_got>0)//噬剑之剑被动：随机破坏对面一件武器
		{
			if(!c1->p1->w[0])break;//如果有多把噬剑之剑，而前几把已经将对面武器全部破坏了，结束发动
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"的噬剑之剑吞噬掉了对面的一件武器!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			
			for(;count<weaponlimit;count++)//噬剑之剑 武器攻+1
				if(c2->p1->weapon[count]==3)
				{c2->p1->exattack_weapon[count]++;count++;break;}
			while(1)
			{
				srand((unsigned)time(NULL)*rand()); 
				count2=rand()%(weaponlimit-0);
				if(!c1->p1->w[count2])continue;//如果该槽没有武器,重新随机
				addtext(StrBuf,search_card(p_name,c1->p1->weapon[count2]));addtext(StrBuf,"被吞噬了!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				
				delete_weapon(c1,count2,this_room);//破坏该槽武器
				c2_got--;
				break;
			}	
		}
		
		break;



	}
	return result;

}

int auto_judge(client_member* c1,int situation,fight_room* this_room)//检索情况是否自动发动（武器效果/人物特效）
{
	int result;
	int count,count2;

	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	result=still_fight;//默认不发动，触发发动则改为true或削弱情况

	switch(situation)
	{
	case i_will_public_card://即将公开手卡
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," 装备了骷髅诡面!因为其不可思议的诡异力量，无法公开手卡!\n");
				room_broadcast(this_room,StrBuf);
				result=i_immune_effect;
				break;
			}
		break;
	
	case i_will_discard_card_B://因他人效果丢弃手卡
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," 装备了骷髅诡面!因为其不可思议的诡异力量，不受他人的丢手卡效果!\n");
				room_broadcast(this_room,StrBuf);
				result=i_immune_effect;
				break;
			}
		break;
	case i_will_discard_card_A://因效果主动丢弃手卡
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," 装备了骷髅诡面!因为其不可思议的诡异力量，不受主动丢手卡效果1!\n");
				room_broadcast(this_room,StrBuf);
				result=i_immune_effect;
				break;
			}
		break;
	case i_card_used://我使用了一张手卡
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==6)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," 的恩空法棒收集到了契机卡片所带来的能量!充能+1!\n");
				room_broadcast(this_room,StrBuf);
				c1->p1->weaponeffect06[count]++;
				weapon_change(c1,this_room);
			}
		
		break;
	
	case i_will_be_effect_A://自主伤血效果
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"  装备了骷髅诡面!因为其不可思议的诡异力量，不受主动伤血效果!\n");
				room_broadcast(this_room,StrBuf);
				c1->p1->weaponeffect06[count]++;
				result=i_immune_effect;
				break;
			}
		
			break;

	case i_will_die://自己血量归零且没不死状态
		for(count=0;count<weaponlimit;count++)//黑曜剑自动触发无敌
			if(c1->p1->overdeath==0&&c1->p1->w[count]==true&&c1->p1->weapon[count]==4&&c1->p1->w_e[count][0]==true&&c1->p1->weapon_effect[count][0]==1043)
			{
				set_buff(c1,overdeath_turn,3,this_room);
	
				room_broadcast(this_room,"黑曜剑发出了前所未有的光芒!\n");
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"  感到自己体内充满了无穷的力量!\n");
				room_broadcast(this_room,StrBuf);
				room_broadcast(this_room,"黑曜剑失去了以往的光泽…………\n");

				c1->p1->w_e[count][1]=false;c1->p1->weapon_effect[count][0]=1044;//原有效果失效，仅剩丢剑伤害效果
				c1->p1->weapon_effect[count][1]=-1;
				weapon_change(c1,this_room);
				
				break;
			}
		
		break;
	case i_get_card://我获取了一张手卡
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->weapon[count]==5&&(c1->p1->blood<c1->p1->bloodlimit))
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"的食腐再生装置触发效果!每抽1卡回1血!\n");
				room_broadcast(this_room,StrBuf);
				recover_blood(1,c1,this_room);//血量增加1
			}
		
		break;
	case i_will_attack://发动普攻宣言了
		for(count=0;count<abilitylimit;count++)
			if(c1->p1->a[count]==true&&c1->p1->ability[count]==10001&&c1->p1->selfeffect_invalid==0)//战士效果：【自己每次主动普攻时】此次普攻结束前自己攻+1
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"自身效果发动!此次普攻结束前自己攻+2!\n");
				room_broadcast(this_room,StrBuf);
				
				attack_change(2,&c1->p1->exattack_time,c1,this_room);
			}
		for(count=0;count<weaponlimit;count++)//冠阳腕月，所有人丢3手卡，抽3卡，卡堆最上方3张卡送入废卡区
			if(c1->p1->w[count]&&c1->p1->weapon[count]==9&&c1->p1->w_valid[count])
			{
				room_broadcast(this_room,"冠阳剜月一经挥动，风云忽变……所有人丢3手卡，抽3卡，卡堆最上方3张卡送入废卡区!\n");
					
				for(count2=0;count2<playernumber;count2++)
				{
					discard_random(3,this_room->c[count2],this_room);//随机丢3卡
					get_card(3,this_room->c[count2],this_room);//抽3卡
				}
				for(count2=3;count2>0;count2--)
					refresh_deck(this_room->deck);
			}
		break;


	case i_will_be_unmoveable:
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count])
			{
				if(c1->p1->weapon[count]==8)//破败披风，不受“无法行动”效果影响
				{
					addtext(StrBuf,c1->p1->name);addtext(StrBuf," 的破败披风无效化了压制效果的影响!\n");
					room_broadcast(this_room,StrBuf);
					result=i_immune_effect;
					break;
				}
			}
			else
				break;
		break;
	}

	return result;
}

int judge_one_die(client_member* c1,fight_room* this_room)//判断他死没有
{
	if(c1->p1->blood<=0&&c1->p1->overdeath==0)//双重判定，过不了第二次判定则是死亡
	{
		c1->p1->blood=0;auto_judge(c1,i_will_die,this_room);//自动触发不死效果
		if(c1->p1->blood==0&&c1->p1->overdeath==0)//如果还没激发不死状态则死亡
		{
			
			c1->p1->alive=false;return one_die;
		}
	}
	return still_fight;//还没死
}

#endif