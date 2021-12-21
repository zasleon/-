
#ifndef INCLUDE_UNO_property_H
#define INCLUDE_UNO_property_H




#define UNO_member_limit	4//一局游戏上限玩家数量
#define UNO_room_limit		4//房间上限数量

//牌色
#define UNO_green	1
#define UNO_red		2
#define UNO_blue	3
#define UNO_yellow	4
#define UNO_black	5



//特殊功能
#define UNO_none	-1//卡槽为空
#define UNO_ADD_2	100
#define UNO_ADD_4	200
#define UNO_reverse	300//反向
#define UNO_forbid	400//跳过下一个人

//房间内某一位子的状态
#define	UNO_empty		0//该位子为空
#define UNO_human		1//人类玩家
#define UNO_simple_AI	2//简单机器人
#define UNO_hard_AI		3//困难机器人

//玩家当前状态
//#define	in_online					6802//处于在线状态
#define UNO_in_room					6803//处于uno房间中
#define UNO_in_game					6804//处于uno游戏中
#define UNO_in_roomlist				6805//处于选择uno房间列表中


struct client_member;
struct UNO_player
{
	client_member* c1;
	char	name[30];
	int		state;//状态栏，人类或机器人
	int		card_effect[108];//手卡内容
	int		card_color[108];
	SYSTEMTIME	enter_time;//进入房间时间
};

static int UNO_room_in_use;//当前有使用的房间数量
static int UNO_empty_room_pointer;
struct UNO_room
{
	int		deck_card_effect[108];//卡组
	int		deck_card_color[108];

	int		tomb_card_effect[108];//废卡区
	int		tomb_card_color[108];

	UNO_player player[UNO_member_limit];
	int		who_is_master;//-1-4,谁是房主，-1表示房间为空
	int		room_member;//当前在房间内人员的总数量
	
};

UNO_room uno_room[UNO_room_limit];

#define UNO_apply_create_room	2000
#define	UNO_apply_enter_room	2001
#define UNO_apply_exit_room		2002
#define UNO_apply_add_robot		2003
#define UNO_apply_start_game	2004
#define UNO_apply_exit_roomlist	0


#define UNO_room_full			2050//房间已满，没法继续创建房间
#define UNO_no_room_in_use		2051//当前没有房间在使用，无法加入房间
#define UNO_create_room_success	2052//创建房间成功
#define UNO_enter_room_success	2053//加入房间成功
#define UNO_someone_get_in		2054//有人进房间了
#define UNO_show_roomlist		2055//向客户端展示房间列表
#define UNO_leave_room_success	2056//离开房间成功
#define UNO_show_roomlist_end	2057//显示房间结束
#define	UNO_cannot_exit_room	2058//当前状态无法退出uno房间
#define	UNO_cannot_enter_room	2059//当前状态无法进入别人的uno房间!
#define	UNO_cannot_create_room	2060//当前状态无法创建uno房间!
#define UNO_cancel_enter_room	2061//取消加入其他人的房间








#endif