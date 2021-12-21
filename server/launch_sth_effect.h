#ifndef INCLUDE_launch_sth_effect_H
#define INCLUDE_launch_sth_effect_H

#include "weapon.h"
//���������������������������������������������������Ը���Ч���Ĵ����������������������������������������������������

int launch_weapon_effect(client_member* c1,int card_No,int whichweapon,int situation,int* locktime,fight_room* this_room)//��������Ч��
{
	int result,count,choice;

	char StrBuf[MAX_PATH]= { 0 };
	client_member* c2;
	if(c1->member_No==this_room->c[0]->member_No)
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(c2==NULL)return force_offline;

	switch(card_No)
	{
	case 1002://1�غ���1�Σ��Լ��ɶ�1�ֿ�����1�����лʽ�1��

				result=discard_choose(1,c1,this_room);//ѡ��1��
				if(result!=card_discard)
					return result;

				addtext(StrBuf,c1->p1->name);addtext(StrBuf," �����лʽ���1��Ч��:");addtext(StrBuf,search_skill(card_No));addtext(StrBuf,"\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

				get_card(1,c1,this_room);//��һ�ſ�
				auto_judge(c1,i_get_card,this_room);
				c1->p1->weaponeffect001[whichweapon]--;//�����ɹ�������Ч��1�غ��ڿ�ʹ�ô���-1

				return still_fight;

	case 1003://�Լ��ɵ�4���չ����ֵ���4���չ���˿����ƻ����лʽ�2��
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"�óлʽ������չ�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			if(c1->p1->weaponeffect002[whichweapon]>1)
				c1->p1->weaponeffect002[whichweapon]--;
			else
				{room_broadcast(this_room,"��Ч���������꣬�лʽ����ƻ�!\n");delete_weapon(c1,whichweapon,this_room);}
			return i_failed_attack_D;
 
	case 1004://���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����1Ѫ���лʽ�3��
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"��ȡ���ò���!\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
				//c2=this_room->c2;
			//else
				//c2=this_room->c1;
            delete_weapon(c1,whichweapon,this_room);//ɾ��һ�ѳлʽ�
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ɳ������еĳлʽ�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			addtext(StrBuf,"�лʽ��ɹ�������");addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���������Ѫ!\n");//������У���ʾ��仰
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ

			return result;

	case 1012:  //"1�غ���2�Σ��Լ��ɶ�1�ֿ����Լ��˻غϹ�+1";������1
			
			result=discard_choose(1,c1,this_room);//ѡ��1��
				if(result!=card_discard)
					return result;

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," ��Ϣ���񡭡�\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"����ϵ����������������е� ������ �Ͼۺ�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"ذ���ϵ���������������!\n");
			room_broadcast(this_room,StrBuf);

			c1->p1->exattack_weapon_turn[whichweapon]++;
			weapon_change(c1,this_room);

			return still_fight;

	case 1013:  //"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����1Ѫ";������2
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"��ȡ���ò���!\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
				//c2=this_room->c2;
			//else
				//c2=this_room->c1;

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ɳ������еļ�����!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//ɾ��һ�Ѽ�����
			
			addtext(StrBuf,"�����гɹ�������");addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���������Ѫ!\n");//������У���ʾ��仰
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ

			return result;
		
	case 1022: cout<<"1����֮��1\n"; //"�Լ�ÿʹ1����1Ѫ���Լ�+1Ѫ";����֮��1
	case 1023:  //���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��+3Ѫ";����֮��2

			choice=choose_someone(c1,"��ָ��һ��+3Ѫ\n",this_room);
			if(choice==i_cancel_effect||choice==force_offline){return choice;}
			c2=this_room->c[choice];

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �׼������еĳ���֮��!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//ɾ��һ�ѳ���֮��

			addtext(StrBuf,"����֮���е���������ӿ����");addtext(StrBuf,c2->p1->name);addtext(StrBuf," ������!��������ֵ�ظ���!\n");
			room_broadcast(this_room,StrBuf);
			recover_blood(3,c2,this_room);//Ѫ������3

			return still_fight;
	case 1032: cout<<"1007�ɽ�֮��1\n"; //"�Լ�ÿ�����˲���������1�Σ�����ƻ� ���Լ������� װ����1��װ����";//�ɽ�֮��1
	case 1033:  ;//"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����1Ѫ";//�ɽ�֮��2	
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"��ȡ���ò���!\n");return still_fight;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
				//c2=this_room->c2;
			//else
				//c2=this_room->c1;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ɳ������е��ɽ�֮��!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//ɾ��һ���ɽ�֮��

			addtext(StrBuf,"�ɽ�֮���ɹ�������");addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���������Ѫ!\n");//������У���ʾ��仰
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ

			return result;

	case 1043:  //"�˴���Ϸ��1�οɷ����������غϹ����Լ���������2�ֻ�";//���׽�1
			set_buff(c1,overdeath_turn,3,this_room);//����״̬����Ϊ3

			room_broadcast(this_room,"���׽�������ǰ��δ�еĹ�â!\n");//��Ч�����������������Լ�Ѫ����0ʱ�Զ�����
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"  �е��Լ����ڳ��������������!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"���׽�ʧȥ�������Ĺ��󡭡�����\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			c1->p1->w_e[whichweapon][1]=false;c1->p1->weapon_effect[whichweapon][0]=1010;//ԭ��Ч��ʧЧ����ʣ�����˺�Ч��
			c1->p1->weapon_effect[whichweapon][1]=-1;
			weapon_change(c1,this_room);
			
			return still_fight;

	case 1044: ;//"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1��50%��2Ѫ";//���׽�2
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"��ȡ���ò���!\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
					//c2=this_room->c2;
				//else
				//	c2=this_room->c1;

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ɳ������еĺ��׽�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

            delete_weapon(c1,whichweapon,this_room);//ɾ��һ�Ѻ��׽�
			
			if(whether_it_happen(50))
			{
				addtext(StrBuf,"���׽��ɹ�������");	addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���������Ѫ!\n");//������У���ʾ��仰
				result=hit_damage(2,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ

				return result;			
			}
			else
				room_broadcast(this_room,"��ϧû���С�������\n");
			return still_fight;

	case 1051: cout<<"1011ʳ������װ��1\n";//"�Լ�ÿ��1�����Լ�Ѫ+1";//ʳ������װ��1
	case 1052: //1�غ���1�Σ��Լ��ɶ�1�ֿ���Ѫ+1;//ʳ������װ��2
			if(c1->p1->blood==c1->p1->bloodlimit){confirm_send_success(c1,"ϵͳ��ʾ��Ѫ��Ϊ��!����ȡ��������Ч��!\n");}
			
			result=discard_choose(1,c1,this_room);//ѡ��1��
				if(result!=card_discard)
					return result;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," ѡ�񷢶�ʳ������װ�õ�2��Ч��!(1�غ���1�Σ��Լ��ɶ�1�ֿ���Ѫ+1)\n");
			room_broadcast(this_room,StrBuf);
			if(c1->p1->blood!=c1->p1->bloodlimit)
			{
				room_broadcast(this_room,"�����ɹ�!��������ֵ�ָ���!\n");
				recover_blood(1,c1,this_room);//Ѫ������1
			}
			else
				room_broadcast(this_room,"�����ɹ�!��������ֵû������!\n");

			c1->p1->weaponeffect051[whichweapon]--;//�����ɹ�������Ч��1�غ��ڿ�ʹ�ô���-1
			return still_fight;

	case 1053: //"�Լ��غ���1�Σ��Լ��ɶ����1��";//ʳ������װ��3
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," ѡ�񷢶�ʳ������װ�õ�3��Ч��!(�Լ��غ���1�Σ��Լ��ɶ����1��)\n");
			room_broadcast(this_room,StrBuf);

			get_card(1,c1,this_room);//��һ�ſ�
			auto_judge(c1,i_get_card,this_room);

			room_broadcast(this_room,"�����ɹ�!\n");
			
			c1->p1->weaponeffect052[whichweapon]--;

			return still_fight;

	case 1061: cout<<"1014���շ���1\n";//"�˿���װ����ʱ���˿�����4ħ��ָʾ��";//���շ���1

	case 1062: cout<<"1015���շ���2\n";//"����ÿ����1�ֿ������˿�����1ħ��ָʾ��";//���շ���2

	case 1063: ;//"���Լ��غϿɷ������Լ�����2ħ��ָʾ��ʹ1����1Ѫ";//���շ���3
			//choice=choose_enemy(c1,this_room);
			//if(choice==force_offline)return force_offline;
			//if(choice==i_cancel_effect){confirm_send_success(c1,"��ȡ���ò���!\n����������ҳ�桭������\n");return i_cancel_effect;}
			//if(this_room->c2->p1->player_No==this_room->player_all[choice]->player_No)
			//	c2=this_room->c2;
			//else
			//	c2=this_room->c1;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," ���𷨰��ͷ��˷��������!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"��.����֮��!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			c1->p1->weaponeffect06[whichweapon]=c1->p1->weaponeffect06[whichweapon]-2;//����2ħ��ָʾ��
			weapon_change(c1,this_room);

			addtext(StrBuf,"���������ɹ�������");addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���������Ѫ!\n");//������У���ʾ��仰
			result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ

			return result;

	case 1071: cout<<"1017���ù���1\n";//"�Լ������κζ������ֿ�������Ч��Ӱ��";//���ù���1
	case 1072: cout<<"1018���ù���2\n";//"�����ϳ�����1������ʱ���Լ�Ѫ������";//���ù���2
	case 1073: cout<<"1019���ù���3\n";//"�Լ������κ�������ѪЧ��Ӱ��";//���ù���3
	case 1081: cout<<"1020�ư�����1\n";//"�Լ���װ����������װ����"; //�ư�����1
	case 1082: //"���Լ��ܵ��չ�ʱ�ɷ������Լ��������˿��ƻ����Լ��ֵ��˴��չ�";//�ư�����2
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," ���ư���������չ�!�ư����类�ƻ���!\n");
			room_broadcast(this_room,StrBuf);
			delete_weapon(c1,whichweapon,this_room);
			return i_failed_attack_D;
	case 1092: cout<<"1022�������¸�1\n";//"���Լ�ÿ�չ�1�Ρ������˶�3�ֿ�����3�����������Ϸ�3�ſ�����Ͽ���";//�������¸�1
	case 1093: //"���Լ��غϿɷ������Լ��������˿��ƻ���ָ��1����2Ѫ,���������˶�5�ֿ�����5��";//�������¸�2
			addtext(StrBuf,c1->p1->name);
			addtext(StrBuf," �۶������е�ϡ���䱦:��������!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"��Χ������ʼ�������!�·���һ˲���ڹ���ǧ�����!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,"������������� 5 ���ֿ�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			delete_weapon(c1,whichweapon,this_room);//ɾ��һ�ѹ�������
			
			for(count=0;count<playernumber;count++)
			{
				discard_random(5,this_room->c[count],this_room);
				confirm_send_success(this_room->c[count],"���⻷���ı仯���������˳������µ�����!ÿ���˸���5���ֿ�!\n");
				get_card(5,this_room->c[count],this_room);auto_judge(this_room->c[count],i_get_card,this_room);
			}

			return still_fight;
	}
	cout<<"����Ч��û�����ɹ�!card_No="<<card_No;
	return still_fight;
}



int character_effect(client_member* c1,int card_No,fight_room* this_room)			//�Լ�����������������Ч��
{
	switch(card_No)
	{
	case 10000:cout<<"�����˲��÷�����Ч��10000!\n";return still_fight;
	case 10001:cout<<"�����˲��÷�����Ч��10001!\n";return still_fight;
	case 10002:cout<<"�����˲��÷�����Ч��10002!\n";return still_fight;
	}
	cout<<"����Ч������ʧ��!card_No="<<card_No;
	return card_No;
}


//����������Ч��,assistΪ�������ţ�stageΪǰ��Ч�������Ч��
//stage:  after_effect����Ч��    fore_effect ǰ��Ч��
int launch_assist_effect(client_member* c1,client_member* c2,int card_No,int stage,int* locktime,fight_room* this_room)
{
	int count,count2;
	int result;
	char StrBuf[MAX_PATH] = { 0 };
	char Buf[MAX_PATH] = { 0 };

	switch(card_No)
	{
	case 30://ʹ ������ ��1Ѫ�������߿ɶ�һ����ܴ˴���Ѫ���������У��� ������ �ڴ˴��չ�����ǰ�����ж�
		if(stage==fore_effect)
		{
			
				confirm_send_success(c1,"�Է�˼���С���\n");
				confirm_send_success(c2,"�Ƿ�һ�ֿ���ܡ�1Ѫ�˺��ʹ˴��չ�����ǰ�����ж���Ч����\n");
				result=discard_choose(1,c2,this_room);
				if(result!=card_discard)//����������ֿ����㣬������������
				{
					if(result==force_offline)return force_offline;

					result=hit_damage(1,c2,c1,this_room,locktime,"�ܵ�����ض���ҡ�Ĺ���Ӱ�죬�ڴ˴��չ�����ǰ�����ж�!\n",i_will_be_effect_B);//��Ѫ
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;
					if(judge_one_die(c2,this_room)==one_die)return one_die;//�ܵ���ѪЧ�������ܻ���
					set_buff(c2,unmove_time,0,this_room);
				}		
			return i_attack;
		}
		return i_attack;
	case 31://�ڴ˴��չ�����ǰ�Լ���+1���Լ��鿴 ������ �������ֿ�����ѡ����1�ż����Լ��ֿ�����ѡ��װ��������ɸ��Լ�װ����
		if(stage!=fore_effect)return still_fight;
	
		attack_change(1,&c1->p1->exattack_time,c1,this_room);//��+1

		plunder_ones_card(c1,c2,1,this_room);//��ȡ�ֿ�1��
		return i_attack;

	case 32:;//"�˴��չ����Լ�װ��������  ����Ч�����չ�������  ��������Ч���� ������ �ܵ��Լ���ǰװ��  ��������2��Ѫ��
		if(stage==fore_effect)
		{	
			count2=0;
			for(count=0;count<cardlimit;count++)
				if(c1->p1->w[count]==true){c1->p1->w_valid[count]=false;count2++;}

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ɳ������е���������!\n");
			room_broadcast(this_room,StrBuf);

			if(count2==0)	
				room_broadcast(this_room,"������û��װ���κ���������\n");
			else
				weapon_change(c1,this_room);
			return i_attack;
		}
		if(stage==after_effect)
		{
			count2=0;//ͳ�Ʒɳ�ȥ��������
			for(count=0;count<weaponlimit;count++)//װ����ԭ
				if(c1->p1->w[count]==true)//����ò���װ��
				{
					c1->p1->w_valid[count]=true;//װ��Ч����ԭ
					count2++;
				}
			if(count2==0)return i_attack;//�Լ���ʵûװ��������ֱ�ӷ��أ��������˺�

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," ֮ǰ�ɳ�ȥ�������ַ��˻���!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			weapon_change(c1,this_room);

			addtext(StrBuf,"������");addtext(StrBuf,c2->p1->name);addtext(StrBuf," ������,�����");addtext(StrBuf,count2*2);addtext(StrBuf,"���˺�!\n");
			result=hit_damage(float(2*count2),c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;

			return i_attack;
		}

	case 33://���Լ�װ������ʱ�ɷ������Լ���1����ʹ ������ ��1Ѫ�������߿ɶ�һ����ܴ˴���Ѫ��
		if(stage==fore_effect)
		{
			get_card(1,c1,this_room);
			auto_judge(c1,i_get_card,this_room);

			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �ӳ�һ������!����");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf," !\n");
			room_broadcast(this_room,StrBuf);


				confirm_send_success(c1,"�Է�˼���С���\n");
				confirm_send_success(c2,"�Ƿ�һ�ֿ���ܡ�1Ѫ�˺���Ч����\n");
				result=discard_choose(1,c2,this_room);
				if(result!=card_discard)
				{
					if(result==force_offline)return force_offline;
					result=hit_damage(1,c2,c1,this_room,locktime,"�ܵ������˺�!Ѫ-1!\n",i_will_be_effect_B);//��Ѫ
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;
				}
				else
					room_broadcast(this_room,"������������˺�!\n");
			
			return i_attack;
		}
		if(stage==after_effect)
		{
			return i_attack;
		}
	case 34://���ι����Լ���+1,�չ��ɹ���30%ʹ���չ��߽������Ĵ˻غϺ��»غϲ����ж�
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//��+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(30))
			{
				addtext(StrBuf,"�����ӻ�����Ч�������ɹ�!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," �������������ж�!�������Ĵ˻غϺ��»غϲ����ж�!\n");
				room_broadcast(this_room,StrBuf);

				set_buff(c2,unmove_turn,2,this_room);
			}
			return i_attack;	
		}
		
	case 35://return "���ι����Լ���+2,�չ��ɹ���50%ʹ���չ��������1��\n\n";
		if(stage==fore_effect)
		{
			attack_change(2,&c1->p1->exattack_time,c1,this_room);//��+2
			return i_attack;
		}
		if(stage==after_effect)
		{

			if(whether_it_happen(50))
			{
				addtext(StrBuf,"�����ӻ�����Ч�������ɹ�!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," ���׵������������!�������1���ֿ�!\n");
				room_broadcast(this_room,StrBuf);
				discard_random(1,c2,this_room);
			}
			return i_attack;
		}
		
	case 36://return "���ι����Լ���+1,�չ��ɹ���40%ʹ���չ�����1Ѫ\n\n";
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//��+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(40))
			{
				addtext(StrBuf,"����ӻ�����Ч�������ɹ�!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," �����������������!\n");//����ʱ�㲥��仰
				result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
				if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event||result==i_failed_effect_B)return result;
			}
			return i_attack;
		}
	case 37://return "���ι����Լ���+1,�չ��ɹ���ʹ���չ��������2������2��\n\n";
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//��+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(100))
			{
				addtext(StrBuf,"��ˮ�ӻ�����Ч�������ɹ�!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," ��ǿ�ҵ�ˮ���������������!�����������2���ֿ�!������µ�����!��2��!\n");
				room_broadcast(this_room,StrBuf);

				discard_random(2,c2,this_room);//�������2���ֿ�
				get_card(2,c2,this_room);
				auto_judge(c2,i_get_card,this_room);
			}	
			return i_attack;
		}
	case 38://return "���ι����Լ���+1,�չ��ɹ����Լ���1��\n\n";
		if(stage==fore_effect)
		{
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//��+1
			return i_attack;
		}
		if(stage==after_effect)
		{
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �յ�ʥ���ף��!�����������!��1��!\n");
			room_broadcast(this_room,StrBuf);

			get_card(1,c1,this_room);
			auto_judge(c1,i_get_card,this_room);

			return i_attack;
		}		
	case 39://return "���ι����Լ���+2,�չ��ɹ���30%ʹ���չ���װ��������ȫ���ƻ�\n\n";
		if(stage==fore_effect)
		{
			attack_change(2,&c1->p1->exattack_time,c1,this_room);//��+1
			return still_fight;
		}
		if(stage==after_effect)
		{
			if(whether_it_happen(30))
			{
				addtext(StrBuf,"ڤ�ļӻ�����Ч�������ɹ�!\n");
				room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				addtext(StrBuf,c2->p1->name);addtext(StrBuf," ��ڤԨ����������Ⱦ!��������ȫ�����ƻ�!\n");
				room_broadcast(this_room,StrBuf);

				delete_weapon_all(c2,this_room);			
			}
			return i_attack;
		}
		
	case 40://���ն  �˴��չ��޷�����ܡ��񵲣��Լ��ܹ����������ұ�Ϊ����
		if(stage!=fore_effect)return still_fight;
		
		c1->p1->whether_attack_T=true;c2->p1->cannot_ED=true;
		c1->p1->hurtmultiple_time=float(c1->p1->hurtmultiple_time*0.5);
		room_broadcast(this_room,"�����չ��л�Ϊ�˹⡭��\n");
		return i_attack;
		
	}
cout<<"��������Ŵ����쳣!card_No="<<card_No;
return still_fight;
}




int launch_card_effect(client_member* c1,client_member* c2,int card_No,int situation,int* locktime,fight_room* this_room)//�����ֿ�
{

	//c1Ϊ�����ߣ�c2Ϊ�������ߣ����ţ�����ʱ�������������������������
	if(card_No>=0&&card_No<=29){confirm_send_success(c1,"�ÿ�Ϊװ���������Լ��غ�ѡ��'3.װ������'�ſɷ���!\n");return false;}
	if(card_No>=30&&card_No<=59){confirm_send_success(c1,"�ÿ�Ϊ�����������Լ��չ�����ʱ�ſɷ���!\n");return false;}
	
	char StrBuf[MAX_PATH] = { 0 };
	int result=still_fight;
	int count;
	int card_number;
	bool whether_get_hurt=false;//˫�۽�ר��
	float p_attack_all=0;//��ն��ɱר��
	int choice;
	
	if(c1->member_No==this_room->c[0]->member_No)//����ģʽ����Ҫ�޸ģ�
		c2=this_room->c[1];
	else
		c2=this_room->c[0];
	if(c2==NULL)return force_offline;


	switch(card_No)
	{
	case 60://��������\n���Լ��չ������ʱ�ɷ�����

			discard_all(c1,this_room);//�����ֿ�
			room_broadcast(this_room,"���ͷų��Լ���ȫ������!�۽������ǰ㽦�䵽�����!\n");
			room_broadcast(this_room,"�����˵�װ��ȫ�����ۻ������!\n");
			delete_weapon_all(c2,this_room);//ɾ��������������������ģʽ����Ҫ�޸ģ�
			room_broadcast(this_room,"�ܵ������۽������ˣ�����������Ѫ-1!\n");

			result=hit_damage(1,c2,c1,this_room,locktime,NULL,i_will_be_effect_B);//��Ѫ
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

			return force_end_event;
	case 61://��������ɱ\n���Լ������1�β�����˫������������4������ʱ �ɷ�����
		room_broadcast(this_room,"����������������ŭ��ʱ��������һ��˲ȭ��ɱ!\n");
		
		result=lightspeed_attack(c1,c2,3,locktime,this_room);
		room_broadcast(this_room,"�˻غϱ�ǿ�ƽ���!!!\n");
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_turn;
	case 62://"[Ч����]���ơ�ͻ��\n���Լ��غ�ʱ�ɷ������Լ�ָ��1��-1Ѫ�Ҵ˻غϲ����ж��������߿ɶ�1�ֿ���ܴ˴���Ѫ�����������Ĵ˻غ����Լ�ֻ�ܶԱ�ָ���߻��Լ�����Ч����������\n\n";
			/*while(true)
			{
				choice=choose_enemy(c1,this_room);
				if(choice==force_offline)return force_offline;
				if(choice==i_cancel_effect){confirm_send_success(c1,"�ѷ���!�޷�ȡ��!������ѡ��\n");continue;}
				break;
			}
			if(this_room->player_all[choice]->player_No==this_room->c1->p1->player_No)
				c2=this_room->c1;
			else
				c2=this_room->c2;*/
			room_broadcast(this_room,"������Ϣ�����������˼�!\n");

			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"��Ҫ��1�ֿ����ܰ�����Ԩ֮��������!\n");
			room_broadcast(this_room,StrBuf);
			memset(StrBuf,0,sizeof(StrBuf));

			confirm_send_success(c2,"��ѡ��1�ֿ�������������Ԩ֮��������!\n");
				confirm_send_success(c1,"�Է�˼���С���\n");
				result=discard_choose(1,c2,this_room);
				if(result!=card_discard)//���û�����ɹ�
				{
					if(result==force_offline)return force_offline;
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�ܵ�����Ԩ֮����ѹ��(���غ��޷��ж���Ѫ-1)!\n");
					result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
					
					if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;
					if(result!=i_failed_effect_B)//���û�����˺����ж�������
						set_buff(c2,unmove_turn,1,this_room);//���Ӳ����ж�buff
				}
				else
					room_broadcast(this_room,"����������Ԩ֮����ѹ��!\n");
			
			return still_fight;
		
	case 63:// "[Ч����]��ա�����\n���Լ��غ�ʱ�ɷ������Լ���1�������鵽�ĸÿ��������Լ���غϵĹ������չ�����������Ϊ1\n\n";
			c1->p1->fight_chance=1;//�Լ���غϵĹ������չ�����������Ϊ1

			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��ȡһ��!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

			get_card(1,c1,this_room);
			auto_judge(c1,i_get_card,this_room);

			if(auto_judge(c1,i_will_public_card,this_room)!=i_immune_effect)//�����ֿ�Ч���Ƿ���Ӱ��
			{
				for(count=cardlimit-1;count>=0;count--)
				{
					if(c1->p1->c[count]==true)
					{
						addtext(StrBuf,"���ÿ�����!�ÿ�Ϊ:\n");
						room_broadcast(this_room,StrBuf);
						memset(StrBuf,0,sizeof(StrBuf));
						addtext(StrBuf,"��");
						addtext(StrBuf,search_card(p_name,c1->p1->card[count]));
						addtext(StrBuf,"��!!!\n");
						room_broadcast(this_room,StrBuf);
						break;
					}
				}
			}

			return still_fight;

	case 64:// "[Ч����]�նݡ�����\n���Լ��ܵ��չ�ʱ�ɷ������Լ���1���Ҷ�ܴ˴β������Լ��ɶ� �����ô��չ��� �չ�1��\n\n";
		room_broadcast(this_room,"��������У�������չ�!\n");
		get_card(1,c1,this_room);
		auto_judge(c1,i_get_card,this_room);
		confirm_send_success(c2,"�Է�˼���С���\n");
		result=ask_whether_do(c1,do_attack);
		if(result==force_offline)return result;

		room_broadcast(this_room,"�������ط������лػ�!\n");
		result=general_attack(c1,c2,nothing_else,locktime,this_room);
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_event;


	case 65:// "[Ч����]��ն����ɱ\n���Լ��ܵ�������Ѫʱ�ɷ������˴���Ѫ��Ч���Ҵ˻غ��Լ�������������2���Լ��ɶ� ʹ�Լ��ܵ�������Ѫ�� �չ�1��\n\n";
		room_broadcast(this_room,"ϣ��֮�⻮�����\n");
		confirm_send_success(c2,"�Է�˼���С���\n");
		result=ask_whether_do(c1,do_attack);
		if(result==force_offline)return result;
		
		p_attack_all=	(c1->p1->attack)	+	(c1->p1->exattack_turn)	+	(c1->p1->exattack_time);//����������
		
		for(count=0;count<2;count++)//���װ�����Ƿ��мӹ�����
			if(c1->p1->w[count]==true&&c1->p1->w_valid[count]==true)
				p_attack_all=	p_attack_all	+   c1->p1->exattack_weapon[count] + c1->p1->exattack_weapon_turn[count] + c1->p1->exattack_weapon_time[count];
		
		attack_change(p_attack_all,&c1->p1->exattack_turn,c1,this_room);

		if(result==cancel_choice)return i_failed_damage;//������Ч���˺��������չ�
		room_broadcast(this_room,"��ˮ֮�󣬾��ػػ�!\n");

		result=general_attack(c1,c2,nothing_else,locktime,this_room);
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_event;
	case 66:// "[Ч����]�컪����\n���Լ�һ�غ��ڳɹ��չ�����2��ʱ�ɷ������Լ�ָ��1�ˣ��Լ��˻غϿ� �����չ� ��4�Σ�ÿ�γ����չ�������ɵ��չ���Ѫǿ�Ʊ�Ϊ1�Ҳ�����ɷ���\n\n";
		/*while(1)
		{
			confirm_send_success(c1,"��ѡ���չ��Ķ���:\n");
			count=choose_enemy(c1,this_room);
			if(count==force_offline)return force_offline;
			if(count==i_cancel_effect)
				{confirm_send_success(c1,"��ƬЧ���ѷ���!�޷�ȡ��!\n");continue;}

			if(this_room->player_all[count]->player_No==this_room->c1->p1->player_No)
				c2=this_room->c1;
			else
				c2=this_room->c2;
			break;
		}*/
		room_broadcast(this_room,"ˢˢˢ�����ĵ�����\n");
		result=lightspeed_attack(c1,c2,4,locktime,this_room);
		if(result==one_die||result==force_offline||result==force_end_turn)return result;
		return force_end_event;
	case 67:// "[Ч����]���м���\n�������غ�ʱ�ɷ������Լ������ֿ����˻غ�֮���3�ֻ����Լ����ܵ�������Ѫ��ʹ�Լ��ܵ�������Ѫ�� ���ܵ�֮ǰ�Լ������ֿ���ֵ��2����Ѫ\n\n";
			if(auto_judge(c1,i_will_discard_card_A,this_room)==i_immune_effect)//����Ƿ��ܶ���Ч��Ӱ�죿
			count=discard_all(c1,this_room);//����ܣ����������㶪������

			c1->p1->godlaserdamage=c1->p1->godlaserdamage + count*2;
			c1->p1->godlaser=3;//�غ���������ӣ�ֻ�����õ�3
			return still_fight;
			
	case 68:// "[Ч����]��ը����\n���Լ��غ�ʱ�ɷ������ƻ��������г������ͽ�翨��ÿ�ƻ�1�ţ�ʹ ���øý��򳡾��� ��1Ѫ\n\n";
		room_broadcast(this_room,"���ϼ�ⲻ���������翨����\n");return still_fight;
	case 69:cout<<"69����ָ��\n";// "[Ч����]����ָ��\n���Լ����ڱ���״̬ʱ�ɷ������Լ�ָ��1�ˣ����Լ�ȫ���ֿ����Լ�װ��������ȫ���������ֿ�\n\n";
	case 70:// "[Ч����]˫�۽�\n�Լ�Ѫ-3�����������Ϸ���2��������װ����\n\n";
		
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"�ڿ�Ծ���ſ�˫��!\n����!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
		
			if(c1->p1->w[weaponlimit-1]==true){room_broadcast(this_room,"����������!û�������ܹ���Զ������!���ֿ��׷���!");return true;}//���װ����������������

			if(auto_judge(c1,i_will_be_effect_A,this_room)!=i_immune_effect)//���û����������Ѫ������
			{
				whether_get_hurt=true;
			}

			for(count=0;count<weaponlimit;count++)//������װ��
			{
				if(c1->p1->w[count]==false)
				{
					srand((unsigned)time(NULL)*rand()); //���ȡװ��
					card_number=rand()%(10-0);
				
					addtext(StrBuf,"��Զ�������� ");addtext(StrBuf,search_card(p_name,card_number));addtext(StrBuf," !��װ������������!\n");

					weapon_arm(c1,card_number,this_room);
					room_broadcast(this_room,StrBuf);
					memset(StrBuf,0,sizeof(StrBuf));
				}
			}

			if(whether_get_hurt)//���û����������Ѫ������
			{

				result=hit_damage(3,c1,NULL,this_room,locktime,"���ڷ����������ٶȹ��죬���ܵ���3Ѫ�˺�!\n",i_will_be_effect_A);//��Ѫ
				if(result==one_die){room_broadcast(this_room,"ȴ������ʧѪ�������!\n");return one_die;}
			}
			return still_fight;
	
	case 71:// "[Ч����]������\n���Լ��չ��ɹ�ʱ�ɷ������Լ��ٽ���1���չ����˴��չ���ɵ��չ���Ѫ����\n\n";
			c1->p1->hurtmultiple_time=c1->p1->hurtmultiple_time*2;//�˺�����
			result=general_attack(c1,c2,nothing_else,locktime,this_room);
			if(result==force_offline||result==one_die||result==force_end_turn)return result;
			return force_end_event;
	case 72:;// "[Ч����]��졤̫��\n���Լ��ܵ��չ�ʱ�ɷ������Լ���ܱ����չ����Լ������ֿ��������ô��չ��� ��Ѫ������������ȫ���ƻ�\n\n";
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"���ü���֮��Ť���黯����Χ�Ŀռ�!\n");
			room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�ܲ��ȶ��ռ䲨����Ӱ�죬Ѫ������!������ȫ�����ƻ�!\n");
			room_broadcast(this_room,StrBuf);
			discard_all(c1,this_room);//�����ֿ�
			blood_change(-(c2->p1->blood/2),c2,this_room);//Ѫ������
			delete_weapon_all(c2,this_room);//����ȫ���ƻ�
			return i_failed_attack_E;//�����
	case 73:// "[Ч����]��Ӱ��\n���˿���Ϊ ��+2 ���������Լ�װ���ϣ���װ���ϴ˿��ĻغϽ���ʱ���˿��Զ����ƻ�\n\n";
			memset(StrBuf,0,sizeof(StrBuf));
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"���һ�ѻ�Ӱ��!\n");
			room_broadcast(this_room,StrBuf);
		
			weapon_arm(c1,10,this_room);
			return still_fight;	
			
	case 74:// "[Ч����]���֡��޽�\n���Լ��ܵ�Ч����Ѫʱ�ɷ������Լ���1�ֿ��е�װ������ָ��1����3Ѫ\n\n";
		
		/*confirm_send_success(c1,"��ָ������������!\n");
		while(1)
		{
			count=choose_enemy(c1,this_room);
			if(count==force_offline)return force_offline;
			if(count==i_cancel_effect)
				{confirm_send_success(c1,"��ƬЧ���ѷ���!�޷�ȡ��!\n");continue;} 
			break;
		}
		if(c1->p1->player_No==this_room->c1->p1->player_No)
			c2=this_room->c2;
		else
			c2=this_room->c1;
		*/
		card_number=discard_choose_weapon(c1,this_room);
		if(card_number==force_offline)return force_offline;

		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"�ٻ�����˪��ħ������ ");
		addtext(StrBuf,search_card(p_name,card_number));addtext(StrBuf," !�����˳�ȥ!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"������!�ܵ���2Ѫ�˺�!\n");
		result=hit_damage(2,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
		if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

		return still_fight;//������������¼�����

	case 75:;// "[Ч����]�����\n�˿������Ļغ��Լ��������2�ֻ��ڣ��Լ��ɶ�1�ֿ�ʹ 1�ζԼ���Ч����Ѫ ��Ч����ֵ�1���չ�\n\n";
			set_buff(c1,hideshield_turn,3,this_room);
			result=ask_whether_do(c1,make_shield);
			if(result==force_offline)return force_offline;
			if(result==cancel_choice)return still_fight;

			result=discard_choose(1,c1,this_room);
			if(result!=card_discard){return result;}
			
			room_broadcast(this_room,"������������ʱ�Ƴ���һ��͸����Ͼ�ּ�Ӳ�ޱȵķ�����!\n");
			
			if(situation==i_will_be_attacked)
			{room_broadcast(this_room,"���ɹ������չ�!\n");return i_failed_attack_D;}	//���չ�
			if(situation==i_will_be_effect_B)
			{room_broadcast(this_room,"���ɹ���������ѪЧ��!\n");return i_failed_effect_B;}//��Ч����ѪЧ��	
			confirm_send_success(c1,"�ڲ���Ҫ�ĳ����·�����Ч����ûɶ��\n");return still_fight;
			
	
	case 76:// "[Ч����]�ƾ֡����\n���Լ��չ�ʧ��ʱ�ɷ������Լ��չ� ʹ�Լ��չ�ʧ���� 1�Σ��Ҵ˴��չ��� ������ �޷��ж����Լ��ɶ������������ֿ���ʹ ������ �ܵ�����������Ѫ\n\n";		
			if(c1->member_No==this_room->c[0]->member_No)
				c2=this_room->c[1];
			else
				c2=this_room->c[0];
			if(c2==NULL)return force_offline;
			addtext(StrBuf,c1->p1->name);addtext(StrBuf,"�����˻��ҵľ���!��");
			addtext(StrBuf,c2->p1->name);addtext(StrBuf,"���������յ�����һ��!\n");
			room_broadcast(this_room,StrBuf);

			set_buff(c2,unmove_time,0,this_room);//�������޷��ж�

			confirm_send_success(c1,"�붪�����������ֿ�!\n");
			count=discard_free_choose(c1,false,this_room);
			if(count>0)result=hit_damage(float(count),c2,c1,this_room,locktime,"���ܵ����Ƹ񼶵������˺���\n",i_will_be_effect_B);//��Ѫ
			if(result==force_offline||result==one_die||result==force_end_turn||result==force_end_event)return result;

			result=general_attack(c1,c2,nothing_else,locktime,this_room);
			if(result==one_die||result==force_offline||result==force_end_turn)return result;
			return force_end_event;
	case 77://"[Ч����]���ء��ػ�\n���Լ��ܵ��չ�ʱ�ɷ������Լ���1�����Լ����չ� �����չ��� 1�Σ����ι����Լ���+1";
			
			addtext(StrBuf,c1->p1->name);addtext(StrBuf," �����ܼ����չ��ĳ�����Լ���Ӱ�죬���»���ΪȦ ��ס���ƣ���̽�µ�����\n");
			room_broadcast(this_room,StrBuf);
			get_card(1,c1,this_room);
			result=ask_whether_do(c1,do_attack);
			if(result==force_offline)return result;

			room_broadcast(this_room,"�������ƺ��������˻ػ���");
			attack_change(1,&c1->p1->exattack_time,c1,this_room);//������+1
			result=general_attack(c1,c2,nothing_else,locktime,this_room);
			if(result==one_die||result==force_offline||result==force_end_turn)return result;
			return force_end_event;	
	case 400://����
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," ��������������!\n");
		room_broadcast(this_room,StrBuf);
		result=general_attack(c1,c2,nothing_else,locktime,this_room);
		return result;

	case 401://��
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," �����˹���!\n");
		room_broadcast(this_room,StrBuf);
		return i_failed_attack_E;
	case 402://+4Ѫ
			
			choice=choose_someone(c1,"��ָ��һ��+4Ѫ\n",this_room);
			if(choice==i_cancel_effect)return still_fight;
			if(choice==force_offline){return choice;}
			c2=this_room->c[choice];

			addtext(StrBuf,c2->p1->name);addtext(StrBuf," �ظ�4��Ѫ��!\n");
			room_broadcast(this_room,StrBuf);
			recover_blood(4,c2,this_room);//Ѫ������4
			return still_fight;
	case 403://403 ��ǹ

		addtext(StrBuf,c1->p1->name);addtext(StrBuf," ���䲻��˦���˰��ܷ���!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�ܵ��˷�����˺�!\n");
		result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
		return result;
	case 404://404 ������
		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"����������ʵ�ǻ��󵾲���!����װ����ȫ���ƻ�\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		delete_weapon_all(c2,this_room);

		return i_failed_attack_E;
	case 405://405 ����
		if(situation==sb_card_used)
			return i_failed_effect;
		else
		{
			special_signal(c1,which_to_hatred);
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}
			switch(atoi(StrBuf))
			{
				case 1://��������Ч��
					memset(StrBuf,0,sizeof(StrBuf));
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"������Ч������Ч��1�غ�!\n");
					room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
					set_buff(c2,silence_turn,1,this_room);break;
				case 2://��������buff
					memset(StrBuf,0,sizeof(StrBuf));
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"������Ӹ�Ч���������!\n");
					room_broadcast(this_room,StrBuf);
					set_buff(c2,buff_clean_all,0,this_room);break;
				case 3://��������buff
					memset(StrBuf,0,sizeof(StrBuf));
					addtext(StrBuf,c2->p1->name);addtext(StrBuf,"������Ӹ�Ч���������!\n");
					room_broadcast(this_room,StrBuf);
					set_buff(c1,buff_clean_all,0,this_room);break;
				case 4://�ƻ�����һ������
					special_signal(c1,get_w_number);
					memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return force_offline;}
					choice=atoi(StrBuf);
					choice--;
					if(choice>=0&&choice<weaponlimit&&c2->p1->w[choice]&&c2->p1->w_valid[choice])
					{
						memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c2->p1->name);addtext(StrBuf,"������");addtext(StrBuf,search_card(p_name,c2->p1->weapon[choice]));addtext(StrBuf,"���ƻ���!\n");
						room_broadcast(this_room,StrBuf);
						delete_weapon(c2,choice,this_room);
					}
					else
					{
						cout<<"����Ļظ���"<<choice<<endl;
					}
					
					break;
				default:cout<<"�����ѡ��"<<atoi(StrBuf)<<endl;break;
			}
		}
		if(judge_one_die(c1,this_room)==one_die||judge_one_die(c2,this_room)==one_die)return one_die;
		return still_fight;
		break;
	case 406://406 ������
		addtext(StrBuf,c1->p1->name);addtext(StrBuf," ���Ź������һ������!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�ܵ��˼��������˺�!\n");
		result=hit_damage(2,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
		return result;
	case 407://407 ��������
		int hit_time=0;//���д���
		addtext(StrBuf,c1->p1->name);addtext(StrBuf,"��");addtext(StrBuf,c2->p1->name);addtext(StrBuf,"Ͷ�������Ʊ���!\n");
		room_broadcast(this_room,StrBuf);memset(StrBuf,0,sizeof(StrBuf));

		for(count=0;count<3;count++)
			if(whether_it_happen(50))
			{
				addtext(StrBuf,c2->p1->name);addtext(StrBuf,"�ܵ��˱���ը�Ѵ����Ľ����˺�!\n");
				result=hit_damage(1,c2,c1,this_room,locktime,StrBuf,i_will_be_effect_B);//��Ѫ
				if(result==one_die||result==force_offline||result==force_end_turn)return result;
				if(result!=i_failed_effect_B)hit_time++;
			}
		if(hit_time>=3)//�������ȫ��������װ��ȫ�����ƻ�
		{
			addtext(StrBuf,"����3��ȫ��!");addtext(StrBuf,c1->p1->name);addtext(StrBuf,"������ȫ�����ƻ�!\n");
			room_broadcast(this_room,StrBuf);
			delete_weapon_all(c2,this_room);
		}

		return still_fight;
	}
	cout<<"����Ī������Ź���!!����Ϊ:"<<card_No<<endl;
	return false;
}


int launch_buff_effect(client_member* c1,int choice,int situation,int* locktime,fight_room* this_room)//��������Ч��
{
	int result=still_fight;
	switch(choice)
	{
		case 75://�����		�˿������Ļغ��Լ��������2�ֻ��ڣ��Լ��ɶ�1�ֿ�ʹ 1�ζԼ���Ч����Ѫ ��Ч����ֵ�1���չ�\n\n";
			if(c1->p1->cannot_ED)
			{confirm_send_success(c1,"�Լ��޷���ܺ͸�!\n");return i_cancel_effect;}
			if(auto_judge(c1,i_will_discard_card_A,this_room)!=i_immune_effect)
			{
				result=discard_choose(1,c1,this_room);
				if(result==few_card){confirm_send_success(c1,"�ֿ�����!�޷�����!\n");return i_cancel_effect;}
				if(result==i_cancel_effect){return i_cancel_effect;}
			}
			room_broadcast(this_room,"������������ʱ�Ƴ���һ��͸����Ͼ�ּ�Ӳ�ޱȵķ����ܸ����˺�!\n");
			return i_failed_damage;	//��Ч���˺�
			
			
	}
	cout<<"�Ӹ�Ч����������choice="<<choice<<endl;
	return still_fight;
}
//�����������������������������������������������������Ը���Ч���Ĵ������������������������������������������������

#endif