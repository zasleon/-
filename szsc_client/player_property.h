#ifndef INCLUDE_player_property_H
#define INCLUDE_player_property_H

#include <iostream>

#include "server_protocol.h"


//����0-1000
//װ����0-29
//������30-59
//Ч����60-300
//������300-369
//��翨370-400

//��ƬЧ��0000-1000
//����Ч��1000-2000
//����Ч��10000-20000

#define playernumber	  2 //Ŀǰֻ����������ɫ
#define weaponlimit		  2//��������Ϊ2
#define cardlimit		  10//�ֿ�����Ϊ10
#define decklimit		  8//Ԥ�迨������Ϊ8��
#define abilitylimit	  4//������������ĸ�
#define weaponeffectlimit 5//Я������Ч�����15��
#define garbagelimit      1000//�Ͽ�����С

//#define you_win     990//��Ӯ��
//#define you_die     991//�����ˣ�˫��ʹ�ã�
//#define you_lose    992//������
#define one_die     993//��������
#define still_fight 994//��Ȼ�ڴ�

#define fore_effect  950//ǰ��Ч��
#define after_effect 951//����Ч��

//����״̬����situation
#define nothing_else         900//û������ˣ����
#define card_effect_finish   998//�ֿ�Ч��ʹ�ý���
#define i_cancel_effect      999//�Ҳ�����Ч��

#define my_turn			     1000//�ҵĻغ�������ʹ��
#define friend_turn          1001//���ѵĻغ�
#define enemy_turn           1002//���˵Ļغ�

#define myturn_attack        1010//�ҵĻغ������������չ�
#define i_will_attack        1010//�Ҽ����չ�


#define i_hit_someone	     1020//�Լ��ɹ���b��Ѫ
#define i_effect			 1021//�Լ��ɹ�����Ч��
#define i_effect_b			 1022//�Լ��ɹ�����Ч����Ѫ
#define i_attack			 1023//�Լ��ɹ������չ�
#define i_attack_T			 1024//�Լ��ɹ���������

#define i_failed_attack_E    1030//�Լ��չ������
#define i_failed_attack_D    1031//�Լ��չ�����
#define i_failed_attack_W    1032//�Լ��չ������˵�����
#define i_failed_effect      1035//�Լ�����Ч��ʧ��
#define i_failed_effect_B    1036//�Լ�������ѪЧ��ʧ��
#define i_failed_damage		 1037//�Լ�û�Զ�������˺�


#define i_will_be_attacked   1050//�Լ������ܵ��չ�
#define i_will_be_effect     1051//�Լ������ܵ�Ч��
#define i_will_be_effect_A   1052//�Լ������ܵ�������ѪЧ��
#define i_will_be_effect_B   1053//�Լ������ܵ����˷�������ѪЧ��
//#define i_will_be_effect_C   1054//�Լ������ܵ��Լ����Լ�������Ч��
#define i_will_be_hit_die    1054//�Լ������ܵ�������Ѫ
#define i_will_be_unmoveable 1055//�Ҽ����ܵ��޷��ж���Ч��

#define i_will_die           1060//�Լ�Ѫ�����㼴������

#define i_be_attacked		 1070//�Լ��Ѿ��ܵ��չ���
#define i_be_effect			 1071//�Լ��Ѿ��ܵ�Ч��
#define i_be_effect_A	     1072//�Լ��Ѿ��ܵ�������ѪЧ��
#define i_be_effect_B        1073//�Լ��Ѿ��ܵ����˷�������ѪЧ��
#define i_be_hit             1074//�Լ��Ѿ��ܵ���Ϊ�����ж�����ɵ���Ѫ��

#define i_escape_attack      2000//�Լ���ܳɹ���һ���չ�
#define i_defend_attack      2001//�Լ��񵲳ɹ���һ���չ�
#define i_erase_effect       2002//�Լ��ɹ�ʹһ��Ч��������Ч
#define i_erase_effect_B     2003//�Լ��ɹ�ʹһ��Ч��Ч��������Ч

#define i_public_card         2050//�Լ������ֿ�
#define i_discard_card_A      2051//�Լ�Ϊ�˷���Ч�����������ֿ�
#define i_discard_card_B      2052//�Լ���Ϊ���˵�Ч�����������ֿ�
#define i_discard             2053//�Լ����ֿ���
#define i_get_card            2054//�Լ��鿨��

#define i_will_discard			2060//�Լ�Ҫ���ֿ���
#define i_will_public_card		2061//�Լ����������ֿ�
#define i_will_discard_card_A	2062//�Լ������������ֿ�
#define i_will_discard_card_B	2063//�Լ���������˵�Ч�����ֿ�

#define i_card_used				2080//�Լ�ʹ����һ���ֿ�
#define i_card_used_S			2081//�Լ��ɹ�ʹ����һ���ֿ�
#define few_card				2082//�Լ��ֿ�����
#define card_discard			2083//���ֿ���������			

#define use_assist				4000//�Լ�ʹ����������
#define has_fought				4001//�Լ�������һ�β���

#define force_end_turn			4050//ǿ�ƽ����غ�
#define	force_end_event			4051//ǿ�ƽ������¼�

#define unmove_time				4060//�˴��ж����޷��ж�
#define unmove_turn				4061//�غ����޷��ж�

#define no_info					187294//û�п���ս��
#define be_in_room				237972//�ڷ�����
#define have_been_changed		154983//������Ϣ��������
#define stay_still				176582//û������

#define game_first				237482//��һ������
#define game_second				252343//�ڶ�������

struct player
{

	int		character_number;//��ɫ���
	int		player_No;//�ڼ������
	bool	whether_myturn;//�Ƿ��ǵ�ǰ��ɫ�Ļغϣ��ǵ�Ϊ1������Ϊ0
	bool	alive;//�Ƿ���,trueΪ�falseΪ��
	float	bloodlimit;//Ѫ������
	float	blood;//��ǰѪ��
	float	attack;								float	exattack_turn;//�ûغ��������ӵĶ��⹥����
												float	exattack_time;//�ôβ����������ӵĶ��⹥����//����������					

	bool a[abilitylimit];						int ability[abilitylimit];//������������ĸ�
	
	bool c[cardlimit];							int card[cardlimit];//ӵ���ֿ�      
	
	bool w_valid[weaponlimit];//������ʱ����Ч��Ч
	bool w[weaponlimit];	   					int weapon[weaponlimit];//װ�����ڷֱ���ʲô�������������ű���
												float exattack_weapon[weaponlimit];//�ڼ������������ӵĹ�����
												float exattack_weapon_turn[weaponlimit];//�ڼ��������۸ûغ����ӵĹ�����
												float exattack_weapon_time[weaponlimit];//�ڼ��������۸ôβ������ӵĹ�����

	bool w_e[weaponlimit][weaponeffectlimit];	int weapon_effect[weaponlimit][weaponeffectlimit];//װ��������ӵ�е�Ч��
	                          
	//int storage_weapon[10];//�ư�����Ч��ר����ͳ���Լ��ж��ٰ����������Բ�ͣ��װ�������л����������ڴ���˴�����������ֻ�ܳ��������������ʹ�á��д���ơ�
    //int speed;//�����ٶȡ��д���ơ�
	int		fight_chance;//�Լ���ǰӵ�е��չ�����
	int		whether_in_attack;//�Ƿ��ڲ�����
	bool	whether_attack_T;//�Ƿ��ڴ���״̬
	bool	cannot_ED;//�Լ��˴��չ��޷���ܺ͸�

    int		un_move_able;//�����ж��Ļغ�
	bool	unmoveable_time;//��ĳ�ж��ڲ����ж�
	int		selfeffect_invalid;//����Ч������Ч���Ļغ�

	int		hideshield;			//�����ʣ��غ���
	int		godlaser;			//���м���ʣ���ֻ���
	int		godlaserdamage;		//���м����˺���ֵ
	int		overdeath;			//��������״̬���ֻ���(���׽�)
	
	int		attacktime_turn;	//�˻غ��Լ��չ��ɹ�����

	float	hurtmultiple_turn;	//�ûغ��Լ���ɵ��չ���Ѫ����
	float	hurtmultiple_time;	//�ô��ж��Լ���ɵ��չ���Ѫ����

	int		weaponeffect001[weaponlimit];//0�ţ��лʽ�������Ч��1ʣ�����
	int		weaponeffect002[weaponlimit];//0�ţ��лʽ�������Ч��2ʣ�����
	int		weaponeffect051[weaponlimit];//5��ʳ������װ��1Ч��ʣ�����
	int		weaponeffect052[weaponlimit];//5��ʳ������װ��2Ч��ʣ�����
	int		weaponeffect06[weaponlimit];//6�Ŷ��շ���ħ��ָʾ�����

};

player* c1;
player* c2;
int* state;//��ǰ����״̬
int* state_p;//��ǰ����״̬
char* replymessage;//�ظ��յ�ר���ַ���
char* getmessage;//�������ݻ����


SOCKET Client;// �ͻ����׽���
bool server_overload;//�������Ƿ���
bool connect_situation;//����״̬
bool create_thread;//�Ƿ��Ѿ������̣߳�
bool claim_overload;//�Ƿ��Ѿ����������أ�
bool stop_send_CGM;

HANDLE hthread;//���ͻ����û������������ݡ��߳̾��
HANDLE hthread_R;//�ͻ��˽������ݾ��

#endif