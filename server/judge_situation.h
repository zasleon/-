#ifndef INCLUDE_judge_situation_H
#define INCLUDE_judge_situation_H


#include "deck_card.h"


//���choice�����Ƿ��������������򷵻�true�����򷵻�false
bool judge_assist(int choice)
{
	if(choice>=30&&choice<60)return true;

	return false;
}
bool judge_weapon(int choice)
{
	if(choice>=0&&choice<30)return true;

	return false;
}
bool judge_weapon_effect(client_member* c1,int card_No,int situation,int* locktime,int whichweapon,fight_room* this_room)
{
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };

	switch(card_No)
	{
	case 1002://1�غ���1�Σ��Լ��ɶ�1�ֿ�����1�����лʽ�1��
			if(c1->p1->whether_in_attack>0){confirm_send_success(c1,"���ڲ�����!�����Ϸ�������!����ʧ��!\n");return false;}
			if(c1->p1->weaponeffect001[whichweapon]!=0)//����ûغ�Ч������ʣ��ʹ�ô���
			{
				if(!auto_judge(c1,i_will_discard_card_A,this_room))//���ûװ������������Ч��
				{
					if(c1->p1->c[0]==false){confirm_send_success(c1,"�ֿ��������٣��޷�������Ч��!\n");return false;}
				}
				return true;	
		   }
		   else
				{confirm_send_success(c1,"�ûغϵĴ�����Ч������������!\n");return false;}
	case 1003://�Լ��ɵ�4���չ����ֵ���4���չ���˿����ƻ����лʽ�2��
			if(situation!=i_will_be_attacked){confirm_send_success(c1,"�Լ�û�ܵ��չ�!\n");return false;}
			if(c1->p1->cannot_ED){confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return false;}
			return true;
 
	case 1004://���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����2Ѫ���лʽ�3��
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�����Ϸ�������!�������Լ��غϻ��Լ����ڲ���״̬�У�\n");return false;}
			return true;
	case 1012:  //"1�غ���2�Σ��Լ��ɶ�1�ֿ����Լ��˻غϹ�+1";������1
			if(c1->p1->exattack_weapon_turn[whichweapon]>=2)//���˵��λ�õļ����е�����Ч��������ʧ��
				{confirm_send_success(c1,"�ûغϵĴ�����Ч������������!");return false;}
			return true;
				
	case 1013:  //"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����1Ѫ";������2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�����Ϸ�������!�������Լ��غϻ��Լ����ڲ���״̬�У�\n");return false;}
			return true;
		
	case 1022:  ;//"�Լ�ÿʹ1����1Ѫ���Լ�+1Ѫ";����֮��1
			{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}

	case 1023:  //���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��+3Ѫ";����֮��2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�����Ϸ�������!�������Լ��غϻ��Լ����ڲ���״̬�У�\n");return false;}
			return true;
	case 1032:  //"�Լ�ÿ�����˲���������1�Σ�����ƻ� ���Լ������� װ����1��װ����";//�ɽ�֮��1
			{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}

	case 1033:  ;//"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����2Ѫ";//�ɽ�֮��2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�����Ϸ�������!�������Լ��غϻ��Լ����ڲ���״̬�У�\n");return false;}
			return true;

	case 1043:  //"�˴���Ϸ��1�οɷ����������غϹ����Լ���������2�ֻ�";//���׽�1
			return true;//��Ч�����������������Լ�Ѫ����0ʱ�Զ�����

	case 1044: ;//"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��50%��3Ѫ";//���׽�2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�����Ϸ�������!�������Լ��غϻ��Լ����ڲ���״̬�У�\n");return false;}	
			return true;

	case 1051: //"�Լ�ÿ��1�����Լ�Ѫ+1";//ʳ������װ��1
			{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}

	case 1052: //1�غ���1�Σ��Լ��ɶ�1�ֿ���Ѫ+1;//ʳ������װ��2	
			if(c1->p1->weaponeffect051[whichweapon]<=0){confirm_send_success(c1,"�ûغϵĴ�����Ч������������!\n");return false;}	
			return true;

	case 1053: //"�Լ��غ���1�Σ��Լ��ɶ����1��";//ʳ������װ��3
			if(c1->p1->whether_myturn!=true){confirm_send_success(c1,"�����Լ��Ļغ�!�����Ϸ�������!����ʧ��!\n");return false;}
			if(c1->p1->weaponeffect052[whichweapon]<=0){confirm_send_success(c1,"����������!����ʧ��!\n");return false;}
			return true;

	case 1061: ;//"�˿���װ����ʱ���˿�����4ħ��ָʾ��";//���շ���1
			{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}

	case 1062: ;//"����ÿ����1�ֿ������˿�����1ħ��ָʾ��";//���շ���2
			{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}

	case 1063: ;//"���Լ��غϿɷ������Լ�����2ħ��ָʾ��ʹ1����1Ѫ";//���շ���3
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�������Լ��غϻ��Լ����ڲ���״̬�У������Ϸ�������!����ʧ��!\n");return false;}
			if(c1->p1->weaponeffect06[whichweapon]<2){confirm_send_success(c1,"ħ��ָʾ�����!����ʧ��!\n");return false;}
			return true;

	case 1071: //"�Լ������κζ������ֿ�������Ч��Ӱ��";//���ù���1
		{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}
	case 1072: //"�����ϳ�����1������ʱ���Լ�Ѫ������";//���ù���2
		{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}
	case 1073: //"�Լ������κ�������ѪЧ��Ӱ��";//���ù���3
		{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}
	case 1081: //"�Լ���װ����������װ����"; //�ư�����1
		{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}
	case 1082: //"���Լ��ܵ��չ�ʱ�ɷ������Լ��������˿��ƻ����Լ��ֵ��˴��չ�";//�ư�����2
		if(c1->p1->cannot_ED){confirm_send_success(c1,"�Լ��޷���ܺ͸�!�����Ϸ�������!\n");return false;}
		return true;

	case 1092: //"���Լ�ÿ�չ�1�Ρ������˶�3�ֿ�����3�����������Ϸ�3�ſ�����Ͽ���";//�������¸�1
		{confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;}

	case 1093: ;//"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����2Ѫ,���������˶�5�ֿ�����5��";//�������¸�2
			if(c1->p1->whether_myturn!=true||c1->p1->whether_in_attack!=false)
				{confirm_send_success(c1,"�����Ϸ��������������Լ��غϻ��Լ����ڲ���״̬�У�!����ʧ��!\n");return false;}
			return true;

	}
	cout<<"����Ч��û�����ɹ�!card_No="<<card_No;	
	return false;
}

bool judge_card(client_member* c1,int choice,int situation,int* locktime)//situation��������������lock_time��������,������Ϸ�������ֱ��ʹ��
{
	if(choice>=0&&choice<=29)
	{
		if(c1->device==win_console)
			confirm_send_success(c1,"�ÿ�Ϊ�����������Լ��غ�ѡ��'3.װ������'�ſɷ���!\n");
		else
			confirm_send_success(c1,"�ÿ�Ϊ�����������Լ��غ�ѡ��ſ�װ��!\n");
		return false;
	}

	if(situation==i_will_attack&&choice>=30&&choice<60)//�������Ϊ����Ҫ�����ˡ������Է���������
	{
		if(choice==33&&c1->p1->w[0]==false)//�Եء����С��Լ�װ������ʱ�ſɷ�����
		{confirm_send_success(c1,"����������������������(��ûװ������)!\n");return false;}
		return true;//����������Ӧ�ö��ܷ���
	}

	if(choice>=30&&choice<=59){confirm_send_success(c1,"�ÿ�Ϊ�����������Լ��չ�����ʱ�ſɷ���!\n");return false;}

	int count;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	
	switch(choice)
	{
//--------------------------------------------------------��Ч��----------------------------------------
	case 400://���������Լ��չ��ɹ�ʱ���Լ��غ��Լ�δ���ڲ���״̬�ɷ������Լ�����һ���չ�
		if(situation==i_attack||situation==i_failed_attack_E||situation==i_failed_attack_D||situation==i_failed_attack_W)//�Լ��չ��ɹ�
			return true;
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)//�Լ��غ��Լ�δ���ڲ���״̬
			return true;
		return false;
	case 401://��
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return false;}
		if(situation==i_will_be_attacked)return true;
		return false;
	case 402://+4Ѫ
		return true;
	case 403://��ǹ
		if(c1->p1->whether_in_attack)
			return true;
		return false;
	case 404://������
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return false;}
		if(situation==i_will_be_attacked)return true;
	case 405://����
		return true;
	case 406://������
		if(situation==i_attack)return true;
		return false;
	case 407://��������
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)
			return true;
		else
			return false;
//--------------------------------------------------------��Ч��----------------------------------------
//------------------------------------------------------һ��Ч����--------------------------------------
	case 60://��������\n���Լ��չ������ʱ�ɷ�����
		if(situation==i_failed_attack_E||situation==i_failed_attack_W||situation==i_failed_attack_D)
			return true;
		else
			return false;
	case 61://��������ɱ\n���Լ������1�β�����˫������������3���ֿ�ʱ �ɷ�����
		if(c1->p1->whether_in_attack>0&&(*locktime)>=4)
			return true;
		else
			confirm_send_success(c1,"������������!\n");
			return false;
	case 62://"[Ч����]���ơ�ͻ��\n���Լ��غ�ʱ�ɷ������Լ�ָ��1��-1Ѫ�Ҵ˻غϲ����ж��������߿ɶ�1�ֿ���ܴ˴���Ѫ�����������Ĵ˻غ����Լ�ֻ�ܶԱ�ָ���߻��Լ�����Ч����������\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)
			return true;
		else
			return false;
	case 63:// "[Ч����]��ա�����\n���Լ��غ�ʱ�ɷ������Լ���1�������鵽�ĸÿ��������Լ���غϵĹ������չ�����������Ϊ1\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)
			return true;
		else
			return false;
	case 64:// "[Ч����]�նݡ�����\n���Լ��ܵ��չ�ʱ�ɷ������Լ���1���Ҷ�ܴ˴β������Լ��˻غϿɶ� �����ô��չ��� �չ�1��\n\n";
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return false;}
		if(situation==i_will_be_attacked)return true;
		return false;

	case 65:// "[Ч����]��ն����ɱ\n���Լ��ܵ�������Ѫʱ�ɷ������˴���Ѫ��Ч���Ҵ˻غ��Լ�������������2���Լ��ɽ���1���չ�\n\n";
		if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return false;}
		if(c1->p1->i_soon_die)return true;//����ӵ�������Ѫ����Ҫ������
		return false;

	case 66:// "[Ч����]�컪����\n���Լ��չ��ɹ��Ҹûغ����Լ��ɹ��չ�����2��ʱ�ɷ������Լ�ָ��1�ˣ��Լ��˻غϿ� �����չ� ��4�Σ�ÿ�γ����չ�������ɵ��չ���Ѫǿ�Ʊ�Ϊ1�Ҳ�����ɷ���\n\n";
		
		if(situation!=i_attack){confirm_send_success(c1,"�Լ���ǰû�����չ��ɹ�״̬!\n");return false;}
		if(c1->p1->attacktime_turn<3)
		{addtext(StrBuf,"��ǰ�غ��Լ��չ��ɹ�����Ϊ:");addtext(StrBuf,c1->p1->attacktime_turn);addtext(StrBuf,",û����2��!\n");
		confirm_send_success(c1,StrBuf);return false;}
		return true;

	case 67:// "[Ч����]���м���\n�������غ�ʱ�ɷ������Լ������ֿ����˻غ�֮���3�ֻ����Լ����ܵ�������Ѫ��ʹ�Լ��ܵ�������Ѫ�� ���ܵ�֮ǰ�Լ������ֿ���ֵ��2����Ѫ\n\n";
		if(c1->p1->c[0]==false){confirm_send_success(c1,"���ֿ�!�˿�Ч���޷�����!\n");return false;}
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)return true;
		return false;

	case 68:// "[Ч����]��ը����\n���Լ��غ�ʱ�ɷ������ƻ��������г������ͽ�翨��ÿ�ƻ�1�ţ�ʹ ���øý��򳡾��� ��3Ѫ\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)return true;
		return false;
	case 69:// "[Ч����]����ָ��\n���Լ����ڱ���״̬ʱ�ɷ������Լ�ָ��1�ˣ����Լ�ȫ���ֿ����Լ�װ��������ȫ���������ֿ�\n\n";
		confirm_send_success(c1,"���޶��˶�սʱʹ��!\n");return false;
			
	case 70:// "[Ч����]˫�۽�\n�Լ�Ѫ-3�����������Ϸ���2��������װ����\n\n";
		if(c1->p1->whether_myturn==true&&c1->p1->whether_in_attack==false)return true;
		return false;
	
	case 71:// "[Ч����]������\n���Լ��չ��ɹ�ʱ�ɷ������Լ��ٽ���1���չ����˴��չ���ɵ��չ���Ѫ����\n\n";
		if(situation==i_attack)return true;
		return false;

	case 72:// "[Ч����]��졤̫��\n���Լ��ܵ��չ�ʱ�ɷ������Լ���ܱ����չ����Լ������ֿ��������ô��չ��� ��Ѫ������\n\n";
		if(c1->p1->cannot_ED){confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return false;}
		if(situation==i_will_be_attacked)return true;
		return false;

	case 73:// "[Ч����]��Ӱ��\n���˿���Ϊ ��+2 ���������Լ�װ���ϣ���װ���ϴ˿��ĻغϽ���ʱ���˿��Զ����ƻ�\n\n";
		if(c1->p1->w[weaponlimit-1]==true){confirm_send_success(c1,"װ��������!���ܷ���!\n");return false;}
		return true;	
	case 74:// "[Ч����]���֡��޽�\n���Լ��ܵ�Ч����Ѫʱ�ɷ������Լ���1�ֿ��е�װ������ָ��1����3Ѫ\n\n";
		if(c1->p1->c[0]==false){confirm_send_success(c1,"���ֿ�Ϊ��!�˿��޷�����!\n");return false;}
		if(situation!=i_be_effect_B&&situation!=i_be_effect_A)return false;

		for(count=0;count<cardlimit;count++)//���������������ֿ�
			if(c1->p1->c[count]==true&&judge_weapon(c1->p1->card[count]))
				break;
		if(count==cardlimit||c1->p1->c[count]==false)
			{confirm_send_success(c1,"��û����������ֿ�!\n");return false;}
		return true;

	case 75:// "[Ч����]�����\n�˿������Ļغ��Լ��������2�ֻ��ڣ��Լ��ɶ�1�ֿ�ʹ 1�ζԼ���Ч����Ѫ ��Ч����ֵ�1���չ�\n\n";
			return true;//����˲��
	
	case 76:// "[Ч����]�ƾ֡����\n���Լ��չ�ʧ��ʱ�ɷ������Լ��չ� ʹ�Լ��չ�ʧ���� 1�Σ��Ҵ˴��չ��� ������ �޷��ж����Լ��ɶ�1�β�����4���������ֿ���ʹ ������ �ܵ���������һ�����Ѫ\n\n";
		if(situation==i_failed_attack_E||situation==i_failed_attack_W||situation==i_failed_attack_D)
			return true;
		else
			return false;
	case 77:;// ���ء��ػ�\n���Լ��ܵ��չ�ʱ�ɷ������Լ���1�����Լ����չ� �����չ��� 1�Σ����ι����Լ���+1";
		if(situation==i_be_attacked)
			return true;
		else
			return false;
	}
	cout<<"����Ī������Ź���!!����Ϊ:"<<choice<<endl;
	return false;
}


bool judge_character_effect(client_member* c1,int choice,int situation,int* locktime)
{
	switch(choice)
	{
		case 10000:
			return true;
		case 10001:confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;
			
		case 10002:confirm_send_success(c1,"��Ч��Ϊ����Ч��!�޷���������!\n");return false;
	}
	cout<<"������Ī�����������Ч�����!���Ϊ:"<<choice<<endl;	
	return false;
}

int auto_judge(client_member* c1,int situation,float* number,int* locktime,fight_room* this_room)//������������
{
	int count;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	int result=still_fight;

	switch(situation)
	{
		case i_hit_someone:
			for(count=0;count<weaponlimit;count++)
				if(c1->p1->blood!=c1->p1->bloodlimit&&c1->p1->w[count]==true&&c1->p1->weapon[count]==2)//����֮��
				{
					recover_blood(*number,c1,this_room);//Ѫ������numberֵ
					addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ĳ���֮�����˺��м�ȡ����������!\n");
					cout<<"��������"<<*number<<endl;
					room_broadcast(this_room,StrBuf);
				}
			break;

	}
	return result;
}
int auto_judge(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime)
{
	int result,choice;
	int count,count2;
	int c1_got,c2_got;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	result=still_fight;//Ĭ�ϲ������������������Ϊtrue���������
	switch(situation)
	{
		case i_card_used_S://�Լ�һ���ֿ������ɹ�
		if(c1->p1->whether_myturn==true)
			for(count=0;count<abilitylimit;count++)
				if(c1->p1->a[count]==true&&c1->p1->ability[count]==10002&&c1->p1->selfeffect_invalid==0)//��ʿЧ�������Լ��غ��Լ�ÿ�γɹ������ֿ�ʱ���Լ���ָ��1��-1Ѫ
				{
					addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ȡ����������!\n");
					room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					//confirm_send_success(c1,"���������ʹ�÷���ָ��1����ɫ��1Ѫ!\n");
					choice=choose_enemy(c1,"���������ָ��1����ɫ-1Ѫ!\n",this_room);
					if(choice==force_offline||choice==i_cancel_effect)break;

					c2=this_room->c[choice];
					
					addtext(StrBuf,c1->p1->name);addtext(StrBuf,"Ͷ������������!\n");
					room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
						
					addtext(StrBuf,"�������������");addtext(StrBuf,c2->p1->name);addtext(StrBuf,"!\n");

					result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
					
					break;
				}
		
		break;
		
	case has_fought://�Լ�������һ�β���
		c1_got=0;c2_got=0;

		if(!c2->p1->w[0]||!c1->p1->w[0])break;//���������һ��û������������
		for(count=0;count<weaponlimit;count++)//����չ������ɽ�֮��
		{
			if(c1->p1->weapon[count]==3)c1_got++;//c1ӵ������1
				
			if(c2->p1->weapon[count]==3)c2_got++;//c2ӵ������1
		}

		count=0;
		while(c1_got>0)//�ɽ�֮������������ƻ�����һ������
		{
			if(!c2->p1->w[0])break;//����ж���ɽ�֮������ǰ�����Ѿ�����������ȫ���ƻ��ˣ���������
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"���ɽ�֮�����ɵ��˶����һ������!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			for(;count<weaponlimit;count++)//�ɽ�֮�� ������+1��count=0�������趨����ֹ�ƻ��������������󹥻���������ͬһ���ϣ�
				if(c1->p1->weapon[count]==3)
				{c1->p1->exattack_weapon[count]++;count++;break;}
			while(1)
			{
				srand((unsigned)time(NULL)*rand()); 
				count2=rand()%(weaponlimit-0);
				if(!c2->p1->w[count2])continue;//����ò�û������,�������
				addtext(StrBuf,search_card(p_name,c2->p1->weapon[count2]));addtext(StrBuf,"��������!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				
				delete_weapon(c2,count2,this_room);//�ƻ��ò�����
				c1_got--;
				break;
			}	
		}
		count=0;
		while(c2_got>0)//�ɽ�֮������������ƻ�����һ������
		{
			if(!c1->p1->w[0])break;//����ж���ɽ�֮������ǰ�����Ѿ�����������ȫ���ƻ��ˣ���������
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"���ɽ�֮�����ɵ��˶����һ������!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			
			for(;count<weaponlimit;count++)//�ɽ�֮�� ������+1
				if(c2->p1->weapon[count]==3)
				{c2->p1->exattack_weapon[count]++;count++;break;}
			while(1)
			{
				srand((unsigned)time(NULL)*rand()); 
				count2=rand()%(weaponlimit-0);
				if(!c1->p1->w[count2])continue;//����ò�û������,�������
				addtext(StrBuf,search_card(p_name,c1->p1->weapon[count2]));addtext(StrBuf,"��������!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				
				delete_weapon(c1,count2,this_room);//�ƻ��ò�����
				c2_got--;
				break;
			}	
		}
		
		break;



	}
	return result;

}

int auto_judge(client_member* c1,int situation,fight_room* this_room)//��������Ƿ��Զ�����������Ч��/������Ч��
{
	int result;
	int count,count2;

	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };
	result=still_fight;//Ĭ�ϲ������������������Ϊtrue���������

	switch(situation)
	{
	case i_will_public_card://���������ֿ�
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," װ�������ù���!��Ϊ�䲻��˼��Ĺ����������޷������ֿ�!\n");
				room_broadcast(this_room,StrBuf);
				result=i_immune_effect;
				break;
			}
		break;
	
	case i_will_discard_card_B://������Ч�������ֿ�
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," װ�������ù���!��Ϊ�䲻��˼��Ĺ����������������˵Ķ��ֿ�Ч��!\n");
				room_broadcast(this_room,StrBuf);
				result=i_immune_effect;
				break;
			}
		break;
	case i_will_discard_card_A://��Ч�����������ֿ�
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," װ�������ù���!��Ϊ�䲻��˼��Ĺ��������������������ֿ�Ч��1!\n");
				room_broadcast(this_room,StrBuf);
				result=i_immune_effect;
				break;
			}
		break;
	case i_card_used://��ʹ����һ���ֿ�
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==6)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf," �Ķ��շ����ռ�����������Ƭ������������!����+1!\n");
				room_broadcast(this_room,StrBuf);
				c1->p1->weaponeffect06[count]++;
				weapon_change(c1,this_room);
			}
		
		break;
	
	case i_will_be_effect_A://������ѪЧ��
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count]==true&&c1->p1->weapon[count]==7)
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"  װ�������ù���!��Ϊ�䲻��˼��Ĺ�������������������ѪЧ��!\n");
				room_broadcast(this_room,StrBuf);
				c1->p1->weaponeffect06[count]++;
				result=i_immune_effect;
				break;
			}
		
			break;

	case i_will_die://�Լ�Ѫ��������û����״̬
		for(count=0;count<weaponlimit;count++)//���׽��Զ������޵�
			if(c1->p1->overdeath==0&&c1->p1->w[count]==true&&c1->p1->weapon[count]==4&&c1->p1->w_e[count][0]==true&&c1->p1->weapon_effect[count][0]==1043)
			{
				set_buff(c1,overdeath_turn,3,this_room);
	
				room_broadcast(this_room,"���׽�������ǰ��δ�еĹ�â!\n");
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"  �е��Լ����ڳ��������������!\n");
				room_broadcast(this_room,StrBuf);
				room_broadcast(this_room,"���׽�ʧȥ�������Ĺ��󡭡�����\n");

				c1->p1->w_e[count][1]=false;c1->p1->weapon_effect[count][0]=1044;//ԭ��Ч��ʧЧ����ʣ�����˺�Ч��
				c1->p1->weapon_effect[count][1]=-1;
				weapon_change(c1,this_room);
				
				break;
			}
		
		break;
	case i_get_card://�һ�ȡ��һ���ֿ�
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->weapon[count]==5&&(c1->p1->blood<c1->p1->bloodlimit))
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ʳ������װ�ô���Ч��!ÿ��1����1Ѫ!\n");
				room_broadcast(this_room,StrBuf);
				recover_blood(1,c1,this_room);//Ѫ������1
			}
		
		break;
	case i_will_attack://�����չ�������
		for(count=0;count<abilitylimit;count++)
			if(c1->p1->a[count]==true&&c1->p1->ability[count]==10001&&c1->p1->selfeffect_invalid==0)//սʿЧ�������Լ�ÿ�������չ�ʱ���˴��չ�����ǰ�Լ���+1
			{
				addtext(StrBuf,c1->p1->name);addtext(StrBuf,"����Ч������!�˴��չ�����ǰ�Լ���+2!\n");
				room_broadcast(this_room,StrBuf);
				
				attack_change(2,&c1->p1->exattack_time,c1,this_room);
			}
		for(count=0;count<weaponlimit;count++)//�������£������˶�3�ֿ�����3�����������Ϸ�3�ſ�����Ͽ���
			if(c1->p1->w[count]&&c1->p1->weapon[count]==9&&c1->p1->w_valid[count])
			{
				room_broadcast(this_room,"��������һ���Ӷ������ƺ��䡭�������˶�3�ֿ�����3�����������Ϸ�3�ſ�����Ͽ���!\n");
					
				for(count2=0;count2<playernumber;count2++)
				{
					discard_random(3,this_room->c[count2],this_room);//�����3��
					get_card(3,this_room->c[count2],this_room);//��3��
				}
				for(count2=3;count2>0;count2--)
					refresh_deck(this_room->deck);
			}
		break;


	case i_will_be_unmoveable:
		for(count=0;count<weaponlimit;count++)
			if(c1->p1->w[count])
			{
				if(c1->p1->weapon[count]==8)//�ư����磬���ܡ��޷��ж���Ч��Ӱ��
				{
					addtext(StrBuf,c1->p1->name);addtext(StrBuf," ���ư�������Ч����ѹ��Ч����Ӱ��!\n");
					room_broadcast(this_room,StrBuf);
					result=i_immune_effect;
					break;
				}
			}
			else
				break;
		break;
	}

	return result;
}

int judge_one_die(client_member* c1,fight_room* this_room)//�ж�����û��
{
	if(c1->p1->blood<=0&&c1->p1->overdeath==0)//˫���ж��������˵ڶ����ж���������
	{
		c1->p1->blood=0;auto_judge(c1,i_will_die,this_room);//�Զ���������Ч��
		if(c1->p1->blood==0&&c1->p1->overdeath==0)//�����û��������״̬������
		{
			
			c1->p1->alive=false;return one_die;
		}
	}
	return still_fight;//��û��
}

#endif