package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/12/17.
 */
public class UNO_room_service extends Thread implements Runnable{

    public static android.app.Activity activity;
    public final static int idconfig=1000;//设置动态按钮的序号
    final static client_socket cc=client_socket.getSingleIntance();
    //public static android.os.Handler handler1=new android.os.Handler();
    @Override
    public void run() {
        android.os.Looper.prepare();

            switch (lobby.state) {
                case lobby.in_UNO_roomlist:
                    UNO_roomlist_signal();
                    return;
                case lobby.in_UNO_room:
                    UNO_room_signal();
                    return;
            }



    }


    public static void UNO_roomlist_signal()
    {

        android.content.Intent i2 ;
        while(true) {

            telecom.listen();//当前必然是在房间列表选择界面
            if (cc.message.equals(Integer.toString(server_protocol.UNO_show_roomlist_end)))//结束接收服务器发来的房间数据，开始监听玩家请求
            {
                telecom.tsleep(30);
                telecom.reply_i_get();
                telecom.tsleep(100);
                break;
            } else {
                UNO_roomlist_set_button(cc.message);//新建进入房间的按钮

            }
            telecom.tsleep(30);
            telecom.reply_i_get();
        }

        //执行到这里必然服务器房间数据接收完毕


        while(true)//用户开始选择进入的房间，同时接收服务端发来的消息
        {
            monitor_all.just_release();
            telecom.listen();
            monitor_all.just_lock();
            try {
                switch (Integer.parseInt(cc.message))//将特殊信号从字符串转换为int类型
                {


                    case server_protocol.UNO_cancel_enter_room://取消进入房间，返回大厅
                        telecom.tsleep(30);
                        telecom.reply_i_get();
                        UNO_roomlist.activity.finish();
                        lobby.state = lobby.online;
                        return;
                    case server_protocol.UNO_enter_room_success://加入别人房间成功
                        telecom.tsleep(30);
                        telecom.reply_i_get();//回复自己受到数据
                        if (lobby.state == lobby.in_UNO_room) break;
                        UNO_room.whether_host = false;

                        i2 = new android.content.Intent(UNO_roomlist.activity, UNO_room.class);
                        UNO_roomlist.activity.startActivity(i2);//启动该页面服务
                        UNO_roomlist.activity.finish();
                        //UNO_room_signal();
                        return;
                    default:
                        telecom.tsleep(30);
                        telecom.reply_i_get();

                }
            } catch (NumberFormatException e) {lobby.add_log(cc.message); }

        }
    }

    private static void UNO_room_signal()//开始在房间内线程处理信号
    {

        while(true)
        {
            monitor_all.just_release();
            telecom.listen();
            monitor_all.just_lock();
            try {
                switch (Integer.parseInt(cc.message))//将特殊信号从字符串转换为int类型
                {
                    case server_protocol.you_are_in_the_lobby://退出房间成功
                        UNO_room.activity.finish();
                        lobby.state = lobby.online;//更改状态为：“在线在大厅”
                        telecom.tsleep(50);
                        telecom.reply_i_get();
                        return;//结束死循环
                    case server_protocol.UNO_leave_room_success://申请退出房间成功
                        UNO_room.activity.finish();
                        lobby.state = lobby.online;//更改状态为：“在线在大厅”
                        telecom.tsleep(50);
                        telecom.reply_i_get();
                        return;//结束死循环
                    case server_protocol.UNO_someone_get_in://有人进入房间了

                        telecom.tsleep(50);
                        telecom.reply_i_get();
                        telecom.listen();//发来进来者名字
                        //lobby_monitor.UNO_add_log1(" 玩家:  " + cc.message + "   进入了房间!");
                        UNO_add_log(" 玩家:  " + cc.message + "   进入了房间!");
                        telecom.tsleep(50);
                        telecom.reply_i_get();
                        break;
                    default:
                        telecom.tsleep(30);
                        telecom.reply_i_get();



                }
            } catch (NumberFormatException e) {lobby.add_log(cc.message); }
        }
    }


    public static boolean start_UNO_room_service(int signal)//在大厅内接受到如下特殊信号
    {
        android.content.Intent i2 ;
        switch (signal)
        {
            case server_protocol.UNO_create_room_success:
                telecom.tsleep(30);
                telecom.reply_i_get();
                UNO_room.whether_host = true;//必然是房间主人
                lobby.state = lobby.in_UNO_room;

                //Intent是一种运行时绑定（run-time binding）机制，它能在程序运行过程中连接源代码文件夹下两个不同的组件。
                i2 = new android.content.Intent(lobby.activity, UNO_room.class);
                lobby.activity.startActivity(i2);//启动该页面服务
                telecom.tsleep(500);
                //UNO_room_signal();
                return true;

            case server_protocol.UNO_show_roomlist:
                //Intent是一种运行时绑定（run-time binding）机制，它能在程序运行过程中连接源代码文件夹下两个不同的组件。
                i2 = new android.content.Intent(lobby.activity, UNO_roomlist.class);
                lobby.activity.startActivity(i2);//启动该页面服务
                telecom.tsleep(30);
                telecom.reply_i_get();//回复自己受到数据
                telecom.tsleep(500);
                //UNO_roomlist_signal();
                return true;
            case server_protocol.UNO_no_room_in_use:
                lobby.add_log("当前没有正在使用的UNO房间!");
                telecom.tsleep(30);
                telecom.reply_i_get();
                return true;
        }
        return false;

    }

    //UNO roomlist展示
    private static String UNO_message;
    public static void UNO_roomlist_set_button(String log)//roomlist动态显示UI,该函数若放在UNO_room_service里无法更新
    {
        UNO_message=log;
        lobby.handler.post(new Runnable() {
            @Override
            public void run() {
                String message1 = UNO_message;

                //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
                android.widget.LinearLayout layout2 = (android.widget.LinearLayout) UNO_roomlist.activity.findViewById(R.id.UNO_roomlist_tablelayout);

                android.widget.TableRow row;
                android.widget.TextView door_number1;
                android.widget.Button door1;

                int count2, doornumber;

                row = new android.widget.TableRow(UNO_roomlist.activity);
                row.setGravity(android.view.Gravity.CENTER);
                door_number1 = new android.widget.TextView(UNO_roomlist.activity);//动态新建一个文字行
                door1 = new android.widget.Button(UNO_roomlist.activity);//动态新建一个按钮
                doornumber = 0;
                for (count2 = 0; message1.charAt(count2) != '\t'; count2++)//几号房间
                {
                    door_number1.setText(door_number1.getText() + String.valueOf(UNO_message.charAt(count2)));

                }

                doornumber = Integer.parseInt(String.valueOf(door_number1.getText()));

                door1.setId(UNO_room_service.idconfig + doornumber);

                door_number1.setText(door_number1.getText() + "号房间  ");
                door1.setText("进入");
                door1.setOnClickListener(UNO_roomlist.listener);


                for (; message1.charAt(count2) == '\t'; count2++) {
                }//跳过中间的\t

                door_number1.setText(door_number1.getText() + "【房主】");
                for (; message1.charAt(count2) != '\t'; count2++) //获取姓名
                    door_number1.setText(door_number1.getText() + String.valueOf(UNO_message.charAt(count2)));

                for (; message1.charAt(count2) == '\t'; count2++) {
                }//跳过中间的\t
                door_number1.setText(door_number1.getText() + "【房内人数】");

                //for (; message1.charAt(count2) != '\n'; count2++) //无法获取\n
                for (; message1.length() > count2; count2++) //获取人数
                    door_number1.setText(door_number1.getText() + String.valueOf(UNO_message.charAt(count2)));

                row.addView(door_number1);
                row.addView(door1);
                layout2.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.WRAP_CONTENT));

            }
        });
    }


    public static String UNO_room_record;
    public static void UNO_add_log(String log)//加日志
    {
        UNO_room_record = log;
        lobby.handler.post(new Runnable() {
            @Override
            public void run() {
                android.view.WindowManager wm = (android.view.WindowManager) UNO_room.activity.getSystemService(android.content.Context.WINDOW_SERVICE);

                android.widget.TableLayout table = (android.widget.TableLayout) UNO_room.activity.findViewById(R.id.UNO_memberlog);
                String history_log = UNO_room_record;
                android.widget.TableRow row = new android.widget.TableRow(UNO_room.activity);
                row.setGravity(android.view.Gravity.CENTER);
                android.widget.TextView log = new android.widget.TextView(UNO_room.activity);
                log.setGravity(android.view.Gravity.CENTER);
                log.setTextSize(22);
                log.setWidth(wm.getDefaultDisplay().getWidth() * 4 / 5);
                log.setText(history_log);
                log.setTextColor(android.graphics.Color.rgb(0, 0, 255));
                row.addView(log);
                table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.TableLayout.LayoutParams.MATCH_PARENT, android.widget.TableLayout.LayoutParams.WRAP_CONTENT));
            }
        });
        telecom.tsleep(50);
        android.widget.ScrollView history_scroll = (android.widget.ScrollView) UNO_room.activity.findViewById(R.id.UNO_room_scrollview);
        history_scroll.post(new Runnable() {
            public void run() {
                android.widget.ScrollView history_scroll = (android.widget.ScrollView) UNO_room.activity.findViewById(R.id.UNO_room_scrollview);
                history_scroll.scrollBy(0, 10000);
            }
        });
    }


}
