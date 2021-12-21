package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class UNO_roomlist extends Activity {
    public static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    public static int count;

    public static android.app.Activity activity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_uno_roomlist);
        count=0;
        activity=this;
        lobby.state = lobby.in_UNO_roomlist;
        UNO_room_service kk=new UNO_room_service();kk.start();


        android.widget.Button leave_room=(android.widget.Button) findViewById(R.id.UNO_exit_roomlist);
        leave_room.setOnClickListener(listener);
    }

    //UNO roomlist展示
    private static String UNO_message;
    public static void UNO_roomlist_set_button(String log)//roomlist动态显示UI,该函数若放在UNO_room_service里无法更新
    {
        UNO_message=log;
        handler.post(new Runnable() {
            @Override
            public void run() {
                String message1 = UNO_message;

                //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
                android.widget.LinearLayout layout2 = (android.widget.LinearLayout) UNO_roomlist.activity.findViewById(R.id.UNO_roomlist_tablelayout);

                android.widget.TableRow row;
                android.widget.TextView door_number1;
                android.widget.Button door1;

                int count2, doornumber;

                row = new android.widget.TableRow(UNO_roomlist.activity);
                row.setGravity(android.view.Gravity.CENTER);
                door_number1 = new android.widget.TextView(UNO_roomlist.activity);//动态新建一个文字行
                door1 = new android.widget.Button(UNO_roomlist.activity);//动态新建一个按钮
                doornumber = 0;
                for (count2 = 0; message1.charAt(count2) != '\t'; count2++)//几号房间
                {
                    door_number1.setText(door_number1.getText() + String.valueOf(UNO_message.charAt(count2)));

                }

                doornumber = Integer.parseInt(String.valueOf(door_number1.getText()));

                door1.setId(UNO_room_service.idconfig + doornumber);

                door_number1.setText(door_number1.getText() + "号房间  ");
                door1.setText("进入");
                door1.setOnClickListener(UNO_roomlist.listener);


                for (; message1.charAt(count2) == '\t'; count2++) {
                }//跳过中间的\t

                door_number1.setText(door_number1.getText() + "【房主】");
                for (; message1.charAt(count2) != '\t'; count2++) //获取姓名
                    door_number1.setText(door_number1.getText() + String.valueOf(UNO_message.charAt(count2)));

                for (; message1.charAt(count2) == '\t'; count2++) {
                }//跳过中间的\t
                door_number1.setText(door_number1.getText() + "【房内人数】");

                //for (; message1.charAt(count2) != '\n'; count2++) //无法获取\n
                for (; message1.length() > count2; count2++) //获取人数
                    door_number1.setText(door_number1.getText() + String.valueOf(UNO_message.charAt(count2)));

                row.addView(door_number1);
                row.addView(door1);
                layout2.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.WRAP_CONTENT));

            }
        });
    }


    public static android.view.View.OnClickListener listener = new android.view.View.OnClickListener() {

        @Override
        public void onClick(android.view.View v) {
            //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);

            switch (v.getId()) {
                case R.id.UNO_exit_roomlist:
                    monitor_all.send(""+server_protocol.UNO_apply_exit_roomlist);
                    return;
            }
			//执行到这里说明点击了不是“返回大厅”的按钮
            android.widget.Button enter_room = (android.widget.Button) activity.findViewById(v.getId());

            monitor_all.send(
                    String.valueOf(v.getId() - UNO_room_service.idconfig)//发送自己想进的房间号
            );
            //enter_room.setEnabled(false);
            return;//返回
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
