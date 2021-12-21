package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/1.
 */
public class game_card {
    public final static int p_name=1;//只显示名字
    public final static int p_details=2;//详细效果

    public static String catalog[][]=new String[10000][server_protocol.weaponeffectlimit];
    public static void ini_catalog()//初始化卡片目录
    {
        int count,count2;
        for(count=0;count<10000;count++)
            for(count2=0;count2<server_protocol.weaponeffectlimit;count2++)
                catalog[count][count2]="";

        //catalog[11][0]="11号卡";catalog[11][1]=
//---------------------------------------------------助攻卡---------------------------------------------------
        catalog[30][0]="决义·地割";
        catalog[30][1]="使对手-1血(对手可丢1卡躲避此伤血),若命中,则对手在此次普攻结束前不能行动\n";
        catalog[31][0]="劈夺斩";
        catalog[31][1]="此次普攻自己攻+1,自己查看 被攻者 所有手卡,并选1张加入自己手卡,若选中装备卡则直接给自己装备上\n";
        catalog[32][0]="剑飞·回斩";
        catalog[32][1]="此次普攻自己装备的所有武器无效化,普攻结束后武器重新有效化且对手受到自己当前武器数量×2的伤血\n";
        catalog[33][0]="霸地·挥刃";
        catalog[33][1]="【自己装备武器】自己抽1卡,使对手-1血（对手可丢1卡躲避此伤血）\n";
        catalog[34][0]="极寒加护";
        catalog[34][1]="此次普攻自己攻+1,普攻成功后30%使对手接下来的此回合和下回合不能行动\n";
        catalog[35][0]="雷霆加护";
        catalog[35][1]="此次普攻自己攻+2,普攻成功后50%使对手随机丢1卡\n";
        catalog[36][0]="炽焰加护";
        catalog[36][1]="此次普攻自己攻+1,普攻成功后40%使对手-1血\n";
        catalog[37][0]="流水加护";
        catalog[37][1]="此次普攻自己攻+1,普攻成功后使被对手随机丢2卡，抽2卡\n";
        catalog[38][0]="神光加护";
        catalog[38][1]="此次普攻自己攻+1,普攻成功后自己抽1卡\n";
        catalog[39][0]="冥幽加护";
        catalog[39][1]="此次普攻自己攻+2,普攻成功后30%使被普攻者装备的武器全部破坏\n";
        catalog[40][0]="神·空·斩";
        catalog[40][1]="此次普攻无法被躲避、格挡，自己总攻击力减半且变为穿攻\n";
//---------------------------------------------------助攻卡---------------------------------------------------
//---------------------------------------------------效果卡---------------------------------------------------
        catalog[60][0]="神起·焰射";
        catalog[60][1]="【自己普攻失败时】自己丢光手卡，使所有对手-1血且装备卡全被破坏\n";
        catalog[61][0]="近击·搏杀";
        catalog[61][1]="【自己参与的1次事件连锁超过4次】\n使 与自己搏斗者 伤3血(此为超速普攻),此回合被强制结束\n";
        catalog[62][0]="阴势·突进";
        catalog[62][1]="【自己回合自己未处于搏斗】\n自己指定1个对手-1血且此回合不能行动(对手可丢1卡躲避此效果),之后的此回合自己只能对被指定者进行普攻\n";
        catalog[63][0]="神空·闪降";
        catalog[63][1]="【自己回合自己未处于搏斗】\n自己抽1卡,将抽到的该卡公开,自己这回合的规则性普攻次数被重置为1\n";
        catalog[64][0]="空遁·闪回";
        catalog[64][1]="【自己即将受到普攻】\n自己抽1卡且躲避此次搏斗,自己可普攻其1次\n";
        catalog[65][0]="光斩·绝杀";
        catalog[65][1]="【自己即将受到致命伤血】\n此次伤血无效化且此回合自己攻击力×2，自己可对其进行1次普攻\n";
        catalog[66][0]="天华绝伦";
        catalog[66][1]="【此回合内自己成功普攻超过2次且自己普攻成功时】\n自己超速普攻 被攻者 4次，每次超速普攻对其造成的普攻伤血强制变为1且不会造成反伤\n";
        catalog[67][0]="神尚激光";
        catalog[67][1]="【己方回合自己未处于搏斗】\n自己丢光手卡,此回合之后的3轮回内自己若受到致命伤血，使自己受到致命伤血者 将受到之前自己丢光手卡数值×2的伤血\n";
        catalog[68][0]="轰炸星球";
        catalog[68][1]="【自己回合自己未处于搏斗】\n破坏场上所有场景卡和结界卡,每破坏1张,使 放置该结界或场景者 -2血\n";
        catalog[69][0]="最后的指望";
        catalog[69][1]="【自己濒死时】\n自己指定1人,将自己全部手卡、装备的武器全部加入他手卡\n";
        catalog[70][0]="双臂剑";
        catalog[70][1]="【自己回合自己未处于搏斗】\n自己血-3,将卡堆最上方的2张武器卡装备上\n";
        catalog[71][0]="离心续";
        catalog[71][1]="【自己普攻成功时】\n自己再对其进行1次普攻,此次普攻造成的普攻伤血翻倍\n";
        catalog[72][0]="混虚·太清";
        catalog[72][1]="【自己即将受到普攻时】\n自己丢光手卡、躲避本次普攻,使对己普攻者 血量减半\n";
        catalog[73][0]="幻影剑";
        catalog[73][1]="【自己武器栏没满】\n此卡当作 攻+2 的武器给自己装备,此武器在此回合结束时自动被破坏\n";
        catalog[74][0]="冷现·巨剑";
        catalog[74][1]="【自己受到效果伤血时】\n自己丢1手卡中的武器卡,指定1人-2血\n";
        catalog[75][0]="隐天盾";
        catalog[75][1]="此卡发动的回合及发动后2轮回内,自己可丢1卡 无效化1次对己的非自主效果伤血 或 格挡1次普攻\n";
        catalog[76][0]="破局·神击";
        catalog[76][1]="【自己普攻失败时】\n自己可丢任意数量的手卡,使 被攻者 受到丢卡数量的伤血.自己普攻 使自己普攻失败者 1次,且此次普攻内 被攻者 无法行动\n";
        catalog[77][0]="旋地·回击";
        catalog[77][1]="【自己受到普攻时】\n自己抽1卡,自己可普攻 发动普攻者 1次,本次攻击自己攻+1\n";

        //---------------------------------------------------效果卡---------------------------------------------------
        //---------------------------------------------------隐效果---------------------------------------------------

        catalog[400][0]="连攻";
        catalog[400][1]="【自己普攻成功时或自己回合自己未处于搏斗状态可发动】\n自己发动一次普攻";

        catalog[401][0]="闪";
        catalog[401][1]="【自己被普攻时】\n躲避那次普攻";

        catalog[402][0]="+4血";
        catalog[402][1]="指定1人回复4血";

        catalog[403][0]="暗枪";
        catalog[403][1]="【自己参与搏斗时】\n使 与自己搏斗者 伤1血";

        catalog[404][0]="稻草人";
        catalog[404][1]="【自己被普攻时】\n躲避那次普攻，普攻者 武器全被破坏";

        catalog[405][0]="憎恨";
        catalog[405][1]="(1)指定1人的人物效果一轮回内无法发动\n";
        catalog[405][2]="(2)破坏场上一张 装备卡 或 场景卡 或 结界卡\n";
        catalog[405][3]="(3)非人物卡 发动无效并破坏\n";
        catalog[405][4]="(4)指定1人，消除其身上所有buff";

        catalog[406][0]="极天束";
        catalog[406][1]="【自己普攻成功时】\n使 被普攻者 伤2血";

        catalog[407][0]="蓝爆冰晶";
        catalog[407][1]="【己方回合自己未处于搏斗】\n指定1人3次50%伤1血，若全中则其装备全部被破坏";

        //---------------------------------------------------隐效果---------------------------------------------------
        //---------------------------------------------------武器卡---------------------------------------------------

        catalog[0][0]="承皇剑";
        catalog[0][1]="攻+1\n";
        catalog[0][2]="(1)1回合限1次，自己可丢1手卡，抽1卡\n";
        catalog[0][3]="(2)自己可挡4次普攻，抵挡第4次普攻后此卡被破坏\n";
        catalog[0][4]="(3)【自己回合可发动】自己主动将此卡破坏，指定1人-1血\n";

        catalog[1][0]="疾风刃";
        catalog[1][1]="攻+1\n";
        catalog[1][2]="(1)1回合限2次，自己可丢1手卡，自己此回合攻+1\n";
        catalog[1][3]="(2)【自己回合可发动】自己主动将此卡破坏，指定1人-1血\n";

        catalog[2][0]="长者之镰";
        catalog[2][1]="攻+2\n";
        catalog[2][2]="(1)自己每使1人伤1血，自己+1血\n";
        catalog[2][3]="(2)【自己回合可发动】自己主动将此卡破坏，指定1人+3血\n";

        catalog[3][0]="噬剑之剑";
        catalog[3][1]="攻+1\n";
        catalog[3][2]="(1)自己每次与人搏斗过后限1次，随机破坏 与自己搏斗者 装备的1张装备卡，破坏后这把武器攻+1\n";
        catalog[3][3]="(2)【自己回合可发动】自己主动将此卡破坏，指定1人-1血\n";

        catalog[4][0]="黑曜剑";
        catalog[4][1]="攻+2\n";
        catalog[4][2]="(1)此卡被装备上时自己-3血\n";
        catalog[4][3]="(2)此次游戏限1次可发动，发动回合过后，自己持续不死2轮回\n";
        catalog[4][4]="(3)【自己回合可发动】自己主动将此卡破坏，指定1人50%-2血\n";

        catalog[5][0]="食腐再生装置";
        catalog[5][1]="(1)自己每抽1卡，自己血+1\n";
        catalog[5][2]="(2)1回合限1次，自己可丢1手卡，血+1\n";
        catalog[5][3]="(3)自己回合限1次，自己可额外抽1卡\n";

        catalog[6][0]="恩空法棒";
        catalog[6][1]="(1)此武器初始有2魔力指示物\n";
        catalog[6][2]="(2)己方每发动1手卡，此武器魔力指示物+1\n";
        catalog[6][3]="(3)【自己回合可发动】自己消耗2魔力指示物使1人-1血\n";

        catalog[7][0]="骷髅诡面";
        catalog[7][1]="(1)自己不受任何丢卡和手卡公开的效果影响\n";
        catalog[7][2]="(2)【场上除己外1人死亡时】自己血量回满\n";
        catalog[7][3]="(3)自己不受任何自主伤血效果影响\n";

        catalog[8][0]="破败披风";
        catalog[8][1]="(1)自己不受“无法行动”效果影响\n";
        catalog[8][2]="(2)【自己受到普攻时可发动】自己主动将此卡破坏，自己抵挡此次普攻\n";
        //catalog[8][3]="(3)自己不受装备卡被破坏效果影响\n";

        catalog[9][0]="冠阳剜月斧";
        catalog[9][1]="攻+2\n";
        catalog[9][2]="(1)【自己每次主动普攻时】所有人丢3卡，抽3卡，卡堆最上方3张卡送入废卡区\n";
        catalog[9][3]="(2)【自己回合可发动】自己主动将此卡破坏，场上所有人随机丢5卡，抽5卡\n";

        catalog[10][0]="幻影剑";
        catalog[10][1]="攻+2\n";
        //---------------------------------------------------武器卡---------------------------------------------------
    }
    public static int get_hideeffect(int number)//根据卡片获取其隐效果
    {
        int attack_plus	=	400;//400 连攻
        int escape		=	401;//401 闪
        int recover		=	402;//402 +4血
        int darkgun		=	403;//403 暗枪
        int scarecrow	=	404;//404 稻草人
        int hatred		=	405;//405 憎恨
        int skylaser	=	406;//406 极天束
        int bluecrystal	=	407;//407 蓝爆冰晶

        switch(number)
        {
            case 0:return hatred;//承皇剑
            case 1:return bluecrystal;//疾风刃
            case 2:return skylaser;//长者之镰
            case 3:return escape;//噬剑之剑
            case 4:return attack_plus;//黑曜剑
            case 5:return hatred;//食腐再生装置
            case 6:return scarecrow;//恩空法棒
            case 7:return escape;//骷髅诡面
            case 8:return skylaser;//破败披风
            case 9:return recover;//冠阳剜月斧
            case 10:;//
            case 11:;//
            case 12:;//
            case 13:;//
            case 14:;//
            case 15:;//
            case 16:;//
            case 17:;//
            case 18:;//
            case 19:;//
            case 20:;//
            case 21:;//
            case 22:;//
            case 23:;//
            case 24:;//
            case 25:;//
            case 26:;//
            case 27:;//
            case 28:;//
            case 29:break;//
            case 30:return escape;//决义·地割
            case 31:return recover;//劈夺斩
            case 32:return escape;//剑飞·回斩
            case 33:return scarecrow;//霸地·挥刃
            case 34:return escape;//极寒加护
            case 35:return recover;//雷霆加护
            case 36:return escape;//炽焰加护
            case 37:return hatred;//流水加护
            case 38:return bluecrystal;//神光加护
            case 39:return scarecrow;//冥幽加护
            case 40:return recover;//神·空·斩
            case 41:;//
            case 42:;//
            case 43:;//
            case 44:;//
            case 45:;//
            case 46:;//
            case 47:;//
            case 48:;//
            case 49:;//
            case 50:;//
            case 51:;//
            case 52:;//
            case 53:;//
            case 54:;//
            case 55:;//
            case 56:;//
            case 57:;//
            case 58:;//
            case 59:;//
            case 60:return bluecrystal;//神起·焰射
            case 61:return hatred;//近击·搏杀
            case 62:return escape;//阴势·突进
            case 63:return skylaser;//神空·闪降
            case 64:return attack_plus;//空遁·闪回
            case 65:return attack_plus;//光斩·绝杀
            case 66:return darkgun;//天华绝伦
            case 67:return attack_plus;//神尚激光
            case 68:return recover;//轰炸星球
            case 69:return attack_plus;//最后的指望
            case 70:return scarecrow;//双臂剑
            case 71:return escape;//离心续
            case 72:return attack_plus;//混虚·太清
            case 73:return recover;//幻影剑
            case 74:return attack_plus;//冷现·巨剑
            case 75:return attack_plus;//隐天盾
            case 76:return escape;//破局·神击
            case 77:return skylaser;//旋地·回击
            case 78:;
            case 79:;
            case 80:;
            case 81:;
            case 82:;
            case 83:;
            case 84:;
            case 85:;
            case 86:;
            case 87:;
            case 88:;
            case 89:;
            case 90:;
            case 91:;
            case 92:;
            case 93:;
            case 94:;
            case 95:;
            case 96:;
            case 97:;
            case 98:;
            case 99:;
            case 100:;


        }
        return 0;
    }




    public static String search_skill(int number)//查询编号代表的武器/人物效果
    {
        int first,second;
        if(number<10000)//确认是武器效果号
        {
            first=number/10%10;//取十位
            second=number%10;//取个位
            if(catalog[first][second].length()<=0)
            {return "奇怪的错误！！！"+number;}
            else
                return catalog[first][second];
        }
        switch(number)
        {
            case 10000:return "这是一棵纹丝不动的、用来练习的 木头";
            case 10001:return "【自己每次主动普攻时】此次普攻结束前自己攻+2";
            case 10002:return "【自己回合自己每次成功发动手卡时】自己可指定1人-1血";
        }

        return "查询编号代表效果异常!\n"+number;
    }
    public static String search_character(int number)//查询编号代表的角色名称
    {

        switch(number)
        {
            case 0:return "木桩";
            case 1:return "战士";
            case 2:return "法士";
        }

        return "号码错误!";
    }
    public static String search_card(int situation,int number)                     //查询编号代表的卡片效果
    {
        String result="";
        int count;
        switch(situation)
        {
            case p_name:
                if(catalog[number][0].length()>0)
                    return catalog[number][0];
                break;
            case p_details:
                for(count=1;count<server_protocol.weaponeffectlimit;count++)
                    if(catalog[number][count].length()>0)
                        result=result+catalog[number][count];
                    else
                        break;
                return result;
        }

        return "莫名其妙号卡"+number;
    }

    public static int set_card_background(int number)//判断是什么类型卡，返回该种类型的背景颜色
    {
        if(number<30&&number>=0)//武器
            return android.graphics.Color.rgb(0, 255, 0);//大绿
        if(number>=30&&number<60)//助攻卡
            return android.graphics.Color.rgb(200, 0, 0);//大红
        return android.graphics.Color.rgb(255, 255, 255);//大白
    }
    public static int set_card_text(int number)//返回该种类型的字体颜色
    {
        if(number<30&&number>=0)//武器
            return android.graphics.Color.rgb(0, 0, 255);//蓝
        if(number>=30&&number<60)//助攻卡
            return android.graphics.Color.rgb(0, 220, 200);//白
        return android.graphics.Color.rgb(0, 0, 0);//黑
    }
    public static int set_card_broad(int number)//返回边框颜色
    {
        if(number<30&&number>=0)//武器
            return android.graphics.Color.rgb(255, 255, 255);//白
        if(number>=30&&number<60)//助攻卡
            return android.graphics.Color.rgb(0, 255, 255);//淡蓝
        return android.graphics.Color.rgb(0, 0, 0);//黑
    }

}
