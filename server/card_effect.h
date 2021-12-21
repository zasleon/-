#include "SZSC_property.h"


#define card_all_number		20000//��Ƭ����
#define card_effect_limit	5//��Ƭ����Ч������������
#define conditionlimit		3//��ɷ�����������
#define benefitlimit		3//��һ�ȡ��������
#define sacrificelimit		3//��������ʱ�����
#define value_numberlimit	3//Ч�����Ӧ��ֵ����������,һ������ֵ��׷�ӵ��Ƕ���Ч��
#define benefit_ex_limit	1//ÿ��Ч����ɺ��׷��Ч����������

#define NONE_CONTENT -1//������





struct effect_Authority
{
	
	char effect_sentence[100];

	int condition[conditionlimit];//����Ч��
	int condition_property[conditionlimit];//�������ԣ�������ɸ��������ɷ�����ͬʱ���������ʱ�ſɷ�����
	int condition_number[conditionlimit][value_numberlimit];//����Ч����Ӧ��ֵ
	

	//���������������ʱ�
	int sacrifice[sacrificelimit];//����Ч��
	int sacrifice_number[sacrificelimit][value_numberlimit];//Ч����Ӧ��ֵ
	

	//��������ȡ����
	int benefit[benefitlimit];//�Լ��������Ч��
	int benefit_number[benefitlimit][value_numberlimit];//�������Զ�Ӧ��ֵ

};
struct card_Authority
{
	effect_Authority effect[card_effect_limit];//Ч��ȫ������
	int effect_property[card_effect_limit];//Ч�����ԣ������������������Զ��������������������

	int card_No;//��Ƭ���
	char card_name[30];//��Ƭ����
	int card_property;//��Ƭ���ԣ�����/Ч��/����/����
};

card_Authority szsc_card[card_all_number];//���п�Ƭ

//��տ�Ƭ����
void clear_szsc_card()
{
	for(int i=0;i<card_all_number;i++)//ȫ�����
	{
		for(int j=0;j<card_effect_limit;j++)
		{
			for(int k=0;k<conditionlimit;k++)
			{
				szsc_card[i].effect[j].condition[k]=NONE_CONTENT;
				szsc_card[i].effect[j].condition_property[k]=NONE_CONTENT;
				for(int l=0;l<value_numberlimit;l++)
					szsc_card[i].effect[j].condition_number[k][l]=NONE_CONTENT;
			}
			for(int k=0;k<sacrificelimit;k++)
			{
				szsc_card[i].effect[j].sacrifice[k]=NONE_CONTENT;
				for(int l=0;l<value_numberlimit;l++)
					szsc_card[i].effect[j].sacrifice_number[k][l]=NONE_CONTENT;
			}
			for(int k=0;k<benefitlimit;k++)
			{
				szsc_card[i].effect[j].benefit[k]=NONE_CONTENT;
				for(int l=0;l<value_numberlimit;l++)
				{
					szsc_card[i].effect[j].benefit_number[k][l]=NONE_CONTENT;
				}
			}
			
		}

	}
}

#define P_weapon_card				6000//������
#define P_effect_card				6001//Ч����
#define P_assist_attack_card		6002//������
#define P_totem_card				6003//��翨
#define P_field_card				6004//������
#define P_character_card			6005//��ɫ��

#define independent_condition		6888//������ɸ�Ч�����ɷ���
#define associate_condition			6889//��Ҫͬʱ�����Ч��
#define passive_effect				6890//������Ч��
#define	active_effect				6891//��Ҫ�����������
#define active_auto_effect			6892//���㷢���������Զ����Ҳ����������

//���øÿ�Ƭ�����ֺ�����
void set_card_property(card_Authority* this_card,int card_No,char* name,int card_property)
{
	this_card->card_No=card_No;
	memset(this_card->card_name,0,sizeof(this_card->card_name));
	strcat(this_card->card_name,name);
	this_card->card_property=card_property;
}
//�趨��Ч˵���ַ�
void add_sentence(card_Authority* this_card,int e_number,char* sentence)
{
	strcat(this_card->effect[e_number].effect_sentence,sentence);
}
//�趨��Ч�����ԣ�������������
void add_EP(card_Authority* this_card,int e_number,int effect_property)
{
	this_card->effect_property[e_number]=effect_property;
}
//�������Ч��
void add_B(card_Authority* this_card,int e_number,int E1,int EV1)//��Ƭ����Ƭ�ڼ���Ч��������Ч����Ч��ֵ
{
	e_number--;
	for(int i=0;i<benefitlimit;i++)
		if(this_card->effect[e_number].benefit[i]==NONE_CONTENT)//�����ǰЧ����Ϊ�գ�����Ч��
		{
			this_card->effect[e_number].benefit[i]=E1;
			this_card->effect[e_number].benefit_number[i][0]=EV1;
			return;
		}
}
void add_B(card_Authority* this_card,int e_number,int E1,int EV1,int EV2)//��Ƭ����Ƭ�ڼ���Ч��������Ч����Ч��ֵ
{
	e_number--;
	for(int i=0;i<benefitlimit;i++)
		if(this_card->effect[e_number].benefit[i]==NONE_CONTENT)//�����ǰЧ����Ϊ�գ�����Ч��
		{
			this_card->effect[e_number].benefit[i]=E1;
			this_card->effect[e_number].benefit_number[i][0]=EV1;
			this_card->effect[e_number].benefit_number[i][1]=EV2;
			return;
		}
}
void add_B(card_Authority* this_card,int e_number,int E1,int EV1,int EV2,int EV3)//��Ƭ����Ƭ�ڼ���Ч��������Ч����Ч��ֵ
{
	e_number--;
	for(int i=0;i<benefitlimit;i++)
		if(this_card->effect[e_number].benefit[i]==NONE_CONTENT)//�����ǰЧ����Ϊ�գ�����Ч��
		{
			this_card->effect[e_number].benefit[i]=E1;
			this_card->effect[e_number].benefit_number[i][0]=EV1;
			this_card->effect[e_number].benefit_number[i][1]=EV2;
			this_card->effect[e_number].benefit_number[i][2]=EV3;
			return;
		}
}
//��������ʱ�
void add_S(card_Authority* this_card,int e_number,int S1,int SV1)
{
	e_number--;
	for(int i=0;i<sacrificelimit;i++)
		if(this_card->effect[e_number].sacrifice[i]==NONE_CONTENT)
		{
			this_card->effect[e_number].sacrifice[i]=S1;
			this_card->effect[e_number].sacrifice_number[i][0]=SV1;
			return;
		}
}
//�������
void add_C(card_Authority* this_card,int e_number,int condition_property,int C1,int CV1)
{
	e_number--;
	for(int i=0;i<card_effect_limit;i++)
		if(this_card->effect[e_number].condition[i]==NONE_CONTENT)
		{
			this_card->effect[e_number].condition[i]=C1;
			this_card->effect[e_number].condition_number[i][0]=CV1;
			this_card->effect[e_number].condition_property[i]=condition_property;
			return;
		}
}




//-----------------------------------condition---------------------------------------
#define my_turn_not_in_fight		1050//�Լ��Ļغ����Լ�δ���ڲ���״̬

#define i_am_wearing_this_weapon	1100//�Լ���װ�����������
#define	i_armed						1101//�Լ�װ��������

#define i_have_card					1111//��������x���ֿ�
#define i_failed_attack				1112//�Լ��չ�ʧ�ܣ���������ܱ��񵲱������ˣ�

#define	we_card_used				1120//����ÿ�η����ֿ�ʱ

#define limit_times_all				2000//�ܹ�����ʹ�ô���
#define one_turn_limit_times		2001//ÿ�غ�����ʹ�ô���
#define my_turn_limit_times			2002//ÿ���Լ��غ�����ʹ�ô���

#define someone_die_but_not_me		2050//���ϳ�����1������

#define lock_time_plenty			2060//���������ﵽҪ��+�㹻ֵ

#define token_plenty				2070//ָʾ�������㹻��+�㹻ֵ

//-----------------------------------condition---------------------------------------
//-----------------------------------sacrifice-----------------------------------
#define i_discard_card			3000//�Ҷ����ֿ�+����
#define i_discard_all_card		3001//�Ҷ��������ֿ�

#define i_consume_token			3005//������ָʾ��

#define i_discard_this_weapon		3010//����������������
#define i_destroy_this_weapon		3011//�������ƻ�������

#define delete_this_effect			3015//��Ч�����꣬��ɾ��

#define use_check_durability		3020//ʹ��ʣ�����-1�����ʣ��Ч��ʹ�ô���
//-----------------------------------sacrifice-----------------------------------
//-----------------------------------benefit---------------------------------------
#define weapon_provide_attack_P		4000//�����ṩ�����ù�����
#define weapon_provide_attack_T		4001//�����ṩ�ĸûغ������Ĺ�����
#define weapon_increase_attack_P	4002//�����������ӵ����ù�����


#define my_this_weapon_get_token	5005//�����������ȡָʾ��

#define i_increase_attack_this_time	5020//�ұ����չ�����������
#define	i_lose_half_attack_time		5021//�ұ����չ�����������
#define penetrating_attack_time		5022//�����չ���Ϊ����
#define	do_overspeed_attack			5023//����һ�γ����չ���+�����չ�����˺�ֵ

#define i_point_effect_B		5050//��ָ��1�˷���Ч������Ѫ
#define i_point_effect_B_R		5051//��ָ��1�˷����������и��ʵ�Ч������Ѫ��+����+��Ѫ��ֵ
#define	i_point_effect_B_CE		5052//��ָ��1�˷���Ч������Ѫ�����ֿɶ�1�ֿ����

#define i_effect_B_CE			5053//�Ҷ��䷢����ѪЧ���������ֿɶ�1�ֿ����
#define i_effect_B_R			5054//��ʹ���ָ�����Ѫ��+����+��Ѫ��ֵ
#define i_effect_B_all_rival	5055//ʹ�����������е�����Ѫ��+��Ѫ��ֵ

#define i_point_cure			5075//��ָ��1�˻ظ�Ѫ��
#define i_cure_myself_const		5080//���Լ��ظ��̶���ֵ��Ѫ��
#define i_cure_myself_damage	5081//���Լ��ظ��Լ�����˺���Ѫ��(����֮��������)
#define i_cure_myself_all		5082//���Լ�Ѫ������


#define i_destroy_his_weapon_random		5100//������ƻ�����һ�����е�����
#define i_destroy_all_rival_weapon		5110//�ƻ��з�����װ��������

#define i_get_overdeath_buff			5110//�һ�ò���buff

#define everyone_discard_card_R			6000//������������ֿ�
#define everyone_get_card				6001//�����˻�ȡ��

#define debuff_unmovable_turn			6011//�ö�����n�غ��޷��ж���+�غ���
#define	debuff_unmovable_time			6012//�ö����ڴ˴��ж�����ǰ�޷��ж�
#define debuff_unmovable_turn_R			6013//�и��ʣ��ö��ּ��غ����޷��ж���+����+�غ���
#define debuff_discard_card_R			6014//�ö�����������ֿ���+����
#define debuff_discard_card_R_R			6015//�и��ʣ��ö�����������ֿ���+����+����
#define	debuff_destroy_all_weapon_R		6016//�и��ʣ�����װ��������ȫ���ƻ���+����

#define	rival_get_card					6025//�ö��ֳ鿨��+����

#define i_check_and_plunder_his_card	6030//�Ҳ鿴�����������ֿ�
#define if_plunder_weapon_then_arm		6031//���������װ������װ��

#define my_weapon_invalid_in_this_attack	6040//�����չ��Լ�����ȫ����Ч��	

#define buff_if_i_attack				6050//���Լ���buff����buff���Լ��չ��ɹ��󷢶�
#define buff_this_attack_cant_miss		6060//�����չ�����

#define i_effect_B_twice_my_invalid_weapon_and_return	6090//��������Լ���������Ч������*2��ֵ��Ч������Ѫ

//-----------------------------------benefit---------------------------------------
void ini_weapon_card()//��ʼ��������Ƭ����
{
	//�лʽ�
	set_card_property(&szsc_card[0],0,"�лʽ�",P_weapon_card);
	add_C(&szsc_card[0],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[0],1,weapon_provide_attack_P,1);//�����ṩ����+1
	add_EP(&szsc_card[0],1,passive_effect);//����Ч��

	add_C(&szsc_card[0],2,associate_condition,one_turn_limit_times,1);//ÿ�غ�����ʹ�ô���,1��
	add_C(&szsc_card[0],2,associate_condition,i_have_card,1);//�����ֿ�
	add_S(&szsc_card[0],2,i_discard_card,1);//�Ҷ���һ���ֿ�
	add_B(&szsc_card[0],2,i_get_card,1);//�ҳ�1��
	add_EP(&szsc_card[0],2,active_effect);//����Ч��
	//add_sentence(&szsc_card[0],2,"player1�����˳лʽ��ĿƼ�֮�ˣ�\n����������������\n");

	add_C(&szsc_card[0],3,associate_condition,i_will_be_attacked,NONE_CONTENT);//���ܵ��չ�ʱ�ɷ���
	add_C(&szsc_card[0],3,associate_condition,limit_times_all,4);//�ܹ���ʹ��4��
	add_S(&szsc_card[0],3,use_check_durability,i_destroy_this_weapon);//���ü���ʹ�ô����Ƿ����꣬��������������ƻ�
	add_B(&szsc_card[0],3,i_defend_attack,1);//�Ҹ����չ�
	add_EP(&szsc_card[0],3,active_effect);//����Ч��
	//add_sentence(&szsc_card[0],2,"player1���лʽ��л�Ϊ��ģʽ��\n");

	add_C(&szsc_card[0],4,associate_condition,my_turn,NONE_CONTENT);//�ҵĻغ�������ʹ��
	add_S(&szsc_card[0],4,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[0],4,i_point_effect_B,1);//��ָ��1�˷���Ч������Ѫ������Ϊ1
	add_EP(&szsc_card[0],4,active_effect);//����Ч��
	//add_sentence(&szsc_card[0],2,"player1�ɳ������еĳлʽ���\n");

	//������
	set_card_property(&szsc_card[1],1,"������",P_weapon_card);
	add_C(&szsc_card[1],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[1],1,weapon_provide_attack_P,1);//�����ṩ����+1
	add_EP(&szsc_card[1],1,passive_effect);//����Ч��

	add_C(&szsc_card[1],2,associate_condition,one_turn_limit_times,2);//ÿ�غ�����ʹ�ô���,2��
	add_C(&szsc_card[1],2,associate_condition,i_have_card,1);//�����ֿ�
	add_S(&szsc_card[1],2,i_discard_card,1);//�Ҷ���һ���ֿ�
	add_B(&szsc_card[1],2,weapon_provide_attack_T,1);//�Լ��˻غϹ�+1
	add_EP(&szsc_card[1],2,active_effect);//����Ч��

	add_C(&szsc_card[1],3,associate_condition,my_turn,NONE_CONTENT);//�ҵĻغ�������ʹ��
	add_S(&szsc_card[1],3,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[1],3,i_point_effect_B,1);//��ָ��1�˷���Ч������Ѫ������Ϊ1
	add_EP(&szsc_card[1],3,active_effect);//����Ч��

	//����֮��
	set_card_property(&szsc_card[2],2,"����֮��",P_weapon_card);
	add_C(&szsc_card[2],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[2],1,weapon_provide_attack_P,2);//�����ṩ����+2
	add_EP(&szsc_card[2],1,passive_effect);//����Ч��

	add_C(&szsc_card[2],2,associate_condition,i_hit_someone,NONE_CONTENT);//����ĳ���ܵ��˺���
	add_B(&szsc_card[2],2,i_cure_myself_damage,NONE_CONTENT);//���Լ��ظ��Լ�����˺���Ѫ��
	add_EP(&szsc_card[2],2,passive_effect);//����Ч��

	add_C(&szsc_card[2],3,associate_condition,my_turn,NONE_CONTENT);//�ҵĻغ�������ʹ��
	add_S(&szsc_card[2],3,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[2],3,i_point_cure,3);//��ָ��1�˻ظ�3Ѫ
	add_EP(&szsc_card[2],3,active_effect);//����Ч��

	//�ɽ�֮��
	set_card_property(&szsc_card[3],3,"�ɽ�֮��",P_weapon_card);
	add_C(&szsc_card[3],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[3],1,weapon_provide_attack_P,1);//�����ṩ����+1
	add_EP(&szsc_card[3],1,passive_effect);//����Ч��

	add_C(&szsc_card[3],2,associate_condition,has_fought,NONE_CONTENT);//����ĳ�˲�����
	add_B(&szsc_card[3],2,i_destroy_his_weapon_random,NONE_CONTENT);//������ƻ������ֳֵ�һ������
	add_B(&szsc_card[3],2,weapon_increase_attack_P,1);//���Լ�������������Թ�+1
	add_EP(&szsc_card[3],2,passive_effect);//����Ч��
	//������Ϊÿ�β����󽫶������һ�����������һ��Ч������������������Ч������ƻ�������
	//������Խ��Խ�࣬����Խ��Խ���ߣ���Ч���ͱ��̫��������Ҫ�ܳ�ʱ���ս�������ƻ����˵���������˺���������ǿ����Ϊս����ֱ���ƻ�һ���������һ������޳ɳ�����

	add_C(&szsc_card[3],3,associate_condition,my_turn,NONE_CONTENT);//�ҵĻغ�������ʹ��
	add_S(&szsc_card[3],3,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[3],3,i_point_effect_B,1);//��ָ��1�˷���Ч������Ѫ������Ϊ1
	add_EP(&szsc_card[3],3,active_effect);//����Ч��

	//���׽�
	set_card_property(&szsc_card[4],4,"���׽�",P_weapon_card);
	add_C(&szsc_card[4],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[4],1,i_be_effect_A,3);//�������ܵ�3Ѫ���˺�
	add_B(&szsc_card[4],1,weapon_provide_attack_P,2);//�����ṩ����+1
	add_EP(&szsc_card[4],1,passive_effect);//����Ч��
	//�����ܵ���Ѫ�����װ���ϴ�����������װ���ϴ��������ܵ���Ч��Ϊ��3Ѫ�����Բ���add_S������add_B

	add_C(&szsc_card[4],2,independent_condition,limit_times_all,1);//������ֻ�ܷ���1�θ�Ч��
	add_C(&szsc_card[4],2,independent_condition,i_will_die,NONE_CONTENT);//�Լ���������ʱ���Զ������
	add_S(&szsc_card[3],2,delete_this_effect,NONE_CONTENT);//����������ӵ�д�Ч��
	add_B(&szsc_card[4],2,i_get_overdeath_buff,3);//�һ�ò���buff���ֻ�
	add_EP(&szsc_card[4],2,active_auto_effect);//����Ч��,Ҳ�ɱ�������
	//�����ƣ��������������ǰ��潣�ģ���˼�������û�ˣ�����buffҲ��ǿ��ֱ����ʧ�������0Ѫ״̬�¸ý����ƻ�����һ�ֱ�ӱ��ж�����
	//���������������̫���ˣ���ֵ�õ���debuff��-3Ѫ�����Ը�Ϊһֱ�и�buff

	add_C(&szsc_card[4],3,associate_condition,my_turn,NONE_CONTENT);//�ҵĻغ�������ʹ��
	add_S(&szsc_card[4],3,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[4],3,i_point_effect_B_R,50,2);//��ָ��1��ָ��1��50%-2Ѫ
	add_EP(&szsc_card[4],3,active_effect);//����Ч��

	//ʳ������װ��
	set_card_property(&szsc_card[5],5,"ʳ������װ��",P_weapon_card);
	add_C(&szsc_card[5],1,associate_condition,i_get_card,NONE_CONTENT);//�Լ�ÿ��1��
	add_B(&szsc_card[5],1,i_cure_myself_const,1);//�Լ�Ѫ+1
	add_EP(&szsc_card[5],1,passive_effect);//����Ч��

	add_C(&szsc_card[5],2,associate_condition,one_turn_limit_times,1);//ÿ�غ�����ʹ�ô���,1��
	add_C(&szsc_card[5],2,associate_condition,i_have_card,1);//�����ֿ�
	add_S(&szsc_card[5],2,i_discard_card,1);//�Լ���1�ֿ�
	add_B(&szsc_card[5],2,i_cure_myself_const,1);//�Լ�Ѫ+1
	add_EP(&szsc_card[5],2,active_effect);//����Ч��

	add_C(&szsc_card[5],3,associate_condition,my_turn_limit_times,1);//�Լ��غ�����ʹ�ô���,1��
	add_B(&szsc_card[5],3,i_get_card,1);//�Լ���1��
	add_EP(&szsc_card[5],3,active_effect);//����Ч��

	//���շ���
	set_card_property(&szsc_card[6],6,"���շ���",P_weapon_card);
	add_C(&szsc_card[6],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[6],1,my_this_weapon_get_token,2);//�����װ����ȡ2ָʾ��
	add_EP(&szsc_card[6],1,passive_effect);//����Ч��

	add_C(&szsc_card[6],2,associate_condition,we_card_used,NONE_CONTENT);//����ÿ�η����ֿ�ʱ
	add_B(&szsc_card[6],2,my_this_weapon_get_token,1);//�����װ����ȡ1ָʾ��
	add_EP(&szsc_card[6],2,passive_effect);//����Ч��

	add_C(&szsc_card[6],3,associate_condition,my_turn,NONE_CONTENT);
	add_C(&szsc_card[6],3,associate_condition,token_plenty,2);//ȷ������������ָʾ��
	add_S(&szsc_card[6],3,i_consume_token,2);//��������ָʾ��
	add_B(&szsc_card[6],3,i_point_effect_B,1);//��ָ��1�˷���Ч������Ѫ������Ϊ1
	add_EP(&szsc_card[6],3,active_effect);//����Ч��

	//���ù���
	set_card_property(&szsc_card[7],7,"���ù���",P_weapon_card);
	add_C(&szsc_card[7],1,independent_condition,i_will_public_card,NONE_CONTENT);//���ܵ��ֿ�����Ч��
	add_C(&szsc_card[7],1,independent_condition,i_will_discard,NONE_CONTENT);//�Լ��ܵ�����Ч��Ӱ��ʱ
	add_B(&szsc_card[7],1,i_immune_effect,1);//�����ߴ˴�Ч��
	add_EP(&szsc_card[7],1,passive_effect);//����Ч��

	add_C(&szsc_card[7],2,associate_condition,someone_die_but_not_me,NONE_CONTENT);//���ϳ�����1������ʱ
	add_B(&szsc_card[7],2,i_cure_myself_all,1);//��Ѫ������
	add_EP(&szsc_card[7],2,passive_effect);//����Ч��

	add_C(&szsc_card[7],3,associate_condition,i_will_be_effect_A,NONE_CONTENT);//������ѪЧ��
	add_B(&szsc_card[7],3,i_immune_effect,1);//�����ߴ˴�Ч��
	add_EP(&szsc_card[7],3,passive_effect);//����Ч��

	//�ư�����
	set_card_property(&szsc_card[8],8,"�ư�����",P_weapon_card);
	add_C(&szsc_card[8],1,associate_condition,i_will_be_unmoveable,NONE_CONTENT);//�޷��ж�
	add_B(&szsc_card[8],1,i_immune_effect,1);//�����ߴ˴�Ч��
	add_EP(&szsc_card[8],1,passive_effect);//����Ч��

	add_C(&szsc_card[8],2,associate_condition,i_will_be_attacked,NONE_CONTENT);//�Լ��ܵ��չ�
	add_S(&szsc_card[8],2,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[8],2,i_defend_attack,NONE_CONTENT);//�Լ��ֵ��˴��չ�
	add_EP(&szsc_card[8],2,active_effect);//����Ч��
	//ԭ�����е�Ч�����Լ�������װ��װ������֮���Ϊ���Լ�����װ�������ƻ�Ч����Ӱ�졣����ûȥʵ��

	//�������¸�
	set_card_property(&szsc_card[9],9,"�ư�����",P_weapon_card);
	add_C(&szsc_card[9],1,associate_condition,i_am_wearing_this_weapon,NONE_CONTENT);//װ����ʱ����
	add_B(&szsc_card[9],1,weapon_provide_attack_P,8);//�����ṩ����+1
	add_EP(&szsc_card[9],1,passive_effect);//����Ч��

	add_C(&szsc_card[9],2,associate_condition,i_will_attack,NONE_CONTENT);//�Ҽ����չ�
	add_B(&szsc_card[9],2,everyone_discard_card_R,3);//�����������3��
	add_B(&szsc_card[9],2,everyone_get_card,3);//�����˳�3��
	add_EP(&szsc_card[9],2,passive_effect);//����Ч��

	add_C(&szsc_card[9],3,associate_condition,i_will_be_attacked,NONE_CONTENT);//�Լ��ܵ��չ�
	add_S(&szsc_card[9],3,i_discard_this_weapon,1);//����������������
	add_B(&szsc_card[9],3,everyone_discard_card_R,5);//�����������5��
	add_B(&szsc_card[9],3,everyone_get_card,5);//�����˳�5��
	add_EP(&szsc_card[9],3,active_effect);//����Ч��

}
void ini_assist_card()
{
	//���塤�ظ�
	set_card_property(&szsc_card[30],30,"���塤�ظ�",P_assist_attack_card);
	add_C(&szsc_card[30],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[30],1,i_effect_B_CE,1,debuff_unmovable_time);//�ö�����1Ѫ�����ֿɶ�1�ֿ���ܴ˴���Ѫ��������������ڴ˴��ж�����ǰ�޷��ж�
	add_EP(&szsc_card[30],1,active_effect);//����Ч��

	//����ն
	set_card_property(&szsc_card[31],31,"����ն",P_assist_attack_card);
	add_C(&szsc_card[31],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[31],1,i_increase_attack_this_time,1);//�����չ���������1
	add_B(&szsc_card[31],1,i_check_and_plunder_his_card,1,if_plunder_weapon_then_arm);//�Ҳ鿴���Ӷᱻ�����ߵ�1���ֿ��������װ��������Լ�װ����
	add_EP(&szsc_card[31],1,active_effect);//����Ч��

	//���ɡ���ն
	set_card_property(&szsc_card[32],32,"���ɡ���ն",P_assist_attack_card);
	add_C(&szsc_card[32],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[32],1,my_weapon_invalid_in_this_attack,1);//�����չ��Լ�����ȫ����Ч��
	add_B(&szsc_card[32],1,buff_if_i_attack,i_effect_B_twice_my_invalid_weapon_and_return);//���Լ��˴��չ��ɹ����������Լ���������Ч������*2��ֵ��Ч������Ѫ������������Ч��
	add_EP(&szsc_card[32],1,active_effect);//����Ч��

	//�Եء�����
	set_card_property(&szsc_card[33],33,"�Եء�����",P_assist_attack_card);
	add_C(&szsc_card[33],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_C(&szsc_card[33],1,associate_condition,i_armed,1);//�Լ�����װ����1������
	add_B(&szsc_card[33],1,i_get_card,1);//�Լ���1��
	add_B(&szsc_card[33],1,i_effect_B_CE,1);//�ö�����1Ѫ�����ֿɶ�1�ֿ���ܴ˴���Ѫ
	add_EP(&szsc_card[33],1,active_effect);//����Ч��

	//�����ӻ�
	set_card_property(&szsc_card[34],34,"�����ӻ�",P_assist_attack_card);
	add_C(&szsc_card[34],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[34],1,i_increase_attack_this_time,1);//�����չ���������1
	add_B(&szsc_card[34],1,buff_if_i_attack,debuff_unmovable_turn_R,30,2);//���Լ��˴��չ��ɹ���,30%ʹ���ֽ������Ĵ˻غϺ��»غϲ����ж�
	add_EP(&szsc_card[34],1,active_effect);//����Ч��

	//�����ӻ�
	set_card_property(&szsc_card[35],35,"�����ӻ�",P_assist_attack_card);
	add_C(&szsc_card[35],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[35],1,i_increase_attack_this_time,2);//�����չ���������2
	add_B(&szsc_card[35],1,buff_if_i_attack,debuff_discard_card_R_R,50,1);//���Լ��˴��չ��ɹ���,50%ʹ�����������1���ֿ�
	add_EP(&szsc_card[35],1,active_effect);//����Ч��

	//����ӻ�
	set_card_property(&szsc_card[36],36,"����ӻ�",P_assist_attack_card);
	add_C(&szsc_card[36],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[36],1,i_increase_attack_this_time,1);//�����չ���������1
	add_B(&szsc_card[36],1,buff_if_i_attack,i_effect_B_R,40,1);//�չ��ɹ���40%ʹ����-1Ѫ
	add_EP(&szsc_card[36],1,active_effect);//����Ч��

	//��ˮ�ӻ�
	set_card_property(&szsc_card[37],37,"��ˮ�ӻ�",P_assist_attack_card);
	add_C(&szsc_card[37],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[37],1,i_increase_attack_this_time,1);//�����չ���������1
	add_B(&szsc_card[37],1,buff_if_i_attack,debuff_discard_card_R,2);//���Լ��˴��չ��ɹ���,ʹ�����������2���ֿ�
	add_B(&szsc_card[37],1,buff_if_i_attack,rival_get_card,2);//���Լ��˴��չ��ɹ���,ʹ���ֳ�2��
	add_EP(&szsc_card[37],1,active_effect);//����Ч��

	//���ӻ�
	set_card_property(&szsc_card[38],38,"���ӻ�",P_assist_attack_card);
	add_C(&szsc_card[38],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[38],1,i_increase_attack_this_time,1);//�����չ���������1
	add_B(&szsc_card[38],1,buff_if_i_attack,i_get_card,1);//���Լ��˴��չ��ɹ���,�Լ���1��
	add_EP(&szsc_card[38],1,active_effect);//����Ч��

	//ڤ�ļӻ�
	set_card_property(&szsc_card[39],39,"ڤ�ļӻ�",P_assist_attack_card);
	add_C(&szsc_card[39],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[39],1,i_increase_attack_this_time,1);//�����չ���������1
	add_B(&szsc_card[39],1,buff_if_i_attack,debuff_destroy_all_weapon_R,30);//���Լ��˴��չ��ɹ���,30%ʹ���չ���װ��������ȫ���ƻ�

	//�񡤿ա�ն
	set_card_property(&szsc_card[40],40,"�񡤿ա�ն",P_assist_attack_card);
	add_C(&szsc_card[40],1,associate_condition,i_will_attack,NONE_CONTENT);//�����չ�ʱ�ɷ���
	add_B(&szsc_card[40],1,buff_this_attack_cant_miss,NONE_CONTENT);//�����չ�����
	add_B(&szsc_card[40],1,i_lose_half_attack_time,NONE_CONTENT);//�˴��չ��Լ�����������
	add_B(&szsc_card[40],1,penetrating_attack_time,NONE_CONTENT);//�˴��չ��Լ���Ϊ����
	
}
void ini_effect_card()
{
	//��������
	set_card_property(&szsc_card[60],60,"��������",P_effect_card);
	add_C(&szsc_card[60],1,independent_condition,i_failed_attack,NONE_CONTENT);//�Լ��չ�ʧ��ʱ�ɷ���
	add_S(&szsc_card[60],1,i_discard_all_card,NONE_CONTENT);//�Ҷ����������ֿ�
	add_B(&szsc_card[60],1,i_effect_B_all_rival,1);//ʹ���ж���Ѫ-1
	add_B(&szsc_card[60],1,i_destroy_all_rival_weapon,NONE_CONTENT);//�з�װ����ȫ�����ƻ�
	add_EP(&szsc_card[60],1,active_effect);//����Ч��

	//��������ɱ
	set_card_property(&szsc_card[61],61,"��������ɱ",P_effect_card);
	add_C(&szsc_card[61],1,independent_condition,lock_time_plenty,5);//�Լ������1���¼�����������4��
	add_B(&szsc_card[61],1,do_overspeed_attack,3);//�����չ��˺�Ϊ3
	add_B(&szsc_card[61],1,force_end_turn,NONE_CONTENT);//�˻غϱ�ǿ�ƽ���
	add_EP(&szsc_card[61],1,active_effect);//����Ч��

	//���ơ�ͻ��
	set_card_property(&szsc_card[62],62,"���ơ�ͻ��",P_effect_card);
	add_C(&szsc_card[62],1,independent_condition,my_turn_not_in_fight,NONE_CONTENT);//�Լ������1���¼�����������4��
	add_B(&szsc_card[30],1,i_effect_B_CE,1,debuff_unmovable_turn,1);//�ö�����1Ѫ�����ֿɶ�1�ֿ���ܴ˴���Ѫ��������������ڴ˻غ��޷��ж�
	add_EP(&szsc_card[62],1,active_effect);//����Ч��
	//ħ�ڿ������
}

void ini_szsc_card()//��ʼ����Ƭ����
{
	clear_szsc_card();
	ini_weapon_card();
	ini_effect_card();
	





}

int random_card(int i);//�����������������迨Ƭ��ţ����ֿ�Ƭ�鵽��Ȩ�ز�ͬ�������ʵ�����
int discard_all(client_member* c1,fight_room* this_room);//�����ֿ�
int supply_card();
void refresh_deck(int* deck);//���ÿ��1�ſ�����õ��ù���,��get_card�����Ż��õ��ú���������ˢ�¿������һ��
void ini_deck(int* deck);
void get_card(int number,client_member* c1,fight_room* this_room);															//�����deck����鿨
void get_specific_card(int number,player* p1);
void show_card_name(player* p1)	;
void refresh_card(client_member* p1,fight_room* this_room);                    //����п�ʹ�ú���Ҫ����ʹ���������,�����ֿ������������ֿ�
int discard_choose(int number,client_member* c1,fight_room* this_room);//���������ֿ�������ѡ��number�����Ŀ�
int discard_free_choose(client_member* c1,bool full,fight_room* this_room);//���������ֿ�������ѡ�����������Ŀ�
int random_choose_card(int cardnumber);									//���ˢ�¿�������deck����;
void discard_random(int number,client_member* p1,fight_room* this_room);//�����������number�����Ŀ�
void discard_card(client_member*c1,int whichcard,fight_room* this_room);
int use_card(client_member*c1,int whichcard,bool hide_effect,int* locktime,fight_room* this_room);
void refresh_hideeffect(client_member* c1);//���¿�Ƭ��Ч��

bool judge_assist(int choice);
bool judge_weapon(int choice);
bool judge_weapon_effect(client_member* c1,int card_No,int situation,int* locktime,int whichweapon,fight_room* this_room);
bool judge_card(client_member* c1,int choice,int situation,int* locktime);//situation��������������lock_time��������,������Ϸ�������ֱ��ʹ��
bool judge_character_effect(client_member* c1,int choice,int situation,int* locktime);

int auto_judge(client_member* c1,int situation,fight_room* this_room);
int auto_judge(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime);
int auto_judge(client_member* c1,int situation,float* number,int* locktime,fight_room* this_room);//��������Ƿ��Զ�����������Ч��/������Ч��
int judge_one_die(client_member* c1,fight_room* this_room);//�ж�����û��

int launch_weapon_effect(client_member* c1,int card_No,int whichweapon,int situation,int* locktime,fight_room* this_room);//��������Ч��
int character_effect(client_member* c1,int card_No,fight_room* this_room);			//�Լ�����������������Ч��
int launch_assist_effect(client_member* c1,client_member* c2,int card_No,int stage,int* locktime,fight_room* this_room);
int launch_card_effect(client_member* c1,client_member* c2,int card_No,int situation,int* locktime,fight_room* this_room);//�����ֿ�
int launch_buff_effect(client_member* c1,int choice,int situation,int* locktime,fight_room* this_room);//��������Ч��

//int choose_enemy(client_member* c1,fight_room* this_room);//ѡ��ָ���Ķ���(�ú���Ϊͨ�ú���u),�������Լ�
//int choose_someone(client_member* c1,fight_room* this_room);//ѡ��ָ���Ķ���(�ú���Ϊͨ�ú���u)�������Լ�
bool whether_it_happen(int percent);
void turnsettle(client_member* c1,fight_room* this_room)						;								//�غϽ���ʱ�������״̬����
void natural_set_playerturn(fight_room* this_room)	;						//��Ȼ�����һغ�
void ini_character(player* p1,int playerNo);								//�Դ�������p�ĸ���״̬���г�ʼ��
void set_buff(client_member* c1,int situation,int number,fight_room* this_room);//����c1״̬��situationΪ״̬��numberΪ�����غ�
void fight_data_clean(player* p1,player* p2,bool clean,fight_room* this_room);//�ô��չ���������Ч��ֵ������
void show_character(player* p1,bool i);															//��ʾ����״̬

int hit_damage(float damage,client_member* c1,client_member* c2,fight_room* this_room,int* locktime,char* StrBuf,int situation);//c2��c1���damage��Ѫ
int fight_back(client_member* c1,client_member* c2,int situation,fight_room* this_room,int* locktime);
int general_attack(client_member* c1,client_member* c2,int situation,int* locktime,fight_room* this_room);
void recover_blood(float number,client_member* c1,fight_room* this_room);
int lightspeed_attack(client_member* c1,client_member* c2,float damage,int* locktime,fight_room* this_room);//�����չ�,c1��c2

void refresh_weapon(player* p1,fight_room* this_room);//��������仯�Ż��õ��ú���������ˢ��������
void delete_weapon(client_member* c1,int whichweapon,fight_room* this_room);//ɾ��ĳ�����ڵ�һ������,�òۺ�Ϊwhichweapon
void delete_weapon_all(client_member* c1,fight_room* this_room);//ɾ�������ȫ������
bool weapon_arm(client_member* c1,int choice,fight_room* this_room);//��������������choice����װ��

void confirm_send_success(client_member* member,char* StrBuf);//���Է���Ϣ�Ƿ��յ�����StrBuf
void check_emptyspace();//���㵱ǰ���޷�������λ
void show_all_client(client_member* member);//��ͻ�����ʾ����������Ա�б�
void exit_the_room(client_member* member);//�û��˳�����
//void client_offline(client_member* member);//�ͻ������߽���
void init_server_set();//��ʼ������������
bool check_empty_room();//�����û�пշ��䣬�еĻ�����true��û���򷵻�false
bool check_in_use_room();//�����û�������÷���
void show_all_room(client_member* member);//��ʾ��ʹ�õķ���
bool check_client_reply(client_member* c1);
//special_transmit.h
void sp_transmit(client_member* c1,float message);//�ػ���Ϣ����
void sp_transmit(client_member* c1,bool message);
void sp_transmit(client_member* c1,int message);
void transmit_card(client_member* c1,player* p1,bool himself);
void card_change(client_member* c1,fight_room* this_room);//�ֿ��仯
void refresh_character_state_F(client_member* c1,fight_room* this_room);//�״μ�¼����״̬ 
void transmit_this_character_F(client_member* c1,player* p1);//�״δ����ɫ��Ϣ������Ч����Ѫ��������
void blood_change(float number,client_member* c1,fight_room* this_room);
void transmit_blood(client_member* c1,player* p1);//Ѫ���仯
void transmit_attack(client_member* c1,player* p1);
void attack_change(float number,float* ex_attack,client_member* c1,fight_room* this_room);//�������仯
void weapon_change(client_member* c1,fight_room* this_room);
void transmit_weapon(client_member* c1,player* p1);//�����仯
void buff_change(client_member* c1,fight_room* this_room);//����buff�仯
void transmit_buff(client_member* c1,player* p1);

void lock_other_N(client_member* c1,fight_room* this_room);
void release_other_N(client_member* c1,fight_room* this_room);


char* search_card(int situation,int number);                     //��ѯ��Ŵ�����ֿ�Ч��
void ini_catalog();//��ʼ����ƬĿ¼