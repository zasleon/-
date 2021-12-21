package com.example.xielm.myapplication;

import android.content.pm.ActivityInfo;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity implements android.widget.Button.OnClickListener{
    private static android.os.Handler handler = new android.os.Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        client_socket.activity=this;

        client_socket.socket_ini();

        android.view.WindowManager wm = (android.view.WindowManager) this.getSystemService(android.content.Context.WINDOW_SERVICE);//获取屏幕大小，做兼容性调整
        android.widget.TextView szsc = (android.widget.TextView) this.findViewById(R.id.textView2);
        szsc.setTextSize(wm.getDefaultDisplay().getWidth()/16);
        //szsc.setTextSize(szsc.getWidth()/2);
        android.widget.TextView welcome = (android.widget.TextView) this.findViewById(R.id.textView4);
        welcome.setTextSize(wm.getDefaultDisplay().getWidth() / 50);
        android.widget.TextView loginterface = (android.widget.TextView) this.findViewById(R.id.textView3);
        loginterface.setTextSize(wm.getDefaultDisplay().getWidth()/50);
        android.widget.Button login_click = (android.widget.Button) findViewById(R.id.login);//登录，跳转页面
        login_click.setOnClickListener(this);
        login_click.setTextSize(wm.getDefaultDisplay().getWidth() / 50);


        android.widget.TextView movie = (android.widget.TextView) this.findViewById(R.id.movie_enter);
        movie.setTextSize(wm.getDefaultDisplay().getWidth() / 50);
        movie.setOnClickListener(this);

    }

    public static boolean video_movie=false;
    public static boolean beshow=false;
    public void onClick(android.view.View v) {
        video_movie=false;
        if (v.getId() == R.id.login)//如果说有多个需要触发点击事件的按钮，需要判断是什么按钮，再触发该按钮的功能
        {
            if (ButtonUtils.isFastDoubleClick(R.id.login)) {return;}//防止疯狂连点

            android.widget.EditText username=(android.widget.EditText)findViewById(R.id.editText2);
            if(username.length()==0){client_socket.add_log("用户名不能为空！");return;}

            client_socket cc=new client_socket ();
            cc.c1=cc;
            cc.start();//开始尝试socket连接
            telecom.tsleep(500);
        }//登录服务结束
        else
        {
            video_movie=true;
            telecom.tsleep(300);
            client_socket cc=new client_socket ();
            cc.c1=cc;
            cc.start();

        }
    }//onClick结束






    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
