#ifndef INCLUDE_SZSC_property_H
#define INCLUDE_SZSC_property_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <fstream>
using namespace std;

//����0-1000
//װ����0-29
//������30-59
//Ч����60-399
//��Ч��400-499
//������500-699
//��翨700-1000

//��ƬЧ��0000-1000
//����Ч��1000-2000
//����Ч��10000-20000


#define playernumber		2 //Ŀǰֻ����������ɫ
#define weaponlimit			2//��������Ϊ2
#define cardlimit			10//�ֿ�����Ϊ10
#define decklimit			8//Ԥ�迨������Ϊ8��
#define abilitylimit		4//������������ĸ�
#define weaponeffectlimit	5//Я������Ч�����15��
#define garbagelimit		1000//�Ͽ�����С
#define roomlimit			250//������ս����������


struct player
{

	char	name[30];//"xx����ң�xx"
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
	bool h[cardlimit];							int hideeffect[cardlimit];//��Ч��		
	
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
	int		whether_use_assist;//�Ƿ�ʹ��������������������Ϊ-1
	bool	i_soon_die;//���Ƿ񼴽�������

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


#define p_name		1//��ѯ����
#define p_details	2//��ѯ��ϸ��Ϣ

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

#define i_escape_attack			2000//�Լ���ܳɹ���һ���չ�
#define i_defend_attack			2001//�Լ��񵲳ɹ���һ���չ�
#define i_erase_effect			2002//�Լ��ɹ�ʹһ��Ч��������Ч
#define i_erase_effect_B		2003//�Լ��ɹ�ʹһ��Ч��Ч��������Ч
#define i_immune_effect			2004//�Լ����ߴ˴�Ч��

#define i_public_card         2050//�Լ������ֿ�
#define i_discard_card_A      2051//�Լ�Ϊ�˷���Ч�����������ֿ�
#define i_discard_card_B      2052//�Լ���Ϊ���˵�Ч�����������ֿ�
#define i_discard             2053//�Լ����ֿ���
#define i_get_card            2054//�Լ��鿨��

#define i_will_discard			2060//�Լ�Ҫ���ֿ���
#define i_will_public_card		2061//�Լ����������ֿ�
#define i_will_discard_card_A	2062//�Լ������������ֿ�
#define i_will_discard_card_B	2063//�Լ���������˵�Ч�����ֿ�

#define sb_card_used			2079//���˷�����һ�ſ�
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
#define overdeath_turn			4062//����״̬�����ֻ���
#define hideshield_turn			4063//����ܳ����ֻ���
#define silence_turn			4064//����Ч������Ĭ�غ���
#define buff_clean_all			4065//����buffȫ��

#define do_attack					11111//�Ƿ���Ҫ�����չ�������ѡ��ʱʹ�ã�
#define make_shield					11112//�����������
#define do_choice					21111//ȷ��ѡ��
#define cancel_choice				31111//ѡ��ܾ�
#define robot_symbol				41111//������ʶ����



#define	battle_choice				6900//�ÿͻ�����ʾս��ѡ��һ��
#define	character_choice			6901//�ÿͻ�����ʾ��ɫѡȡ����


#define event_happen				7001//ĳ�¼�������,ǿ���������˵������ж��������
#define event_end					7002//�¼�������������˵������ж��������
#define event_happen_N				7003//�¼���������˼��
#define your_event					7004//����¼�

#define SZSC_apply_create_room		7010
#define	SZSC_apply_enter_room		7011
#define SZSC_apply_exit_room		7012
#define SZSC_apply_add_robot		7013
#define SZSC_apply_remove_robot		7014
#define SZSC_apply_start_game		7015
#define SZSC_apply_exit_roomlist	0



//#define lock_action					7004//��ͼ�ν���ͻ��ˡ���ֹ��������������
//#define release_action				7005//��ͼ�ν���ͻ��ˡ���������

//#define refresh_state				7081//��ʼ��������������
//#define please_reload				7082//���´���
#define refresh_first_state			7100//�״δ�����������
#define refresh_state_CM			7101//�����Լ������ֿ�����
#define refresh_state_CH			7102//��������ֿ�����
#define refresh_state_blood_M		7103//����Ѫ����Ϣ
#define refresh_state_blood_H		7104//���¶���Ѫ����Ϣ
#define refresh_state_buff_M		7105//�����Լ�buff��Ϣ
#define refresh_state_buff_H		7106//���¶���buff��Ϣ
#define refresh_state_attack_M		7107//�����Լ���������Ϣ
#define refresh_state_attack_H		7108//���¶��ֹ�������Ϣ
#define refresh_state_weapon_M		7109//�����Լ�������Ϣ
#define refresh_state_weapon_H		7110//���¶���������Ϣ

#define get_in_room					7200//����뷿����
#define leave_room					7201//���˳�������
#define someone_get_in				7202//���˽��뷿����
#define you_are_guest				7203//���Ƿ������
#define you_are_host				7204//���Ƿ�������
#define please_choose_room			7205//��ѡ���ĸ�����
#define stop_show_room				7206//ֹͣ��ʾ��ѡ��ķ���
#define enter_room_success			7207//����뷿��ɹ���
#define someone_leave				7208//�����뿪������
#define someone_offline				7209//���˵�����
#define choice_complete				7210//��ɫȫѡ����
#define you_win						7211//��Ϸ��������Ӯ��
#define you_lose					7212//��Ϸ������������
#define game_standoff				7213//��Ϸ������ƽ��
#define you_not_in_room				7214//�㲻�ڷ�����
#define no_room_in_use				7215//û�з�����Խ���
#define reopen_choice				7216//����ѡ���ɫ
#define cancel_enter_room			7217//ȡ�����뷿��

#define	start_turn_settle			7300//��ʼ�غϽ���
#define	start_another_turn			7301//���غϿ�ʼ
#define	show_weapon_list			7302//��ͼ�ν���ͻ��ˡ���ʾ������������Ч��
#define	get_c_number				7303//��ͼ�ν���ͻ��ˡ���Ҫװ��������������������Ҫװ���ڼ����ֿ�
#define	get_we_number				7304//��ͼ�ν���ͻ��ˡ���ȡ�����ڼ���Ч��
#define show_buff_list				7305//��ͼ�ν���ͻ��ˡ���ʾbuff��Ч��
#define chooseYN					7306//��ͼ�ν���ͻ��ˡ������Ƿ�ѡ��һ����涼��һ���ʻ�
#define whether_fight_back			7307//�Ƿ񷴻������Ƿ��ж�����0:���ж���1:�����ֿ���2:��������Ч����3:��������Ч����4.�Ӹ�Ч����\n��ѡ��
#define show_rivals_card_P			7308//��ʾ���������ֿ�������
#define show_enemy_list				7309//��ʾ����Ŀ¼
#define show_alive_list				7310//��ʾ�����Ŀ¼
#define now_is_your_turn			7311//ȷ������Ļغ�
#define whether_launch_hideeffect	7312//�Ƿ񷢶���Ƭ��Ч����
#define card_launch					7313//�����˿�Ƭ
#define delete_which_weapon			7314//��ͼ�ν���ͻ��ˡ�������һ��������
#define which_to_hatred				7315//��ʲô�������ޣ�����
#define get_w_number				7316//ѡ�����ĸ�һ������

#define	state_discard_one_card		7400//��ͼ�ν���ͻ��ˡ����롰���������ֿ����Ĳ���״̬
#define	state_discard_muti_card		7401//��ͼ�ν���ͻ��ˡ����롱���������ֿ����Ĳ���״̬
#define	state_return_normal			7402//��ͼ�ν���ͻ��ˡ��ص�ԭʼ��ʹ���ֿ����Ĳ���״̬
#define state_discard_free_card		7403//��ͼ�ν���ͻ��ˡ����롰ѡ���������ֿ����Ĳ���״̬
#define state_choose_one_card		7404//��ͼ�ν���ͻ��ˡ����롰���������ֿ����Ĳ���״̬
#define state_fight_back			7405//��ͼ�ν���ͻ��ˡ����롰�����������Ĳ���״̬


int empty_player_pointer;//ָ��ǰ�������ͻ��˿�λ
int empty_room_pointer;//ָ��ǰ�����λ

struct client_member;

struct fight_room//ս������
{	
	bool room_in_use;//falseΪ�գ�trueΪռ��

	client_member* c[playernumber];//����������Ӫ

	//client_member* c2;
	//bool c2_in_use;//p2�Ƿ�����
	//bool member_full;//��ȫ��ȫ���Ƿ�ﵽ��һ����Ϸ������
	//player* player_all[playernumber];
	int ready_number;//˫����ɫ�Ƿ�ѡ��
	int ready_number2;//�غϽ����Ƿ����

	int deck[decklimit];//���飬Q��ΪʲôҪԤ�迨�飿����ֱ��������ţ�A����Щ��ɫ�С�Ԥ��δ��(��Ԥ�⼴���鵽�Ŀ�)���������������ҪԤ�迨��
	int playerturn;//˭�Ļغϣ���ʼΪ0������ѡ������ȿ�ʼ�غ�
	int passturns;//���ֻ��������Ϊ1
	bool turn_force_end;//�ûغ��Ƿ�ǿ�ƽ�����

};
fight_room player_room[roomlimit];


























#endif