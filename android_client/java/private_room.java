package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class private_room extends Activity {
    public static android.app.Activity activity;

    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作

    final client_socket cc=client_socket.getSingleIntance();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_private_room);
        activity=this;
        lobby.state=lobby.in_room;

        android.widget.Button leave_room=(android.widget.Button) findViewById(R.id.button5);
        leave_room.setOnClickListener(listener);
        android.widget.Button start_fight=(android.widget.Button) findViewById(R.id.button6);
        start_fight.setOnClickListener(listener);
        android.widget.Button add_R=(android.widget.Button) findViewById(R.id.add_Robot);
        add_R.setOnClickListener(listener);
        android.widget.Button delete_R=(android.widget.Button) findViewById(R.id.delete_Robot);
        delete_R.setOnClickListener(listener);
        if(lobby_monitor.whether_host)
            start_fight.setEnabled(true);
        else
            start_fight.setEnabled(false);
    }

    // private Thread thread = new Thread(new Runnable() {@Override public void run() {}}

    private android.view.View.OnClickListener listener = new android.view.View.OnClickListener(){

        @Override
        public void onClick(android.view.View v) {
            //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
            switch (v.getId()) {
                case R.id.button5:
                    monitor_all.send(""+server_protocol.SZSC_apply_exit_room);//退出房间
                    break;
                case R.id.button6:
                    if(!lobby_monitor.whether_host)return;
                    monitor_all.send(""+server_protocol.SZSC_apply_start_game);//开始游戏
                    break;
                case R.id.add_Robot:
                    monitor_all.send(""+server_protocol.SZSC_apply_add_robot);
                    break;
                case R.id.delete_Robot:
                    monitor_all.send(""+server_protocol.SZSC_apply_remove_robot);
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

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_private_room, menu);
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
