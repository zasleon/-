package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class UNO_room extends Activity {
    public static android.app.Activity activity;

    public static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作

    final client_socket cc=client_socket.getSingleIntance();
    public static boolean whether_host;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_uno_room);
        activity=this;

        lobby.state = lobby.in_UNO_room;
        UNO_room_service kk=new UNO_room_service();kk.start();

        android.widget.Button start_game=(android.widget.Button) findViewById(R.id.UNO_start_game);
        start_game.setOnClickListener(listener);
        if(whether_host)
            start_game.setEnabled(true);
        else
            start_game.setEnabled(false);

        android.widget.Button exit_room=(android.widget.Button) findViewById(R.id.UNO_exit_room);
        exit_room.setOnClickListener(listener);

    }







    private android.view.View.OnClickListener listener = new android.view.View.OnClickListener(){

        @Override
        public void onClick(android.view.View v) {
            //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
            switch (v.getId()) {
                case R.id.UNO_exit_room:
                    monitor_all.send(""+server_protocol.UNO_apply_exit_room);//退出房间
                    break;

            }
        }
    };


    @Override//防止用户退回登录界面
    public boolean dispatchKeyEvent(android.view.KeyEvent event) {
        if (event.getKeyCode() == android.view.KeyEvent.KEYCODE_BACK ) {
            return true;
        } else {
            return super.dispatchKeyEvent(event);
        }
    }


}
