
#ifndef INCLUDE_UNO_property_H
#define INCLUDE_UNO_property_H




#define UNO_member_limit	4//һ����Ϸ�����������
#define UNO_room_limit		4//������������

//��ɫ
#define UNO_green	1
#define UNO_red		2
#define UNO_blue	3
#define UNO_yellow	4
#define UNO_black	5



//���⹦��
#define UNO_none	-1//����Ϊ��
#define UNO_ADD_2	100
#define UNO_ADD_4	200
#define UNO_reverse	300//����
#define UNO_forbid	400//������һ����

//������ĳһλ�ӵ�״̬
#define	UNO_empty		0//��λ��Ϊ��
#define UNO_human		1//�������
#define UNO_simple_AI	2//�򵥻�����
#define UNO_hard_AI		3//���ѻ�����

//��ҵ�ǰ״̬
//#define	in_online					6802//��������״̬
#define UNO_in_room					6803//����uno������
#define UNO_in_game					6804//����uno��Ϸ��
#define UNO_in_roomlist				6805//����ѡ��uno�����б���


struct client_member;
struct UNO_player
{
	client_member* c1;
	char	name[30];
	int		state;//״̬��������������
	int		card_effect[108];//�ֿ�����
	int		card_color[108];
	SYSTEMTIME	enter_time;//���뷿��ʱ��
};

static int UNO_room_in_use;//��ǰ��ʹ�õķ�������
static int UNO_empty_room_pointer;
struct UNO_room
{
	int		deck_card_effect[108];//����
	int		deck_card_color[108];

	int		tomb_card_effect[108];//�Ͽ���
	int		tomb_card_color[108];

	UNO_player player[UNO_member_limit];
	int		who_is_master;//-1-4,˭�Ƿ�����-1��ʾ����Ϊ��
	int		room_member;//��ǰ�ڷ�������Ա��������
	
};

UNO_room uno_room[UNO_room_limit];

#define UNO_apply_create_room	2000
#define	UNO_apply_enter_room	2001
#define UNO_apply_exit_room		2002
#define UNO_apply_add_robot		2003
#define UNO_apply_start_game	2004
#define UNO_apply_exit_roomlist	0


#define UNO_room_full			2050//����������û��������������
#define UNO_no_room_in_use		2051//��ǰû�з�����ʹ�ã��޷����뷿��
#define UNO_create_room_success	2052//��������ɹ�
#define UNO_enter_room_success	2053//���뷿��ɹ�
#define UNO_someone_get_in		2054//���˽�������
#define UNO_show_roomlist		2055//��ͻ���չʾ�����б�
#define UNO_leave_room_success	2056//�뿪����ɹ�
#define UNO_show_roomlist_end	2057//��ʾ�������
#define	UNO_cannot_exit_room	2058//��ǰ״̬�޷��˳�uno����
#define	UNO_cannot_enter_room	2059//��ǰ״̬�޷�������˵�uno����!
#define	UNO_cannot_create_room	2060//��ǰ״̬�޷�����uno����!
#define UNO_cancel_enter_room	2061//ȡ�����������˵ķ���








#endif