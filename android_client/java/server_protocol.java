package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/1.
 */
public class server_protocol {
    public final static int weaponlimit = 2;//武器上限为2
    public final static int cardlimit = 10;//手卡上限为10
    public final static int decklimit = 8;//预设卡组数量为8张
    public final static int abilitylimit = 4;//自身能力最多四个
    public final static int weaponeffectlimit = 5;//携带武器效果最多15个
    public final static int garbagelimit = 1000;//废卡区大小

    public final static int start_link					=6666;//告诉客户端成功通讯连接了（当客户端收不到这个数据时会告诉自己“服务器可能超载了”）
    public final static int please_cls					=6667;//让客户端刷新屏幕system("cls");
    public final static int show_choice					=6668;//让客户端显示选择界面
    public final static int show_user					=6669;//让客户端显示所有用户状态
    public final static int stop_show_user				=6700;//停止显示所有用户状态
    public final static int client_get_message			=6701;//客户端发来自己接收到信息
    public final static int show_video  			        =6702;

    public final static int apply_refresh_online_member=-4;//请求刷新页面

    public final static int in_game						=6800;//处于游戏状态
    public final static int in_room						=6801;//处于在房间状态
    public final static int in_online					=6802;//处于在线状态

    public final static int battle_choice				=6900;//让客户端显示战斗选项一览
    public final static int character_choice			=6901;//让客户端显示角色选取界面

    public final static int you_are_in_the_lobby       =7000;//返回大厅界面
    public final static int force_offline				=7000;//客户端下线了
    public final static int event_happen				=7001;//某事件发生了,强制锁定别人的自由行动输入输出
    public final static int event_end					=7002;//事件结束，解除别人的自由行动输入输出
    public final static int event_happen_N				=7003;//事件发生，不思考
    public final static int your_event					=7004;//你的事件
    //public final static int lock_action					=7004;//【图形界面客户端】防止恶意或过快点击输入
    //public final static int release_action				=7005;//【图形界面客户端】允许输入


    public final static int SZSC_apply_create_room		=7010;
    public final static int SZSC_apply_enter_room		=7011;
    public final static int SZSC_apply_exit_room		=7012;
    public final static int SZSC_apply_add_robot		=7013;
    public final static int SZSC_apply_remove_robot	=7014;
    public final static int SZSC_apply_start_game		=7015;
    public final static int SZSC_apply_exit_roomlist	=0;


    //#define refresh_state				7081//开始传输人物数据了
//#define please_reload				7082//重新传输
    public final static int refresh_first_state			=7100;//首次传输人物数据
    public final static int refresh_state_CM			=7101;//传输自己人物手卡数据
    public final static int refresh_state_CH			=7102;//传输对手手卡数据
    public final static int refresh_state_blood_M		=7103;//更新血量信息
    public final static int refresh_state_blood_H		=7104;//更新对手血量信息
    public final static int refresh_state_buff_M		=7105;//更新自己buff信息
    public final static int refresh_state_buff_H		=7106;//更新对手buff信息
    public final static int refresh_state_attack_M		=7107;//更新自己攻击力信息
    public final static int refresh_state_attack_H		=7108;//更新对手攻击力信息
    public final static int refresh_state_weapon_M		=7109;//更新自己武器信息
    public final static int refresh_state_weapon_H		=7110;//更新对手武器信息

    public final static int get_in_room					=7200;//你进入房间了
    public final static int leave_room					=7201;//退出房间了
    public final static int someone_get_in				=7202;//有人进入房间了
    public final static int you_are_guest				=7203;//你是房间宾客
    public final static int you_are_host				=7204;//你是房间主人
    public final static int please_choose_room			=7205;//请选择哪个房间
    public final static int stop_show_room				=7206;//停止显示可选择的房间
    public final static int enter_room_success			=7207;//你进入房间成功了
    public final static int someone_leave				=7208;//有人离开房间了
    public final static int someone_offline				=7209;//有人掉线了
    public final static int choice_complete				=7210;//角色全选完了
    public final static int you_win						=7211;//游戏结束，你赢了
    public final static int you_lose					    =7212;//游戏结束，你输了
    public final static int game_standoff				=7213;//游戏结束，平局
    public final static int you_not_in_room				=7214;//你不在房间中
    public final static int no_room_in_use				=7215;//没有房间可以进入
    public final static int reopen_choice				=7216;//重新选择角色
    public final static int cancel_enter_room			=7217;//取消加入房间

    public final static int start_turn_settle			=7300;//开始回合结算
    public final static int start_another_turn			=7301;//本回合开始
    public final static int show_weapon_list			=7302;//【图形界面客户端】显示武器栏武器和效果
    public final static int get_c_number				=7303;//【图形界面客户端】想要装备武器，让他发送来想要装备第几张手卡
    public final static int get_we_number				=7304;//【图形界面客户端】获取发动第几个效果
    public final static int show_buff_list				=7305;//【图形界面客户端】显示buff栏效果
    public final static int chooseYN					    =7306;//【图形界面客户端】做“是否”选择，一般后面都跟一句问话
    public final static int whether_fight_back			=7307;//是否反击？你是否行动？（0:不行动；1:发动手卡；2:发动个人效果；3:发动武器效果；4.加附效果）\n请选择：
    public final static int show_rivals_card_P			=7308;//显示对手所有手卡并抢夺
    public final static int show_enemy_list				=7309;//显示敌人目录
    public final static int show_alive_list				=7310;//显示存活者目录
    public final static int now_is_your_turn			=7311;//确认是你的回合
    public final static int whether_launch_hideeffect	=7312;//是否发动卡片隐效果？
    public final static int card_launch					=7313;//发动了卡片
    public final static int delete_which_weapon			=7314;//【图形界面客户端】丢弃哪一个武器？
    public final static int which_to_hatred				=7315;//对什么发动憎恨？？？get_w_number
    public final static int get_w_number				=7316;//选中了哪一个武器？

    public final static int state_discard_one_card		=7400;//【图形界面客户端】进入“丢弃单张手卡“的操作状态
    public final static int state_discard_muti_card	=7401;//【图形界面客户端】进入”丢弃多张手卡”的操作状态
    public final static int state_return_normal			=7402;//【图形界面客户端】回到原始“使用手卡”的操作状态
    public final static int state_discard_free_card	=7403;//【图形界面客户端】进入“选择任意张手卡“的操作状态
    public final static int state_choose_one_card		=7404;//【图形界面客户端】进入“发动单张手卡”的操作状态
    public final static int state_fight_back			=7405;//【图形界面客户端】进入“发动反击”的操作状态

    public final static int android_phone				=9999;//安卓手机端
    public final static int win_console					=9998;//win32控制台版

    public final static int UNO_apply_create_room	    =2000;//
    public final static int UNO_apply_enter_room	    =2001;
    public final static int UNO_apply_exit_room		    =2002;
    public final static int UNO_apply_add_robot		    =2003;
    public final static int UNO_apply_exit_roomlist	=0;

    public final static int UNO_room_full			    =2050;//没法继续创建房间
    public final static int UNO_no_room_in_use		    =2051;//当前没有房间在使用，无法加入房间
    public final static int UNO_create_room_success	=2052;//创建房间成功
    public final static int UNO_enter_room_success	    =2053;//加入房间成功
    public final static int UNO_someone_get_in		    =2054;//有人进房间了
    public final static int UNO_show_roomlist          =2055;
    public final static int UNO_leave_room_success     =2056;
    public final static int UNO_show_roomlist_end      =2057;
    public final static int UNO_cannot_exit_room       =2058;//当前状态无法退出uno房间!
    public final static int UNO_cannot_enter_room	    =2059;//当前状态无法进入别人的uno房间!
    public final static int UNO_cannot_create_room     =2060;//当前状态无法创建uno房间!
    public final static int UNO_cancel_enter_room      =2061;//取消加入房间
}
