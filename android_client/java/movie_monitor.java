package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/9.
 */
public class movie_monitor extends Thread implements Runnable {
    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    public static int mobileWidth;
    public static int mobileHeight;
    private final static int buttonNo=3333;
    private static final int pic_number=12;

    private static String []source_path= new String [pic_number];//地址
    private static String []source_name= new String [pic_number];//名称
    private static int    []source_episode_number= new int [pic_number];//剧集数量
    public static int getin=0;
    public static int get_start=0;
    @Override
    public void run()
    {
        android.view.WindowManager wm = (android.view.WindowManager) movie_lobby.activity.getSystemService(android.content.Context.WINDOW_SERVICE);//获取屏幕大小，做兼容性调整
        mobileWidth=wm.getDefaultDisplay().getWidth();
        mobileHeight=wm.getDefaultDisplay().getHeight();
        ini_video_test();

        for(int i=0;i<pic_number;) {
            show_image(i);
            telecom.tsleep(200);i=i+2;
        }
        while(true)
        {
            if(getin!=0)
            {
                telecom.tsleep(200);
                if(true) {//开启mp3播放服务
                    android.content.Intent i2 = new android.content.Intent(movie_lobby.activity, mp3_player.class);
                    movie_lobby.activity.startActivity(i2);
                    telecom.tsleep(300);
                    String mp3_image_path = "http://10.125.39.63:8088/media/aaa.jpg";
                    String mp3_path = "http://10.125.39.63:8088/media/11.mp4";
                    //String mp3_path = "https://m801.music.126.net/20211126125900/4228dc87241d37416cf27f461b9d9f48/jdyyaac/obj/w5rDlsOJwrLDjj7CmsOj/8614123554/d3e5/49e0/0fef/8b034ba68f8b6797ead8f8575feae86f.m4a";
                    mp3_player.mp3_ini(mp3_path, mp3_image_path);
                }

                //android.content.Intent i2 = new android.content.Intent(client_socket.activity, video_player.class);
                getin=0;
            }
            telecom.tsleep(500);
        }
    }

    private static int count;
    private static void show_image(int i)//显示一行两个图片和配字
    {
        count=i;
        handler.post(new Runnable() {
            @Override
            public void run() {
                int image_1=count;
                int image_2=image_1+1;

                //andrid.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
                android.widget.TableLayout layout = (android.widget.TableLayout) movie_lobby.activity.findViewById(R.id.movie_table);
                android.widget.TableRow row_image = new android.widget.TableRow(movie_lobby.activity);//新建行，用于存放图片
                android.widget.TableRow row_text = new android.widget.TableRow(movie_lobby.activity);//新建行，用于存放文字
                row_image.setGravity(android.view.Gravity.CENTER);//内容居中显示
                row_text.setGravity(android.view.Gravity.CENTER);//内容居中显示

                android.widget.TextView movie_name1 = new android.widget.TextView(movie_lobby.activity);//动态新建一个文字行
                android.widget.TextView movie_name2 = new android.widget.TextView(movie_lobby.activity);//动态新建一个文字行
                movie_name1.setGravity(android.view.Gravity.CENTER);//内容居中显示
                movie_name2.setGravity(android.view.Gravity.CENTER);//内容居中显示

                //android.widget.TextView slot = new android.widget.TextView(movie_lobby.activity);//动态新建一个文字行
                android.widget.ImageButton imagebutton1=new android.widget.ImageButton(movie_lobby.activity);
                android.widget.ImageButton imagebutton2=new android.widget.ImageButton(movie_lobby.activity);

                imagebutton1.setId(buttonNo + image_1);
                imagebutton2.setId(buttonNo + image_2);

                android.widget.TableRow.LayoutParams params= new android.widget.TableRow.LayoutParams( (int)(mobileWidth * 0.5), (int) (mobileHeight * 0.4));//控制图片大小
                params.setMargins(0, 0, -20, 0);//让按钮相互贴近，设置边框为负值
                imagebutton1.setLayoutParams(params);
                params.setMargins(-20, 0, 0, 0);
                imagebutton2.setLayoutParams(params);
                //imagebutton1.setScaleType(android.widget.ImageButton.ScaleType.CENTER_CROP);//填充屏幕，导致按钮间存在间隔
                //imagebutton2.setScaleType(android.widget.ImageButton.ScaleType.CENTER_CROP);//填充屏幕，导致按钮间存在间隔
                com.bumptech.glide.Glide.with(movie_lobby.activity).load(source_path[image_1]).fitCenter().into(imagebutton1);//源地址内容显示到目标View中
                com.bumptech.glide.Glide.with(movie_lobby.activity).load(source_path[image_2]).fitCenter().into(imagebutton2);//源地址内容显示到目标View中

                movie_name1.setText(source_name[image_1]);//视频文本名字
                movie_name2.setText(source_name[image_2]);//视频文本名字

                imagebutton1.setBackgroundColor(android.graphics.Color.TRANSPARENT);//图片后按钮变透明
                imagebutton2.setBackgroundColor(android.graphics.Color.TRANSPARENT);//图片后按钮变透明
                imagebutton1.setOnClickListener(movie_lobby.listener);
                imagebutton2.setOnClickListener(movie_lobby.listener);

                row_image.addView(imagebutton1);row_image.addView(imagebutton2);
                layout.addView(row_image);
                row_text.addView(movie_name1);row_text.addView(movie_name2);
                layout.addView(row_text);

            }
        });
    }

    private static void ini_video_info()//初始化内容
    {
        for(int i=0;i<pic_number;i++)
        {
            source_name[i]="???";source_path[i]="???";
        }
    }
    private static void ini_video_test()//初始化内容
    {//sourcename不宜太长，容易导致显示有问题
        source_name[0] = "俺妹第一季";
        source_path[0] = "https://bkimg.cdn.bcebos.com/pic/c995d143ad4bd11373f02ada76e5b30f4bfbfaedaf8b?x-bce-process=image/resize,m_lfit,w_268,limit_1/format,f_jpg";
        source_episode_number[0]=16;
        source_name[1] = "俺妹第二季";
        source_path[1] = "http://pic.xiaomingming.org/FileUpload/2679.jpg";
        source_episode_number[1]=16;
        source_name[2] = "天使心跳";
        source_path[2] = "http://pic.xiaomingming.org/FileUpload/1747.jpg";
        source_name[3] = "日在校园";
        source_path[3] = "http://pic.xiaomingming.org/FileUpload/1630.jpg";
        source_name[4] = "男子高中生的日常";
        source_path[4] = "http://pic.xiaomingming.org/FileUpload/1669.jpg";
        source_name[5] = "女子高生の無駄づかい";
        source_path[5] = "http://p.xiaomingming.org/FileUpload/201951020304329746.jpg";
        source_name[6] = "日常";
        source_path[6] = "http://pic.xiaomingming.org/FileUpload/653.jpg";
        source_name[7] = "pop子和pipi美的日常";
        source_path[7] = "http://p.xiaomingming.org/FileUpload/20174217115035077.jpg";
        source_name[8] = "日常悠哉大王";
        source_path[8] = "http://pic.xiaomingming.org/FileUpload/4042.jpg";
        source_name[9] = "铠甲勇士第一季";
        source_path[9] = "http://pic.xiaomingming.org/FileUpload/1576.jpg";
        source_name[10] = "铠甲勇士第二季";
        source_path[10] = "http://pic.xiaomingming.org/FileUpload/2667.jpg";
        source_name[11] = "铠甲勇士刑天后传";
        source_path[11] = "http://pic.xiaomingming.org/FileUpload/4469.jpg";
    }

}
