package com.example.xielm.myapplication;



/**
 * Created by Xielm on 2021/4/7.
 */
public class movie_lobby_load extends Thread implements Runnable{

    private String address;
    private String movie_name_this;
    private com.example.xielm.myapplication.ImageViewPlus imageViewPlus;
    private android.widget.ImageView imageView;
    private android.widget.TextView  movie_number;
    private int choice;
    //private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作

    public static final int imageviewPlus=1;
    public static final int imageview=2;

    public static void refresh_one_movie(int id,String httpaddress,int Pchoice)
    {
        movie_lobby_load myThread=new movie_lobby_load();
        myThread.set_property(id, httpaddress,Pchoice);

        Thread thread=new Thread(myThread);thread.start();
    }


    private void set_property(int id,String httpaddress,int Pchoice)
    {
        choice=Pchoice;
        address=httpaddress;
        switch (Pchoice) {
            case imageviewPlus:
                imageViewPlus = (com.example.xielm.myapplication.ImageViewPlus) mp3_player.activity.findViewById(id);
                break;
            case imageview:
                imageView=(android.widget.ImageView) mp3_player.activity.findViewById(id);
                break;

        }
        //movie_number=(android.widget.TextView) movie_episode_select.activity.findViewById(movie_name_id);
        //movie_name_this=movie_name;
    }
    @Override
    public void run()
    {
        //final android.graphics.Bitmap bitmap = getPicture("http://10.125.39.201:10080/frecord/warriors.jpg");
        final android.graphics.Bitmap bitmap = getPicture(address);
        /*try {Thread.sleep(500);} catch (InterruptedException e) {e.printStackTrace();}*///线程休眠
        //发送一个对象
        switch (choice) {
            case imageviewPlus:
                imageViewPlus.post(new Runnable() {
                    @Override
                    public void run() {
                        //movie_number.setText(movie_name_this);
                        imageViewPlus.setImageBitmap(bitmap);//在ImageView中显示从网络上获取到的图片
                    }
                });
                break;
            case imageview:
                imageView.post(new Runnable() {
                    @Override
                    public void run() {
                        //movie_number.setText(movie_name_this);
                        imageView.setImageBitmap(bitmap);
                    }
                });
                break;
        }
    }

    private android.graphics.Bitmap getPicture(String path) {
        android.graphics.Bitmap bm = null;
        try {
            java.net.URL url = new java.net.URL(path);//创建URL对象

            java.net.URLConnection conn = url.openConnection();//获取URL对象对应的连接
            conn.connect();//打开连接
            java.io.InputStream is = conn.getInputStream();//获取输入流对象
            bm = android.graphics.BitmapFactory.decodeStream(is);//根据输入流对象创建bitmap对象
        } catch (java.net.MalformedURLException e1) {
            e1.printStackTrace();
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }
        return bm;
    }

}
