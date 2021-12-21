package com.example.xielm.myapplication;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Created by Xielm on 2021/4/1.
 */
public class client_socket extends Thread implements Runnable{
    public static client_socket c1;//该类的唯一对象
    public static Socket c_socket;//创建唯一socket
    public static Boolean whether_close;//socket是否被关闭
    public static Boolean whether_create;//是否创建过了socket
    public static String message;//专门存放刚接收到的服务器数据
    public static android.app.Activity activity;
    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作

    @Override
    public void run() {

        while (true) {
            if(MainActivity.video_movie)
            {
                if(!MainActivity.beshow) {
                    MainActivity.beshow = true;

                    android.content.Intent i2 = new android.content.Intent(client_socket.activity, movie_lobby.class);
                    //android.content.Intent i2 = new android.content.Intent(client_socket.activity, video_player.class);
                    client_socket.activity.startActivity(i2);
                    telecom.tsleep(500);

                            //String address="https://vip.w.xk.miui.com/10d7ff4c9368d411b254bb5c62d6e849";//网络点播
                            //String address="http://10.125.39.201:10080/frecord/warriors(2WEI).mp4";
                            //String address="http://10.125.39.201:10080/frecord/dondiablo.mp4";
                            //String address = "https://video.dious.cc/20200621/ksmyusvo/1000kb/hls/index.m3u8";
                            //video_player.play_this_video(address);
                        }
                    }else

            if (!whether_create) {//初次尝试连接，如果没返回7070表明服务器超载
                //103.46.128.21花生壳 34024//10.125.39.201 8001本地
                try {c_socket = new java.net.Socket(InetAddress.getByName("10.125.39.63"), 8001);
                    //try {c_socket = new java.net.Socket(InetAddress.getByName("103.46.128.53"), 12313);
                } catch (java.io.IOException ex1) {add_log("socket创建失败！");return; }

                //if(telecom.recv()){k5 = true;return;}//接受服务器发来的数据
                telecom.listen();

                if(message.equals("系统超载啦")){add_log("系统超载啦！");return;}//如果发来的是系统超载啦，表明服务器到达负荷上限，无法提供服务
                if(message.equals(Integer.toString(server_protocol.start_link)))//到此步，则已经完成初次连接，服务器应该发来start_link
                {
                    whether_create = true;whether_close=false;
                    add_log("连接完成！");
                    telecom.reply_i_get();//回复自己受到数据
                }
                else//发来奇怪的东西，或什么都没发
                {add_log("发来奇怪的东西："+message);return;}
                //执行到此步说明没有任何问题，进入服务大厅界面
                telecom.tsleep(300);
                telecom.send(String.valueOf(server_protocol.android_phone));//自己是安卓端
                telecom.tsleep(300);
                android.widget.EditText username=(android.widget.EditText)activity.findViewById(R.id.editText2);
                telecom.send(String.valueOf(username.getText()));//发送自己的用户名给服务器
                //Intent是一种运行时绑定（run-time binding）机制，它能在程序运行过程中连接源代码文件夹下两个不同的组件。
                android.content.Intent i = new android.content.Intent(activity, lobby.class);
                monitor_all.stop_send(500);
                activity.startActivity(i);//启动该页面服务
            }

            telecom.tsleep(10000);
            if(whether_close){whether_create=false;return;}

        }
    }

    public static String log;
    public static void add_log(String record)
    {
        log=record;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String history_log = log;
                android.widget.TextView netlog = (android.widget.TextView) activity.findViewById(R.id.netalarm);
                log = "";
                netlog.setText(history_log);
            }
        });
    }


    public static void socket_ini(){//所有静态变量初始化
        message="";
        whether_create=false;//默认没有创建socket
        whether_close=false;//默认被强制关闭socket

    }

    public static client_socket  getSingleIntance(){//获取一个已经创建了的socket

        return  c1 ;
    }
}
