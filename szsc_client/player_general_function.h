#include "No_instruction.h"

using namespace std;



void ini_character(player* p1,int playerNo)								//对传入的玩家p的各种状态进行初始化
{
	p1->character_number=412;
	p1->bloodlimit=0;
	p1->blood=0;
	p1->attack=0;

	p1->player_No=playerNo;//第几个玩家
	p1->alive=true;//置为存活状态
	p1->fight_chance=0;//普攻次数置0
	p1->whether_myturn=false;
	p1->un_move_able=0;//不可行动的回合
	p1->unmoveable_time=false;//当前行动是否不可行动置0
	p1->selfeffect_invalid=0;//自身效果被无效化的回合置0
	p1->exattack_turn=0;//该回合自身增加的额外攻击力置0
	p1->exattack_time=0;//该次搏斗自身增加的额外攻击力置0

	p1->overdeath=0;//不死状态可持续轮回数量
	p1->whether_in_attack=false;//不处于搏斗状态
	p1->whether_attack_T=false;//自己不处于穿攻状态
	p1->attacktime_turn=0;//此回合自己普攻成功次数置0
	p1->hideshield=0;////隐天盾剩余回合数置0
	p1->godlaser=0;//神尚激光剩余轮回数
	p1->godlaserdamage=0;//神尚激光造成伤害数
	p1->hurtmultiple_turn=1;//该回合伤害倍率为1
	p1->hurtmultiple_time=1;//该次伤害倍率为1
	p1->cannot_ED=false;//此次攻击无法躲避格挡置否

	int i,k;
	for(i=0;i<cardlimit;i++)
		p1->c[i]=false;//手卡全部清空

	for(i=0;i<abilitylimit;i++)
	{
		p1->a[i]=false;//个人效果全部清空
	}
	
	for(i=0;i<weaponlimit;i++)
	{
		p1->w_valid[i]=true;//个人武器全部置为有效
		p1->w[i]=false;//武器全部清空
		p1->exattack_weapon[i]=0;//所有装备武器增加的攻击力置0
		p1->exattack_weapon_turn[i]=0;//所有装备武器该回合增加的攻击力置0
		p1->exattack_weapon_time[i]=0;//所有装备武器该次搏斗增加的攻击力置0
		for(k=0;k<weaponeffectlimit;k++)
			p1->w_e[i][k]=false;//所有装备效果清空
		p1->weaponeffect001[i]=0;//承皇剑“丢、抽卡”机会为0
		p1->weaponeffect002[i]=0;//承皇剑“丢、抽卡”机会为0
		p1->weaponeffect051[i]=0;//食腐再生装置1机会为0
		p1->weaponeffect052[i]=0;//食腐再生装置2机会为0
		p1->weaponeffect06[i]=0;//6号恩空法棒魔力指示物个数
	}

}
bool judge_bool(char* StrBuf)//判断字符串是true还是false
{
	if(strlen(StrBuf)==1)
	{
		if(StrBuf[0]=='1')
			return true;
		else
			return false;
	}
	//else
	//	cout<<"奇怪……"<<StrBuf;

	return false;

}

void reply_i_get()//告诉服务端自己收到数据了，请发送下一个
{

	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);//回复自己确认开始接受数据传输
}




void set_property(int* k)//设置人物数据值，根据发来的数据，赋予
{
	if(0>=recv(Client, getmessage, MAX_PATH, 0))return;
	*k=atoi(getmessage);
	memset(getmessage,0,sizeof(getmessage));
	Sleep(10);
	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
}
void set_property(bool* k)
{
	if(0>=recv(Client, getmessage, MAX_PATH, 0))return;
	*k=judge_bool(getmessage);
	memset(getmessage,0,sizeof(getmessage));
	Sleep(10);
	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
}
void set_property(float* k)
{
	if(0>=recv(Client, getmessage, MAX_PATH, 0))return;

	*k=atof(getmessage);
	memset(getmessage,0,sizeof(getmessage));
	Sleep(10);
	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
	/*
	int d,f;
	d=atoi(getmessage);
	memset(getmessage,0,sizeof(getmessage));
	Sleep(10);
	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
	if(0>=recv(Client, getmessage, MAX_PATH, 0))return;
	f=atoi(getmessage);
	memset(getmessage,0,sizeof(getmessage));
	Sleep(10);
	*k=d+f/100;
	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);
*/	
  }











bool control_number()//防止多开
{
	/*
	HANDLE hObject;

	hObject = ::CreateMutex(NULL,FALSE, LPCWSTR("scsz1"));  //创建一次
    if(GetLastError() == ERROR_ALREADY_EXISTS)  //如果scsz1已经被创建了
    {  
        CloseHandle(hObject); //本次创建停止
		hObject = ::CreateMutex(NULL,FALSE, LPCWSTR("scsz2"));  //创建scsz2
		if(GetLastError() == ERROR_ALREADY_EXISTS)//最多支持双开，如果检测到两个名字都创建过了，退出程序
		{
			CloseHandle(hObject);
			cout<<"抱歉，为防止服务器超负荷，一台电脑最多支持双开!\n请谅解!谢谢配合!\n本程序将于10秒后自动关闭……\n";  
			Sleep(10000);

			return false;  
		}
		else
			*state_p=game_second;//scsz1没被创建，这是第一个程序
    }
	else
		*state_p=game_first;//scsz1没被创建，这是第一个程序
	return true;
	*/
	return true;
	
}