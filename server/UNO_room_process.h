#ifndef INCLUDE_UNO_room_process_H
#define INCLUDE_UNO_room_process_H

#include"server_general_function.h"

void UNO_ini_room()//���з����ʼ��
{
	UNO_empty_room_pointer=0;//ָ���һ��
	int i,j;
	for(i=0;i<UNO_room_limit;i++)
	{
		uno_room[i].room_member=0;//����������Ϊ0
		uno_room[i].who_is_master=-1;//����Ϊ��
		for(j=0;j<UNO_member_limit;j++)
		{
			uno_room[i].player[j].state=UNO_empty;//λ��״̬��Ϊ��
			
		}
	}
}

void UNO_create_room(client_member* c1)//�û���������
{
	if(UNO_room_in_use==UNO_room_limit)//�������ȫ��
	{special_signal(c1,UNO_room_full);return;}
	if(c1->state!=in_online)//��ǰ״̬�޷�����uno����!
	{special_signal(c1,UNO_cannot_create_room);return;}

	//����һ����Ȼ�ܴ�������
	int this_room_number=UNO_empty_room_pointer;
	
	while(uno_room[this_room_number].who_is_master!=-1)
	{
		cout<<"UNO ���󣡣���\n";
		Sleep(100);
		this_room_number=UNO_empty_room_pointer;
		if(UNO_room_in_use==UNO_room_limit)//����ʧ��!
		{special_signal(c1,UNO_room_full);return;}
	}

	uno_room[this_room_number].who_is_master=0;//��ǰ���䷿������Ϊλ��0��
	UNO_room_in_use++;

	int temp=UNO_empty_room_pointer;
	if(UNO_room_in_use!=UNO_room_limit)//�������û��
		while(true)//��Ѱ��һ���շ��䣬����UNO_empty_room_pointer
		{
			temp++;//����һ������
			if(temp==UNO_room_limit)temp=0;//���������һ�����䣬��ͷ��ʼ��

			if(uno_room[temp].who_is_master==-1)//����÷���Ϊ��
			{
				UNO_empty_room_pointer=temp;//ȷ�Ϸ���ţ�����UNO_empty_room_pointer
				break;//����ѭ��
			}
		
		}

	uno_room[this_room_number].player[0].c1=c1;
	uno_room[this_room_number].player[0].state=UNO_human;//��λ��״̬����Ϊ�������
	c1->uno_player=&uno_room[this_room_number].player[0];
	c1->room_No=this_room_number;//ȷ��������ڷ����
	special_signal(c1,UNO_create_room_success);//����ɹ�!
	c1->state=UNO_in_room;//�������״̬Ϊ����uno�����ڡ�
	uno_room[this_room_number].room_member++;//����������Ϊ1
	
}


void UNO_show_room_in_use(client_member* c1)//����չʾ�����ܼ���ķ���
{
	special_signal(c1,UNO_show_roomlist);
	char StrBuf[MAX_PATH] = {0};
	char Buf[MAX_PATH] = {0};

	int count;
	for(count=0;count<UNO_room_limit;count++)
		if(uno_room[count].who_is_master!=-1)
		{
			memset(StrBuf,0,sizeof(StrBuf));//�����+��������+����
			addtext(StrBuf,count+1);//�ŷ���
			addtext(StrBuf,"\t");
			addtext(StrBuf,uno_room[count].player[uno_room[count].who_is_master].c1->client_name);
			addtext(StrBuf,"\t");
			addtext(StrBuf,uno_room[count].room_member);
			//strcat(StrBuf,"(�÷�������!)");
			
			addtext(StrBuf,"\n");
			confirm_send_success(c1,StrBuf);
		}
		
		special_signal(c1,UNO_show_roomlist_end);
}

void UNO_enter_room(client_member* c1)//�û�������˵ķ���
{

	if(UNO_room_in_use==0)//���û��ʹ���ŵķ���
	{special_signal(c1,UNO_no_room_in_use);return;}
	if(c1->state!=in_online)//��ǰ״̬�޷�������˵�uno����!
	{special_signal(c1,UNO_cannot_enter_room);return;}

	//ִ�е������Ȼ��ѡ����˵ķ��䲢����
	c1->state=UNO_in_roomlist;
	UNO_show_room_in_use(c1);//����չʾ�����ܼ���ķ���

	char StrBuf[MAX_PATH] = { 0 };//��������洢���յ�������
	int choice;//�洢���������ѡ��
	while(true)//ѡ�񷿼���ѭ��
	{
		memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,get_client_message(c1));if(c1->F_offline){return;}//�������ѡ�������ʱ�Ͽ�ͨѶ

		cout<<"�û����룺"<<StrBuf<<endl;
		if(atoi(StrBuf)==UNO_apply_exit_roomlist)//ѡ��ȡ�����뷿��!��������ԭҳ�桭��
		{
			special_signal(c1,UNO_cancel_enter_room);
			c1->state=in_online;
			return;
		}
		
						
		choice=atoi(StrBuf);//ת������Ϊ����
						
		if(choice==0)//confirm_send_success(c1,"��������Чֵ!\n");
		{continue;}
		if(choice>UNO_room_limit||choice<0)//confirm_send_success(c1,"���볬��������������!\n");
		{continue;}

		choice--;

		if(uno_room[choice].who_is_master==-1)//confirm_send_success(c1,"�÷�����û��!\n");
		{continue;}
		if(uno_room[choice].room_member==UNO_member_limit)//confirm_send_success(c1,"�÷���������!\n");
		{continue;}
							
		c1->state=UNO_in_room;//ִ�е���һ����Ȼ����ɹ�
		uno_room[choice].room_member++;//����������+1
		c1->guest=true;//���뷿������Ƿ������
		c1->room_No=choice;
		c1->state=UNO_in_room;//�������״̬Ϊ����uno�����ڡ�

		for(int i=0;i<UNO_member_limit;i++)
			if(uno_room[choice].player[i].state==UNO_empty)//�����ǰλ��Ϊ��
			{
				uno_room[choice].player[i].state=UNO_human;//��λ��״̬����Ϊ�������
				c1->uno_player=&uno_room[choice].player[i];//�����ʶ
				uno_room[choice].player[i].c1=c1;//�����ʶ
				GetLocalTime(&c1->uno_player->enter_time);//��ȡ��ǰʱ�䣬���ڷ����䶯
				break;
			}

		c1->room_No=choice;//���淿�����Ϣ

		special_signal(c1,UNO_enter_room_success);//����ɹ�!
		

		for(int i=0;i<UNO_member_limit;i++)//���й㲥���˼��뷿��
		{
			client_member* c2;
			if(uno_room[choice].player[i].state!=UNO_human)//�����ǰλ���ϲ��ǻ��ˣ�����һ��
				continue;
			if(uno_room[choice].player[i].c1->member_No!=c1->member_No)//�����ǰλ���ϲ��Ǳ���
				c2=uno_room[choice].player[i].c1;
			else
				continue;

			SuspendThread(hthread[c2->member_No-1]);//��ס���ļ����߳�
			special_signal(c2,UNO_someone_get_in);//���߷������˽�����
			memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//���߷������˽���������
			confirm_send_success(c2,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
			ResumeThread(hthread[c2->member_No-1]);//������ļ����߳�

		}

		break;//ִ�е�����˵������ɹ��ҽ������
	}//ѡ�񷿼���ѭ��

}

bool compare_enter_time(SYSTEMTIME challenger,SYSTEMTIME former_master)//����true��ʾǰ�߱Ⱥ��ߴ�
{
	bool result=false;//���ȷ�������
	if(challenger.wYear<former_master.wYear)
		result=true;
	else
	{
		if(challenger.wMonth<former_master.wMonth)
			result=true;
		else
		{
			if(challenger.wDay<former_master.wDay)
				result=true;
			else
			{
				if(challenger.wHour<former_master.wHour)
					result=true;
				else
				{
					if(challenger.wMinute<former_master.wMinute)
						result=true;
					else
						if(challenger.wMilliseconds<former_master.wMilliseconds)
							result=true;
				}
			}
		}
	}
	return result;
}

void UNO_exit_room(client_member* c1)//�û��˳�����
{
	char StrBuf[MAX_PATH] = { 0 };

	if(c1->state!=UNO_in_room)//��ǰ״̬�޷��˳�uno����!
	{special_signal(c1,UNO_cannot_exit_room);return;}

	for(int i=0;i<UNO_member_limit;i++)
		if(uno_room[c1->room_No].player[i].c1!=NULL&&uno_room[c1->room_No].player[i].c1->member_No==c1->member_No)//�ҵ�Ҫ�˳���
		{
			
			if(uno_room[c1->room_No].who_is_master==i)//����˳���Ϊ��������,����������ʩ
			{
				int who_will_be_the_master=-1;

				for(int j=UNO_member_limit-1;j>=0;j--)
					if(i!=j&&uno_room[c1->room_No].player[j].state==UNO_human)//������λ�Ӽ�飬����ǰλ�����ǻ���
					{
						if(who_will_be_the_master==-1)//�����ǰ��ûȷ�Ϲ������򲹣�����ǰλ���ϵ�����Ϊ������
							who_will_be_the_master=j;
						else//����з������ˣ��Ƚ����ߵĽ��뷿��ʱ��
						{
							if(compare_enter_time(uno_room[c1->room_No].player[j].enter_time,uno_room[c1->room_No].player[who_will_be_the_master].enter_time))
								who_will_be_the_master=j;
						}

					}
				uno_room[c1->room_No].who_is_master=who_will_be_the_master;//ȷ���·���
				if(who_will_be_the_master!=-1)//����ǰ���䲻Ϊ��
					uno_room[c1->room_No].player[who_will_be_the_master].c1->guest=false;
				else//û����,�÷���û��ʹ��
				{
					for(int k=0;k<UNO_member_limit;k++)
					{
						uno_room[i].player[k].state=UNO_empty;//λ��״̬��Ϊ��
					}
					UNO_room_in_use--;//�ܷ���ʹ������-1
				}
			}//���������������
			for(int i=0;i<UNO_member_limit;i++)//���й㲥���˼��뷿��
			{
				client_member* c2;
				if(uno_room[c1->room_No].player[i].state!=UNO_human)//�����ǰλ���ϲ��ǻ��ˣ�����һ��
					continue;
				if(uno_room[c1->room_No].player[i].c1->member_No!=c1->member_No)//�����ǰλ���ϲ��Ǳ���
					c2=uno_room[c1->room_No].player[i].c1;
				else
					continue;

				SuspendThread(hthread[c2->member_No-1]);//��ס���ļ����߳�
				special_signal(c2,UNO_someone_get_in);//���߷������˽�����
				memset(StrBuf,0,sizeof(StrBuf));addtext(StrBuf,c1->client_name);addtext(StrBuf,"\n");//���߷������˽���������
				confirm_send_success(c2,StrBuf);memset(StrBuf,0,sizeof(StrBuf));
				ResumeThread(hthread[c2->member_No-1]);//������ļ����߳�

			}
			uno_room[c1->room_No].player[i].c1=NULL;//����ȡ������
			c1->uno_player=NULL;
			uno_room[c1->room_No].room_member--;//����������-1
			c1->state=in_online;//�����û�״̬Ϊ�����ߡ�
			c1->guest=false;
			uno_room[c1->room_No].player[i].state=UNO_empty;//��ǰλ��״̬����Ϊ��
			special_signal(c1,UNO_leave_room_success);//�뿪�ɹ�!
			
			
			
			
			break;
		}


}



#endif