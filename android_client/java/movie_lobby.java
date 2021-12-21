package com.example.xielm.myapplication;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.AbsoluteLayout;
import android.widget.ImageView;
import android.widget.RelativeLayout;

public class movie_lobby extends Activity {

    public static android.app.Activity activity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_movie_lobby);
        activity=this;

        side_drawer();//设置侧拉框内容


        movie_monitor kk = new movie_monitor();
        kk.start();
    }
    private static void side_drawer()
    {

        android.widget.Button personal_info = (android.widget.Button)activity.findViewById(R.id.personal_info);//点击该按钮，弹出侧拉框
        final android.support.v4.widget.DrawerLayout mDrawerLayout = (android.support.v4.widget.DrawerLayout)activity.findViewById(R.id.movie_lobby);
        final android.widget.RelativeLayout mFragmentContainerView=(android.widget.RelativeLayout)activity.findViewById(R.id.left_drawer);
        personal_info.setOnClickListener(new android.view.View.OnClickListener() {
            @Override
            public void onClick(android.view.View v) {
                mDrawerLayout.openDrawer(mFragmentContainerView);// 按钮按下，将抽屉打开
            }
        });

        //设置顶部图片
        android.widget.ImageView ss=(android.widget.ImageView)activity.findViewById(R.id.user_image);
        ss.setScaleType(ImageView.ScaleType.FIT_CENTER );
        com.bumptech.glide.Glide.with(activity).load("https://gimg2.baidu.com/image_search/src=http%3A%2F%2Fwww.huiyuanjiaxiao.cn%2Fimg.php%3Fpic1.win4000.com%2Fwallpaper%2Fa%2F55349fb752abc.jpg&refer=http%3A%2F%2Fwww.huiyuanjiaxiao.cn&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg?sec=1621498288&t=3dd61e42e09634e24bea9c1f879a65f0").into(ss);

        //设置选项菜单
        android.widget.ArrayAdapter<String> arrayAdapter= new android.widget.ArrayAdapter<String> (
                activity, android.R.layout.simple_list_item_1,data);
        android.widget.ListView listView = (android.widget.ListView)activity.findViewById(R.id.left_listview);
        listView.setAdapter(arrayAdapter);
        listView.setOnItemClickListener(new android.widget.AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(android.widget.AdapterView<?> arg0, android.view.View arg1, int arg2, long arg3) {
                android.widget.Toast.makeText(activity, "这是" + data[arg2], android.widget.Toast.LENGTH_SHORT).show();
            }
        });
    }
    private static String [] data = {"个人信息","个人消息","创作者中心","设置","夜间模式","定时开关","个性装扮","帮助与反馈","关于"};


    public static android.view.View.OnClickListener listener = new android.view.View.OnClickListener() {

        @Override
        public void onClick(android.view.View v) {
            //android.widget.Toast t = android.widget.Toast.makeText(getApplicationContext(), "网络异常！", android.widget.Toast.LENGTH_SHORT);
            telecom.tsleep(500);
            movie_monitor.getin = 1;
            return;//返回
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_movie_lobby, menu);
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
