package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class lobby extends Activity {
    public static String[] data = {
            "1号\t\t在线\t\t小张",
            "2号\t\t房间\t\t小明",
            "3号\t\t房间\t\t呼呼",
            "4号（你）\t\t在线\t\t小李",
            "5号\t\t在线\t\t系统管理员",
            ""};

    final public static int online=1;
    final public static int roomlist=2;
    final public static int choose_character=3;
    final public static int in_room=4;
    final public static int video=5;
    final public static int other=6;

    final public static int in_UNO_room=20;
    final public static int in_UNO_roomlist=21;
    final public static int in_UNO_game=22;


    public static int state;
    public static android.app.Activity activity;//lobby
    public static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lobby);
        state=online;//最初处于在线状态

        activity=this;
        lobby_monitor.activity=this;
        lobby_monitor kk=new lobby_monitor();kk.start();//启动lobby监听服务，如果该句在线程中执行，将会使变更UI的语句失效
        monitor_all mm=new monitor_all();mm.start();//启动全局监听发送功能函数，所有客户端主动发送消息都要经过那里

        android.widget.Button refresh=(android.widget.Button) findViewById(R.id.button4);refresh.setOnClickListener(listener);
        android.widget.Button get_in_room=(android.widget.Button) findViewById(R.id.button3);get_in_room.setOnClickListener(listener);
        android.widget.Button create_room=(android.widget.Button) findViewById(R.id.button2);create_room.setOnClickListener(listener);
        android.widget.Button get_video=(android.widget.Button) findViewById(R.id.button);get_video.setOnClickListener(listener);

        //UNO
        android.widget.Button UNO_create_room=(android.widget.Button) findViewById(R.id.UNO_create_room);UNO_create_room.setOnClickListener(listener);
        android.widget.Button UNO_enter_room=(android.widget.Button) findViewById(R.id.UNO_enter_room);UNO_enter_room.setOnClickListener(listener);

    }


    private android.view.View.OnClickListener listener = new android.view.View.OnClickListener() {

        @Override
        public void onClick(android.view.View v) {
            state=online;
            android.widget.TextView t7 = (android.widget.TextView) findViewById(R.id.server_message);
            android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
            //if (!telecom.recv()) {t.show();private_room.this.finish();lobby.counter = -1;return;}
            switch (v.getId()) {//如果说有多个需要触发点击事件的按钮，需要判断是什么按钮，再触发该按钮的功能
                case R.id.button2://创建房间
                    monitor_all.send(""+server_protocol.SZSC_apply_create_room);
                    break;
                case R.id.button3://加入房间
                    monitor_all.send(""+server_protocol.SZSC_apply_enter_room);
                    break;
                case R.id.button4:
                    //lobby_monitor.fresh_memberlist();//monitor_all.send("4");
                    monitor_all.send(""+server_protocol.apply_refresh_online_member);
                    break;
                case R.id.button:
                    monitor_all.send("8");
                    break;
                case R.id.UNO_create_room:
                    monitor_all.send(""+server_protocol.UNO_apply_create_room);
                    break;
                case R.id.UNO_enter_room:
                    monitor_all.send(""+server_protocol.UNO_apply_enter_room);
                    break;
            }

        }
    };

    private static String log;
    public static void add_log(String message)//x显示服务器回复说明
    {
        log=message;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String history_log = log;
                android.widget.TextView netlog = (android.widget.TextView) activity.findViewById(R.id.server_message);
                log = "";
                netlog.setText(history_log);
            }
        });
    }

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
        getMenuInflater().inflate(R.menu.menu_lobby, menu);
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
