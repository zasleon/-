#include "winsock2.h"
#include <stdio.h>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")
#include <iostream>
#include <conio.h>

using namespace std;

#define start_link					6666//告诉客户端成功通讯连接了（当客户端收不到这个数据时会告诉自己“服务器可能超载了”）
#define please_cls					6667//让客户端刷新屏幕system("cls");
#define show_choice					6668//让客户端显示选择界面
#define show_user					6669//让客户端显示所有用户状态
#define	stop_show_user				6700//停止显示所有用户状态
#define client_get_message			6701//客户端发来自己接收到信息
#define show_video					6702//视频功能

#define	in_game						6800//处于游戏状态
#define in_room						6801//处于在房间状态
#define	in_online					6802//处于在线状态

#define	battle_choice				6900//让客户端显示战斗选项一览
#define	character_choice			6901//让客户端显示角色选取界面

#define force_offline				7000//客户端下线了
#define event_happen				7001//某事件发生了,强制锁定别人的自由行动输入输出
#define event_end					7002//事件结束，解除别人的自由行动输入输出
#define event_happen_N				7003//事件发生，不思考
#define lock_action					7004//【图形界面客户端】防止恶意或过快点击输入
#define release_action				7005//【图形界面客户端】允许输入

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
#define leave_room					7201//退出房间了
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

#define	state_discard_one_card		7400//【图形界面客户端】进入“丢弃单张手卡“的操作状态
#define	state_discard_muti_card		7401//【图形界面客户端】进入”丢弃多张手卡”的操作状态
#define	state_return_normal			7402//【图形界面客户端】回到原始“使用手卡”的操作状态
#define state_discard_free_card		7403//【图形界面客户端】进入“选择任意张手卡“的操作状态
#define state_choose_one_card		7404//【图形界面客户端】进入“发动单张手卡”的操作状态
#define state_fight_back			7405//【图形界面客户端】进入“发动反击”的操作状态

#define android_phone				9999//安卓手机端
#define win_console					9998//win32控制台版

#define p_name		1//查询名字
#define p_details	2//查询详细信息