#include "winsock2.h"
#include <stdio.h>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")
#include <iostream>
#include <conio.h>

using namespace std;

#define start_link					6666//���߿ͻ��˳ɹ�ͨѶ�����ˣ����ͻ����ղ����������ʱ������Լ������������ܳ����ˡ���
#define please_cls					6667//�ÿͻ���ˢ����Ļsystem("cls");
#define show_choice					6668//�ÿͻ�����ʾѡ�����
#define show_user					6669//�ÿͻ�����ʾ�����û�״̬
#define	stop_show_user				6700//ֹͣ��ʾ�����û�״̬
#define client_get_message			6701//�ͻ��˷����Լ����յ���Ϣ
#define show_video					6702//��Ƶ����

#define	in_game						6800//������Ϸ״̬
#define in_room						6801//�����ڷ���״̬
#define	in_online					6802//��������״̬

#define	battle_choice				6900//�ÿͻ�����ʾս��ѡ��һ��
#define	character_choice			6901//�ÿͻ�����ʾ��ɫѡȡ����

#define force_offline				7000//�ͻ���������
#define event_happen				7001//ĳ�¼�������,ǿ���������˵������ж��������
#define event_end					7002//�¼�������������˵������ж��������
#define event_happen_N				7003//�¼���������˼��
#define lock_action					7004//��ͼ�ν���ͻ��ˡ���ֹ��������������
#define release_action				7005//��ͼ�ν���ͻ��ˡ���������

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
#define leave_room					7201//�˳�������
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

#define	state_discard_one_card		7400//��ͼ�ν���ͻ��ˡ����롰���������ֿ����Ĳ���״̬
#define	state_discard_muti_card		7401//��ͼ�ν���ͻ��ˡ����롱���������ֿ����Ĳ���״̬
#define	state_return_normal			7402//��ͼ�ν���ͻ��ˡ��ص�ԭʼ��ʹ���ֿ����Ĳ���״̬
#define state_discard_free_card		7403//��ͼ�ν���ͻ��ˡ����롰ѡ���������ֿ����Ĳ���״̬
#define state_choose_one_card		7404//��ͼ�ν���ͻ��ˡ����롰���������ֿ����Ĳ���״̬
#define state_fight_back			7405//��ͼ�ν���ͻ��ˡ����롰�����������Ĳ���״̬

#define android_phone				9999//��׿�ֻ���
#define win_console					9998//win32����̨��

#define p_name		1//��ѯ����
#define p_details	2//��ѯ��ϸ��Ϣ