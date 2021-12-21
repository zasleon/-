#include "SZSC_property.h"


#define card_all_number		20000//卡片总数
#define card_effect_limit	5//卡片带有效果的数量上限
#define conditionlimit		3//达成发动条件上限
#define benefitlimit		3//玩家获取收益上限
#define sacrificelimit		3//玩家消耗资本上限
#define value_numberlimit	3//效果后对应数值的数量上限,一般多个数值后追加的是额外效果
#define benefit_ex_limit	1//每个效果完成后的追加效果数量上限

#define NONE_CONTENT -1//无内容





struct effect_Authority
{
	
	char effect_sentence[100];

	int condition[conditionlimit];//条件效果
	int condition_property[conditionlimit];//条件属性（独立达成该条件即可发动，同时满足该条件时才可发动）
	int condition_number[conditionlimit][value_numberlimit];//条件效果对应数值
	

	//满足条件后，消耗资本
	int sacrifice[sacrificelimit];//消耗效果
	int sacrifice_number[sacrificelimit][value_numberlimit];//效果对应数值
	

	//消耗完后获取收益
	int benefit[benefitlimit];//自己获得收益效果
	int benefit_number[benefitlimit][value_numberlimit];//收益特性对应数值

};
struct card_Authority
{
	effect_Authority effect[card_effect_limit];//效果全部内容
	int effect_property[card_effect_limit];//效果属性，被动（满足条件后自动触发），玩家主动发动

	int card_No;//卡片序号
	char card_name[30];//卡片名字
	int card_property;//卡片属性，武器/效果/场景/助攻
};

card_Authority szsc_card[card_all_number];//所有卡片

//清空卡片内容
void clear_szsc_card()
{
	for(int i=0;i<card_all_number;i++)//全部清空
	{
		for(int j=0;j<card_effect_limit;j++)
		{
			for(int k=0;k<conditionlimit;k++)
			{
				szsc_card[i].effect[j].condition[k]=NONE_CONTENT;
				szsc_card[i].effect[j].condition_property[k]=NONE_CONTENT;
				for(int l=0;l<value_numberlimit;l++)
					szsc_card[i].effect[j].condition_number[k][l]=NONE_CONTENT;
			}
			for(int k=0;k<sacrificelimit;k++)
			{
				szsc_card[i].effect[j].sacrifice[k]=NONE_CONTENT;
				for(int l=0;l<value_numberlimit;l++)
					szsc_card[i].effect[j].sacrifice_number[k][l]=NONE_CONTENT;
			}
			for(int k=0;k<benefitlimit;k++)
			{
				szsc_card[i].effect[j].benefit[k]=NONE_CONTENT;
				for(int l=0;l<value_numberlimit;l++)
				{
					szsc_card[i].effect[j].benefit_number[k][l]=NONE_CONTENT;
				}
			}
			
		}

	}
}

#define P_weapon_card				6000//武器卡
#define P_effect_card				6001//效果卡
#define P_assist_attack_card		6002//助攻卡
#define P_totem_card				6003//结界卡
#define P_field_card				6004//场景卡
#define P_character_card			6005//角色卡

#define independent_condition		6888//独立达成该效果即可发动
#define associate_condition			6889//需要同时满足该效果
#define passive_effect				6890//纯被动效果
#define	active_effect				6891//需要玩家主动激活
#define active_auto_effect			6892//满足发动条件后自动激活，也可主动激活

//设置该卡片的名字和属性
void set_card_property(card_Authority* this_card,int card_No,char* name,int card_property)
{
	this_card->card_No=card_No;
	memset(this_card->card_name,0,sizeof(this_card->card_name));
	strcat(this_card->card_name,name);
	this_card->card_property=card_property;
}
//设定特效说明字符
void add_sentence(card_Authority* this_card,int e_number,char* sentence)
{
	strcat(this_card->effect[e_number].effect_sentence,sentence);
}
//设定该效果属性（被动或主动）
void add_EP(card_Authority* this_card,int e_number,int effect_property)
{
	this_card->effect_property[e_number]=effect_property;
}
//添加收益效果
void add_B(card_Authority* this_card,int e_number,int E1,int EV1)//卡片，卡片第几个效果，收益效果，效果值
{
	e_number--;
	for(int i=0;i<benefitlimit;i++)
		if(this_card->effect[e_number].benefit[i]==NONE_CONTENT)//如果当前效果栏为空，填入效果
		{
			this_card->effect[e_number].benefit[i]=E1;
			this_card->effect[e_number].benefit_number[i][0]=EV1;
			return;
		}
}
void add_B(card_Authority* this_card,int e_number,int E1,int EV1,int EV2)//卡片，卡片第几个效果，收益效果，效果值
{
	e_number--;
	for(int i=0;i<benefitlimit;i++)
		if(this_card->effect[e_number].benefit[i]==NONE_CONTENT)//如果当前效果栏为空，填入效果
		{
			this_card->effect[e_number].benefit[i]=E1;
			this_card->effect[e_number].benefit_number[i][0]=EV1;
			this_card->effect[e_number].benefit_number[i][1]=EV2;
			return;
		}
}
void add_B(card_Authority* this_card,int e_number,int E1,int EV1,int EV2,int EV3)//卡片，卡片第几个效果，收益效果，效果值
{
	e_number--;
	for(int i=0;i<benefitlimit;i++)
		if(this_card->effect[e_number].benefit[i]==NONE_CONTENT)//如果当前效果栏为空，填入效果
		{
			this_card->effect[e_number].benefit[i]=E1;
			this_card->effect[e_number].benefit_number[i][0]=EV1;
			this_card->effect[e_number].benefit_number[i][1]=EV2;
			this_card->effect[e_number].benefit_number[i][2]=EV3;
			return;
		}
}
//添加消耗资本
void add_S(card_Authority* this_card,int e_number,int S1,int SV1)
{
	e_number--;
	for(int i=0;i<sacrificelimit;i++)
		if(this_card->effect[e_number].sacrifice[i]==NONE_CONTENT)
		{
			this_card->effect[e_number].sacrifice[i]=S1;
			this_card->effect[e_number].sacrifice_number[i][0]=SV1;
			return;
		}
}
//添加条件
void add_C(card_Authority* this_card,int e_number,int condition_property,int C1,int CV1)
{
	e_number--;
	for(int i=0;i<card_effect_limit;i++)
		if(this_card->effect[e_number].condition[i]==NONE_CONTENT)
		{
			this_card->effect[e_number].condition[i]=C1;
			this_card->effect[e_number].condition_number[i][0]=CV1;
			this_card->effect[e_number].condition_property[i]=condition_property;
			return;
		}
}




//-----------------------------------condition---------------------------------------
#define my_turn_not_in_fight		1050//自己的回合且自己未处于搏斗状态

#define i_am_wearing_this_weapon	1100//自己正装备上这件武器
#define	i_armed						1101//自己装备有武器

#define i_have_card					1111//我有至少x张手卡
#define i_failed_attack				1112//自己普攻失败（包含被躲避被格挡被稻草人）

#define	we_card_used				1120//己方每次发动手卡时

#define limit_times_all				2000//总共可以使用次数
#define one_turn_limit_times		2001//每回合限制使用次数
#define my_turn_limit_times			2002//每次自己回合限制使用次数

#define someone_die_but_not_me		2050//场上除己外1人死亡

#define lock_time_plenty			2060//连锁次数达到要求，+足够值

#define token_plenty				2070//指示物数量足够，+足够值

//-----------------------------------condition---------------------------------------
//-----------------------------------sacrifice-----------------------------------
#define i_discard_card			3000//我丢弃手卡+张数
#define i_discard_all_card		3001//我丢弃所有手卡

#define i_consume_token			3005//我消耗指示物

#define i_discard_this_weapon		3010//我主动丢弃此武器
#define i_destroy_this_weapon		3011//我主动破坏此武器

#define delete_this_effect			3015//此效果用完，被删除

#define use_check_durability		3020//使用剩余次数-1，检查剩余效果使用次数
//-----------------------------------sacrifice-----------------------------------
//-----------------------------------benefit---------------------------------------
#define weapon_provide_attack_P		4000//武器提供的永久攻击力
#define weapon_provide_attack_T		4001//武器提供的该回合上升的攻击力
#define weapon_increase_attack_P	4002//武器额外增加的永久攻击力


#define my_this_weapon_get_token	5005//我这件武器获取指示物

#define i_increase_attack_this_time	5020//我本次普攻攻击力上升
#define	i_lose_half_attack_time		5021//我本次普攻攻击力减半
#define penetrating_attack_time		5022//本次普攻变为穿攻
#define	do_overspeed_attack			5023//发动一次超速普攻，+超速普攻造成伤害值

#define i_point_effect_B		5050//我指定1人发动效果性伤血
#define i_point_effect_B_R		5051//我指定1人发动带有命中概率的效果性伤血，+概率+伤血数值
#define	i_point_effect_B_CE		5052//我指定1人发动效果性伤血，对手可丢1手卡躲避

#define i_effect_B_CE			5053//我对其发动伤血效果，，对手可丢1手卡躲避
#define i_effect_B_R			5054//我使对手概率伤血，+概率+伤血数值
#define i_effect_B_all_rival	5055//使除己方外所有敌人伤血，+伤血数值

#define i_point_cure			5075//我指定1人回复血量
#define i_cure_myself_const		5080//我自己回复固定数值的血量
#define i_cure_myself_damage	5081//我自己回复自己造成伤害的血量(长者之镰，特殊)
#define i_cure_myself_all		5082//我自己血量回满


#define i_destroy_his_weapon_random		5100//我随机破坏对面一件持有的武器
#define i_destroy_all_rival_weapon		5110//破坏敌方所有装备的武器

#define i_get_overdeath_buff			5110//我获得不死buff

#define everyone_discard_card_R			6000//所有人随机丢手卡
#define everyone_get_card				6001//所有人获取卡

#define debuff_unmovable_turn			6011//让对手在n回合无法行动，+回合数
#define	debuff_unmovable_time			6012//让对手在此次行动结束前无法行动
#define debuff_unmovable_turn_R			6013//有概率，让对手几回合内无法行动，+概率+回合数
#define debuff_discard_card_R			6014//让对手随机丢弃手卡，+张数
#define debuff_discard_card_R_R			6015//有概率，让对手随机丢弃手卡，+概率+张数
#define	debuff_destroy_all_weapon_R		6016//有概率，对手装备的武器全部破坏，+概率

#define	rival_get_card					6025//让对手抽卡，+张数

#define i_check_and_plunder_his_card	6030//我查看并抢夺他的手卡
#define if_plunder_weapon_then_arm		6031//如果抢夺了装备卡则装备

#define my_weapon_invalid_in_this_attack	6040//本次普攻自己武器全部无效化	

#define buff_if_i_attack				6050//给自己上buff，该buff在自己普攻成功后发动
#define buff_this_attack_cant_miss		6060//本次普攻必中

#define i_effect_B_twice_my_invalid_weapon_and_return	6090//给予对手自己武器被无效化数量*2数值的效果性伤血

//-----------------------------------benefit---------------------------------------
void ini_weapon_card()//初始化武器卡片内容
{
	//承皇剑
	set_card_property(&szsc_card[0],0,"承皇剑",P_weapon_card);
	add_C(&szsc_card[0],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[0],1,weapon_provide_attack_P,1);//武器提供：攻+1
	add_EP(&szsc_card[0],1,passive_effect);//被动效果

	add_C(&szsc_card[0],2,associate_condition,one_turn_limit_times,1);//每回合限制使用次数,1次
	add_C(&szsc_card[0],2,associate_condition,i_have_card,1);//我有手卡
	add_S(&szsc_card[0],2,i_discard_card,1);//我丢弃一张手卡
	add_B(&szsc_card[0],2,i_get_card,1);//我抽1卡
	add_EP(&szsc_card[0],2,active_effect);//主动效果
	//add_sentence(&szsc_card[0],2,"player1激活了承皇剑的科技之核！\n更新了现有契机！\n");

	add_C(&szsc_card[0],3,associate_condition,i_will_be_attacked,NONE_CONTENT);//我受到普攻时可发动
	add_C(&szsc_card[0],3,associate_condition,limit_times_all,4);//总共可使用4次
	add_S(&szsc_card[0],3,use_check_durability,i_destroy_this_weapon);//检查该技能使用次数是否用完，用完则此武器被破坏
	add_B(&szsc_card[0],3,i_defend_attack,1);//我格挡了普攻
	add_EP(&szsc_card[0],3,active_effect);//主动效果
	//add_sentence(&szsc_card[0],2,"player1将承皇剑切换为盾模式！\n");

	add_C(&szsc_card[0],4,associate_condition,my_turn,NONE_CONTENT);//我的回合我主动使用
	add_S(&szsc_card[0],4,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[0],4,i_point_effect_B,1);//我指定1人发动效果性伤血，威力为1
	add_EP(&szsc_card[0],4,active_effect);//主动效果
	//add_sentence(&szsc_card[0],2,"player1飞出了手中的承皇剑！\n");

	//疾风刃
	set_card_property(&szsc_card[1],1,"疾风刃",P_weapon_card);
	add_C(&szsc_card[1],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[1],1,weapon_provide_attack_P,1);//武器提供：攻+1
	add_EP(&szsc_card[1],1,passive_effect);//被动效果

	add_C(&szsc_card[1],2,associate_condition,one_turn_limit_times,2);//每回合限制使用次数,2次
	add_C(&szsc_card[1],2,associate_condition,i_have_card,1);//我有手卡
	add_S(&szsc_card[1],2,i_discard_card,1);//我丢弃一张手卡
	add_B(&szsc_card[1],2,weapon_provide_attack_T,1);//自己此回合攻+1
	add_EP(&szsc_card[1],2,active_effect);//主动效果

	add_C(&szsc_card[1],3,associate_condition,my_turn,NONE_CONTENT);//我的回合我主动使用
	add_S(&szsc_card[1],3,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[1],3,i_point_effect_B,1);//我指定1人发动效果性伤血，威力为1
	add_EP(&szsc_card[1],3,active_effect);//主动效果

	//长者之镰
	set_card_property(&szsc_card[2],2,"长者之镰",P_weapon_card);
	add_C(&szsc_card[2],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[2],1,weapon_provide_attack_P,2);//武器提供：攻+2
	add_EP(&szsc_card[2],1,passive_effect);//被动效果

	add_C(&szsc_card[2],2,associate_condition,i_hit_someone,NONE_CONTENT);//我让某人受到伤害了
	add_B(&szsc_card[2],2,i_cure_myself_damage,NONE_CONTENT);//我自己回复自己造成伤害的血量
	add_EP(&szsc_card[2],2,passive_effect);//被动效果

	add_C(&szsc_card[2],3,associate_condition,my_turn,NONE_CONTENT);//我的回合我主动使用
	add_S(&szsc_card[2],3,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[2],3,i_point_cure,3);//我指定1人回复3血
	add_EP(&szsc_card[2],3,active_effect);//主动效果

	//噬剑之剑
	set_card_property(&szsc_card[3],3,"噬剑之剑",P_weapon_card);
	add_C(&szsc_card[3],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[3],1,weapon_provide_attack_P,1);//武器提供：攻+1
	add_EP(&szsc_card[3],1,passive_effect);//被动效果

	add_C(&szsc_card[3],2,associate_condition,has_fought,NONE_CONTENT);//我与某人搏斗了
	add_B(&szsc_card[3],2,i_destroy_his_weapon_random,NONE_CONTENT);//我随机破坏对面手持的一件武器
	add_B(&szsc_card[3],2,weapon_increase_attack_P,1);//我自己这件武器永久性攻+1
	add_EP(&szsc_card[3],2,passive_effect);//被动效果
	//最初设计为每次搏斗后将对手随机一个武器的随机一个效果消除，消除完所有效果后才破坏该武器
	//后来卡越出越多，决斗越来越告诉，这效果就变得太漫长，需要很长时间的战斗才能破坏别人的武器，因此后来决定加强，变为战斗后直接破坏一武器，而且还有无限成长能力

	add_C(&szsc_card[3],3,associate_condition,my_turn,NONE_CONTENT);//我的回合我主动使用
	add_S(&szsc_card[3],3,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[3],3,i_point_effect_B,1);//我指定1人发动效果性伤血，威力为1
	add_EP(&szsc_card[3],3,active_effect);//主动效果

	//黑曜剑
	set_card_property(&szsc_card[4],4,"黑曜剑",P_weapon_card);
	add_C(&szsc_card[4],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[4],1,i_be_effect_A,3);//我自主受到3血的伤害
	add_B(&szsc_card[4],1,weapon_provide_attack_P,2);//武器提供：攻+1
	add_EP(&szsc_card[4],1,passive_effect);//被动效果
	//不是受到伤血后才能装备上此武器，而是装备上此武器后受到的效果为伤3血，所以不是add_S，而是add_B

	add_C(&szsc_card[4],2,independent_condition,limit_times_all,1);//此武器只能发动1次该效果
	add_C(&szsc_card[4],2,independent_condition,i_will_die,NONE_CONTENT);//自己即将死亡时会自动激活发动
	add_S(&szsc_card[3],2,delete_this_effect,NONE_CONTENT);//此武器不再拥有此效果
	add_B(&szsc_card[4],2,i_get_overdeath_buff,3);//我获得不死buff两轮回
	add_EP(&szsc_card[4],2,active_auto_effect);//主动效果,也可被动激活
	//最初设计，不死魂魄能力是伴随剑的，意思是如果剑没了，不死buff也会强制直接消失，如果在0血状态下该剑会破坏，玩家会直接被判定死亡
	//后来觉得这样设计太弱了，不值得当初debuff的-3血，所以改为一直有该buff

	add_C(&szsc_card[4],3,associate_condition,my_turn,NONE_CONTENT);//我的回合我主动使用
	add_S(&szsc_card[4],3,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[4],3,i_point_effect_B_R,50,2);//我指定1人指定1人50%-2血
	add_EP(&szsc_card[4],3,active_effect);//主动效果

	//食腐再生装置
	set_card_property(&szsc_card[5],5,"食腐再生装置",P_weapon_card);
	add_C(&szsc_card[5],1,associate_condition,i_get_card,NONE_CONTENT);//自己每抽1卡
	add_B(&szsc_card[5],1,i_cure_myself_const,1);//自己血+1
	add_EP(&szsc_card[5],1,passive_effect);//被动效果

	add_C(&szsc_card[5],2,associate_condition,one_turn_limit_times,1);//每回合限制使用次数,1次
	add_C(&szsc_card[5],2,associate_condition,i_have_card,1);//我有手卡
	add_S(&szsc_card[5],2,i_discard_card,1);//自己丢1手卡
	add_B(&szsc_card[5],2,i_cure_myself_const,1);//自己血+1
	add_EP(&szsc_card[5],2,active_effect);//主动效果

	add_C(&szsc_card[5],3,associate_condition,my_turn_limit_times,1);//自己回合限制使用次数,1次
	add_B(&szsc_card[5],3,i_get_card,1);//自己抽1卡
	add_EP(&szsc_card[5],3,active_effect);//主动效果

	//恩空法棒
	set_card_property(&szsc_card[6],6,"恩空法棒",P_weapon_card);
	add_C(&szsc_card[6],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[6],1,my_this_weapon_get_token,2);//我这件装备获取2指示物
	add_EP(&szsc_card[6],1,passive_effect);//被动效果

	add_C(&szsc_card[6],2,associate_condition,we_card_used,NONE_CONTENT);//己方每次发动手卡时
	add_B(&szsc_card[6],2,my_this_weapon_get_token,1);//我这件装备获取1指示物
	add_EP(&szsc_card[6],2,passive_effect);//被动效果

	add_C(&szsc_card[6],3,associate_condition,my_turn,NONE_CONTENT);
	add_C(&szsc_card[6],3,associate_condition,token_plenty,2);//确认至少有两个指示物
	add_S(&szsc_card[6],3,i_consume_token,2);//消耗两个指示物
	add_B(&szsc_card[6],3,i_point_effect_B,1);//我指定1人发动效果性伤血，威力为1
	add_EP(&szsc_card[6],3,active_effect);//主动效果

	//骷髅诡面
	set_card_property(&szsc_card[7],7,"骷髅诡面",P_weapon_card);
	add_C(&szsc_card[7],1,independent_condition,i_will_public_card,NONE_CONTENT);//我受到手卡公开效果
	add_C(&szsc_card[7],1,independent_condition,i_will_discard,NONE_CONTENT);//自己受到丢卡效果影响时
	add_B(&szsc_card[7],1,i_immune_effect,1);//我免疫此次效果
	add_EP(&szsc_card[7],1,passive_effect);//被动效果

	add_C(&szsc_card[7],2,associate_condition,someone_die_but_not_me,NONE_CONTENT);//场上除己外1人死亡时
	add_B(&szsc_card[7],2,i_cure_myself_all,1);//我血量回满
	add_EP(&szsc_card[7],2,passive_effect);//被动效果

	add_C(&szsc_card[7],3,associate_condition,i_will_be_effect_A,NONE_CONTENT);//自主伤血效果
	add_B(&szsc_card[7],3,i_immune_effect,1);//我免疫此次效果
	add_EP(&szsc_card[7],3,passive_effect);//被动效果

	//破败披风
	set_card_property(&szsc_card[8],8,"破败披风",P_weapon_card);
	add_C(&szsc_card[8],1,associate_condition,i_will_be_unmoveable,NONE_CONTENT);//无法行动
	add_B(&szsc_card[8],1,i_immune_effect,1);//我免疫此次效果
	add_EP(&szsc_card[8],1,passive_effect);//被动效果

	add_C(&szsc_card[8],2,associate_condition,i_will_be_attacked,NONE_CONTENT);//自己受到普攻
	add_S(&szsc_card[8],2,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[8],2,i_defend_attack,NONE_CONTENT);//自己抵挡此次普攻
	add_EP(&szsc_card[8],2,active_effect);//主动效果
	//原本还有的效果：自己可无限装备装备卡。之后改为：自己不受装备卡被破坏效果的影响。但都没去实现

	//冠阳剜月斧
	set_card_property(&szsc_card[9],9,"破败披风",P_weapon_card);
	add_C(&szsc_card[9],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//装备上时发动
	add_B(&szsc_card[9],1,weapon_provide_attack_P,8);//武器提供：攻+1
	add_EP(&szsc_card[9],1,passive_effect);//被动效果

	add_C(&szsc_card[9],2,associate_condition,i_will_attack,NONE_CONTENT);//我即将普攻
	add_B(&szsc_card[9],2,everyone_discard_card_R,3);//所有人随机丢3卡
	add_B(&szsc_card[9],2,everyone_get_card,3);//所有人抽3卡
	add_EP(&szsc_card[9],2,passive_effect);//被动效果

	add_C(&szsc_card[9],3,associate_condition,i_will_be_attacked,NONE_CONTENT);//自己受到普攻
	add_S(&szsc_card[9],3,i_discard_this_weapon,1);//我主动丢弃此武器
	add_B(&szsc_card[9],3,everyone_discard_card_R,5);//所有人随机丢5卡
	add_B(&szsc_card[9],3,everyone_get_card,5);//所有人抽5卡
	add_EP(&szsc_card[9],3,active_effect);//主动效果

}
void ini_assist_card()
{
	//决义·地割
	set_card_property(&szsc_card[30],30,"决义·地割",P_assist_attack_card);
	add_C(&szsc_card[30],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[30],1,i_effect_B_CE,1,debuff_unmovable_time);//让对象伤1血，对手可丢1手卡躲避此次伤血，若命中则对手在此次行动结束前无法行动
	add_EP(&szsc_card[30],1,active_effect);//主动效果

	//劈夺斩
	set_card_property(&szsc_card[31],31,"劈夺斩",P_assist_attack_card);
	add_C(&szsc_card[31],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[31],1,i_increase_attack_this_time,1);//本次普攻攻击力加1
	add_B(&szsc_card[31],1,i_check_and_plunder_his_card,1,if_plunder_weapon_then_arm);//我查看并掠夺被攻击者的1张手卡，如果是装备卡则给自己装备上
	add_EP(&szsc_card[31],1,active_effect);//主动效果

	//剑飞·回斩
	set_card_property(&szsc_card[32],32,"剑飞·回斩",P_assist_attack_card);
	add_C(&szsc_card[32],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[32],1,my_weapon_invalid_in_this_attack,1);//本次普攻自己武器全部无效化
	add_B(&szsc_card[32],1,buff_if_i_attack,i_effect_B_twice_my_invalid_weapon_and_return);//当自己此次普攻成功后给予对手自己武器被无效化数量*2数值的效果性伤血且武器重新有效化
	add_EP(&szsc_card[32],1,active_effect);//主动效果

	//霸地·挥刃
	set_card_property(&szsc_card[33],33,"霸地·挥刃",P_assist_attack_card);
	add_C(&szsc_card[33],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_C(&szsc_card[33],1,associate_condition,i_armed,1);//自己至少装备有1件武器
	add_B(&szsc_card[33],1,i_get_card,1);//自己抽1卡
	add_B(&szsc_card[33],1,i_effect_B_CE,1);//让对象伤1血，对手可丢1手卡躲避此次伤血
	add_EP(&szsc_card[33],1,active_effect);//主动效果

	//极寒加护
	set_card_property(&szsc_card[34],34,"极寒加护",P_assist_attack_card);
	add_C(&szsc_card[34],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[34],1,i_increase_attack_this_time,1);//本次普攻攻击力加1
	add_B(&szsc_card[34],1,buff_if_i_attack,debuff_unmovable_turn_R,30,2);//当自己此次普攻成功后,30%使对手接下来的此回合和下回合不能行动
	add_EP(&szsc_card[34],1,active_effect);//主动效果

	//雷霆加护
	set_card_property(&szsc_card[35],35,"雷霆加护",P_assist_attack_card);
	add_C(&szsc_card[35],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[35],1,i_increase_attack_this_time,2);//本次普攻攻击力加2
	add_B(&szsc_card[35],1,buff_if_i_attack,debuff_discard_card_R_R,50,1);//当自己此次普攻成功后,50%使对手随机丢弃1张手卡
	add_EP(&szsc_card[35],1,active_effect);//主动效果

	//炽焰加护
	set_card_property(&szsc_card[36],36,"炽焰加护",P_assist_attack_card);
	add_C(&szsc_card[36],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[36],1,i_increase_attack_this_time,1);//本次普攻攻击力加1
	add_B(&szsc_card[36],1,buff_if_i_attack,i_effect_B_R,40,1);//普攻成功后40%使对手-1血
	add_EP(&szsc_card[36],1,active_effect);//主动效果

	//流水加护
	set_card_property(&szsc_card[37],37,"流水加护",P_assist_attack_card);
	add_C(&szsc_card[37],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[37],1,i_increase_attack_this_time,1);//本次普攻攻击力加1
	add_B(&szsc_card[37],1,buff_if_i_attack,debuff_discard_card_R,2);//当自己此次普攻成功后,使对手随机丢弃2张手卡
	add_B(&szsc_card[37],1,buff_if_i_attack,rival_get_card,2);//当自己此次普攻成功后,使对手抽2卡
	add_EP(&szsc_card[37],1,active_effect);//主动效果

	//神光加护
	set_card_property(&szsc_card[38],38,"神光加护",P_assist_attack_card);
	add_C(&szsc_card[38],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[38],1,i_increase_attack_this_time,1);//本次普攻攻击力加1
	add_B(&szsc_card[38],1,buff_if_i_attack,i_get_card,1);//当自己此次普攻成功后,自己抽1卡
	add_EP(&szsc_card[38],1,active_effect);//主动效果

	//冥幽加护
	set_card_property(&szsc_card[39],39,"冥幽加护",P_assist_attack_card);
	add_C(&szsc_card[39],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[39],1,i_increase_attack_this_time,1);//本次普攻攻击力加1
	add_B(&szsc_card[39],1,buff_if_i_attack,debuff_destroy_all_weapon_R,30);//当自己此次普攻成功后,30%使被普攻者装备的武器全部破坏

	//神·空·斩
	set_card_property(&szsc_card[40],40,"神·空·斩",P_assist_attack_card);
	add_C(&szsc_card[40],1,associate_condition,i_will_attack,NONE_CONTENT);//发动普攻时可发动
	add_B(&szsc_card[40],1,buff_this_attack_cant_miss,NONE_CONTENT);//本次普攻必中
	add_B(&szsc_card[40],1,i_lose_half_attack_time,NONE_CONTENT);//此次普攻自己攻击力减半
	add_B(&szsc_card[40],1,penetrating_attack_time,NONE_CONTENT);//此次普攻自己变为穿攻
	
}
void ini_effect_card()
{
	//神起·焰射
	set_card_property(&szsc_card[60],60,"神起·焰射",P_effect_card);
	add_C(&szsc_card[60],1,independent_condition,i_failed_attack,NONE_CONTENT);//自己普攻失败时可发动
	add_S(&szsc_card[60],1,i_discard_all_card,NONE_CONTENT);//我丢弃我所有手卡
	add_B(&szsc_card[60],1,i_effect_B_all_rival,1);//使所有对手血-1
	add_B(&szsc_card[60],1,i_destroy_all_rival_weapon,NONE_CONTENT);//敌方装备卡全部被破坏
	add_EP(&szsc_card[60],1,active_effect);//主动效果

	//近击·搏杀
	set_card_property(&szsc_card[61],61,"近击·搏杀",P_effect_card);
	add_C(&szsc_card[61],1,independent_condition,lock_time_plenty,5);//自己参与的1次事件里连锁超过4次
	add_B(&szsc_card[61],1,do_overspeed_attack,3);//超速普攻伤害为3
	add_B(&szsc_card[61],1,force_end_turn,NONE_CONTENT);//此回合被强制结束
	add_EP(&szsc_card[61],1,active_effect);//主动效果

	//阴势·突进
	set_card_property(&szsc_card[62],62,"阴势·突进",P_effect_card);
	add_C(&szsc_card[62],1,independent_condition,my_turn_not_in_fight,NONE_CONTENT);//自己参与的1次事件里连锁超过4次
	add_B(&szsc_card[30],1,i_effect_B_CE,1,debuff_unmovable_turn,1);//让对象伤1血，对手可丢1手卡躲避此次伤血，若命中则对手在此回合无法行动
	add_EP(&szsc_card[62],1,active_effect);//主动效果
	//魔腾开大罢了
}

void ini_szsc_card()//初始化卡片内容
{
	clear_szsc_card();
	ini_weapon_card();
	ini_effect_card();
	





}

int random_card(int i);//根据送入的随机数赋予卡片序号，部分卡片抽到的权重不同，可以适当调整
int discard_all(client_member* c1,fight_room* this_room);//丢光手卡
int supply_card();
void refresh_deck(int* deck);//玩家每抽1张卡后会用到该功能,即get_card函数才会用到该函数，用于刷新卡组最后一张
void ini_deck(int* deck);
void get_card(int number,client_member* c1,fight_room* this_room);															//人物从deck卡组抽卡
void get_specific_card(int number,player* p1);
void show_card_name(player* p1)	;
void refresh_card(client_member* p1,fight_room* this_room);                    //如果有卡使用后，需要马上使用这个函数,消耗手卡后重新整理手卡
int discard_choose(int number,client_member* c1,fight_room* this_room);//丢卡，由手卡持有者选择丢number数量的卡
int discard_free_choose(client_member* c1,bool full,fight_room* this_room);//丢卡，由手卡持有者选择丢任意数量的卡
int random_choose_card(int cardnumber);									//随机刷新卡，加入deck卡组;
void discard_random(int number,client_member* p1,fight_room* this_room);//丢卡，随机丢number数量的卡
void discard_card(client_member*c1,int whichcard,fight_room* this_room);
int use_card(client_member*c1,int whichcard,bool hide_effect,int* locktime,fight_room* this_room);
void refresh_hideeffect(client_member* c1);//更新卡片隐效果

bool judge_assist(int choice);
bool judge_weapon(int choice);
bool judge_weapon_effect(client_member* c1,int card_No,int situation,int* locktime,int whichweapon,fight_room* this_room);
bool judge_card(client_member* c1,int choice,int situation,int* locktime);//situation发动条件场景，lock_time连锁次数,如果符合发动条件直接使用
bool judge_character_effect(client_member* c1,int choice,int situation,int* locktime);

int auto_judge(client_member* c1,int situation,fight_room* this_room);
int auto_judge(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime);
int auto_judge(client_member* c1,int situation,float* number,int* locktime,fight_room* this_room);//检索情况是否自动发动（武器效果/人物特效）
int judge_one_die(client_member* c1,fight_room* this_room);//判断他死没有

int launch_weapon_effect(client_member* c1,int card_No,int whichweapon,int situation,int* locktime,fight_room* this_room);//发动武器效果
int character_effect(client_member* c1,int card_No,fight_room* this_room);			//自己主动发动各种人物效果
int launch_assist_effect(client_member* c1,client_member* c2,int card_No,int stage,int* locktime,fight_room* this_room);
int launch_card_effect(client_member* c1,client_member* c2,int card_No,int situation,int* locktime,fight_room* this_room);//发动手卡
int launch_buff_effect(client_member* c1,int choice,int situation,int* locktime,fight_room* this_room);//发动附加效果

//int choose_enemy(client_member* c1,fight_room* this_room);//选择指定的对象(该函数为通用函数u),不包括自己
//int choose_someone(client_member* c1,fight_room* this_room);//选择指定的对象(该函数为通用函数u)，包括自己
bool whether_it_happen(int percent);
void turnsettle(client_member* c1,fight_room* this_room)						;								//回合结束时所有玩家状态结算
void natural_set_playerturn(fight_room* this_room)	;						//自然变更玩家回合
void ini_character(player* p1,int playerNo);								//对传入的玩家p的各种状态进行初始化
void set_buff(client_member* c1,int situation,int number,fight_room* this_room);//设置c1状态，situation为状态，number为持续回合
void fight_data_clean(player* p1,player* p2,bool clean,fight_room* this_room);//该次普攻产生的特效数值被消除
void show_character(player* p1,bool i);															//显示人物状态

int hit_damage(float damage,client_member* c1,client_member* c2,fight_room* this_room,int* locktime,char* StrBuf,int situation);//c2对c1造成damage伤血
int fight_back(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime);
int general_attack(client_member* c1,client_member* c2,int situation,int* locktime,fight_room* this_room);
void recover_blood(float number,client_member* c1,fight_room* this_room);
int lightspeed_attack(client_member* c1,client_member* c2,float damage,int* locktime,fight_room* this_room);//超速普攻,c1打c2

void refresh_weapon(player* p1,fight_room* this_room);//玩家武器变化才会用到该函数，用于刷新武器槽
void delete_weapon(client_member* c1,int whichweapon,fight_room* this_room);//删除某个槽内的一把武器,该槽号为whichweapon
void delete_weapon_all(client_member* c1,fight_room* this_room);//删除该玩家全部武器
bool weapon_arm(client_member* c1,int choice,fight_room* this_room);//将所给的武器号choice进行装备

void confirm_send_success(client_member* member,char* StrBuf);//检测对方消息是否收到报文StrBuf
void check_emptyspace();//计算当前有无服务器空位
void show_all_client(client_member* member);//向客户端显示所有在线人员列表
void exit_the_room(client_member* member);//用户退出房间
//void client_offline(client_member* member);//客户端下线结算
void init_server_set();//初始化服务器配置
bool check_empty_room();//检查有没有空房间，有的话返回true，没有则返回false
bool check_in_use_room();//检查有没有人在用房间
void show_all_room(client_member* member);//显示被使用的房间
bool check_client_reply(client_member* c1);
//special_transmit.h
void sp_transmit(client_member* c1,float message);//特化信息传输
void sp_transmit(client_member* c1,bool message);
void sp_transmit(client_member* c1,int message);
void transmit_card(client_member* c1,player* p1,bool himself);
void card_change(client_member* c1,fight_room* this_room);//手卡变化
void refresh_character_state_F(client_member* c1,fight_room* this_room);//首次记录人物状态 
void transmit_this_character_F(client_member* c1,player* p1);//首次传输角色信息、人物效果、血量、攻击
void blood_change(float number,client_member* c1,fight_room* this_room);
void transmit_blood(client_member* c1,player* p1);//血量变化
void transmit_attack(client_member* c1,player* p1);
void attack_change(float number,float* ex_attack,client_member* c1,fight_room* this_room);//攻击力变化
void weapon_change(client_member* c1,fight_room* this_room);
void transmit_weapon(client_member* c1,player* p1);//武器变化
void buff_change(client_member* c1,fight_room* this_room);//个人buff变化
void transmit_buff(client_member* c1,player* p1);

void lock_other_N(client_member* c1,fight_room* this_room);
void release_other_N(client_member* c1,fight_room* this_room);


char* search_card(int situation,int number);                     //查询编号代表的手卡效果
void ini_catalog();//初始化卡片目录