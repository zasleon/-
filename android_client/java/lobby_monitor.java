package com.example.xielm.myapplication;
/**
 * Created by Xielm on 2021/4/1.
 */
public class lobby_monitor extends Thread implements Runnable{
    public static android.app.Activity activity;
    //public static android.app.Activity activity_R;//roomlist
    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    final static client_socket cc=client_socket.getSingleIntance();
    //public static Boolean whether_stop;//是否要停止继续监听
    @Override
    public void run() {
        android.os.Looper.prepare();//若不加这句话，无法正常调用其他类里的静态变量以及函数
        while (true) {
            if(lobby.state==lobby.other){telecom.tsleep(1000);continue;}
            if(lobby.state==lobby.in_UNO_room||lobby.state==lobby.in_UNO_roomlist||lobby.state==lobby.in_UNO_game){telecom.tsleep(1000);continue;}
            monitor_all.just_release();
            telecom.listen();
            monitor_all.just_lock();

            if(cc.message.length()==4) {//一般发来都是特殊信号
                try {
                    if(!UNO_room_service.start_UNO_room_service(Integer.parseInt(cc.message)))//若是UNO游戏特殊信号，进行处理
                    process_signal(Integer.parseInt(cc.message));//将特殊信号从字符串转换为int类型
                } catch (NumberFormatException e) {lobby.add_log(cc.message); }
            }else {
                switch (lobby.state)
                {
                    case lobby.online:
                        lobby.add_log(cc.message);
                        break;
                    case lobby.roomlist:
                        roomlist.add_log(cc.message);
                        break;
                    case lobby.choose_character:
                    case lobby.in_room:
                        add_log(cc.message);
                        break;
                    case lobby.video:
                        video_player.add_log(cc.message);
                        break;
                    default:
                        break;

                }
                telecom.tsleep(100);
                telecom.reply_i_get();
            }
            telecom.tsleep(100);


        }

    }


    public static boolean whether_host;
    public static void process_signal(int number)//处理特殊信号
    {

        android.content.Intent i2 ;
        switch (number) {
            case server_protocol.get_in_room:
                telecom.tsleep(30);
                telecom.reply_i_get();
                if (lobby.state == lobby.in_room) break;
                whether_host = true;
                lobby.state = lobby.in_room;
                //Intent是一种运行时绑定（run-time binding）机制，它能在程序运行过程中连接源代码文件夹下两个不同的组件。
                i2 = new android.content.Intent(lobby.activity, private_room.class);
                lobby.activity.startActivity(i2);//启动该页面服务
                telecom.tsleep(30);
                break;
            case server_protocol.no_room_in_use:
                lobby.add_log("当前没有使用正在使用的房间!");
                telecom.tsleep(30);
                telecom.reply_i_get();
                break;
            //----------------------------------------------roomlist-------------------------------------------
            case server_protocol.please_choose_room://lobby.add_log("成功显示房间列表！");
                telecom.tsleep(30);
                telecom.reply_i_get();//回复自己受到数据
                //Intent是一种运行时绑定（run-time binding）机制，它能在程序运行过程中连接源代码文件夹下两个不同的组件。
                if (lobby.state == lobby.roomlist) break;
                lobby.state = lobby.roomlist;
                i2 = new android.content.Intent(lobby.activity, roomlist.class);
                lobby.activity.startActivity(i2);//启动该页面服务
                while (true) {
                    telecom.listen();

                    if (cc.message.equals(Integer.toString(server_protocol.stop_show_room)))//结束接收服务器发来的房间数据，开始监听玩家请求
                    {
                        telecom.tsleep(30);
                        telecom.reply_i_get();
                        telecom.tsleep(100);
                        roomlist.open_roomlist_leave();//开启回退选项
                        break;
                    } else {
                        set_button(cc.message);//新建进入房间的按钮
                    }
                    telecom.tsleep(30);
                    telecom.reply_i_get();
                }
                //button3服务结束
                break;

            case server_protocol.cancel_enter_room://取消加入房间
                telecom.tsleep(30);
                telecom.reply_i_get();
                roomlist.activity.finish();
                lobby.state = lobby.online;
                break;
            case server_protocol.enter_room_success:
                telecom.tsleep(30);
                telecom.reply_i_get();//回复自己受到数据
                if (lobby.state == lobby.in_room) break;
                whether_host = false;
                lobby.state = lobby.in_room;
                i2 = new android.content.Intent(roomlist.activity, private_room.class);
                roomlist.activity.startActivity(i2);//启动该页面服务
                roomlist.activity.finish();

                break;
            case server_protocol.show_user:
                telecom.tsleep(50);
                telecom.reply_i_get();
                fresh_memberlist();
                break;

            //----------------------------------------------choose_character-------------------------------------------
            case server_protocol.choice_complete:
                i2 = new android.content.Intent(choose_characters.activity, fight_room.class);
                choose_characters.activity.startActivity(i2);//进入战斗房间
                telecom.tsleep(50);
                telecom.reply_i_get();
                choose_characters.activity.finish();//角色选择界面关闭
                lobby.state = lobby.other;
                monitor_all.stop_send(7000);
                break;
            case server_protocol.someone_offline://选角色时有人掉线
                monitor_all.stop_send(1000);
                choose_characters.activity.finish();
                telecom.reply_i_get();
                lobby.state = lobby.in_room;
                break;
            case server_protocol.reopen_choice:
                if (lobby.state != lobby.choose_character) {
                    if (lobby.state == lobby.online) {
                        i2 = new android.content.Intent(lobby.activity, choose_characters.class);
                        lobby.activity.startActivity(i2);
                    }
                    if (lobby.state == lobby.in_room) {
                        i2 = new android.content.Intent(private_room.activity, choose_characters.class);
                        private_room.activity.startActivity(i2);
                    }
                    telecom.tsleep(200);
                }
                choose_characters.reopen_choice();
                telecom.tsleep(100);
                lobby.state = lobby.choose_character;
                telecom.reply_i_get();
                break;
            //----------------------------------------------private_room-------------------------------------------
            case server_protocol.character_choice://进入选择角色界面
                if (lobby.state == lobby.choose_character){telecom.tsleep(50);break;}
                lobby.state=lobby.choose_character;
                i2 = new android.content.Intent(private_room.activity, choose_characters.class);
                private_room.activity.startActivity(i2);
                telecom.tsleep(50);
                break;
            case server_protocol.someone_get_in://有人进入房间了
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.listen();//发来进来者名字
                add_log(" 玩家:  " + cc.message + "   进入了房间!");
                telecom.tsleep(50);
                telecom.reply_i_get();
                break;
            case server_protocol.you_not_in_room:
                private_room.activity.finish();
                telecom.tsleep(50);
                lobby.state=lobby.online;
                telecom.reply_i_get();
                break;
            case server_protocol.leave_room:
                private_room.activity.finish();
                lobby.state=lobby.online;
                telecom.tsleep(50);
                telecom.reply_i_get();
                break;

            case server_protocol.someone_leave:
                telecom.tsleep(50);
                telecom.reply_i_get();
                telecom.listen();//发来离开者姓名或“你成为房间主人you_are_host”
                if (!cc.message.equals(Integer.toString(server_protocol.you_are_host))){
                    add_log(" 玩家:  " + cc.message + "  离开了房间!");
                    telecom.tsleep(50);
                    telecom.reply_i_get();
                    break;
                }
                else{
                    telecom.tsleep(50);
                    telecom.reply_i_get();//自己变成房间主人
                    telecom.listen();//发来离开者姓名
                    add_log("玩家:  " + cc.message + "   离开了房间!\n你变成房间主人!");
                    telecom.tsleep(50);
                    telecom.reply_i_get();
                    handler.post(new Runnable() {//“开始”按钮能按了
                        @Override
                        public void run() {
                            android.widget.Button start_fight = (android.widget.Button) private_room.activity.findViewById(R.id.button6);
                            whether_host = true;
                            start_fight.setEnabled(true);
                        }
                    });
                }
                break;
            case server_protocol.event_happen_N:
                telecom.reply_i_get();
                telecom.tsleep(100);
                telecom.reply_i_get();
                break;
            case server_protocol.event_end:
                telecom.tsleep(50);
                telecom.reply_i_get();
                break;
            //-------------------------------------------------sp-----------------------------------------
            case server_protocol.show_video:
                telecom.tsleep(30);
                telecom.reply_i_get();
                lobby.state=lobby.video;
                i2 = new android.content.Intent(lobby.activity, video_player.class);
                lobby.activity.startActivity(i2);

                //String address="https://vip.w.xk.miui.com/10d7ff4c9368d411b254bb5c62d6e849";//网络点播
                String address="http://10.125.39.201:10080/frecord/warriors(2WEI).mp4";
                //String address="http://10.125.39.201:10080/frecord/dondiablo.mp4";
                video_player.play_this_video(address);

                break;


                /*android.net.Uri uri = android.net.Uri.parse("http://10.125.39.201:10080/frecord/dondiablo.mp4");
                android.content.Intent intent = new android.content.Intent(android.content.Intent.ACTION_VIEW, uri);
                intent.addFlags(intent.FLAG_ACTIVITY_NEW_TASK);
                //intent.setData(uri);
                lobby.activity.startActivity(intent);
                */

            default:
                telecom.tsleep(30);
                telecom.reply_i_get();
                break;


        }
    }
    public static String UNO_room_record1;
    public static void UNO_add_log1(String log)//加日志
    {
        UNO_room_record1 = log;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String message1 = UNO_room_record1;

                //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
                android.widget.LinearLayout layout2 = (android.widget.LinearLayout) UNO_room.activity.findViewById(R.id.UNO_memberlog);

                android.widget.TableRow row;
                android.widget.TextView door_number1;

                row = new android.widget.TableRow(UNO_room.activity);
                row.setGravity(android.view.Gravity.CENTER);
                door_number1 = new android.widget.TextView(UNO_room.activity);//动态新建一个文字行
                door_number1.setText(message1);

                row.addView(door_number1);
                layout2.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.WRAP_CONTENT));
            }
        });
    }






    /*
                    telecom.listen();
                    video_player.add_log("文件大小：" + cc.message);
                    int big=Integer.parseInt(cc.message);
                    telecom.tsleep(34);
                    telecom.reply_i_get();
                    int recvnumber=big/1024;
                    while(recvnumber>0)
                    {
                        telecom.listen();
                        recvnumber--;
                        telecom.reply_i_get();
                    }*/
    public static String record;
    public static void add_log(String log)//加日志
    {
        record = log;
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.view.WindowManager wm = (android.view.WindowManager) private_room.activity.getSystemService(android.content.Context.WINDOW_SERVICE);

                android.widget.TableLayout table = (android.widget.TableLayout) private_room.activity.findViewById(R.id.memberlog);
                String history_log = record;
                android.widget.TableRow row = new android.widget.TableRow(private_room.activity);
                row.setGravity(android.view.Gravity.CENTER);
                android.widget.TextView log = new android.widget.TextView(private_room.activity);
                log.setGravity(android.view.Gravity.CENTER);
                log.setTextSize(22);
                log.setWidth(wm.getDefaultDisplay().getWidth() * 4 / 5);
                log.setText(history_log);
                log.setTextColor(android.graphics.Color.rgb(0, 0, 255));
                row.addView(log);
                table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.TableLayout.LayoutParams.MATCH_PARENT,android.widget.TableLayout.LayoutParams.WRAP_CONTENT));
            }
        });
        telecom.tsleep(50);
        android.widget.ScrollView history_scroll = (android.widget.ScrollView) private_room.activity.findViewById(R.id.scrollView2);
        history_scroll.post(new Runnable() {
            public void run() {
                android.widget.ScrollView history_scroll = (android.widget.ScrollView) private_room.activity.findViewById(R.id.scrollView2);
                history_scroll.scrollBy(0, 10000);
            }
        });
    }

    private static String message;
    public static void set_button(String log)//roomlist动态显示UI
    {
        message=log;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String message1=message;
                //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
                android.widget.LinearLayout layout2 = (android.widget.LinearLayout) roomlist.activity.findViewById(R.id.rooomlist);
                android.widget.TableRow row;
                android.widget.TextView door_number1;
                android.widget.Button door1;

                int count2, doornumber;

                row = new android.widget.TableRow(roomlist.activity);
                row.setGravity(android.view.Gravity.CENTER);
                door_number1 = new android.widget.TextView(roomlist.activity);//动态新建一个文字行
                door1 = new android.widget.Button(roomlist.activity);//动态新建一个按钮
                doornumber = 0;
                for (count2 = 0; message1.charAt(count2) != '号'; count2++)//几号房间
                {
                    door_number1.setText(door_number1.getText() + String.valueOf(message.charAt(count2)));
                    doornumber = Integer.parseInt(String.valueOf(door_number1.getText()));
                }
                door1.setId(roomlist.idconfig + doornumber);
                door_number1.setText(door_number1.getText() + "号房间  ");
                door1.setText("进入");
                door1.setOnClickListener(roomlist.listener);
                for (; message1.charAt(count2) != '\t'; count2++) {
                }//跳过中间的"号房间"
                for (; message1.charAt(count2) == '\t'; count2++) {
                }//跳过中间的\t
                door_number1.setText(door_number1.getText() + "【P1】");
                for (; message1.length() > count2; count2++)//人名
                {
                    door_number1.setText(door_number1.getText() + String.valueOf(message1.charAt(count2)));
                    while (message1.charAt(count2) == '\t') {
                        count2++;
                        if (message1.charAt(count2) != '\t') {
                            door_number1.setText(door_number1.getText() + "  【P2】");
                            count2--;
                            door1.setEnabled(false);
                            break;
                        }
                    }
                }
                row.addView(door_number1);
                row.addView(door1);
                layout2.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.WRAP_CONTENT));

            }
        });
    }

    public static void first_show_list()//lobby首次显示在线人员列表
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.widget.TableLayout table = (android.widget.TableLayout) lobby.activity.findViewById(R.id.memberlist);
                table.removeAllViews();//清空原有布局
                android.widget.TableRow row;
                android.widget.TextView c_number, c_name, c_state, topic;

                row = new android.widget.TableRow(lobby.activity);// create a new TableRow
                row.setGravity(android.view.Gravity.CENTER);
                /*c_number = new android.widget.TextView(activity);
                // c_number.setLayoutParams(new android.widget.Gallery.LayoutParams(
                //android.view.ViewGroup.LayoutParams.MATCH_PARENT, android.view.ViewGroup.LayoutParams.MATCH_PARENT));
                c_number.setText("编号");
                c_number.setWidth(30);
                c_number.setTextSize(20);
                c_state = new android.widget.TextView(activity);
                c_state.setText("状态");
                c_state.setWidth(150);
                c_state.setTextSize(20);
                c_name = new android.widget.TextView(activity);
                c_name.setText("用户名");
                c_name.setWidth(200);
                c_name.setTextSize(20);
                row.addView(c_number);
                row.addView(c_state);
                row.addView(c_name);*/
                topic = new android.widget.TextView(lobby.activity);
                topic.setText("编号\t\t状态\t\t用户名");
                topic.setTextSize(30);
                row.addView(topic);
                table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.MATCH_PARENT));

            }
        });
    }
    private static String listlog;
    public static void fresh_memberlist()//lobby刷新在线人员列表
    {
        first_show_list();
        while(true) {
            telecom.listen();
            if (cc.message.equals(Integer.toString(server_protocol.stop_show_user))) {
                telecom.tsleep(50);
                telecom.reply_i_get();
                return;
            }
            listlog = cc.message;
            handler.post(new Runnable() {
                @Override
                public void run() {
                    String llog = listlog;
                    android.widget.TableLayout table = (android.widget.TableLayout) lobby.activity.findViewById(R.id.memberlist);
                    android.widget.TableRow row;
                    row = new android.widget.TableRow(lobby.activity);// create a new TableRow
                    row.setGravity(android.view.Gravity.CENTER);
                    android.widget.TextView log = new android.widget.TextView(lobby.activity);
                    log.setText(llog);
                    log.setTextSize(20);
                    row.addView(log);
                    table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.MATCH_PARENT));
/*
                int count, count2;
                for (count = 0; lobby.data[count].length() != 0; count++) {//输出所有人信息开始
                    row = new android.widget.TableRow(activity);// create a new TableRow
                    row.setGravity(android.view.Gravity.CENTER);

                    for (count2 = 0; lobby.data[count].charAt(count2) != '\t'; count2++) {
                    }

                    c_number = new android.widget.TextView(activity);
                    c_number.setText((count + 1) + "号");
                    count2 = count2 + 2;
                    c_state = new android.widget.TextView(activity);
                    for (; lobby.data[count].charAt(count2) != '\t'; count2++) {
                        c_state.setText(c_state.getText() + String.valueOf(lobby.data[count].charAt(count2)));
                    }
                    count2 = count2 + 2;
                    c_name = new android.widget.TextView(activity);
                    for (; lobby.data[count].length() > count2; count2++) {
                        c_name.setText(c_name.getText() + String.valueOf(lobby.data[count].charAt(count2)));
                    }

                    // add the TextView  to the new TableRow
                    row.addView(c_number);
                    row.addView(c_name);
                    row.addView(c_state);
                    // add the TableRow to the TableLayout
                    table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.WRAP_CONTENT));

                }//输出所有人信息结束
                //button4刷新列表功能结束*/
                }

            });
            telecom.tsleep(100);
            telecom.reply_i_get();
        }
    }



}
