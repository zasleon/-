package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/1.
 */
public class monitor_all extends Thread implements Runnable{
    public static String message;
    private static int counter;
    private static int lock;//如果上锁，封锁用户输出
    public static void send(String send_message)//用户要发的消息
    {
        if(lock>0||counter>=0)return;
        counter++;
        if(counter>0)return;
        message=send_message;
    }
    @Override
    public void run() {
        lock=2;
        counter=-1;
        while(true) {
            if (counter == 0&& message.length() > 0)
            {
                telecom.tsleep(300);
                if(lock>0){counter=-1;continue;}
                telecom.send(message);
                message = "";
                stop_send(500);
                counter = -1;
            }
            else
                telecom.tsleep(400);
        }

    }
    public static void just_lock(){
        lock=lock+2;
    }
    public static void just_release()
    {
        lock=lock-2;

    }

    private static int sleepnumber;
    public static void stop_send(int number)
    {
        just_lock();
        sleepnumber=number;
        Thread thread=new Thread(new Runnable() {
            @Override
            public void run(){

                telecom.tsleep(sleepnumber);
                just_release();}
        });
        thread.start();
    }
}
