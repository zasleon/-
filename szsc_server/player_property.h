#ifndef INCLUDE_player_property_H
#define INCLUDE_player_property_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <fstream>
using namespace std;

//卡号0-1000
//装备卡0-29
//助攻卡30-59
//效果卡60-399
//隐效果400-499
//场景卡500-699
//结界卡700-1000

//卡片效果0000-1000
//武器效果1000-2000
//人物效果10000-20000


#define playernumber	  2 //目前只建立两个角色
#define weaponlimit		  2//武器上限为2
#define cardlimit		  10//手卡上限为10
#define decklimit		  8//预设卡组数量为8张
#define abilitylimit	  4//自身能力最多四个
#define weaponeffectlimit 5//携带武器效果最多15个
#define garbagelimit      1000//废卡区大小

#define p_name		1//查询名字
#define p_details	2//查询详细信息

//#define you_win     990//你赢了
//#define you_die     991//你死了（双打使用）
//#define you_lose    992//你输了
#define one_die     993//有人死了
#define still_fight 994//仍然在打

#define fore_effect  950//前续效果
#define after_effect 951//后续效果

//设置状态场合situation
#define nothing_else         900//没别的事了，填充
#define card_effect_finish   998//手卡效果使用结束
#define i_cancel_effect      999//我不发动效果

#define my_turn			     1000//我的回合我主动使用
#define friend_turn          1001//队友的回合
#define enemy_turn           1002//敌人的回合

#define myturn_attack        1010//我的回合我主动发起普攻
#define i_will_attack        1010//我即将普攻


#define i_hit_someone	     1020//自己成功让b伤血
#define i_effect			 1021//自己成功发动效果
#define i_effect_b			 1022//自己成功发动效果伤血
#define i_attack			 1023//自己成功发动普攻
#define i_attack_T			 1024//自己成功发动穿攻

#define i_failed_attack_E    1030//自己普攻被躲避
#define i_failed_attack_D    1031//自己普攻被格挡
#define i_failed_attack_W    1032//自己普攻命中了稻草人
#define i_failed_effect      1035//自己发动效果失败
#define i_failed_effect_B    1036//自己发动伤血效果失败
#define i_failed_damage		 1037//自己没对对面造成伤害


#define i_will_be_attacked   1050//自己即将受到普攻
#define i_will_be_effect     1051//自己即将受到效果
#define i_will_be_effect_A   1052//自己即将受到自主伤血效果
#define i_will_be_effect_B   1053//自己即将受到别人发动的伤血效果
//#define i_will_be_effect_C   1054//自己即将受到自己给自己发动的效果
#define i_will_be_hit_die    1054//自己即将受到致命伤血
#define i_will_be_unmoveable 1055//我即将受到无法行动的效果

#define i_will_die           1060//自己血量归零即将死亡

#define i_be_attacked		 1070//自己已经受到普攻了
#define i_be_effect			 1071//自己已经受到效果
#define i_be_effect_A	     1072//自己已经受到自主伤血效果
#define i_be_effect_B        1073//自己已经受到别人发动的伤血效果
#define i_be_hit             1074//自己已经受到因为别人行动而造成的伤血了

#define i_escape_attack			2000//自己躲避成功了一次普攻
#define i_defend_attack			2001//自己格挡成功了一次普攻
#define i_erase_effect			2002//自己成功使一次效果发动无效
#define i_erase_effect_B		2003//自己成功使一次效果效果发动无效
#define i_immune_effect			2004//自己免疫此次效果

#define i_public_card         2050//自己公开手卡
#define i_discard_card_A      2051//自己为了发动效果，主动丢手卡
#define i_discard_card_B      2052//自己因为别人的效果，被动丢手卡
#define i_discard             2053//自己丢手卡了
#define i_get_card            2054//自己抽卡了

#define i_will_discard			2060//自己要丢手卡了
#define i_will_public_card		2061//自己即将公开手卡
#define i_will_discard_card_A	2062//自己即将主动丢手卡
#define i_will_discard_card_B	2063//自己即将因别人的效果丢手卡

#define sb_card_used			2079//有人发动了一张卡
#define i_card_used				2080//自己使用了一张手卡
#define i_card_used_S			2081//自己成功使用了一张手卡
#define few_card				2082//自己手卡不足
#define card_discard			2083//丢手卡流程走完			

#define use_assist				4000//自己使用了助攻卡
#define has_fought				4001//自己参与了一次搏斗

#define force_end_turn			4050//强制结束回合
#define	force_end_event			4051//强制结束该事件

#define unmove_time				4060//此次行动内无法行动
#define unmove_turn				4061//回合内无法行动
#define overdeath_turn			4062//不死状态持续轮回数
#define hideshield_turn			4063//隐天盾持续轮回数
#define silence_turn			4064//个人效果被沉默回合数
#define buff_clean_all			4065//个人buff全清


struct player
{

	char	name[30];//"xx号玩家：xx"
	int		character_number;//角色序号
	int		player_No;//第几个玩家
	bool	whether_myturn;//是否是当前角色的回合？是的为1，不是为0
	bool	alive;//是否存活,true为活，false为死
	float	bloodlimit;//血量上限
	float	blood;//当前血量
	float	attack;								float	exattack_turn;//该回合自身增加的额外攻击力
												float	exattack_time;//该次搏斗自身增加的额外攻击力//基础攻击力					

	bool a[abilitylimit];						int ability[abilitylimit];//自身能力最多四个
	
	bool c[cardlimit];							int card[cardlimit];//拥有手卡		
	bool h[cardlimit];							int hideeffect[cardlimit];//隐效果		
	
	bool w_valid[weaponlimit];//武器暂时性有效无效
	bool w[weaponlimit];	   					int weapon[weaponlimit];//装备槽内分别是什么武器，以武器号表明
												float exattack_weapon[weaponlimit];//第几格武器槽增加的攻击力
												float exattack_weapon_turn[weaponlimit];//第几格武器槽该回合增加的攻击力
												float exattack_weapon_time[weaponlimit];//第几格武器槽该次搏斗增加的攻击力

	bool w_e[weaponlimit][weaponeffectlimit];	int weapon_effect[weaponlimit][weaponeffectlimit];//装备武器后拥有的效果
	                          
	//int storage_weapon[10];//破败披风效果专属，统计自己有多少把武器，可以不停与装备武器切换，即披风内存放了大量武器，但只能抽出其中两个进行使用【有待设计】
    //int speed;//人物速度【有待设计】
	int		fight_chance;//自己当前拥有的普攻次数
	int		whether_in_attack;//是否处于搏斗中
	bool	whether_attack_T;//是否处于穿攻状态
	bool	cannot_ED;//自己此次普攻无法躲避和格挡
	int		whether_use_assist;//是否使用了助攻卡？不是则置为-1
	bool	i_soon_die;//我是否即将死亡？

    int		un_move_able;//不可行动的回合
	bool	unmoveable_time;//在某行动内不可行动
	int		selfeffect_invalid;//自身效果被无效化的回合

	int		hideshield;			//隐天盾剩余回合数
	int		godlaser;			//神尚激光剩余轮回数
	int		godlaserdamage;		//神尚激光伤害数值
	int		overdeath;			//持续不死状态的轮回数(黑曜剑)
	
	int		attacktime_turn;	//此回合自己普攻成功次数

	float	hurtmultiple_turn;	//该回合自己造成的普攻伤血倍率
	float	hurtmultiple_time;	//该次行动自己造成的普攻伤血倍率

	int		weaponeffect001[weaponlimit];//0号（承皇剑）武器效果1剩余次数
	int		weaponeffect002[weaponlimit];//0号（承皇剑）武器效果2剩余次数
	int		weaponeffect051[weaponlimit];//5号食腐再生装置1效果剩余次数
	int		weaponeffect052[weaponlimit];//5号食腐再生装置2效果剩余次数
	int		weaponeffect06[weaponlimit];//6号恩空法棒魔力指示物个数

};
//player player_all[playernumber];//p[0]永远是自己





























#endif