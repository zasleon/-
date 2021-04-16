#ifndef INCLUDE_server_property_H
#define INCLUDE_server_property_H

#include "iostream"
#include "winsock2.h"
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>

#include "player_property.h"
using namespace std;
#pragma comment(lib,"Ws2_32.lib")

#define roomlimit					250//服务器战斗房间上限
#define memberlimit					20//服务器承载人数上限
#define sleeptime					50//服务器稍微延迟发送数据时间

#define do_attack					11111//是否需要进行普攻？（做选择时使用）
#define make_shield					11112//发动隐藏天盾
#define do_choice					21111//确认选择
#define cancel_choice				31111//选择拒绝
#define robot_symbol				41111//机器人识别码

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
#define your_event					7004//你的事件
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

#define android_phone				9999//安卓手机端
#define win_console					9998//win32控制台版

int client_number;//总连接人员
int empty_player_pointer;//指向当前服务器客户端空位
int empty_room_pointer;//指向当前房间空位

struct client_member//客户端
{
	int member_No;
	bool member_in_use;//客户端该位置是否被占用？false为空，true为占用
	char client_name[30];//用户名

	int device;//手机或者电脑
	int state;//客户状态：房间/在线/战斗中
	int room_No;//自己所在房间号
	bool guest;//在房间中是否是宾客？true为宾客，false可能为主人或不在房间内

	bool F_offline;

	player* p1;
	player body;

	bool be_locked; 

	char message[MAX_PATH];
	bool not_be_read;
	bool listen;//是否监听中
	HANDLE listen_this;
	char p_message[MAX_PATH];

};

client_member Membership[memberlimit+1];
SOCKET g_ServerSocket, g_ClientSocket[memberlimit+1]; //创建服务端 客户端套接字
HANDLE hthread[memberlimit+1];

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
client_member Robot[roomlimit];//房间机器人

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


void addtext(char* StrBuf,int number);
void addtext(char* StrBuf,char* Buf);
void addtext(char* StrBuf,float number);


#endif