#ifndef INCLUDE_server_property_H
#define INCLUDE_server_property_H

#include "iostream"
#include "winsock2.h"
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>

#include "player_property.h"
using namespace std;
#pragma comment(lib,"Ws2_32.lib")

#define roomlimit					250//������ս����������
#define memberlimit					20//������������������
#define sleeptime					50//��������΢�ӳٷ�������ʱ��

#define do_attack					11111//�Ƿ���Ҫ�����չ�������ѡ��ʱʹ�ã�
#define make_shield					11112//�����������
#define do_choice					21111//ȷ��ѡ��
#define cancel_choice				31111//ѡ��ܾ�
#define robot_symbol				41111//������ʶ����

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
#define your_event					7004//����¼�
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

#define android_phone				9999//��׿�ֻ���
#define win_console					9998//win32����̨��

int client_number;//��������Ա
int empty_player_pointer;//ָ��ǰ�������ͻ��˿�λ
int empty_room_pointer;//ָ��ǰ�����λ

struct client_member//�ͻ���
{
	int member_No;
	bool member_in_use;//�ͻ��˸�λ���Ƿ�ռ�ã�falseΪ�գ�trueΪռ��
	char client_name[30];//�û���

	int device;//�ֻ����ߵ���
	int state;//�ͻ�״̬������/����/ս����
	int room_No;//�Լ����ڷ����
	bool guest;//�ڷ������Ƿ��Ǳ��ͣ�trueΪ���ͣ�false����Ϊ���˻��ڷ�����

	bool F_offline;

	player* p1;
	player body;

	bool be_locked; 

	char message[MAX_PATH];
	bool not_be_read;
	bool listen;//�Ƿ������
	HANDLE listen_this;
	char p_message[MAX_PATH];

};

client_member Membership[memberlimit+1];
SOCKET g_ServerSocket, g_ClientSocket[memberlimit+1]; //��������� �ͻ����׽���
HANDLE hthread[memberlimit+1];

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
client_member Robot[roomlimit];//���������

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


void addtext(char* StrBuf,int number);
void addtext(char* StrBuf,char* Buf);
void addtext(char* StrBuf,float number);


#endif