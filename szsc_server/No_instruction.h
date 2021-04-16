#ifndef INCLUDE_No_instruction_H
#define INCLUDE_No_instruction_H


#include "player_property.h"

using namespace std;



//0  ľ׮   Ѫ15  ��4
//1  սʿ   Ѫ5  ��3
//2  ��ʿ   Ѫ5  ��3
void set_character(int choice,player* p1)//�˺����Դ�������p��������ʼ��Ϊ���Ϊchoice������
{
	p1->character_number=choice;
switch(choice)
	{

	case 0://ľ׮
		p1->character_number=0;		p1->blood=15;p1->bloodlimit=15;p1->attack=4;
		p1->a[0]=true;				p1->ability[0]=10000;
		break;

	case 1://սʿ
		p1->character_number=1;		p1->blood=8;p1->bloodlimit=8;p1->attack=3;
		p1->a[0]=true;				p1->ability[0]=10001;
		break;
    
	case 2://��ʿ
		p1->character_number=2;		p1->blood=8;p1->bloodlimit=8;p1->attack=3;
		p1->a[0]=true;				p1->ability[0]=10002;
		break;
	}
}


char* search_character(int number)//��ѯ��Ŵ����Ľ�ɫ���ƻ�����Ч��
{
	
		switch(number)
		{
			case 0:return "ľ׮";
			case 1:return "սʿ";
			case 2:return "��ʿ";
		}
	
	return "�������!";
}

char* catalog[2000][6];

char* search_card(int situation,int number)                     //��ѯ��Ŵ������ֿ�Ч��
{

	switch(situation)
	{
		case p_name:
			if(catalog[number][0]!=NULL)return catalog[number][0];
			break;
		case p_details:
			if(catalog[number][1]!=NULL)return catalog[number][1];
			break;
	}
	cout<<"������number="<<number<<endl;

	return "Ī������ſ�";
}



void ini_catalog()//��ʼ����ƬĿ¼
{
	

	//catalog[11][0]="11�ſ�";catalog[11][1]=
//---------------------------------------------------������---------------------------------------------------
	catalog[30][0]="���塤�ظ�";
	catalog[30][1]="ʹ����-1Ѫ(���ֿɶ�1����ܴ���Ѫ),������,������ڴ˴��չ�����ǰ�����ж�\n";
	catalog[31][0]="����ն";
	catalog[31][1]="�˴��չ��Լ���+1,�Լ��鿴 ������ �����ֿ�,��ѡ1�ż����Լ��ֿ�,��ѡ��װ������ֱ�Ӹ��Լ�װ����\n";
	catalog[32][0]="���ɡ���ն";
	catalog[32][1]="�˴��չ��Լ�װ��������������Ч��,�չ�����������������Ч���Ҷ����ܵ��Լ���ǰ����������2����Ѫ\n";
	catalog[33][0]="�Եء�����";
	catalog[33][1]="���Լ�װ���������Լ���1��,ʹ����-1Ѫ�����ֿɶ�1����ܴ���Ѫ��\n";
	catalog[34][0]="�����ӻ�";
	catalog[34][1]="�˴��չ��Լ���+1,�չ��ɹ���30%ʹ���ֽ������Ĵ˻غϺ��»غϲ����ж�\n";
	catalog[35][0]="�����ӻ�";
	catalog[35][1]="�˴��չ��Լ���+2,�չ��ɹ���50%ʹ���������1��\n";
	catalog[36][0]="����ӻ�";
	catalog[36][1]="�˴��չ��Լ���+1,�չ��ɹ���40%ʹ����-1Ѫ\n";
	catalog[37][0]="��ˮ�ӻ�";
	catalog[37][1]="�˴��չ��Լ���+1,�չ��ɹ���ʹ�����������2������2��\n";
	catalog[38][0]="���ӻ�";
	catalog[38][1]="�˴��չ��Լ���+1,�չ��ɹ����Լ���1��\n";
	catalog[39][0]="ڤ�ļӻ�";
	catalog[39][1]="�˴��չ��Լ���+2,�չ��ɹ���30%ʹ���չ���װ��������ȫ���ƻ�\n";
	catalog[40][0]="�񡤿ա�ն";
	catalog[40][1]="�˴��չ��޷�����ܡ��񵲣��Լ��ܹ����������ұ�Ϊ����\n";
//---------------------------------------------------������---------------------------------------------------
//---------------------------------------------------Ч����---------------------------------------------------
	catalog[60][0]="��������";
	catalog[60][1]="���Լ��չ�ʧ��ʱ���Լ������ֿ���ʹ���ж���-1Ѫ��װ����ȫ���ƻ�\n";
	catalog[61][0]="��������ɱ";
	catalog[61][1]="���Լ������1���¼���������4�Ρ�ʹ ���Լ������� ��3Ѫ(��Ϊ�����չ�),�˻غϱ�ǿ�ƽ���\n";
	catalog[62][0]="���ơ�ͻ��";
	catalog[62][1]="���Լ��غ��Լ�δ���ڲ������Լ�ָ��1������-1Ѫ�Ҵ˻غϲ����ж�(���ֿɶ�1����ܴ�Ч��),֮��Ĵ˻غ��Լ�ֻ�ܶԱ�ָ���߽����չ�\n";
	catalog[63][0]="��ա�����";
	catalog[63][1]="���Լ��غ��Լ�δ���ڲ������Լ���1��,���鵽�ĸÿ�����,�Լ���غϵĹ������չ�����������Ϊ1\n";
	catalog[64][0]="�նݡ�����";
	catalog[64][1]="���Լ������ܵ��չ����Լ���1���Ҷ�ܴ˴β���,�Լ����չ���1��\n";
	catalog[65][0]="��ն����ɱ";
	catalog[65][1]="���Լ������ܵ�������Ѫ���˴���Ѫ��Ч���Ҵ˻غ��Լ���������2���Լ��ɶ������1���չ�\n";
	catalog[66][0]="�컪����";
	catalog[66][1]="���˻غ����Լ��ɹ��չ�����2�����Լ��չ��ɹ�ʱ���Լ������չ� ������ 4�Σ�ÿ�γ����չ�������ɵ��չ���Ѫǿ�Ʊ�Ϊ1�Ҳ�����ɷ���\n";
	catalog[67][0]="���м���";
	catalog[67][1]="�������غ��Լ�δ���ڲ������Լ������ֿ�,�˻غ�֮���3�ֻ����Լ����ܵ�������Ѫ��ʹ�Լ��ܵ�������Ѫ�� ���ܵ�֮ǰ�Լ������ֿ���ֵ��2����Ѫ\n";
	catalog[68][0]="��ը����";
	catalog[68][1]="���Լ��غ��Լ�δ���ڲ������ƻ��������г������ͽ�翨,ÿ�ƻ�1��,ʹ ���øý��򳡾��� -2Ѫ\n";
	catalog[69][0]="����ָ��";
	catalog[69][1]="���Լ�����ʱ���Լ�ָ��1��,���Լ�ȫ���ֿ���װ��������ȫ���������ֿ�\n";
	catalog[70][0]="˫�۽�";
	catalog[70][1]="���Լ��غ��Լ�δ���ڲ������Լ�Ѫ-3,���������Ϸ���2��������װ����\n";
	catalog[71][0]="������";
	catalog[71][1]="���Լ��չ��ɹ�ʱ���Լ��ٶ������1���չ�,�˴��չ���ɵ��չ���Ѫ����\n";
	catalog[72][0]="���顤̫��";
	catalog[72][1]="���Լ������ܵ��չ�ʱ���Լ������ֿ�����ܱ����չ�,ʹ�Լ��չ��� Ѫ������\n";
	catalog[73][0]="��Ӱ��";
	catalog[73][1]="���Լ�������û�����˿����� ��+2 ���������Լ�װ��,�������ڴ˻غϽ���ʱ�Զ����ƻ�\n";
	catalog[74][0]="���֡��޽�";
	catalog[74][1]="���Լ��ܵ�Ч����Ѫʱ���Լ���1�ֿ��е�������,ָ��1��-2Ѫ\n";
	catalog[75][0]="�����";
	catalog[75][1]="�˿������Ļغϼ�������2�ֻ���,�Լ��ɶ�1�� ��Ч��1�ζԼ��ķ�����Ч����Ѫ �� ��1���չ�\n";
	catalog[76][0]="�ƾ֡����";
	catalog[76][1]="���Լ��չ�ʧ��ʱ���Լ��ɶ������������ֿ�,ʹ ������ �ܵ�������������Ѫ.�Լ��չ� ʹ�Լ��չ�ʧ���� 1��,�Ҵ˴��չ��� ������ �޷��ж�\n";
	catalog[77][0]="���ء��ػ�";
	catalog[77][1]="���Լ��ܵ��չ�ʱ���Լ���1��,�Լ����չ� �����չ��� 1��,���ι����Լ���+1\n";

	//---------------------------------------------------Ч����---------------------------------------------------
	//---------------------------------------------------��Ч��---------------------------------------------------
	
	catalog[400][0]="����";
	catalog[400][1]="���Լ��չ��ɹ�ʱ���Լ��غ��Լ�δ���ڲ���״̬�ɷ������Լ�����һ���չ�\n";

	catalog[401][0]="��";
	catalog[401][1]="���Լ����չ�ʱ������Ǵ��չ�\n";

	catalog[402][0]="+4Ѫ";
	catalog[402][1]="ָ��1�˻ظ�4Ѫ\n";

	catalog[403][0]="��ǹ";
	catalog[403][1]="���Լ����벫��ʱ��ʹ ���Լ������� ��1Ѫ\n";

	catalog[404][0]="������";
	catalog[404][1]="���Լ����չ�ʱ������Ǵ��չ����չ��� ����ȫ���ƻ�\n";

	catalog[405][0]="����";
	catalog[405][1]="(1)ָ��1�˵�����Ч��һ�ֻ����޷�����\n";
	catalog[405][2]="(2)�ƻ�����һ�� װ���� �� ������ �� ��翨\n";
	catalog[405][3]="(3)�����￨ ������Ч���ƻ�\n";
	catalog[405][4]="(4)ָ��1�ˣ���������������buff\n";

	catalog[406][0]="������";
	catalog[406][1]="���Լ��չ��ɹ�ʱ��ʹ ���չ��� ��2Ѫ\n";

	catalog[407][0]="��������";
	catalog[407][1]="�������غ��Լ�δ���ڲ�����ָ��1��3��50%��1Ѫ����ȫ������װ��ȫ�����ƻ�\n";

	//---------------------------------------------------��Ч��---------------------------------------------------
	//---------------------------------------------------������---------------------------------------------------

	catalog[0][0]="�лʽ�";
	catalog[0][1]="��+1\n";
	catalog[0][2]="(1)1�غ���1�Σ��Լ��ɶ�1�ֿ�����1��\n";
	catalog[0][3]="(2)�Լ��ɵ�4���չ����ֵ���4���չ���˿����ƻ�\n";
	catalog[0][4]="(3)���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��-1Ѫ\n";
 
	catalog[1][0]="������";
	catalog[1][1]="��+1\n";
	catalog[1][2]="(1)1�غ���2�Σ��Լ��ɶ�1�ֿ����Լ��˻غϹ�+1\n";
	catalog[1][3]="(2)���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��-1Ѫ\n";

	catalog[2][0]="����֮��";
	catalog[2][1]="��+2\n";
	catalog[2][2]="(1)�Լ�ÿʹ1����1Ѫ���Լ�+1Ѫ\n";
	catalog[2][3]="(2)���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��+3Ѫ\n";

	catalog[3][0]="�ɽ�֮��";
	catalog[3][1]="��+1\n";
	catalog[3][2]="(1)�Լ�ÿ�����˲���������1�Σ�����ƻ� ���Լ������� װ����1��װ�������ƻ������������+1\n";
	catalog[3][3]="(2)���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��-1Ѫ\n";

	catalog[4][0]="���׽�";
	catalog[4][1]="��+2\n";
	catalog[4][2]="(1)�˿���װ����ʱ�Լ�-3Ѫ\n";
	catalog[4][3]="(2)�˴���Ϸ��1�οɷ����������غϹ����Լ���������2�ֻ�\n";
	catalog[4][4]="(3)���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��50%-2Ѫ\n";

	catalog[5][0]="ʳ������װ��";
	catalog[5][1]="(1)�Լ�ÿ��1�����Լ�Ѫ+1\n";
	catalog[5][2]="(2)1�غ���1�Σ��Լ��ɶ�1�ֿ���Ѫ+1\n";
	catalog[5][3]="(3)�Լ��غ���1�Σ��Լ��ɶ����1��\n";

	catalog[6][0]="���շ���";
	catalog[6][1]="(1)��������ʼ��2ħ��ָʾ��\n";
	catalog[6][2]="(2)����ÿ����1�ֿ���������ħ��ָʾ��+1\n";
	catalog[6][3]="(3)���Լ��غϿɷ������Լ�����2ħ��ָʾ��ʹ1��-1Ѫ\n";

	catalog[7][0]="���ù���";
	catalog[7][1]="(1)�Լ������κζ������ֿ�������Ч��Ӱ��\n";
	catalog[7][2]="(2)�����ϳ�����1������ʱ���Լ�Ѫ������\n";
	catalog[7][3]="(3)�Լ������κ�������ѪЧ��Ӱ��\n";

	catalog[8][0]="�ư�����";
	catalog[8][1]="(1)�Լ����ܡ��޷��ж���Ч��Ӱ��\n";
	catalog[8][2]="(2)���Լ��ܵ��չ�ʱ�ɷ������Լ��������˿��ƻ����Լ��ֵ��˴��չ�\n";
	//catalog[8][3]="(3)�Լ�����װ�������ƻ�Ч��Ӱ��\n";

	catalog[9][0]="�������¸�";
	catalog[9][1]="��+2\n";
	catalog[9][2]="(1)���Լ�ÿ�������չ�ʱ�������˶�3������3�����������Ϸ�3�ſ�����Ͽ���\n";
	catalog[9][3]="(2)���Լ��غϿɷ������Լ��������˿��ƻ������������������5������5��\n";

	catalog[10][0]="��Ӱ��";
	catalog[10][1]="��+2\n";
	//---------------------------------------------------������---------------------------------------------------
}

char* search_skill(int number)//��ѯ��Ŵ���������/��ƬЧ��
{

	int first,second;
	if(number<10000)//ȷ��������Ч����
	{
		first=number/10%10;//ȡʮλ
		second=number%10;//ȡ��λ
		if(catalog[first][second]==NULL)
		{cout<<"��ֵĴ��󣡣���\n";return " ";}
		else
			return catalog[first][second];
	}

	switch(number)
	{
		case 10000:return "����һ����˿�����ġ�������ϰ�� ľͷ";
		case 10001:return "���Լ�ÿ�������չ�ʱ���˴��չ�����ǰ�Լ���+2";
		case 10002:return "���Լ��غ��Լ�ÿ�γɹ������ֿ�ʱ���Լ���ָ��1��-1Ѫ";
	}
	
return "��ѯ��Ŵ���Ч���쳣!\n";
}
int get_hideeffect(int number)//���ݿ�Ƭ��ȡ����Ч��
{
	int attack_plus	=	400;//400 ����
	int escape		=	401;//401 ��
	int recover		=	402;//402 +4Ѫ
	int darkgun		=	403;//403 ��ǹ
	int scarecrow	=	404;//404 ������
	int hatred		=	405;//405 ����
	int skylaser	=	406;//406 ������
	int bluecrystal	=	407;//407 ��������

	switch(number)
	{
	case 0:return hatred;//�лʽ�
    case 1:return bluecrystal;//������
	case 2:return skylaser;//����֮��
	case 3:return escape;//�ɽ�֮��
	case 4:return attack_plus;//���׽�
	case 5:return hatred;//ʳ������װ��
	case 6:return scarecrow;//���շ���
	case 7:return escape;//���ù���
	case 8:return skylaser;//�ư�����
	case 9:return recover;//�������¸�
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
	case 30:return escape;//���塤�ظ�
	case 31:return recover;//����ն
	case 32:return escape;//���ɡ���ն
	case 33:return scarecrow;//�Եء�����
	case 34:return escape;//�����ӻ�
	case 35:return recover;//�����ӻ�
	case 36:return escape;//����ӻ�
	case 37:return hatred;//��ˮ�ӻ�
	case 38:return bluecrystal;//���ӻ�
	case 39:return scarecrow;//ڤ�ļӻ�
	case 40:return recover;//�񡤿ա�ն
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
	case 60:return bluecrystal;//��������
	case 61:return hatred;//��������ɱ
	case 62:return escape;//���ơ�ͻ��
	case 63:return skylaser;//��ա�����
	case 64:return attack_plus;//�նݡ�����
	case 65:return attack_plus;//��ն����ɱ
	case 66:return darkgun;//�컪����
	case 67:return attack_plus;//���м���
	case 68:return recover;//��ը����
	case 69:return attack_plus;//����ָ��
	case 70:return scarecrow;//˫�۽�
	case 71:return escape;//������
	case 72:return attack_plus;//���顤̫��
	case 73:return recover;//��Ӱ��
	case 74:return attack_plus;//���֡��޽�
	case 75:return attack_plus;//�����
	case 76:return escape;//�ƾ֡����
	case 77:return skylaser;//���ء��ػ�
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
	cout<<"��ֵ�number="<<number<<endl;
	return attack_plus;//����
}


#endif