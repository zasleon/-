package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/1.
 */
public class telecom extends Thread implements Runnable{
    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    public static java.io.InputStream oos;
    public static java.io.BufferedReader br;
    @Override
    public void run() {

        try {oos = client_socket.c_socket.getInputStream();//创建socket接收流
        }catch(java.io.IOException ex2){client_socket.add_log("socket创建输入流失败！");return ;}

        try {br = new java.io.BufferedReader(new java.io.InputStreamReader(oos, "gbk"));//创建接收内存
        }catch(java.io.UnsupportedEncodingException ex3){client_socket.add_log("socket创建读取内存失败！");return ;}

        try{client_socket.message=br.readLine();//获取接收内存字符串，到第一个“\n”为主
        }catch(java.io.IOException ex4){client_socket.add_log("socket读取内存失败！");return ;}

    }
    public static void listen() {
        try {oos = client_socket.c_socket.getInputStream();//创建socket接收流
        }catch(java.io.IOException ex2){client_socket.whether_close=true;return ;}

        try {br = new java.io.BufferedReader(new java.io.InputStreamReader(oos, "gbk"));//创建接收内存
        }catch(java.io.UnsupportedEncodingException ex3){client_socket.whether_close=true;return ;}

        try{
            client_socket.message=br.readLine();//获取接收内存字符串，到第一个“\n”为主
        }catch(java.io.IOException ex4){client_socket.whether_close=true;return ;}

    }





    public static Boolean recv()
    {
        client_socket.message="";//先清空之前接受到的消息
        telecom tt=new telecom ();
        tt.start();
        telecom.tsleep(200);//如果不等久一点，线程不一定走完！导致初始登录失败等异常情况！
        tt.interrupt();
        if(client_socket.message.length()==0){return false;}
        return true;
    }
    public static void tsleep(int sleeptime)
    {
        try {Thread.sleep(sleeptime);} catch (InterruptedException ex){return;}
    }

    public static void send(String message)
    {
        //client_socket cc = client_socket.getSingleIntance();
        try {

            java.io.OutputStream outputStream = client_socket.c_socket.getOutputStream();
            if (outputStream != null) {
                outputStream.write(message.getBytes("gbk"));
                outputStream.flush();
            }
        } catch (java.io.IOException e) {
            client_socket.whether_close = true;
        }
    }

    public static void reply_i_get()
    {
        client_socket cc = client_socket.getSingleIntance();
        java.io.OutputStream outputStream;
        try{
            outputStream = cc.c_socket.getOutputStream();//回复收到

            if (outputStream != null) {
                outputStream.write(Integer.toString(server_protocol.client_get_message).getBytes("gbk"));
                outputStream.flush();
            }
        } catch (java.io.IOException e) {client_socket.whether_close=true;}
    }
}
