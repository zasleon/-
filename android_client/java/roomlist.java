package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class roomlist extends Activity {
    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    public static int count;
    public final static int idconfig=1000;//设置动态按钮的序号
    public static android.app.Activity activity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_roomlist);
        count=0;
        activity=this;
        android.widget.Button leave_room=(android.widget.Button) findViewById(R.id.button7);
        leave_room.setEnabled(false);
        leave_room.setOnClickListener(listener);
    }

    public static android.view.View.OnClickListener listener = new android.view.View.OnClickListener() {

        @Override
        public void onClick(android.view.View v) {
            //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);

            switch (v.getId()) {
                case R.id.button7:
                    monitor_all.send("0");
                    return;
            }
            android.widget.Button enter_room = (android.widget.Button) activity.findViewById(v.getId());

            monitor_all.send(
                    String.valueOf(v.getId() - idconfig)//发送自己想进的房间号
            );
            //enter_room.setEnabled(false);
            return;//返回
        }
    };

    private static String log;
    public static void add_log(String message)
    {
        log=message;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String history_log = log;
                android.widget.TextView netlog = (android.widget.TextView) activity.findViewById(R.id.roomlist_tips);
                log = "";
                netlog.setText(history_log);
            }
        });
    }

    public static void open_roomlist_leave()//开放离开选择进入房间按钮
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.widget.Button leave_room = (android.widget.Button) roomlist.activity.findViewById(R.id.button7);
                leave_room.setEnabled(true);
            }});
    }



    @Override//防止用户退回登录界面
    public boolean dispatchKeyEvent(android.view.KeyEvent event) {
        if (event.getKeyCode() == android.view.KeyEvent.KEYCODE_BACK ) {
            return true;
        } else {
            return super.dispatchKeyEvent(event);
        }
    }


}
