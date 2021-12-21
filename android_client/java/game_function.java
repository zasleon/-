package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/1.
 */
import android.graphics.Color;
import android.view.Gravity;
public class game_function extends Thread implements Runnable{//游戏房间处理信息
    final static client_socket cc = client_socket.getSingleIntance();
    public static android.app.Activity activity;
    private static android.os.Handler handler = new android.os.Handler();

    public static game_function mm;
    private static boolean stop_reply;//是否需要回复自己受到信息？true为不回复，false为回复
    private static boolean leave_this_room;//是否退出房间
    public static boolean my_turn;//是否是我的回合？
    private static String my_name;//自己全称：x号玩家：xx
    final private static int fastsleep=20;

    public static int colorkind;//变换颜色种类
    final public static int colorkindlimit=4;//上限颜色种类
    final public static int green_background=1;
    final public static int blue_background=2;
    final public static int yellow_background=3;
    final public static int cyan_background=4;

    public static void start_service()
    {
        game_card.ini_catalog();
        game_function.mm = new game_function();
        game_function.mm.start();
    }
    @Override
    public void run()
    {//开始监听发来什么消息//开始监听发来什么消息
        //final android.widget.TableLayout history = (android.widget.TableLayout) activity.findViewById(R.id.history);
        stop_reply = false;//默认需要回复
        leave_this_room=false;//不用退出房间
        colorkind=green_background;
        record = "";

        while (true) {

            monitor_all.just_release();
            telecom.listen();//开始监听服务器发来的数据，一般该线程都阻塞在这里
            //fight_room.system_event = true;//收到服务器数据了，封锁用户发送数据的行为，开始处理发来的数据
            monitor_all.just_lock();
            if (cc.message.length() == 4)//如果是特殊信号，处理特殊信号
                process_signal(Integer.parseInt(cc.message));
            else {
                add_log(cc.message);//如果不是特殊信号，在历史log里进行输出

                if(cc.message.length()<=2)stop_reply=true;
            }

            if(leave_this_room)//战斗房间关闭
            {
                activity.finish();
                monitor_all.stop_send(500);
                lobby.state=lobby.in_room;
                return;
            }
            telecom.tsleep(80);
            if (!stop_reply) //如果说需要回复“收到”，stop_reply=true表示不用回复
                telecom.reply_i_get();//回复自己收到该数据了
            stop_reply = false;//无论是否需要回复，统统置为需要回复
            monitor_all.stop_send(500);

            //if (!lock_send)fight_room.system_event = false;//如果没封锁用户输入，解放用户“输入”的行为
        }
    }


    private static void process_signal(int signal)//处理服务器发来的特殊信号
    {
        int effect_number;
        switch (signal) {
            case server_protocol.your_event:
                change_event_background_color();//变换事件背景颜色
                break;
            case server_protocol.event_happen:
            case server_protocol.event_happen_N:
                change_event_background_color();//变换事件背景颜色
                telecom.tsleep(50);

                break;
            case server_protocol.event_end:
                break;
            case server_protocol.you_lose:lobby_monitor.add_log("游戏结束!\n你输了!!!");
                activity.finish();
                telecom.tsleep(50);
                telecom.reply_i_get();
                leave_this_room=true;
                stop_reply=true;
                break;
            case server_protocol.you_win:lobby_monitor.add_log("游戏结束!\n你赢了!!!");
                activity.finish();
                telecom.tsleep(50);
                telecom.reply_i_get();
                leave_this_room=true;
                stop_reply=true;
                break;
            case server_protocol.game_standoff:lobby_monitor.add_log("游戏结束!\n平局!!!");
                activity.finish();
                telecom.tsleep(50);
                telecom.reply_i_get();
                leave_this_room=true;
                stop_reply=true;
                break;
            case server_protocol.someone_offline://选角色时有人掉线
                activity.finish();
                telecom.tsleep(50);
                telecom.reply_i_get();
                leave_this_room=true;
                stop_reply=true;
                break;
            case server_protocol.someone_leave:
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.listen();//发来离开者姓名或“你成为房间主人you_are_host”
                if (!cc.message.equals(Integer.toString(server_protocol.you_are_host))){
                    lobby_monitor.add_log(" 玩家:  " + cc.message + "  离开了房间!");
                    telecom.tsleep(50);
                    telecom.reply_i_get();
                }
                else{
                    telecom.tsleep(50);
                    telecom.reply_i_get();//自己变成房间主人
                    telecom.listen();//发来离开者姓名
                    lobby_monitor.add_log("玩家:  " + cc.message + "   离开了房间!\n你变成房间主人!");
                    telecom.tsleep(50);
                    telecom.reply_i_get();
                    handler.post(new Runnable() {
                        @Override
                        public void run() {//变更按钮
                            android.widget.Button start_fight = (android.widget.Button) private_room.activity.findViewById(R.id.button6);
                            lobby_monitor.whether_host = true;
                            start_fight.setEnabled(true);//“开始”按钮能按了
                        }
                    });
                }
                break;
            case server_protocol.start_turn_settle:
                change_event_background_color();
                add_log("开始进行回合结算!");
                monitor_all.stop_send(3000);
                whether_my_turn(false);//刷新是否是你的回合，默认不是
                break;
            case server_protocol.now_is_your_turn:
                whether_my_turn(true);//确认当前是我的回合
                break;

            case server_protocol.start_another_turn:
                add_log("你可以开始新一回合的行动了!");
                change_event_background_color();//变换事件背景颜色
                break;
            case server_protocol.card_launch:
                telecom.tsleep(30);
                telecom.reply_i_get();
                telecom.listen();//接收发动卡片者名字
                add_log(cc.message + "发动卡片:");
                telecom.tsleep(30);
                telecom.reply_i_get();
                telecom.listen();//接收发动卡片号
                effect_number=Integer.parseInt(cc.message);
                while(effect_number==server_protocol.card_launch) {
                    telecom.tsleep(30);
                    telecom.reply_i_get();
                    effect_number = Integer.parseInt(cc.message);
                }
                if(effect_number<10000)
                    add_log("【" + game_card.search_card(game_card.p_name, effect_number) + "】\n" + "【卡片效果】\n" + game_card.search_card(game_card.p_details, effect_number));
                else
                    add_log("【奇怪卡片号】"+effect_number);
                telecom.tsleep(20);
                telecom.reply_i_get();
                stop_reply = true;
                break;
            case server_protocol.delete_which_weapon:
                show_weapon_list();
                stop_reply = true;
                break;
            case server_protocol.show_enemy_list:
                show_enemy_aim_list();
                stop_reply = true;
                break;
            case server_protocol.show_alive_list:
                show_alive_aim_list();
                stop_reply = true;
                break;
            case server_protocol.chooseYN://做是否选择
                stop_reply = true;
                show_YNchoose();
                break;
            case server_protocol.show_buff_list:
                telecom.tsleep(50);
                telecom.reply_i_get();
                show_buff_effect_list();
                stop_reply = true;
                break;
            case server_protocol.show_weapon_list:
                telecom.tsleep(50);
                telecom.reply_i_get();
                stop_reply = true;
                show_weapon_effect_list();
                break;
            case server_protocol.show_rivals_card_P:
                telecom.tsleep(50);
                rival.ini_card();//清空本地内存中的对手手卡原有内容
                telecom.reply_i_get();
                refresh_rival_card_Info();//获取对手手卡信息
                show_rival_card_plunder();//用列表显示对手手卡信息并选择一张并抢夺
                stop_reply = true;
                break;
            case server_protocol.which_to_hatred:
                hatred_what();
                stop_reply = true;
                break;
            case server_protocol.get_c_number:
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.tsleep(100);
                telecom.send(String.valueOf(fight_room.whichcard));//发送自己想用第几张
                stop_reply = true;
                break;
            case server_protocol.get_w_number://发送第几个武器
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.tsleep(100);
                telecom.send(String.valueOf(fight_room.whichweapon));
                stop_reply = true;
                break;
            case server_protocol.get_we_number://获取该武器第几个效果
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.tsleep(100);
                telecom.send(String.valueOf(fight_room.whicheffect));
                stop_reply = true;
                break;
            case server_protocol.whether_launch_hideeffect://是否发动隐藏效果？
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.tsleep(100);
                if(fight_room.use_hideeffect)
                    telecom.send("0");
                else
                    telecom.send("1");
                stop_reply = true;
                break;
            case server_protocol.state_fight_back://开始反击行动
            case server_protocol.state_discard_free_card://因为没规定数量丢卡，所以不知道什么时候停止丢，需要用户手动确认选择完成
            case server_protocol.state_discard_muti_card://达到数量直接提交选择。只有清空和选择操作，没有确认所有操作
            case server_protocol.state_discard_one_card:
            case server_protocol.state_return_normal:
            case server_protocol.state_choose_one_card://选择单张手卡发动
                change_button_UI(signal);
                break;

            case server_protocol.refresh_state_weapon_M:
                refresh_info_Weapon(true);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_weapon_H:
                refresh_info_Weapon(false);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_blood_M:
                refresh_info_Blood(true);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_blood_H:
                refresh_info_Blood(false);
                stop_reply = true;
                break;
            case server_protocol.refresh_first_state:
                refresh_info_F();//首次传输人物信息
                stop_reply = true;
                break;
            case server_protocol.refresh_state_CH:
                refresh_info_card(false);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_CM:
                refresh_info_card(true);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_attack_M:
                refresh_info_Attack(true);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_attack_H:
                refresh_info_Attack(false);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_buff_M:
                refresh_info_Buff(true);
                stop_reply = true;
                break;
            case server_protocol.refresh_state_buff_H:
                refresh_info_Buff(false);
                stop_reply = true;
                break;

            default://如果无法找到信号特征，显示出信号
                add_log(cc.message);
        }
    }

    public static String record;
    public static void add_log(String log)//添加最新的历史记录
    {
        record = log;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String history_log = game_function.record;
                game_function.record = "";
                history_log = process_message(history_log);
                final android.widget.TableLayout history = (android.widget.TableLayout) activity.findViewById(R.id.history);
                android.widget.TableRow row = new android.widget.TableRow(activity);
                row.setGravity(android.view.Gravity.CENTER);
                android.widget.TextView log = new android.widget.TextView(activity);
                log.setGravity(android.view.Gravity.CENTER);
                android.view.WindowManager wm = (android.view.WindowManager) private_room.activity.getSystemService(android.content.Context.WINDOW_SERVICE);

                log.setBackgroundColor(set_event_background_color());
                //log.setWidth(wm.getDefaultDisplay().getWidth() * 99 / 100);
                log.setWidth(history.getWidth() - 2);
                log.setTextSize(22);
                log.setTextColor(Color.BLACK);
                log.setText(history_log);
                row.addView(log);
                history.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.TableLayout.LayoutParams.MATCH_PARENT,android.widget.TableLayout.LayoutParams.WRAP_CONTENT));
                telecom.tsleep(100);
            }
        });
        //视图拉到底，显示最新信息

        android.widget.ScrollView history_scroll = (android.widget.ScrollView) activity.findViewById(R.id.scrollViewlog);
        history_scroll.post(new Runnable() {
            public void run() {
                android.widget.ScrollView history_scroll = (android.widget.ScrollView) activity.findViewById(R.id.scrollViewlog);
                history_scroll.scrollBy(0, 10000);
            }
        });

    }
    private static String process_message(String message)//处理日志，将其中客户的名字改为“你”
    {
        int count;
        count=message.indexOf(my_name);
        while(count!=-1) {
            message = message.substring(0, count) + "你" + message.substring(count + my_name.length(),message.length());
            count=message.indexOf(my_name);
        }

        return message;
    }



    public static void hatred_what()//对什么使用憎恨
    {
        telecom.tsleep(50);
        telecom.reply_i_get();
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);

                builder.setCancelable(false);//在做决定前此消息框不会消失
                builder.setTitle("请选择一个目标发动憎恨效果");//选中

                int rival_weapon_number=0;int count,count2;
                for(count=0;count<server_protocol.weaponlimit;count++)//统计对手有效的武器数量
                {
                    if(rival.w[count]&&rival.w_valid[count])rival_weapon_number++;
                }
                String itemlist[] = new String[3+rival_weapon_number];
                itemlist[0] = "对手人物效果";
                itemlist[1] = "对手加附效果";
                itemlist[2] = "自己的加附效果";
                //itemlist[3] = 对手第一个武器;
                //itemlist[4] = 对手第二个武器;
                count2=1;
                for(count=0;count<(2+rival_weapon_number)&&count<server_protocol.weaponlimit;count++)//统计对手武器数量
                {
                    if(rival.w[count]&&rival.w_valid[count]) {
                        itemlist[2 + count2] = game_card.search_card(game_card.p_name, rival.weapon[count]);
                        count2++;
                    }
                }
                final String items[] = itemlist;
//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来
                        if(which<=2)
                            monitor_all.send(String.valueOf(which+1));//发送自己想选第几个东西
                        else
                        {
                            monitor_all.send(String.valueOf(4));//摧毁对面武器
                            int count2=-1;
                            for(int count=0;count<server_protocol.weaponlimit;count++)//统计对手武器数量
                            {
                                if(rival.w[count]&&rival.w_valid[count]) {
                                    count2++;
                                    if(which==3+count2) {
                                        fight_room.whichweapon = count + 1;
                                        break;
                                    }
                                }
                            }

                        }
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }
    private static String enemy_message;
    public static void show_enemy_aim_list()//显示所有敌人的选项栏
    {
        telecom.tsleep(50);
        telecom.reply_i_get();
        telecom.listen();
        enemy_message=cc.message;
        telecom.tsleep(50);
        telecom.reply_i_get();
        handler.post(new Runnable() {
            @Override
            public void run() {
                String qustion=enemy_message;
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                    }
                });
                builder.setCancelable(false);//在做决定前此消息框不会消失
                builder.setTitle(qustion);//选中敌人做什么

                String memberlist[] = new String[1];
                memberlist[0] = game_card.search_character(rival.character_number);
                final String items[] = memberlist;

//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来
                        monitor_all.send(String.valueOf(rival.player_No));//发送自己想选第几个敌人
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }
    private static String alive_message;
    public static void show_alive_aim_list()//显示所有活人的选项栏
    {
        telecom.tsleep(50);
        telecom.reply_i_get();
        telecom.listen();
        alive_message=cc.message;
        telecom.tsleep(50);
        telecom.reply_i_get();
        handler.post(new Runnable() {
            @Override
            public void run() {
                String qustion=alive_message;
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                    }
                });
                builder.setCancelable(false);//在做决定前此消息框不会消失
                builder.setTitle(qustion);//选中敌人做什么

                String memberlist[] = new String[2];
                memberlist[0] = game_card.search_character(rival.character_number);
                memberlist[1] = game_card.search_character(character.character_number)+"(你)";
                final String items[] = memberlist;

//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来
                        if (which == 0)
                            monitor_all.send(String.valueOf(rival.player_No));//发送自己想选第几个角色
                        else
                            monitor_all.send(String.valueOf(character.player_No));//发送自己想选第几个角色
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_allmember_info()//显示角色信息(所有人状态)
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);

                builder.setTitle("角色信息");
                String info;int count,count2;
                info="【对手信息】\n【姓名】";
                info=info + game_card.search_character(rival.character_number);
                if(rival.overdeath>1)info=info+ "(持续 "+ rival.overdeath +"轮回不死之身)";
                else if(rival.overdeath==1)info=info+ "(仅本轮回不死之身!)";
                if(rival.un_move_able>0)info=info + "(" + rival.un_move_able + "回合内无法行动）";
                else if(rival.unmoveable_time)info=info +"(暂时无法行动！)";
                info=info +"\n";
                info=info + "【个人技能】\n";
                info=info + game_card.search_skill(rival.ability[0]);info=info +"\n";
                for(count=0;count<server_protocol.weaponlimit;count++)
                    if(rival.w[count]==true)
                    {
                        info=info + "【武器" + (count+1) +"】"+game_card.search_card(game_card.p_name, rival.weapon[count]);
                        if(rival.weapon[count]==6)
                            info=info+"(充能+"+rival.weaponeffect06[count]+")"+"  ";
                        else
                            info=info+"(攻+"+(rival.exattack_weapon[count]+rival.exattack_weapon_turn[count])+")"+"  ";
                        info=info+"\n";
                    }
                for(count=0;count<server_protocol.weaponlimit;count++)
                {
                    if(rival.w[count])
                    {
                        info=info+"【武器"+(count+1) ;
                        if(rival.weapon_effect[0][0]==-1){info=info+"】\t无特殊效果\n";continue;}//幻影剑无特殊效果
                        else{info=info+"的效果】\n";}
                        for(count2=0;count2<server_protocol.weaponeffectlimit;count2++)
                        {
                            if(rival.weapon_effect[count][count2]!=-1)//进行角色装备武器的效果显示，如果有效果则显示
                            {info=info+game_card.search_skill(rival.weapon_effect[count][count2]);}
                            else
                                break;
                        }
                    }
                }

                info=info +"\n";

                info=info +"【你的信息】 \n【姓名】";
                info=info + game_card.search_character(character.character_number);
                if(character.overdeath>1)info=info+ "(持续 "+ character.overdeath +"轮回不死之身)";
                else if(character.overdeath==1)info=info+ "(仅本轮回不死之身!)";
                if(character.un_move_able>0)info=info + "(" + character.un_move_able + "回合内无法行动）";
                else if(character.unmoveable_time)info=info +"(暂时无法行动！)";
                info=info +"\n";
                info=info + "【个人技能】\n";
                info=info + game_card.search_skill(character.ability[0]);info=info +"\n";
                for(count=0;count<server_protocol.weaponlimit;count++)
                    if(character.w[count]==true)
                    {
                        info=info + "【武器" + (count+1) +"】"+game_card.search_card(game_card.p_name, character.weapon[count]);
                        if(character.weapon[count]==6)
                            info=info+"(充能+"+character.weaponeffect06[count]+")"+"  ";
                        else
                            info=info+"(攻+"+(character.exattack_weapon[count]+character.exattack_weapon_turn[count])+")"+"  ";
                        info=info+"\n";
                    }
                for(count=0;count<server_protocol.weaponlimit;count++)
                {
                    if(character.w[count])
                    {
                        info=info+"【武器"+(count+1) ;
                        if(character.weapon_effect[0][0]==-1){info=info+"】\t无特殊效果\n";continue;}//幻影剑无特殊效果
                        else{info=info+"的效果】\n";}
                        for(count2=0;count2<server_protocol.weaponeffectlimit;count2++)
                        {
                            if(character.weapon_effect[count][count2]!=-1)//进行角色装备武器的效果显示，如果有效果则显示
                            {info=info+game_card.search_skill(character.weapon_effect[count][count2]);}
                            else
                                break;
                        }
                    }
                }

                builder.setMessage(info);


                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_help()//显示部分说明教程
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);

                builder.setTitle("新手指导");
                String info;int count,count2;
                info="红色卡：助攻卡\n绿色卡：武器卡\n白色卡：效果卡\n\n";
                info=info+"红色数字：血量\n黄色数字：攻击力\n\n";
                info=info+"●【发动普攻】按钮上方，若显示“你的回合！”则表示当前是你的回合\n\n";
                info=info+"●每当到了你的回合时，你会获得一次普攻机会\n\n";
                info=info+"●发动普攻后，若击中对手，则会根据双方攻击力差额给予攻击力较低者伤害\n\n";
                info=info+"●你发动普攻后，系统会询问你是否发动手卡，你可以使用【助攻卡】强化自己的攻击\n\n";
                info=info+"●武器卡只能在【你的回合】且【你没参与搏斗】的情况下进行装备\n\n";
                info=info+"●效果卡满足发动条件即可使用，若无发动条件，即表明可瞬发\n\n";
                info=info+"●血量先归为0且没处于无敌状态的一方被判定为失败\n";
                builder.setMessage(info);

                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_weapon_effect_list()//显示“发动武器效果”的选项栏
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                        //Toast.makeText(AlertDialogActivity.this, "这是取消按钮", Toast.LENGTH_SHORT).show();
                    }
                });
                builder.setCancelable(false);//无法通过点击无关东西关闭

                builder.setTitle("请选择效果并发动！");
                int count, count2, weaponeffectnumber, recentnumber;
                weaponeffectnumber = 0;
                for (count = 0; count < character.weaponlimit; count++)//统计自己武器效果总数
                {
                    if (character.w[count])
                        for (count2 = 0; count2 < character.weaponeffectlimit; count2++) {
                            if (character.w_e[count][count2])
                                weaponeffectnumber++;//总数增加
                            else
                                break;
                        }
                    else
                        break;
                }
                String effectcollect[] = new String[weaponeffectnumber];//武器效果总览
                String neweffect = "";//取当前武器的某个效果，加入武器效果总览
                recentnumber = 0;//当前统计到第几个效果，上限为weaponeffectnumber
                for (count = 0; count < character.weaponlimit; count++)//将一条条武器效果存入武器效果总览
                {
                    if (character.w[count])
                        for (count2 = 0; count2 < character.weaponeffectlimit; count2++) {
                            if (character.w_e[count][count2]) {
                                neweffect = game_card.search_skill(character.weapon_effect[count][count2]);
                                effectcollect[recentnumber] = neweffect;
                                recentnumber++;
                            } else
                                break;
                        }
                    else
                        break;
                }
                final String items[] = effectcollect;

//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来

                        int count, count2, recentnumber;
                        count2 = 0;
                        recentnumber = -1;
                        for (count = 0; count < character.weaponlimit; count++) {
                            if (character.w[count])
                                for (count2 = 0; count2 < character.weaponeffectlimit; count2++) {
                                    if (character.w_e[count][count2])
                                        recentnumber++;
                                    else
                                        break;
                                    if (recentnumber == which) break;
                                }
                            else
                                break;
                            if (recentnumber == which) break;
                        }
                        monitor_all.send(String.valueOf(count + 1));//发送自己想发动第几个武器的效果
                        fight_room.whicheffect = count2 + 1;//将发动该武器的第几个效果的效果号进行存储，等到服务器询问get_we_number提供给他该效果号
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_buff_effect_list()//显示“发动加附效果”的选项栏
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                    }
                });
                builder.setCancelable(false);//在做决定前此消息框不会消失

                builder.setTitle("请选择效果并发动！");
                int buffnumber = 0;//buff总数
                if (character.hideshield > 0) buffnumber++;
                String bufflist[] = new String[buffnumber];
                if (character.hideshield > 0)
                    bufflist[0] = "隐天盾";

                final String items[] = bufflist;

//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来
                        monitor_all.send(String.valueOf(which + 1));//发送自己想发动第几个buff的效果
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_YNchoose()//显示“做是否选择”
    {
        telecom.tsleep(50);
        telecom.reply_i_get();//回复自己受到chooseYN信号
        telecom.listen();
        fight_room.choosesituation = cc.message;
        telecom.tsleep(50);
        telecom.reply_i_get();//回复自己收到了选择场景信息

        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                    }
                });
                builder.setPositiveButton("确定", new android.content.DialogInterface.OnClickListener() {//添加确定
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("1");
                    }
                });
                builder.setCancelable(false);//在做决定前此消息框不会消失

                builder.setTitle(fight_room.choosesituation);
                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_rival_card_plunder()//显示对手手卡并选择一张进行抢夺
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                    }
                });
                builder.setCancelable(false);//在做决定前此消息框不会消失

                builder.setTitle("请选择想要抢夺的手卡！");
                int cardnumber = 0;//确认对手手卡总数
                int count;//计数器
                for (count = 0; count < rival.cardlimit; count++) {
                    if (rival.c[count])
                        cardnumber++;
                    else
                        break;
                }
                String card_all[] = new String[cardnumber];
                for (count = 0; count < cardnumber; count++)
                    card_all[count] = game_card.search_card(game_card.p_name, rival.card[count]);
                final String items[] = card_all;
//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来
                        monitor_all.send(String.valueOf(which + 1));//发送自己想抢夺第几个手卡
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }
    public static void show_weapon_list()//显示武器列表，选择一个武器进行丢弃
    {
        telecom.tsleep(50);
        telecom.reply_i_get();
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(activity);
                builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() {//添加取消
                    @Override
                    public void onClick(android.content.DialogInterface dialogInterface, int i) {
                        monitor_all.send("0");
                    }
                });
                builder.setCancelable(false);//在做决定前此消息框不会消失

                builder.setTitle("请选择需要丢弃的武器：");
                int weaponnumber = 0;//buff总数
                int count = 0;
                for (count = 0; count < server_protocol.weaponlimit; count++) {
                    if (character.w[count])
                        weaponnumber++;
                    else
                        break;
                }
                String weaponlist[] = new String[weaponnumber];
                for (count = 0; count < weaponnumber; count++)
                    weaponlist[count] = game_card.search_card(game_card.p_name, character.weapon[count]);

                final String items[] = weaponlist;

//-1代表没有条目被选中
                builder.setSingleChoiceItems(items, -1, new android.content.DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(android.content.DialogInterface dialog, int which) {
                        String item = items[which];//1.把选中的条目取出来
                        monitor_all.send(String.valueOf(which + 1));//发送自己想丢第几个武器
                        dialog.dismiss();//把对话框关闭
                    }
                });
                builder.show();//展示武器所有效果
            }
        });
    }

    public static int cnumber;
    public static void refresh_info_card(boolean myself)//更新手卡
    {
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();//回复自己收到接收手卡变更信号
        telecom.listen();//获取牌总数
        int count;
        cnumber = Integer.parseInt(cc.message);//获取牌总数
        while(cnumber==server_protocol.refresh_state_CH||cnumber==server_protocol.refresh_state_CM)
        {
            telecom.tsleep(fastsleep);
            telecom.reply_i_get();//回复自己收到接收手卡变更信号
        }
        if (!myself) {//如果是敌人的手卡更新了，只在UI上更新对手手卡数量
            handler.post(new Runnable() {
                @Override
                public void run() {
                    android.widget.TextView cardnumber = (android.widget.TextView) activity.findViewById(R.id.rival_card_number);
                    cardnumber.setText("手牌数：" + game_function.cnumber);
                }
            });
            telecom.tsleep(fastsleep);
            telecom.reply_i_get();
            return;
        }
        telecom.tsleep(fastsleep);
        character.ini_card();//清空自己原有手卡
        telecom.reply_i_get();
        for (count = 0; count < cnumber; count++) {
            telecom.listen();
            character.c[count] = true;
            character.card[count] = Integer.parseInt(cc.message);
            telecom.tsleep(fastsleep);
            telecom.reply_i_get();
        }
        change_card();//UI更新自己手卡
    }
    public static void refresh_info_F()//首次更新人物信息，首先是自己人物序号、个人效果、初始血量、初始攻击信息，再是对手信息
    {
        int count;
        Boolean myself;
        myself = true;
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();//回复自己收到refresh_first_state
        while (true) {
            if (myself) {
                character.character_number = set_property_Int();//角色序号\
                while(character.character_number==server_protocol.refresh_first_state)
                {
                    character.character_number = set_property_Int();//角色序号\
                }
                character.player_No = set_property_Int();//第几个角色\
                character.blood = set_property_Float();//血量
                character.attack = set_property_Float();//攻击力
                for (count = 0; count < character.abilitylimit; count++) {
                    character.a[count] = set_property_Boolean();
                    character.ability[count] = set_property_Int();
                }
                my_name=String.valueOf(character.player_No)+"号玩家:"+game_card.search_character(character.character_number)+" ";
                myself = false;//先传输自己信息，再传输对手信息
            } else {//传输对手信息
                rival.character_number = set_property_Int();
                rival.player_No = set_property_Int();//第几个角色\
                rival.blood = set_property_Float();
                rival.attack = set_property_Float();
                for (count = 0; count < character.abilitylimit; count++) {
                    rival.a[count] = set_property_Boolean();
                    rival.ability[count] = set_property_Int();
                }
                //到此步，对手信息传输完了，结束
                change_blood();//对页面上的数据进行更新
                change_attack();
                change_rival_name();//对页面上对手姓名更新
                break;
            }

        }


    }
    public static void refresh_info_Weapon(boolean myself)//武器信息更新
    {
        if (myself)
            character.ini_weapon();//清空原有武器信息
        else
            rival.ini_weapon();
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();//回复自己接收到更新武器信息信号
        int count, count2;
        if (myself) {
            for (count = 0; count < character.weaponlimit; count++) {
                character.w_valid[count] = set_property_Boolean();
                character.w[count] = set_property_Boolean();
                character.weapon[count] = set_property_Int();
                character.exattack_weapon[count] = set_property_Float();
                for (count2 = 0; count2 < character.weaponeffectlimit; count2++) {
                    character.weapon_effect[count][count2] = set_property_Int();
                    if (character.weapon_effect[count][count2] != -1)
                        character.w_e[count][count2] = true;
                }
                character.weaponeffect06[count] = set_property_Int();
            }
        } else {
            for (count = 0; count < character.weaponlimit; count++) {
                rival.w_valid[count] = set_property_Boolean();
                rival.w[count] = set_property_Boolean();
                rival.weapon[count] = set_property_Int();
                rival.exattack_weapon[count] = set_property_Float();
                for (count2 = 0; count2 < rival.weaponeffectlimit; count2++) {
                    rival.weapon_effect[count][count2] = set_property_Int();
                    if (rival.weapon_effect[count][count2] != -1)
                        rival.w_e[count][count2] = true;
                }
                rival.weaponeffect06[count] = set_property_Int();
            }
        }
        //到此步，信息传输完了，结束
        change_blood();//对页面上的数据进行更新
        change_attack();

    }
    public static void refresh_info_Blood(boolean myself)//血量信息更新
    {
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        if (myself)
            character.blood = set_property_Float();
        else
            rival.blood = set_property_Float();
        change_blood();
    }
    public static void refresh_info_Attack(boolean myself)//更新双方攻击力
    {
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        if (myself)
            character.attack = set_property_Float();
        else
            rival.attack = set_property_Float();
        change_attack();
    }
    public static void refresh_info_Buff(boolean myself)//更新双方加附效果
    {
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        if (myself) {
            character.hideshield = set_property_Int();
            character.overdeath = set_property_Int();
        } else {
            rival.hideshield = set_property_Int();
            rival.overdeath = set_property_Int();
        }

    }
    public static void refresh_rival_card_Info()//获取对手手卡详细信息
    {
        int count;
        int card_all_number = set_property_Int();//接收对手手卡总数
        for (count = 0; count < card_all_number; count++) {
            rival.c[count] = true;
            rival.card[count] = set_property_Int();
        }
    }


    public static void change_button_UI(int choice)//更改UI按钮
    {
        switch (choice) {
            case server_protocol.state_discard_free_card://因为没规定数量丢卡，所以不知道什么时候停止丢，需要用户手动确认选择完成
                fight_room.mystate = fight_room.state_choose_free;
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        android.widget.Button end_turn = (android.widget.Button) activity.findViewById(R.id.end_operation);
                        end_turn.setText("确认选择");
                        android.widget.Button attack = (android.widget.Button) activity.findViewById(R.id.attack_choice);
                        attack.setEnabled(false);
                        android.widget.Button launch_effect = (android.widget.Button) activity.findViewById(R.id.launcheffect);
                        launch_effect.setText("清空选择");
                    }
                });
                break;

            case server_protocol.state_discard_muti_card://达到数量直接提交选择。只有清空和选择操作，没有确认所有操作
                fight_room.mystate = fight_room.state_choose_muti;//选择丢弃多张手卡
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        android.widget.Button end_turn = (android.widget.Button) activity.findViewById(R.id.end_operation);
                        end_turn.setText("清除原有选择//取消丢卡");
                        android.widget.Button attack = (android.widget.Button) activity.findViewById(R.id.attack_choice);
                        attack.setEnabled(false);
                        android.widget.Button launch_effect = (android.widget.Button) activity.findViewById(R.id.launcheffect);
                        launch_effect.setEnabled(false);
                    }
                });
                break;
            case server_protocol.state_discard_one_card://选择1张手卡进行丢弃
                fight_room.mystate = fight_room.state_choose_one;
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        android.widget.Button end_turn = (android.widget.Button) activity.findViewById(R.id.end_operation);
                        android.widget.Button attack = (android.widget.Button) activity.findViewById(R.id.attack_choice);
                        attack.setEnabled(false);
                        android.widget.Button launch_effect = (android.widget.Button) activity.findViewById(R.id.launcheffect);
                        launch_effect.setEnabled(false);
                        end_turn.setText("取消丢卡");
                    }
                });
                break;
            case server_protocol.state_return_normal://返回最初样子
                fight_room.mystate = fight_room.state_normal;
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        android.widget.Button end_turn = (android.widget.Button) activity.findViewById(R.id.end_operation);
                        end_turn.setText("结束回合");
                        android.widget.Button attack = (android.widget.Button) activity.findViewById(R.id.attack_choice);
                        attack.setEnabled(true);
                        android.widget.Button launch_effect = (android.widget.Button) activity.findViewById(R.id.launcheffect);
                        launch_effect.setEnabled(true);
                        launch_effect.setText("发动效果");
                    }
                });
                break;
            case server_protocol.state_choose_one_card://选择单张手卡发动
                fight_room.mystate = fight_room.state_choose_one;
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        android.widget.Button end_turn = (android.widget.Button) activity.findViewById(R.id.end_operation);
                        android.widget.Button attack = (android.widget.Button) activity.findViewById(R.id.attack_choice);
                        attack.setEnabled(false);
                        android.widget.Button launch_effect = (android.widget.Button) activity.findViewById(R.id.launcheffect);
                        launch_effect.setEnabled(false);
                        end_turn.setText("不行动");
                    }
                });
                break;
            case server_protocol.state_fight_back:
                fight_room.mystate = fight_room.state_fight_back;
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        android.widget.Button end_turn = (android.widget.Button) activity.findViewById(R.id.end_operation);
                        android.widget.Button attack = (android.widget.Button) activity.findViewById(R.id.attack_choice);
                        attack.setEnabled(false);
                        android.widget.Button launch_effect = (android.widget.Button) activity.findViewById(R.id.launcheffect);
                        launch_effect.setEnabled(true);
                        launch_effect.setText("发动效果");
                        end_turn.setText("不行动");
                    }
                });
                break;

        }
    }
    public static void change_rival_name() //对手姓名更新
    {
        handler.post(new Runnable() {
                         @Override
                         public void run() {
                             android.widget.TextView rival_name = (android.widget.TextView) activity.findViewById(R.id.rival_name);
                             rival_name.setText(game_card.search_character(rival.character_number));
                         }
                     }
        );
    }
    public static void change_blood() //UI血量更新
    {

        handler.post(new Runnable() {
                         @Override
                         public void run() {
                             android.widget.TextView blood = (android.widget.TextView) activity.findViewById(R.id.rival_blood);
                             blood.setText(String.valueOf(rival.blood));
                             blood = (android.widget.TextView) activity.findViewById(R.id.my_blood);
                             blood.setText(String.valueOf(character.blood));
                         }
                     }

        );
    }
    public static void change_attack() //UI攻击力更新
    {

        handler.post(new Runnable() {
                         @Override
                         public void run() {
                             android.widget.TextView attack;
                             float attack_all;
                             int count;
                             attack = (android.widget.TextView) activity.findViewById(R.id.rival_attack);
                             attack_all = rival.attack;
                             for (count = 0; count < server_protocol.weaponlimit; count++) {
                                 if (rival.w[count]&&rival.w_valid[count])
                                     attack_all = attack_all + rival.exattack_weapon[count];
                             }
                             attack.setText(String.valueOf(attack_all));
                             attack = (android.widget.TextView) activity.findViewById(R.id.my_attack);
                             attack_all = character.attack;
                             for (count = 0; count < server_protocol.weaponlimit; count++) {
                                 if (character.w[count]&&character.w_valid[count])
                                     attack_all = attack_all + character.exattack_weapon[count];
                             }
                             attack.setText(String.valueOf(attack_all));
                         }
                     }

        );
    }
    public static void change_card() //UI更新手卡
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                int count;
                android.widget.TableLayout card_slot = (android.widget.TableLayout) game_function.activity.findViewById(R.id.linearLayout);
                card_slot.removeAllViews();//清空原有布局
                android.widget.TableRow row;
                android.widget.Button card;
                row = new android.widget.TableRow(game_function.activity);
                for (count = 0; count < character.cardlimit && character.c[count]; count++) {
                    card = new android.widget.Button(game_function.activity);
                    card.setId(count + 1);//设置按钮id号
                    android.graphics.drawable.GradientDrawable drawable=new  android.graphics.drawable.GradientDrawable(); //(android.graphics.drawable.GradientDrawable) card.getBackground();
                    drawable.setColor(game_card.set_card_background(character.card[count]));//设置按钮背景颜色
                    drawable.setStroke(1, game_card.set_card_broad(character.card[count]));//设置按钮边框的宽度和颜色
                    card.setBackground(drawable);//将该背景赋予按钮
                    card.setTextColor(game_card.set_card_text(character.card[count]));//设置按钮字体颜色


                    card.setText(game_card.search_card(game_card.p_name, character.card[count]));//显示卡片名称
                    card.setText(card.getText() + "\n" + game_card.search_card(game_card.p_details, character.card[count]));//显示卡片效果
                    card.setText(card.getText() + "\n\n" + game_card.search_card(game_card.p_name, game_card.get_hideeffect(character.card[count])));

                    card.setOnClickListener(new android.widget.Button.OnClickListener() {
                        @Override
                        public void onClick(android.view.View v) {
                            fight_room.whichcard = Integer.valueOf(v.getId());//由于无法传递自己想用哪个卡，就用全局变量把卡号传递
                            fight_room.show_card_choice(v);
                        }
                    });

                    android.view.WindowManager wm = (android.view.WindowManager) activity.getSystemService(android.content.Context.WINDOW_SERVICE);//获取屏幕大小，做兼容性调整
                    card.setWidth(wm.getDefaultDisplay().getWidth()*45/100);//设置宽度
                    card.setGravity(Gravity.LEFT);
                    card.setHeight(card_slot.getHeight()-1);

                    row.addView(card);

                    //card_slot.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.WRAP_CONTENT));
                }
                card_slot.addView(row);

            }
        });
    }
    public static void whether_my_turn(boolean whether_my_turn)//UI更新是否是你的回合
    {

        if(whether_my_turn)
        {
            my_turn=true;
            handler.post(new Runnable() {
                @Override
                public void run() {
                    android.widget.TextView log=(android.widget.TextView) activity.findViewById(R.id.whether_your_turn);
                    log.setText("你的回合!");
                }});
        }
        else
        {
            my_turn=false;
            handler.post(new Runnable() {
                @Override
                public void run() {
                    android.widget.TextView log=(android.widget.TextView) activity.findViewById(R.id.whether_your_turn);
                    log.setText("");
                }});

        }
    }


    public static float set_property_Float()//接收浮点数并处理
    {
        telecom.listen();
        int d = Integer.parseInt(cc.message);//整数部分
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        telecom.listen();
        float f = Integer.parseInt(cc.message);//小数部分
        float whole = d + f / 100;
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        return whole;
    }
    public static int set_property_Int()//接收int并处理
    {
        telecom.listen();
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        return Integer.parseInt(cc.message);
    }
    public static Boolean set_property_Boolean()//接收bool并处理,但其实服务器发来的是整型数据，根据0或1判断为false还是true
    {
        telecom.listen();
        telecom.tsleep(fastsleep);
        telecom.reply_i_get();
        if (Integer.parseInt(cc.message) == 1)
            return true;
        else
            return false;
    }


    public static int set_event_background_color()//设置背景颜色
    {
        switch(colorkind)
        {
            case blue_background:return android.graphics.Color.rgb(0, 200, 255);//蓝色
            case green_background:return android.graphics.Color.rgb(0, 255, 0);
            case yellow_background:return android.graphics.Color.rgb(255, 255, 0);
            case cyan_background:return android.graphics.Color.rgb(0, 255, 255);
        }
        return android.graphics.Color.rgb(255, 255, 255);
    }
    public static void change_event_background_color()//变换事件背景颜色
    {
        colorkind++;
        if(colorkind>colorkindlimit)colorkind=green_background;
    }
}
