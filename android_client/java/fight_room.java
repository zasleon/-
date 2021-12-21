package com.example.xielm.myapplication;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.PopupMenu;

public class fight_room extends Activity {

    //public static boolean system_event;//如果在进行数据传输，用户其他发送行为被停止
    public static int whichcard;//全局变量，存储选择发动哪张手卡
    public static int whicheffect;//全局变量，要发动该武器的效果几
    public static int whichweapon;//全局变量，要选中第几个武器
    public static boolean use_hideeffect;//是否使用隐效果
    public static String choosesituation;//做“是否”选择的场景

    public final static int state_fight_back=5;
    public final static int state_choose_free=4;
    public final static int state_choose_muti=3;
    public final static int state_choose_one=2;
    public final static int state_normal=1;
    public static int mystate;//是否是选择卡操作

    private static android.os.Handler handler=new android.os.Handler();//处理变更UI的线程操作
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation() != android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);}//强制性变为竖屏
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fight_room);

        //system_event=true;//初始封锁用户行为，防止他乱按键发数据导致错误交流
        mystate=state_normal;
        character.ini_character();rival.ini_character();//初始化双方人物数据
        game_function.activity=this;game_function.start_service();

        android.view.WindowManager wm = (android.view.WindowManager) this.getSystemService(android.content.Context.WINDOW_SERVICE);
        android.widget.Button attack=(android.widget.Button) findViewById(R.id.attack_choice);//进行普攻
        attack.setOnClickListener(listener);
        attack.setTextSize(wm.getDefaultDisplay().getWidth() / 65);
        android.widget.Button end_turn=(android.widget.Button) findViewById(R.id.end_operation);//结束回合
        end_turn.setOnClickListener(listener);
        end_turn.setTextSize(wm.getDefaultDisplay().getWidth() / 65);
        android.widget.Button charater_info=(android.widget.Button) findViewById(R.id.battle_info);//角色信息
        charater_info.setOnClickListener(listener);
        charater_info.setTextSize(wm.getDefaultDisplay().getWidth() / 65);
        android.widget.Button launch_effect=(android.widget.Button) findViewById(R.id.launcheffect);//发动效果
        launch_effect.setOnClickListener(listener);
        launch_effect.setTextSize(wm.getDefaultDisplay().getWidth() / 65);
        android.widget.TextView kkk = (android.widget.TextView) this.findViewById(R.id.whether_your_turn);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth() / 70);
        kkk = (android.widget.TextView) this.findViewById(R.id.rival_name);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth() / 75);
        kkk = (android.widget.TextView) this.findViewById(R.id.rival_card_number);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth()/75);
        kkk = (android.widget.TextView) this.findViewById(R.id.rival_blood);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth()/45);
        kkk = (android.widget.TextView) this.findViewById(R.id.rival_attack);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth()/45);
        kkk = (android.widget.TextView) this.findViewById(R.id.my_blood);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth()/45);
        kkk = (android.widget.TextView) this.findViewById(R.id.my_attack);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth()/45);
        kkk = (android.widget.TextView) this.findViewById(R.id.F_topmiddle);
        kkk.setTextSize(wm.getDefaultDisplay().getWidth()/80);
        //show_my_card();
    }




    public static void refresh_history(String log)//（手动）更新屏幕显示历史记录信息
    {
        android.widget.TableLayout table = (android.widget.TableLayout) game_function.activity.findViewById(R.id.history);
        android.widget.TableRow row;
        android.widget.TextView log1;
        log1 = new android.widget.TextView(game_function.activity);
        row = new android.widget.TableRow(game_function.activity);
        row.setGravity(android.view.Gravity.CENTER);
        log1.setText(log);
        log1.setTextSize(20);
        row.addView(log1);
        table.addView(row, new android.widget.TableLayout.LayoutParams(android.widget.Gallery.LayoutParams.WRAP_CONTENT, android.widget.Gallery.LayoutParams.MATCH_PARENT));
        android.widget.ScrollView history_scroll = (android.widget.ScrollView)game_function.activity.findViewById(R.id.scrollViewlog);
        history_scroll.post(new Runnable() {
            public void run() {
                android.widget.ScrollView history_scroll = (android.widget.ScrollView) game_function.activity.findViewById(R.id.scrollViewlog);
                history_scroll.scrollBy(0, 10000);
            }
        });
    }


    public static void show_card_choice(android.view.View v)//点击“卡片”显示功能菜单
    {
        PopupMenu popupMenu = new PopupMenu(game_function.activity, v);
        switch(mystate) {
            case state_choose_free:
            case state_choose_muti:
            case state_choose_one:
                popupMenu.inflate(R.menu.menu_card_choose);//专门选择数量手卡,"选择"
                break;
            case state_normal:
            case state_fight_back:
                popupMenu.inflate(R.menu.menu_fight_room);//“使用”
                break;
        }

        popupMenu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                switch (menuItem.getItemId()) {
                    case R.id.use://使用手卡
                        use_hideeffect=false;
                        switch(mystate)
                        {
                            case state_normal:
                                if (character.card[whichcard - 1] >= 0 && character.card[whichcard - 1] <= 29&&!game_function.my_turn)
                                {
                                    game_function.add_log("当前不是你的回合，无法装备武器！");
                                    return true;
                                }
                                monitor_all.send("3");//我要使用手卡了
                                break;
                            case state_fight_back:
                                monitor_all.send("1");//我要使用手卡了
                                break;
                        }
                        break;
                    case R.id.use_hideeffect://使用隐效果
                        use_hideeffect=true;
                        switch(mystate)
                        {
                            case state_normal:;
                                monitor_all.send("3");//我要使用手卡了
                                break;
                            case state_fight_back:
                                monitor_all.send("1");//我要使用手卡了
                                break;
                        }
                        break;
                    //case R.id.disacard:break;
                    case R.id.choose:
                        monitor_all.send(String.valueOf(whichcard));
                        break;
                }
                return true;
            }
        });
        popupMenu.show();
    }

    public static void show_effect_choice(android.view.View v)//点击“发动效果”显示功能菜单(武器效果或人物效果或加附效果或丢弃武器)
    {
        PopupMenu popupMenu = new PopupMenu(game_function.activity, v);
        popupMenu.inflate(R.menu.menu_effect_choose);
        popupMenu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                switch (menuItem.getItemId()) {
                    case R.id.weapon_effect://refresh_history("你发动了武器效果");
                        if(!character.w[0])
                        {game_function.add_log("你当前没装备武器！");break;}
                        switch (mystate) {
                            case state_normal:
                                monitor_all.send("5");break;
                            case state_fight_back:
                                monitor_all.send("3");break;
                        }
                        break;
                    case R.id.character_effect://refresh_history("你发动了角色效果");
                        switch (mystate) {
                            case state_normal:
                                monitor_all.send("2");break;
                            case state_fight_back:
                                monitor_all.send("2");break;
                        }
                        break;
                    case R.id.buff_effect://refresh_history("你发动了加附效果");
                        switch (mystate) {
                            case state_normal:
                                monitor_all.send("8");break;
                            case state_fight_back:
                                monitor_all.send("4");break;
                        }
                    case R.id.drop_weapon:
                        if(!character.w[0])
                        {game_function.add_log("你当前没装备武器！");break;}
                        if(!game_function.my_turn)
                        {game_function.add_log("现在不是你的回合，你无法丢弃武器！");break;}
                        switch (mystate) {
                            case state_normal:
                                monitor_all.send("4");break;
                            case state_fight_back:
                                game_function.add_log("当前你处于反击状态，无法丢弃武器！");break;
                        }
                        break;
                }
                return true;
            }
        });
        popupMenu.show();
    }

    //对用户按钮产生反应
    public android.view.View.OnClickListener listener = new android.view.View.OnClickListener()
    {
        @Override
        public void onClick(android.view.View v) {
            if(v.getId()==R.id.battle_info){game_function.show_allmember_info();return;}//查看角色信息
            telecom.tsleep(50);
            switch (v.getId()) {
                case R.id.attack_choice://你按下了普攻
                    monitor_all.send("1");
                    break;//发动普攻
                case R.id.end_operation://refresh_history("你按下了结束回合");
                    switch (mystate)
                    {
                        case state_choose_free://确认选择
                            monitor_all.send("y");break;
                        case state_choose_muti://"清除原有选择"
                        case state_choose_one://取消丢卡
                        case state_fight_back://不反击
                            monitor_all.send("0");break;
                        case state_normal:
                            monitor_all.send("7");break;//回合结束
                    }
                    break;//结束回合
                case R.id.launcheffect:
                    switch (mystate)
                    {
                        case state_choose_free:
                            monitor_all.send("0");break;
                        case state_normal:
                        case state_fight_back://反击
                            show_effect_choice(v);break;//显示发动武器或个人效果或加附效果菜单
                    }
                    break;
                default:
            }
        }
    };


    @Override//防止用户退回登录界面
    public boolean dispatchKeyEvent(android.view.KeyEvent event) {
        if (event.getKeyCode() == android.view.KeyEvent.KEYCODE_BACK) {
            return true;
        } else {
            return super.dispatchKeyEvent(event);
        }
    }

    //系统自带函数
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_find_help, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.help_me) {
            game_function.show_help();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
