#include "client.h"



#undef main;
void main()
{
	//47.102.218.193
	//10.125.39.201 ����	8001
	//103.46.128.21 ������	34024
	//182.92.237.216

	//char my_ip[50]="103.46.128.21";// ����Ŀ�������IP
	//int portnumber=34024;//����Ŀ��������˿ں�
	char my_ip[50]="10.125.39.201";// ����Ŀ�������IP
	int portnumber=8001;//����Ŀ��������˿ں�
	//char my_ip[50]="222.222.221.215";// ����Ŀ�������IP
	//int portnumber=8001;//����Ŀ��������˿ں�

	connect_situation=false;//����״̬���ɹ���ʧ�ܣ���ʼ����Ϊʧ��
	bool connect_failed_claim=false;//�Ƿ�����ʾ����ʧ�ܣ�Ϊ��ֻ��ʾһ�Σ�
	bool connect_success_claim=false;//�Ƿ�����ʾ���ӳɹ�

	server_overload=true;//��ֹ���������أ���Ϊtrue��˼�Ƿ�����������������
	create_thread=false;//��δ�����߳�
	claim_overload=false;//��δ��������������
	//cout<<"�����������ip:";cin>>my_ip;

	player player_all[playernumber];//������Ϸ�����б�p[0]��Զ���Լ���Ŀǰ���������
	c1=&player_all[0];
	c2=&player_all[1];

	int data_state=no_info;state=&data_state;//������Ϣ��no_infoΪû�н�����Ϸ
	
	int process_state;state_p=&process_state;//������Ϣ��Ϊ�ڼ������򣿵�һ��ڶ�
	if(!control_number())return;//��ֹ����࿪
	
	char reply[260] = { 0 };sprintf(reply,"%d",client_get_message);replymessage=reply;//replymessageָ��"�ظ��յ�"ר���ַ���
	
	char StrBuf[260] = { 0 };getmessage=StrBuf;//getmessageָ��ר���������շ������������ݵĻ����ַ�����ר�Ž�������������Ϣ
	
	int count;
	for(count=0;count<playernumber;count++)
	ini_character(&player_all[count],162359);




	while(1)//�������ӿ�ʼ
	{

		if(connect_situation==false)//�������״̬Ϊʧ��
		{
			
			if(Connection(my_ip, portnumber)==true)//��ʼ�������ӣ�������ؽ���ɹ�
			{
				connect_situation=true;connect_failed_claim=false;//����״̬��Ϊ�ɹ�	
			}
			else 
			{
				if(!connect_failed_claim)
					{cout<<"\nĿǰ���Ӳ���������!\n";connect_failed_claim=true;connect_success_claim=false;}//���û��ʾ��ʧ�ܣ�������ʾ�������ʾ���ˣ������˲���
				Sleep(1000);
			}
			
			//else printf("���ӳ�������!\n�����Ƿ�����û����!Ҳ�����Ǳ��ؾ�������ַ���ô���!\n���������뱾�ؾ�����ip:");cin>>my_ip;	
		}

		if(connect_situation)//������ӳɹ�����ʼ��������
		{
			if(!connect_success_claim)//�Ƿ������������ӳɹ����������û�У�ִ����������
			{			
				
				if(!create_thread)//�Ƿ񴴽������ա��������ݵĶ��̣߳����û�У����д���
				{
					hthread_R	=	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Receive_Message, NULL, NULL, NULL);//������Ϣ
					hthread		=	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Send_Message, NULL, NULL, NULL);//������Ϣ
					create_thread=true;
				}
				
				Sleep(5000);//��΢�ȴ����룬���ܷ��������ݣ�̽��������Ƿ���,�����еȴ�����������ݴ�����ң���ʾ����
				//û����ʱ�ᷢ��start_link������������û���أ����Կ�ʼ����

				if(server_overload&&!claim_overload)//���̽�⵽������������û������������������
				{
					cout<<"\n��⵽������!\n";
					cout<<"���ӳ�ʱ!���ܷ����������Ѵ︺������!\n";
					claim_overload=true;
					//return;
				}
				if(!server_overload)//���û̽�⵽����������,���ӳɹ�!
				{
					claim_overload=false;connect_success_claim=true;//��������
				}

			}
		}//���ӳɹ����������ݽ���
		
	}//�������ӽ�����������ֻҪexe���ţ��ͻ������оͲ��������

}
