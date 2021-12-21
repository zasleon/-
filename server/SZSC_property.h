#ifndef INCLUDE_SZSC_property_H
#define INCLUDE_SZSC_property_H

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


#define playernumber		2 //目前只建立两个角色
#define weaponlimit			2//武器上限为2
#define cardlimit			10//手卡上限为10
#define decklimit			8//预设卡组数量为8张
#define abilitylimit		4//自身能力最多四个
#define weaponeffectlimit	5//携带武器效果最多15个
#define garbagelimit		1000//废卡区大小
#define roomlimit			250//服务器战斗房间上限


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

#define do_attack					11111//是否需要进行普攻？（做选择时使用）
#define make_shield					11112//发动隐藏天盾
#define do_choice					21111//确认选择
#define cancel_choice				31111//选择拒绝
#define robot_symbol				41111//机器人识别码



#define	battle_choice				6900//让客户端显示战斗选项一览
#define	character_choice			6901//让客户端显示角色选取界面


#define event_happen				7001//某事件发生了,强制锁定别人的自由行动输入输出
#define event_end					7002//事件结束，解除别人的自由行动输入输出
#define event_happen_N				7003//事件发生，不思考
#define your_event					7004//你的事件

#define SZSC_apply_create_room		7010
#define	SZSC_apply_enter_room		7011
#define SZSC_apply_exit_room		7012
#define SZSC_apply_add_robot		7013
#define SZSC_apply_remove_robot		7014
#define SZSC_apply_start_game		7015
#define SZSC_apply_exit_roomlist	0



//#define lock_action					7004//【图形界面客户端】防止恶意或过快点击输入
//#define release_action				7005//【图形界面客户端】允许输入

//#define refresh_state				7081//开始传输人物数据了
//#define please_reload				7082//重新传输
#define refresh_first_state			7100//首次传输人物数据
#define refresh_state_CM			7101//传输自己人物手卡数据
#define refresh_state_CH			7102//传输对手手卡数据
#define refresh_state_blood_M		7103//更新血量信息
#define refresh_state_blood_H		7104//更新对手血量信息
#define refresh_state_buff_M		7105//更新自己buff信息
#define refresh_state_buff_H		7106//更新对手buff信息
#define refresh_state_attack_M		7107//更新自己攻击力信息
#define refresh_state_attack_H		7108//更新对手攻击力信息
#define refresh_state_weapon_M		7109//更新自己武器信息
#define refresh_state_weapon_H		7110//更新对手武器信息

#define get_in_room					7200//你进入房间了
#define leave_room					7201//你退出房间了
#define someone_get_in				7202//有人进入房间了
#define you_are_guest				7203//你是房间宾客
#define you_are_host				7204//你是房间主人
#define please_choose_room			7205//请选择哪个房间
#define stop_show_room				7206//停止显示可选择的房间
#define enter_room_success			7207//你进入房间成功了
#define someone_leave				7208//有人离开房间了
#define someone_offline				7209//有人掉线了
#define choice_complete				7210//角色全选完了
#define you_win						7211//游戏结束，你赢了
#define you_lose					7212//游戏结束，你输了
#define game_standoff				7213//游戏结束，平局
#define you_not_in_room				7214//你不在房间中
#define no_room_in_use				7215//没有房间可以进入
#define reopen_choice				7216//重新选择角色
#define cancel_enter_room			7217//取消加入房间

#define	start_turn_settle			7300//开始回合结算
#define	start_another_turn			7301//本回合开始
#define	show_weapon_list			7302//【图形界面客户端】显示武器栏武器和效果
#define	get_c_number				7303//【图形界面客户端】想要装备武器，让他发送来想要装备第几张手卡
#define	get_we_number				7304//【图形界面客户端】获取发动第几个效果
#define show_buff_list				7305//【图形界面客户端】显示buff栏效果
#define chooseYN					7306//【图形界面客户端】做“是否”选择，一般后面都跟一句问话
#define whether_fight_back			7307//是否反击？你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.加附效果）\n请选择：
#define show_rivals_card_P			7308//显示对手所有手卡并抢夺
#define show_enemy_list				7309//显示敌人目录
#define show_alive_list				7310//显示存活者目录
#define now_is_your_turn			7311//确认是你的回合
#define whether_launch_hideeffect	7312//是否发动卡片隐效果？
#define card_launch					7313//发动了卡片
#define delete_which_weapon			7314//【图形界面客户端】丢弃哪一个武器？
#define which_to_hatred				7315//对什么发动憎恨？？？
#define get_w_number				7316//选择了哪个一武器？

#define	state_discard_one_card		7400//【图形界面客户端】进入“丢弃单张手卡“的操作状态
#define	state_discard_muti_card		7401//【图形界面客户端】进入”丢弃多张手卡”的操作状态
#define	state_return_normal			7402//【图形界面客户端】回到原始“使用手卡”的操作状态
#define state_discard_free_card		7403//【图形界面客户端】进入“选择任意张手卡“的操作状态
#define state_choose_one_card		7404//【图形界面客户端】进入“发动单张手卡”的操作状态
#define state_fight_back			7405//【图形界面客户端】进入“发动反击”的操作状态


int empty_player_pointer;//指向当前服务器客户端空位
int empty_room_pointer;//指向当前房间空位

struct client_member;

struct fight_room//战斗房间
{	
	bool room_in_use;//false为空，true为占用

	client_member* c[playernumber];//房间主人阵营

	//client_member* c2;
	//bool c2_in_use;//p2是否有人
	//bool member_full;//人全不全，是否达到开一局游戏的条件
	//player* player_all[playernumber];
	int ready_number;//双方角色是否选完
	int ready_number2;//回合结算是否完成

	int deck[decklimit];//卡组，Q：为什么要预设卡组？不能直接随机发放？A：有些角色有“预测未来(即预测即将抽到的卡)”的能力，因此需要预设卡。
	int playerturn;//谁的回合，初始为0，即先选完的人先开始回合
	int passturns;//总轮回数，最初为1
	bool turn_force_end;//该回合是否被强制结束？

};
fight_room player_room[roomlimit];


























#endif