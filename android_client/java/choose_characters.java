package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class choose_characters extends Activity {
    private static android.os.Handler handler=new android.os.Handler();
    final client_socket cc=client_socket.getSingleIntance();
    public static android.app.Activity activity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_characters);
        activity=this;
        android.view.WindowManager wm = (android.view.WindowManager) this.getSystemService(android.content.Context.WINDOW_SERVICE);
        android.widget.Button c1=(android.widget.Button) findViewById(R.id.warrior);//选战士
        c1.setOnClickListener(listener);
        android.widget.Button c2=(android.widget.Button) findViewById(R.id.wizard);//选法师
        c2.setOnClickListener(listener);
        android.widget.TextView w1=(android.widget.TextView) findViewById(R.id.warrior_info);//选战士
        w1.setTextSize(wm.getDefaultDisplay().getWidth() / 55);
        w1.setText(game_card.search_skill(10001));
        android.widget.TextView w2=(android.widget.TextView) findViewById(R.id.wizard_info);//选战士
        w2.setTextSize(wm.getDefaultDisplay().getWidth() / 55);
        w2.setText(game_card.search_skill(10002));
        telecom.tsleep(400);
    }

    private android.view.View.OnClickListener listener = new android.view.View.OnClickListener(){
        @Override
        public void onClick(android.view.View v) {

            switch(v.getId())
            {
                case R.id.warrior:monitor_all.send("1");break;//选战士
                case R.id.wizard:monitor_all.send("2");break;//选法师
                default:break;
            }
            android.widget.Button choice = (android.widget.Button) findViewById(R.id.warrior);
            choice.setEnabled(false);
            choice = (android.widget.Button) findViewById(R.id.wizard);
            choice.setEnabled(false);
            android.widget.TextView waitress = (android.widget.TextView) findViewById(R.id.waitress);
            waitress.setText("请等待其他玩家选完角色……");
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
    public static void reopen_choice()
    {
        handler.post(new Runnable() {
            @Override
            public void run() {
                android.widget.Button choice = (android.widget.Button) activity.findViewById(R.id.warrior);
                choice.setEnabled(true);
                choice = (android.widget.Button) activity.findViewById(R.id.wizard);
                choice.setEnabled(true);
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_choose_characters, menu);
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
