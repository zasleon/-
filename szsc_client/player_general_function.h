#include "No_instruction.h"

using namespace std;



void ini_character(player* p1,int playerNo)								//�Դ�������p�ĸ���״̬���г�ʼ��
{
	p1->character_number=412;
	p1->bloodlimit=0;
	p1->blood=0;
	p1->attack=0;

	p1->player_No=playerNo;//�ڼ������
	p1->alive=true;//��Ϊ���״̬
	p1->fight_chance=0;//�չ�������0
	p1->whether_myturn=false;
	p1->un_move_able=0;//�����ж��Ļغ�
	p1->unmoveable_time=false;//��ǰ�ж��Ƿ񲻿��ж���0
	p1->selfeffect_invalid=0;//����Ч������Ч���Ļغ���0
	p1->exattack_turn=0;//�ûغ��������ӵĶ��⹥������0
	p1->exattack_time=0;//�ôβ����������ӵĶ��⹥������0

	p1->overdeath=0;//����״̬�ɳ����ֻ�����
	p1->whether_in_attack=false;//�����ڲ���״̬
	p1->whether_attack_T=false;//�Լ������ڴ���״̬
	p1->attacktime_turn=0;//�˻غ��Լ��չ��ɹ�������0
	p1->hideshield=0;////�����ʣ��غ�����0
	p1->godlaser=0;//���м���ʣ���ֻ���
	p1->godlaserdamage=0;//���м�������˺���
	p1->hurtmultiple_turn=1;//�ûغ��˺�����Ϊ1
	p1->hurtmultiple_time=1;//�ô��˺�����Ϊ1
	p1->cannot_ED=false;//�˴ι����޷���ܸ��÷�

	int i,k;
	for(i=0;i<cardlimit;i++)
		p1->c[i]=false;//�ֿ�ȫ�����

	for(i=0;i<abilitylimit;i++)
	{
		p1->a[i]=false;//����Ч��ȫ�����
	}
	
	for(i=0;i<weaponlimit;i++)
	{
		p1->w_valid[i]=true;//��������ȫ����Ϊ��Ч
		p1->w[i]=false;//����ȫ�����
		p1->exattack_weapon[i]=0;//����װ���������ӵĹ�������0
		p1->exattack_weapon_turn[i]=0;//����װ�������ûغ����ӵĹ�������0
		p1->exattack_weapon_time[i]=0;//����װ�������ôβ������ӵĹ�������0
		for(k=0;k<weaponeffectlimit;k++)
			p1->w_e[i][k]=false;//����װ��Ч�����
		p1->weaponeffect001[i]=0;//�лʽ��������鿨������Ϊ0
		p1->weaponeffect002[i]=0;//�лʽ��������鿨������Ϊ0
		p1->weaponeffect051[i]=0;//ʳ������װ��1����Ϊ0
		p1->weaponeffect052[i]=0;//ʳ������װ��2����Ϊ0
		p1->weaponeffect06[i]=0;//6�Ŷ��շ���ħ��ָʾ�����
	}

}
bool judge_bool(char* StrBuf)//�ж��ַ�����true����false
{
	if(strlen(StrBuf)==1)
	{
		if(StrBuf[0]=='1')
			return true;
		else
			return false;
	}
	//else
	//	cout<<"��֡���"<<StrBuf;

	return false;

}

void reply_i_get()//���߷�����Լ��յ������ˣ��뷢����һ��
{

	send(Client, replymessage, (1 + ::strlen(replymessage)), 0);//�ظ��Լ�ȷ�Ͽ�ʼ�������ݴ���
}




void set_property(int* k)//������������ֵ�����ݷ��������ݣ�����
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











bool control_number()//��ֹ�࿪
{
	/*
	HANDLE hObject;

	hObject = ::CreateMutex(NULL,FALSE, LPCWSTR("scsz1"));  //����һ��
    if(GetLastError() == ERROR_ALREADY_EXISTS)  //���scsz1�Ѿ���������
    {  
        CloseHandle(hObject); //���δ���ֹͣ
		hObject = ::CreateMutex(NULL,FALSE, LPCWSTR("scsz2"));  //����scsz2
		if(GetLastError() == ERROR_ALREADY_EXISTS)//���֧��˫���������⵽�������ֶ��������ˣ��˳�����
		{
			CloseHandle(hObject);
			cout<<"��Ǹ��Ϊ��ֹ�����������ɣ�һ̨�������֧��˫��!\n���½�!лл���!\n��������10����Զ��رա���\n";  
			Sleep(10000);

			return false;  
		}
		else
			*state_p=game_second;//scsz1û�����������ǵ�һ������
    }
	else
		*state_p=game_first;//scsz1û�����������ǵ�һ������
	return true;
	*/
	return true;
	
}