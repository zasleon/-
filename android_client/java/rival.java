package com.example.xielm.myapplication;

/**
 * Created by Xielm on 2021/4/1.
 */
public class rival {
    public final static int weaponlimit = 2;//武器上限为2
    public final static int cardlimit = 10;//手卡上限为10
    public final static int decklimit = 8;//预设卡组数量为8张
    public final static int abilitylimit = 4;//自身能力最多四个
    public final static int weaponeffectlimit = 5;//携带武器效果最多15个
    public final static int garbagelimit = 1000;//废卡区大小

    public static int character_number;//角色序号
    public static int player_No;//第几个玩家
    public static Boolean whether_myturn;//是否是当前角色的回合？是的为1，不是为0
    public static Boolean alive;//是否存活,true为活，false为死
    public static float bloodlimit;//血量上限
    public static float blood;//当前血量
    public static float attack;

    public static Boolean a[] = new Boolean[abilitylimit];
    public static int ability[] = new int[abilitylimit];//自身能力最多四个

    public static Boolean c[] = new Boolean[cardlimit];
    public static int card[] = new int[cardlimit];//拥有手卡

    public static Boolean w_valid[] = new Boolean[weaponlimit];//武器暂时性有效无效
    public static Boolean w[] = new Boolean[weaponlimit];
    public static int weapon[] = new int[weaponlimit];//装备槽内分别是什么武器，以武器号表明
    public static float exattack_weapon[] = new float[weaponlimit];//第几格武器槽增加的攻击力
    public static float exattack_weapon_turn[] = new float[weaponlimit];//第几格武器槽该回合增加的攻击力
    public static float exattack_weapon_time[] = new float[weaponlimit];//第几格武器槽该次搏斗增加的攻击力

    public static Boolean w_e[][] = new Boolean[weaponlimit][weaponeffectlimit];
    public static int weapon_effect[][] = new int[weaponlimit][weaponeffectlimit];//装备武器后拥有的效果

    //int storage_weapon[10];//破败披风效果专属，统计自己有多少把武器，可以不停与装备武器切换，即披风内存放了大量武器，但只能抽出其中两个进行使用【有待设计】
    //int speed;//人物速度【有待设计】
    //int		fight_chance;//自己当前拥有的普攻次数
    //int		whether_in_attack;//是否处于搏斗中
    //bool	whether_attack_T;//是否处于穿攻状态
    //bool	cannot_ED;//自己此次普攻无法躲避和格挡

    public static int		un_move_able;//不可行动的回合
    public static boolean	unmoveable_time;//在某行动内不可行动
    //int		selfeffect_invalid;//自身效果被无效化的回合

    public static int hideshield;            //隐天盾剩余回合数
    //int		godlaser;			//神尚激光剩余轮回数
    //int		godlaserdamage;		//神尚激光伤害数值
    public static int overdeath;            //持续不死状态的轮回数(黑曜剑)

    //int		attacktime_turn;	//此回合自己普攻成功次数

    //float	hurtmultiple_turn;	//该回合自己造成的普攻伤血倍率
    //float	hurtmultiple_time;	//该次行动自己造成的普攻伤血倍率

    //int		weaponeffect001[weaponlimit];//0号（承皇剑）武器效果1剩余次数
    //int		weaponeffect002[weaponlimit];//0号（承皇剑）武器效果2剩余次数
    //int		weaponeffect051[weaponlimit];//5号食腐再生装置1效果剩余次数
    //int		weaponeffect052[weaponlimit];//5号食腐再生装置2效果剩余次数
    public static int weaponeffect06[] = new int[weaponlimit];//6号恩空法棒魔力指示物个数

    public static void ini_card()//清空手卡
    {
        int count;
        for(count=0;count<character.cardlimit;count++) {
            c[count] = false;
            card[count]=-1;
        }

    }
    public static void ini_weapon() //清空武器
    {
        int i,k;
        for (i = 0; i < weaponlimit; i++) {
            w_valid[i] = true;//个人武器全部置为有效
            w[i] = false;//武器全部清空
            weapon[i] = -1;
            exattack_weapon[i] = 0;//所有装备武器增加的攻击力置0
            exattack_weapon_turn[i] = 0;//所有装备武器该回合增加的攻击力置0
            exattack_weapon_time[i] = 0;//所有装备武器该次搏斗增加的攻击力置0
            for (k = 0; k < weaponeffectlimit; k++) {
                w_e[i][k] = false;//所有装备效果清空
                weapon_effect[i][k] = -1;
            }
            weaponeffect06[i] = 0;//6号恩空法棒魔力指示物个数
        }

    }

    public static void ini_character() {
        player_No = 0;//第几个玩家
        alive = true;//置为存活状态

        whether_myturn = false;


        overdeath = 0;//不死状态可持续轮回数量
        un_move_able=0;
        unmoveable_time=false;

        hideshield = 0;////隐天盾剩余回合数置0
        ini_weapon();

        int i, k;
        for (i = 0; i < cardlimit; i++)
            c[i] = false;//手卡全部清空

        for (i = 0; i < abilitylimit; i++) {
            a[i] = false;//个人效果全部清空
            ability[i] = -1;
        }
    }
}
