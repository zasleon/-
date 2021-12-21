package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.PopupMenu;


public class video_player extends Activity {

    private int videoWidth;//视频宽
    private int videoHeight;//视频高
    private int mobileWidth;//屏幕宽
    private int mobileHeight;//屏幕高
    private static int video_duration=0;

    private static int progress;//进度条进度
    private static boolean nature_refresh=true;//自然更新进度条

    private static final int state_empty=0;//在退出该页面时onDestroy，视频状态变为空
    private static final int state_loaded=1;//在surfaceCreated，视频状态变为已加载
    private static int video_state=state_empty;//视频状态，初始为空

    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    public static android.app.Activity activity;

    private static android.media.MediaPlayer    mediaPlayer=null;
    private static android.view.SurfaceView     surfaceview;//显示界面
    private static android.view.SurfaceHolder   surfaceHolder=null;//显示界面的容器
    private static android.widget.TextView      current_time;//滑动进度条时同步更新时间栏，而不是滑动完了才更新（如果不加static程序会崩溃）
    private static android.widget.TextView      max_duration;//视频文件总时长
    private static android.widget.TextView      jump_to_this_position;//跳转到该时间点
    private static android.widget.Button        video_rate;//播放速率

    private static com.example.xielm.myapplication.VerticalProgressBar volume_progressbar;//自定义进度条
    private static com.example.xielm.myapplication.VerticalProgressBar brightness_progressbar;//自定义进度条
    private static android.widget.TextView      volume_signal;//自定义进度条名字
    private static android.widget.TextView      brightness_signal;//自定义进度条名字


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActionBar().hide();//隐藏标题栏
        //getSupportActionBar().hide();
        getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);//全屏
        setContentView(R.layout.activity_video_player);
        activity=this;//方便static类型函数处理
        video_state=state_empty;
        this.setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//横向播放

        android.view.WindowManager wm = (android.view.WindowManager) this.getSystemService(android.content.Context.WINDOW_SERVICE);//获取屏幕大小，做兼容性调整
        mobileWidth=wm.getDefaultDisplay().getWidth();
        mobileHeight=wm.getDefaultDisplay().getHeight();

        current_time=(android.widget.TextView) findViewById(R.id.current_time);//滑动进度条时同步更新时间栏，而不是滑动完了才更新（如果不加static程序会崩溃）
        max_duration=(android.widget.TextView) findViewById(R.id.max_duration);//总时长
        jump_to_this_position=(android.widget.TextView) findViewById(R.id.jump_to_this_position);//快进/后退到xx：xx
        jump_to_this_position.setTextSize(mobileHeight / 50);//设置字体大小
        skbProgress=(android.widget.SeekBar) findViewById(R.id.seekBar);//获取进度条
        skbProgress.setEnabled(false);//一开始进度条不让碰，直到视频加载完成后运行碰
        surfaceview=(android.view.SurfaceView) findViewById(R.id.surfaceView);//播放画面surfaceview和容器surfaceholder初始化
        video_rate=(android.widget.Button) findViewById(R.id.video_rate);//倍速
        surfaceHolder=surfaceview.getHolder();//surfaceHolder是surfaceview容器
        surfaceHolder.addCallback(new android.view.SurfaceHolder.Callback()//配置surfaceview，mediaplayer的创建在create中实现
        {
            @Override
            public void surfaceChanged(android.view.SurfaceHolder arg0, int arg1, int arg2, int arg3) {
                //add_log("mediaPlayer surface changed");
            }
            @Override
            public void surfaceCreated(android.view.SurfaceHolder arg0) {//刚进入该页面、或切屏回该页面、或熄屏后回来该页面后时，该函数会会触发
                try {
                    if(video_state!=state_loaded) {//if(mediaPlayer==null)//切屏时会触发surfaceDestroyed销毁surface
                        // 再切回来会重新执行surfaceCreated，如果因此而重新执行了mediaPlayer = new android.media.MediaPlayer();这句话，会导致屏幕黑屏
                        //并且在退出该页面activity时会导致ondestroy函数销毁不了mediaplayer，因为多次重新分配（且前几次动态分配的内存都没销毁的话）会导致前几次地址缺失，销毁不了
                        //就会导致现象：“即便退出了视频播放页面，却还在后台播放”
                        //因此需要严格管控mediaplayer的创建和销毁，销毁后mediaPlayer置空null，给mediaPlayer动态分配内存前判断是否为null
                        mediaPlayer = new android.media.MediaPlayer();//申请内存，分配给mediaplayer播放
                    }
                    mediaPlayer.setDisplay(surfaceHolder);
                    mediaPlayer.setAudioStreamType(android.media.AudioManager.STREAM_MUSIC);
                    mediaPlayer.setOnBufferingUpdateListener(new android.media.MediaPlayer.OnBufferingUpdateListener()//更新进度条加载缓冲量
                    {
                        int i;

                        @Override
                        public void onBufferingUpdate(android.media.MediaPlayer arg0, int bufferingProgress) {//更新进度条加载缓冲量
                            skbProgress.setSecondaryProgress(bufferingProgress);//显示已加载缓冲量
                            int currentProgress = skbProgress.getMax() * mediaPlayer.getCurrentPosition() / mediaPlayer.getDuration();
                            //android.util.Log.e(currentProgress + "% play", bufferingProgress + "% buffer");
                        }
                    });
                    mediaPlayer.setOnCompletionListener(new android.media.MediaPlayer.OnCompletionListener() //播放完成后做什么？暂停播放
                    {
                        int i;

                        @Override//播放完成时调用,由于stop()会彻底释放资源，所以调用pause()
                        public void onCompletion(android.media.MediaPlayer arg0) {
                            current_time.setText(time_switch_int_to_string(mediaPlayer.getDuration()));
                            mediaPlayer.pause();
                        }
                    });
                    if(video_state==state_loaded&&!mediaPlayer.isPlaying())
                    {
                        if(mediaPlayer.getCurrentPosition()!=0)
                            mediaPlayer.seekTo(mediaPlayer.getCurrentPosition());//如果以暂停方式切屏，切回来后会导致黑屏，除非继续播放画面才能恢复。用该句可以避免“以暂停方式切屏，切回来后会导致黑屏”
                    }

                } catch (Exception e) {
                    add_log("mediaPlayer error");
                }
                add_log("mediaPlayer surface created");
            }
            @Override//销毁surface
            public void surfaceDestroyed(android.view.SurfaceHolder arg0) {
                //mediaPlayer.pause();//如果想在切屏回该页面、或熄屏后暂停播放，可开放这句话
                add_log("mediaPlayer surface destroyed");
            }

        });
        video_rate.setOnClickListener(new android.view.View.OnClickListener()//设置速率播放
        {
            int i;
            @Override
            public void onClick(android.view.View v) {
                show_rate(v);//显示倍速果菜
            }
            private void show_rate(android.view.View v)//显示速率菜单，根据所选速率改变播放速度
            {
                PopupMenu popupMenu = new PopupMenu(video_player.activity, v);
                popupMenu.inflate(R.menu.menu_video_player);
                popupMenu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                        switch (menuItem.getItemId()) {
                            case R.id.rate_2000:mediaPlayer.setPlaybackParams(new android.media.PlaybackParams().setSpeed(2.0f));break;
                            case R.id.rate_1500:mediaPlayer.setPlaybackParams(new android.media.PlaybackParams().setSpeed(1.5f));break;
                            case R.id.rate_1250:mediaPlayer.setPlaybackParams(new android.media.PlaybackParams().setSpeed(1.25f));break;
                            case R.id.rate_1000:mediaPlayer.setPlaybackParams(new android.media.PlaybackParams().setSpeed(1.0f));break;
                            case R.id.rate_500:mediaPlayer.setPlaybackParams(new android.media.PlaybackParams().setSpeed(0.5f));break;
                            case R.id.rate_250:mediaPlayer.setPlaybackParams(new android.media.PlaybackParams().setSpeed(0.25f));break;
                        }
                        return true;
                    }
                });
                popupMenu.show();
            }
        });
        //========================================自定义进度条部分========================================
        volume_progressbar=(com.example.xielm.myapplication.VerticalProgressBar)findViewById(R.id.volumeProgressBar);//音量变化进度条（该进度条为自定义设置）
        brightness_progressbar=(com.example.xielm.myapplication.VerticalProgressBar)findViewById(R.id.brightnessProgressBar);//亮度变化进度条（该进度条为自定义设置）
        volume_progressbar.setVisibility(android.view.View.INVISIBLE);//音量进度条设置为不可见（该进度条为自定义设置）
        brightness_progressbar.setVisibility(android.view.View.INVISIBLE);//亮度进度条设置为不可见（该进度条为自定义设置）
        volume_signal=(android.widget.TextView) findViewById(R.id.volume_signal);//进度条上名字不可见（该进度条为自定义设置）
        volume_signal.setVisibility(android.view.View.INVISIBLE);
        brightness_signal=(android.widget.TextView) findViewById(R.id.brightness_signal);//进度条上名字不可见（该进度条为自定义设置）
        brightness_signal.setVisibility(android.view.View.INVISIBLE);
    }

    public static void play_this_video(String address) //使用该函数进入播放影片，该函数需要在线程中执行才能正常播放
    {
        //mediaplayer的创建和初始化工作都在surfaceCreated函数中创建，这里只负责设置源视频地址，并启动播放
        telecom.tsleep(500);//需要等待一些时间完成初始化工作，过快播放会导致程序直接崩溃
        try{mediaPlayer.setDataSource(address);}catch(java.io.IOException ex1)
        {client_socket.add_log("mediaPlayer创建失败！"); return;}
        mediaPlayer.prepareAsync();
        mediaPlayer.setOnPreparedListener(new android.media.MediaPlayer.OnPreparedListener() //当播放前续准备全部完成后，播放
        {
            int i;

            @Override
            public void onPrepared(android.media.MediaPlayer arg0) {                // 装载完毕回调
                mediaPlayer.start();//开启播放
                video_duration=mediaPlayer.getDuration();
                skbProgress.setEnabled(true);//一开始进度条不让碰，直到视频加载完成后运行碰
                telecom.tsleep(100);
                video_state = state_loaded;//状态变为加载完成
                mTimer.schedule(mTimerTask, 0, 100);//进度条随时间变动
                /*
                videoWidth = mediaPlayer.getVideoWidth();
                videoHeight = mediaPlayer.getVideoHeight();
                if (videoHeight != 0 && videoWidth != 0) {
                    arg0.start();
                }*/
            }
        });
        skbProgress.setOnSeekBarChangeListener(new android.widget.SeekBar.OnSeekBarChangeListener() //监视进度条是否被拖动
        {
            int i;

            @Override
            public void onProgressChanged(android.widget.SeekBar seekBar, int progress, boolean fromUser) {//当手指触碰进度条上点想对它进行划动变动时会触发该函数
                // 原本是(progress/seekBar.getMax())*player.mediaPlayer.getDuration()
                video_player.progress = progress * mediaPlayer.getDuration() / seekBar.getMax();
                current_time.setText(time_switch_int_to_string(video_player.progress));//手指拖动进度条时，进度条上的点所移动到的时间点数据同步更新到textview current_time上
            }

            @Override
            public void onStartTrackingTouch(android.widget.SeekBar seekBar) {//当手指刚触碰进度条上点想对它进行划动变动时会触发该函数
            }

            @Override
            public void onStopTrackingTouch(android.widget.SeekBar seekBar) {//进度条拖动结束时
                // seekTo()的参数是相对与影片时间的数字，而不是与seekBar.getMax()相对的数字
                mediaPlayer.seekTo(progress);//定位到视频某时间位置
                if (mediaPlayer.isPlaying() != true) mediaPlayer.start();//如果当前处于暂停没播放状态，重新激活播放
            }
        });


    }

    //-------------------------------调整亮度与音量与快进/后退到指定位置----------------------------------------------------
    private static float first_point_X=0;//滑动起始点x值
    private static float first_point_Y=0;//滑动起始点y值
    private static float calculate_point_X=0;//计算起点的x值（某种契机下计算起点不是滑动起始点）
    private static float calculate_point_Y=0;//计算起点的y值（某种契机下计算起点不是滑动起始点）
    private static float first_state=0;//初始状态值（修改前的（当前的）音量/亮度/时间进度）
    private static final int Progress_max=100;//进度条最大值
    private static final int move_position_rate=40;

    private static int now_change_what=0;//现在正在修改什么,往往填入以下4个值决定
    private final int changing_nothing=0;//不打算改变任何东西
    private final int brightness_changing=1;//改亮度
    private final int volume_changing=2;//该音量
    private final int movie_forward_backward_changing=3;//画面快进或倒退

    private void setBrightness(android.view.MotionEvent e1,android.view.MotionEvent e2) //手指滑动调节亮度
    {
        android.view.WindowManager.LayoutParams lp = getWindow().getAttributes();
        lp.screenBrightness= (calculate_point_Y - e2.getY())/mobileHeight + first_state;//计算calculate_point_Y与当前滑动位置在y轴上的距离，设置系统亮度对应值

        if(lp.screenBrightness>=0&&lp.screenBrightness<=1)//当系统亮度对应值在系统亮度范围内
            getWindow().setAttributes(lp);//修改系统亮度

        if(lp.screenBrightness<0)lp.screenBrightness=0;if(lp.screenBrightness>1)lp.screenBrightness=1;//超出阈值则设为阈值
        if(lp.screenBrightness==0||lp.screenBrightness==1)//当亮度达到上下限，滑动最初点设置为手指达到亮度上/下限的点
        {
            calculate_point_Y=e2.getY();//根据手指达到亮度上/下限的点的y值进行亮度变化计算
            first_state=lp.screenBrightness;//亮度变化起始亮度变为上/下限
        }
        brightness_progressbar.setProgress((int)(Progress_max*lp.screenBrightness));//更新亮度进度条
    }
    private void setVolume(android.view.MotionEvent e1,android.view.MotionEvent e2)//手指滑动调节音量
    {
        android.media.AudioManager mAudioManager = (android.media.AudioManager) getSystemService(android.content.Context.AUDIO_SERVICE);
        float max=mAudioManager.getStreamMaxVolume(android.media.AudioManager.STREAM_MUSIC);//获取系统最大音量
        double set_Volume= (float)((calculate_point_Y - e2.getY())*max/mobileHeight + first_state);//计算calculate_point_Y与当前滑动位置在y轴上的距离，设置系统音量对应值

        if(set_Volume>=0&&set_Volume<=max)//当系统音量对应值在系统可播放音量的范围内
            mAudioManager.setStreamVolume(android.media.AudioManager.STREAM_MUSIC, (int) set_Volume, android.media.AudioManager.FLAG_PLAY_SOUND);//修改系统音量

        if(set_Volume<0)set_Volume=0;if(set_Volume>max)set_Volume=max;//超出阈值则设为阈值
        if(set_Volume==0||set_Volume==max)//当音量达到上/下限，滑动最初点设置为手指达到音量上/下限的点
        {
            calculate_point_Y=e2.getY();//根据手指达到音量上/下限的点的y值进行音量变化计算
            first_state=mAudioManager.getStreamVolume(android.media.AudioManager.STREAM_MUSIC);//音量变化起始音量变为上/下限
        }
        volume_progressbar.setProgress((int)(Progress_max*set_Volume/max));//更新音量进度条
    }
    private void setMovie(android.view.MotionEvent e1,android.view.MotionEvent e2) //横向划动，快进或倒退到指定时间位置处
    {
        if(video_state!=state_loaded){telecom.tsleep(500);return ;}//如果视频没加载完，无视手势

        if (((int) first_state + (int) (e2.getX() - first_point_X) * 10) > 0)
            move_position = (int) first_state + (int) (e2.getX() - first_point_X) * move_position_rate;

        if(move_position<0){move_position=0;first_state=0;}//如果往后退超过了起点，将“即将移动到的指定时间位置”改为0起始处
        if(move_position>mediaPlayer.getDuration()){move_position=mediaPlayer.getDuration();first_state = 0;}//如果超过总时长

        current_time.setText(time_switch_int_to_string(move_position));//停止current_time自动对齐mediaplayer播放的更新时间，变为显示划动后即将到的指定时间位置

        if(e2.getX() - first_point_X>0)
            jump_to_this_position.setText("快进到 "+time_switch_int_to_string(move_position));
        else
            jump_to_this_position.setText("后退到 "+time_switch_int_to_string(move_position));
    }

    private static final double FLING_MIN_VELOCITY = 1;
    private static int move_position=0;//定位快进/后退到指定位置的时间
    private android.view.GestureDetector mGestureDetector;
    @Override
    protected void onResume()//判断手指动作是什么？是划动还是点击
    {
        mGestureDetector = new android.view.GestureDetector(
                new android.view.GestureDetector.OnGestureListener() {
                    public boolean onFling(android.view.MotionEvent e1,android.view.MotionEvent e2, float velocityX, float velocityY) {
                        return true;
                    }
                    public boolean onScroll(android.view.MotionEvent e1, android.view.MotionEvent e2,float distanceX, float distanceY) {
                        if(e1.getY()<mobileHeight/6)return true;//触碰点过高，容易和下拉顶部菜单冲突，驳回功能请求

                        if((e1.getY()!=first_point_Y||e1.getX()!=first_point_X)||now_change_what==changing_nothing)//以滑动最初点为起点进行调整，只有当手指初次触碰点变化了才会执行其中内容
                        {
                            now_change_what=changing_nothing;//先设为啥也不想改，如果划动幅度够大了则改动数据
                            if(Math.abs(distanceY) > FLING_MIN_VELOCITY||Math.abs(distanceX) > FLING_MIN_VELOCITY)//划动幅度够大
                            {
                                first_point_X=e1.getX();//记录初始点x值
                                first_point_Y=e1.getY();//记录初始点y值
                                calculate_point_Y=first_point_Y;//根据最初点的y值进行变化计算
                                //if(e1.getX() - e2.getX() > FLING_MIN_DISTANCE||e2.getX() - e1.getX() > FLING_MIN_DISTANCE)
                                if(Math.abs(e1.getX() - e2.getX()) > Math.abs(e1.getY() - e2.getY()))//横着划动幅度大于竖直划动幅度，则为“快进/后退到指定位置”
                                {
                                    jump_to_this_position.setVisibility(android.view.View.VISIBLE);
                                    now_change_what = movie_forward_backward_changing;//更新“正在修改状态”为“快进/后退到指定位置”
                                    nature_refresh=false;//取消“current_time自动对齐mediaplayer播放时间并更新”
                                    if(video_state==state_loaded)
                                        first_state=mediaPlayer.getCurrentPosition();//获取当前播放时间
                                    else
                                        return true;
                                }
                                else//竖直划动幅度大于横向划动幅度
                                    if(e1.getX() > mobileWidth/2)//如果是在右半边屏幕划动手指
                                    {
                                        volume_signal.setVisibility(android.view.View.VISIBLE);
                                        volume_progressbar.setVisibility(android.view.View.VISIBLE);
                                        now_change_what = volume_changing;//设置当前在修改音量
                                        android.media.AudioManager mAudioManager = (android.media.AudioManager) getSystemService(android.content.Context.AUDIO_SERVICE);
                                        first_state=mAudioManager.getStreamVolume(android.media.AudioManager.STREAM_MUSIC);//设置音量变化起始音量
                                    }
                                    else//左半边屏幕划动手指
                                    {
                                        brightness_signal.setVisibility(android.view.View.VISIBLE);
                                        brightness_progressbar.setVisibility(android.view.View.VISIBLE);
                                        now_change_what = brightness_changing;//设置当前在修改亮度
                                        first_state=getWindow().getAttributes().screenBrightness;//获取当前亮度，并将该亮度设置为起始亮度
                                    }
                            }
                        }
                        //test_info(e1,e2,distanceX,distanceY);//显示测试信息
                        if(Math.abs(distanceY) > FLING_MIN_VELOCITY||Math.abs(distanceX) > FLING_MIN_VELOCITY)//划动幅度够大
                            switch(now_change_what)
                            {
                                case brightness_changing:
                                    setBrightness(e1,e2);
                                    break;
                                case volume_changing:
                                    setVolume(e1, e2);
                                    break;
                                case movie_forward_backward_changing:
                                    setMovie(e1, e2);
                                    break;
                                case changing_nothing:break;
                            }
                        return true;
                    }

                    public boolean onSingleTapUp(android.view.MotionEvent e) {
                        return false;
                    }
                    public boolean onDown(android.view.MotionEvent e) {
                        return false;
                    }
                    public void onLongPress(android.view.MotionEvent e) {
                    }
                    public void onShowPress(android.view.MotionEvent e) {// TODO Auto-generated method stub
                    }
                });
        super.onResume();
    }

    //-------------------------------调整亮度与音量与快进/后退到指定位置----------------------------------------------------

    //--------------------------------显示、隐藏进度条，暂停继续播放------------------------------
    private long firstClick_ACTION_UP = 0;//记录首次/最近一次点击时间
    private long secondClick_ACTION_UP = 0;//记录本次点击时间
    private final int totalTime = 300;//两次点击时间间隔，单位毫秒
    @Override
    public boolean onTouchEvent(android.view.MotionEvent event) //手指触摸屏幕事件发生
    {
        boolean result = mGestureDetector.onTouchEvent(event);
        //如果是上划下滑事件,越过下面步骤（即整个if(result){...}之后那些东西）。如果不加这句话，上下滑在 某种契机 下会导致锁栏
        //某种契机：在有进度条的情况下下滑导致进度条不再显示，单点也没用，只有再进行一次反方向上下滑才能重新让进度条显示
        //锁栏：手指单击屏幕却不显示进度条
        if (result)//如果被判定为在屏幕上划动
        {
            if(event.getAction()== android.view.MotionEvent.ACTION_UP)//如果是手指抬起了，隐藏部分UI
            {
                if(!nature_refresh) {//如果是左右划动，执行快进后退到某一时间点动作时，抬起手指后跳转到该时间点（在视频加载完成的情况下跳转，否则卡死）
                    if(video_duration==0||video_state!=state_loaded)return result;
                    jump_to_this_position.setVisibility(android.view.View.INVISIBLE);
                    mediaPlayer.seekTo(move_position);//定位到视频某时间位置
                    if (move_position != mediaPlayer.getDuration() && !mediaPlayer.isPlaying())//如果定位到的地方不是视频末尾且当前没在播放
                        mediaPlayer.start();
                    nature_refresh = true;//允许mediaplayer播放自动更新
                }
                brightness_progressbar.setVisibility(android.view.View.INVISIBLE);
                brightness_signal.setVisibility(android.view.View.INVISIBLE);
                volume_progressbar.setVisibility(android.view.View.INVISIBLE);
                volume_signal.setVisibility(android.view.View.INVISIBLE);
            }
            return result;
        }//if (result)判定结束，判定结束则基本确定不是scroll动作（手指在屏幕上划动动作）

        if(event.getAction()== android.view.MotionEvent.ACTION_UP)//如果是“手指抬起了”的动作
        {
            //硬修复scroll结束时可能接收不到ACTION_UP动作的bug，即手指划动后手指离开屏幕时，可能这个抬手的动作手势会没与划动衔接，被判定为单个ACTION_UP从而来到这里
            // 这里的硬修复，用于隐藏自定义音量/亮度进度条vertical_processbar、快进/后退显示textview
            if(!nature_refresh) {
                if(video_duration==0||video_state!=state_loaded)return super.onTouchEvent(event);//如果视频未加载完成，不进行跳转，否则可能会卡死
                nature_refresh = true;//允许mediaplayer播放自动更新
                jump_to_this_position.setVisibility(android.view.View.INVISIBLE);
                mediaPlayer.seekTo(move_position);//定位到视频某时间位置
                if (move_position != mediaPlayer.getDuration() && !mediaPlayer.isPlaying())//如果定位到的地方不是视频末尾且当前没在播放
                    mediaPlayer.start();
                return super.onTouchEvent(event);
            }
            if(brightness_progressbar.getVisibility()==android.view.View.VISIBLE||volume_progressbar.getVisibility()==android.view.View.VISIBLE) {
                brightness_progressbar.setVisibility(android.view.View.INVISIBLE);
                brightness_signal.setVisibility(android.view.View.INVISIBLE);
                volume_progressbar.setVisibility(android.view.View.INVISIBLE);
                volume_signal.setVisibility(android.view.View.INVISIBLE);
                return super.onTouchEvent(event);
            }
            //修复结束-===============================================================================================================

            if(firstClick_ACTION_UP==0)//如果没进行首次点击
                firstClick_ACTION_UP = System.currentTimeMillis();//获取当前时间，赋予首次点击点击时间
            else{
                secondClick_ACTION_UP = System.currentTimeMillis();//获取当前时间，赋予最近一次
                if (secondClick_ACTION_UP-firstClick_ACTION_UP<totalTime) {//如果两次点击屏幕时间过短
                    if (mediaPlayer.isPlaying())//判断当前是否正在播放
                    {
                        mediaPlayer.pause();//暂停播放
                    }
                    else
                        mediaPlayer.start();//当前没在播放，开始继续播放
                    firstClick_ACTION_UP = secondClick_ACTION_UP;//将当前点击时间设为最近一次点击时间
                    return super.onTouchEvent(event);
                }
                firstClick_ACTION_UP = secondClick_ACTION_UP;//将当前点击时间设为最近一次点击时间
            }
            if(skbProgress.getVisibility()==android.view.View.VISIBLE)//获取当前进度条状态
            {
                android.widget.TextView pointer=(android.widget.TextView) findViewById(R.id.max_duration);
                pointer.setVisibility(android.view.View.INVISIBLE);
                pointer=(android.widget.TextView) findViewById(R.id.current_time);
                pointer.setVisibility(android.view.View.INVISIBLE);
                android.widget.Button pointer2=(android.widget.Button)findViewById(R.id.video_rate);//调整速率按钮
                pointer2.setVisibility(android.view.View.INVISIBLE);
                skbProgress.setVisibility(android.view.View.INVISIBLE);//隐藏进度条
            }
            else {
                android.widget.TextView pointer=(android.widget.TextView) findViewById(R.id.max_duration);
                pointer.setVisibility(android.view.View.VISIBLE);
                pointer=(android.widget.TextView) findViewById(R.id.current_time);
                pointer.setVisibility(android.view.View.VISIBLE);
                android.widget.Button pointer2=(android.widget.Button)findViewById(R.id.video_rate);//调整速率按钮
                pointer2.setVisibility(android.view.View.VISIBLE);
                skbProgress.setVisibility(android.view.View.VISIBLE);//显示进度条
            }
        }
        return super.onTouchEvent(event);
    }
    //--------------------------------显示、隐藏进度条，暂停继续播放------------------------------

    //----------------------进度条随时间变动----------------------
    private static android.widget.SeekBar skbProgress;
    private static java.util.Timer mTimer=new java.util.Timer();
    private static java.util.TimerTask mTimerTask = new java.util.TimerTask()
    {
        int i;
        @Override
        public void run() {

            if(mediaPlayer==null)
                return;
            if (mediaPlayer.isPlaying() && !skbProgress.isPressed() && nature_refresh) {//如果当前媒体正在播放，且进度条没被手指拖动，且没有手指位移划动快进/后退动作
                handleProgress.sendEmptyMessage(0);
            }

        }
    };
    private static android.os.Handler handleProgress = new android.os.Handler()
    {
        int i;
        public void handleMessage(android.os.Message msg) {
            if (mediaPlayer==null||!mediaPlayer.isPlaying())return;
            int duration = mediaPlayer.getDuration();//获取总时长
            if (mediaPlayer==null||!mediaPlayer.isPlaying())return;
            int position = mediaPlayer.getCurrentPosition();//获取mediaplayer当前播放到的时间点
            String durationstring=time_switch_int_to_string(duration);// duration / 1000 后单位为“秒”
            if(position>=duration)position=duration;//莫名其妙有时候会比总时长多出一秒，用这句来修复
            String currentpositionstring=time_switch_int_to_string(position);//转化为字符串


            if (duration > 0) {
                long pos = skbProgress.getMax() * position / duration;
                skbProgress.setProgress((int) pos);//更新进度条上点的位置
                max_duration.setText(durationstring);//更新mediaplayer播放内容总时长
                current_time.setText(currentpositionstring);//实时更新mediaplayer播放的当前时间点
            }
        };
    };
    private static String time_switch_int_to_string(int time)//将数字转换为“00:00:00”字符串形式表示，输入mediaplayer获取的当前time或总时长int后，将转化后的字符串结果返回
    {
        String result="";
        if(time / 1000<3600) {
            int minute=time / 1000 / 60;
            int second=(time / 1000) % 60;
            if(minute>=10)
                result = minute+":";
            else
                result ="0"+minute+":";
            if(second>=10)
                result=result+second;
            else
                result=result+"0"+second;
        }
        else {
            int hour    =time / 1000 / 60 / 60;
            int minute  =(time / 1000 - 3600 * (time / 1000 / 60 / 60)) / 60;
            int second  =(time / 1000) % 60;
            if(hour>=10)
                result=hour+":";
            else
                result="0"+hour+":";
            if(minute>=10)
                result=result+minute+":";
            else
                result=result+"0"+minute+":";
            if(second>=10)
                result=result+second;
            else
                result=result+"0"+second;
        }
        return result;

    }
    //----------------------进度条随时间变动----------------------


    //----------------------杂项-----------------------------------------------------------------------------------------------
    @Override
    protected void onDestroy() //销毁该页面时，在activity结束的时候回收资源
    {
        if(mediaPlayer!=null&&mediaPlayer.isPlaying())//不加mediaPlayer!=null会导致闪退
        {
            mediaPlayer.stop();//stop自带释放资源功能    if(mediaPlayer != null) {mediaPlayer.release();mediaPlayer = null;}
        }
        video_duration=0;
        video_state=state_empty;
        //mediaPlayer=null;//这句话会导致直接卡死。。。不得不加状态量：video_state
        super.onDestroy();
    }

    private static android.widget.TextView tt;//测试部分，仅供测试调试时使用
    private void test_info(android.view.MotionEvent e1, android.view.MotionEvent e2,float distanceX, float distanceY) {
        tt=(android.widget.TextView)findViewById(R.id.ttt);
        tt.setVisibility(android.view.View.VISIBLE);
        switch(now_change_what)
        {
            case brightness_changing:tt.setText("brightness_changing");break;
            case volume_changing:tt.setText("volume_changing");break;
            case movie_forward_backward_changing:if((e2.getX() - e1.getX())>0)tt.setText("move forward");else tt.setText("move backward");break;
            case changing_nothing:tt.setText("changing_nothing");break;
        }
        tt.setText(tt.getText() +
                "\ny: " + (e1.getY() - e2.getY()) +
                "\nx: " + (e2.getX() - e1.getX()) +
                "\n 本次滑动 y轴方向速度   " + Math.abs(distanceY) +
                "\n 本次滑动 x轴方向速度   " + Math.abs(distanceX));
    }

    public static String record;
    public static void add_log(String log)//个人加日志方法
    {
        /*
        record = log;
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.view.WindowManager wm = (android.view.WindowManager) activity.getSystemService(android.content.Context.WINDOW_SERVICE);

                android.widget.TableLayout table = (android.widget.TableLayout) activity.findViewById(R.id.videolog);
                String history_log = record;
                android.widget.TableRow row = new android.widget.TableRow(activity);
                row.setGravity(android.view.Gravity.CENTER);
                android.widget.TextView log = new android.widget.TextView(activity);
                log.setGravity(android.view.Gravity.CENTER);
                log.setTextSize(22);
                log.setWidth(wm.getDefaultDisplay().getWidth() * 4 / 5);
                log.setText(history_log);
                log.setTextColor(android.graphics.Color.rgb(0, 0, 255));
                row.addView(log);
                table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.TableLayout.LayoutParams.MATCH_PARENT,android.widget.TableLayout.LayoutParams.WRAP_CONTENT));
                telecom.tsleep(200);
            }
        });

        android.widget.ScrollView history_scroll = (android.widget.ScrollView) activity.findViewById(R.id.scrollView3);
        history_scroll.post(new Runnable() {
            public void run() {
                android.widget.ScrollView history_scroll = (android.widget.ScrollView) activity.findViewById(R.id.scrollView3);
                history_scroll.scrollBy(0, 10000);
            }
        });
        */
    }
    //----------------------杂项-----------------------------------------------------------------------------------------------

    //=======================================以下为系统自带函数=====================================================================
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        //getMenuInflater().inflate(R.menu.menu_video_player, menu);
        //return true;
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return true;/*
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);*/
    }
}
